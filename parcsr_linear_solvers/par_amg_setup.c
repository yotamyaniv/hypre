/*BHEADER**********************************************************************
 * (c) 1998   The Regents of the University of California
 *
 * See the file COPYRIGHT_and_DISCLAIMER for a complete copyright
 * notice, contact person, and disclaimer.
 *
 * $Revision$
 *********************************************************************EHEADER*/

#include "headers.h"
#include "par_amg.h"

#define DEBUG 0

/*****************************************************************************
 *
 * Routine for driving the setup phase of AMG
 *
 *****************************************************************************/

int
hypre_BoomerAMGSetup( void               *amg_vdata,
                   hypre_ParCSRMatrix *A,
                   hypre_ParVector    *f,
                   hypre_ParVector    *u         )
{
   MPI_Comm 	      comm = hypre_ParCSRMatrixComm(A); 

   hypre_ParAMGData   *amg_data = amg_vdata;

   /* Data Structure variables */

   hypre_ParCSRMatrix **A_array;
   hypre_ParVector    **F_array;
   hypre_ParVector    **U_array;
   hypre_ParVector     *Vtemp;
   hypre_ParCSRMatrix **P_array;
   int                **CF_marker_array;   
   double              *relax_weight;
   double               strong_threshold;
   double               max_row_sum;
   double               trunc_factor;

   int      num_variables;
   int      max_levels; 
   int      amg_ioutdat;
   int      debug_flag;
 
   /* Local variables */
   int                 *CF_marker;
   hypre_ParCSRMatrix  *S;
   hypre_ParCSRMatrix  *P;
   hypre_ParCSRMatrix  *A_H;

   int       old_num_levels, num_levels;
   int       level;
   int       local_size, i;
   int       coarse_size;
   int       coarsen_type;
   int       measure_type;
   int       setup_type;
   int       fine_size;
   double    size;
   int       not_finished_coarsening = 1;
   int       Setup_err_flag = 0;
   int       coarse_threshold = 9;
   int       j;
   int       num_procs,my_id;
   int     *grid_relax_type = hypre_ParAMGDataGridRelaxType(amg_data);

   double    wall_time;   /* for debugging instrumentation */

   MPI_Comm_size(comm, &num_procs);   
   MPI_Comm_rank(comm,&my_id);

   old_num_levels = hypre_ParAMGDataNumLevels(amg_data);
   max_levels = hypre_ParAMGDataMaxLevels(amg_data);
   amg_ioutdat = hypre_ParAMGDataIOutDat(amg_data);
   coarsen_type = hypre_ParAMGDataCoarsenType(amg_data);
   measure_type = hypre_ParAMGDataMeasureType(amg_data);
   setup_type = hypre_ParAMGDataSetupType(amg_data);
   debug_flag = hypre_ParAMGDataDebugFlag(amg_data);
   relax_weight = hypre_ParAMGDataRelaxWeight(amg_data);
   
   if (setup_type == 0) return Setup_err_flag;

   A_array = hypre_ParAMGDataAArray(amg_data);
   P_array = hypre_ParAMGDataPArray(amg_data);
   CF_marker_array = hypre_ParAMGDataCFMarkerArray(amg_data);

   grid_relax_type[3] = hypre_ParAMGDataUserCoarseRelaxType(amg_data); 
   if (A_array || P_array || CF_marker_array)
   {
      for (j = 1; j < old_num_levels; j++)
      {
        if (A_array[j])
        {
           hypre_ParCSRMatrixDestroy(A_array[j]);
           A_array[j] = NULL;
        }
      }

      for (j = 0; j < old_num_levels-1; j++)
      {
         if (P_array[j])
         {
            hypre_ParCSRMatrixDestroy(P_array[j]);
            P_array[j] = NULL;
         }

         if (CF_marker_array[j])
            hypre_TFree(CF_marker_array[j]);
      }
   }

   if (A_array == NULL)
      A_array = hypre_CTAlloc(hypre_ParCSRMatrix*, max_levels);
   if (P_array == NULL)
      P_array = hypre_CTAlloc(hypre_ParCSRMatrix*, max_levels-1);
   if (CF_marker_array == NULL)
      CF_marker_array = hypre_CTAlloc(int*, max_levels-1);

   hypre_ParCSRMatrixSetNumNonzeros(A);
   A_array[0] = A;
   /*----------------------------------------------------------
    * Initialize hypre_ParAMGData
    *----------------------------------------------------------*/

   num_variables = hypre_ParCSRMatrixNumRows(A);

   hypre_ParAMGDataNumVariables(amg_data) = num_variables;

   not_finished_coarsening = 1;
   level = 0;
  
   strong_threshold = hypre_ParAMGDataStrongThreshold(amg_data);
   max_row_sum = hypre_ParAMGDataMaxRowSum(amg_data);
   trunc_factor = hypre_ParAMGDataTruncFactor(amg_data);

   /*-----------------------------------------------------
    *  Enter Coarsening Loop
    *-----------------------------------------------------*/

   while (not_finished_coarsening)
   {
      fine_size = hypre_ParCSRMatrixGlobalNumRows(A_array[level]);

      /*-------------------------------------------------------------
       * Select coarse-grid points on 'level' : returns CF_marker
       * for the level.  Returns strength matrix, S  
       *--------------------------------------------------------------*/
     
      if (debug_flag==1) wall_time = time_getWallclockSeconds();
      if (debug_flag==3)
      {
          printf("\n ===== Proc = %d     Level = %d  =====\n",
                        my_id, level);
          fflush(NULL);
      }
      if (relax_weight[level] == 0.0)
      {
	 hypre_ParCSRMatrixScaledNorm(A_array[level], &relax_weight[level]);
	 if (relax_weight[level] != 0.0)
	    relax_weight[level] = (4.0/3.0)/relax_weight[level];
	 else
	   printf (" Warning ! Matrix norm is zero !!!");
      }
      if (max_levels > 1)
      {
         if (coarsen_type == 6)
         {
	    hypre_BoomerAMGCoarsenFalgout(A_array[level],
                                    strong_threshold, max_row_sum,
                                    debug_flag, &S, &CF_marker, &coarse_size); 
         }
         else if (coarsen_type)
         {
	    hypre_BoomerAMGCoarsenRuge(A_array[level],
                                 strong_threshold, max_row_sum,
                                 measure_type, coarsen_type, debug_flag,
                                 &S, &CF_marker, &coarse_size); 
         }
         else
         {
	    hypre_BoomerAMGCoarsen(A_array[level],
                             strong_threshold, max_row_sum,
                             debug_flag, &S, &CF_marker, &coarse_size); 
         }
 
         if (debug_flag==1)
         {
            wall_time = time_getWallclockSeconds() - wall_time;
            printf("Proc = %d    Level = %d    Coarsen Time = %f\n",
                       my_id,level, wall_time); 
	    fflush(NULL);
         }

         CF_marker_array[level] = CF_marker;
      
#if DEBUG
   if (amg_ioutdat == -3)
   {  
      char  filename[255];
      FILE *fp;
      int   i;

      /* print out strength matrix */
      sprintf(filename, "zout_S_%02d.ysmp", level);
      hypre_ParCSRMatrixPrint(S, filename);

      /* print out C/F marker */
      sprintf(filename, "zout_CF_%02d.%d", level, my_id);
      fp = fopen(filename, "w");
      num_variables = hypre_ParCSRMatrixNumRows(A_array[level]);
      for (i = 0; i < num_variables; i++)
      {
         fprintf(fp, "%d\n", CF_marker[i]);
      }
      fclose(fp);
   } 
#endif

         /*-------------------------------------------------------------
          * Build prolongation matrix, P, and place in P_array[level] 
          *--------------------------------------------------------------*/
         if (debug_flag==1) wall_time = time_getWallclockSeconds();

         hypre_BoomerAMGBuildInterp(A_array[level], CF_marker_array[level], S,
                              debug_flag, trunc_factor, &P);

         if (debug_flag==1)
         {
            wall_time = time_getWallclockSeconds() - wall_time;
            printf("Proc = %d    Level = %d    Build Interp Time = %f\n",
                       my_id,level, wall_time);
	    fflush(NULL);
         }

         P_array[level] = P; 
         hypre_ParCSRMatrixDestroy(S);
         S = NULL;
         coarse_size = hypre_ParCSRMatrixGlobalNumCols(P);
      }
      else
      {
	 local_size = hypre_CSRMatrixNumRows(hypre_ParCSRMatrixDiag(A));
         CF_marker = hypre_CTAlloc(int, local_size );
	 for (i=0; i < local_size ; i++)
	    CF_marker[i] = 1;
         CF_marker_array = hypre_CTAlloc(int*, 1);
	 CF_marker_array[level] = CF_marker;
	 coarse_size = fine_size;
      }

      /* if no coarse-grid, stop coarsening, and set the
       * coarsest solve to be a single sweep of Jacobi */
      /* RDF/JEJ: Why is coarse_size being set twice? The following
       * code should move up just behind the coarsening routines */
      if ((coarse_size == 0) ||
          (coarse_size == fine_size))
      {
         int     *num_grid_sweeps =
            hypre_ParAMGDataNumGridSweeps(amg_data);
         int    **grid_relax_points =
            hypre_ParAMGDataGridRelaxPoints(amg_data);
         if (grid_relax_type[3] == 9)
	 {
	    grid_relax_type[3] = grid_relax_type[0];
	    num_grid_sweeps[3] = 1;
	    grid_relax_points[3][0] = 0; 
	 }
	 if (P_array)
            hypre_ParCSRMatrixDestroy(P_array[level]);
         if (level > 0)
         {
            /* note special case treatment of CF_marker is necessary
             * to do CF relaxation correctly when num_levels = 1 */
            hypre_TFree(CF_marker_array[level]);
         }

         break; 
      }

      /*-------------------------------------------------------------
       * Build coarse-grid operator, A_array[level+1] by R*A*P
       *--------------------------------------------------------------*/

      if (debug_flag==1) wall_time = time_getWallclockSeconds();

      hypre_BoomerAMGBuildCoarseOperator(P_array[level], A_array[level] , 
                                      P_array[level], &A_H);

      if (debug_flag==1)
      {
         wall_time = time_getWallclockSeconds() - wall_time;
         printf("Proc = %d    Level = %d    Build Coarse Operator Time = %f\n",
                       my_id,level, wall_time);
	 fflush(NULL);
      }

      ++level;
      hypre_ParCSRMatrixSetNumNonzeros(A_H);
      A_array[level] = A_H;

      size = ((double) fine_size )*.75;
      if (coarsen_type > 0 && coarse_size >= (int) size)
      {
	coarsen_type = 0;      
      }

      if ( (level+1 >= max_levels) || 
           (coarse_size <= coarse_threshold) )
      {
         not_finished_coarsening = 0;
      }
   } 
   
   /*-----------------------------------------------------------------------
    * enter all the stuff created, A[level], P[level], CF_marker[level],
    * for levels 1 through coarsest, into amg_data data structure
    *-----------------------------------------------------------------------*/

   num_levels = level+1;
   hypre_ParAMGDataNumLevels(amg_data) = num_levels;
   hypre_ParAMGDataCFMarkerArray(amg_data) = CF_marker_array;
   hypre_ParAMGDataAArray(amg_data) = A_array;
   hypre_ParAMGDataPArray(amg_data) = P_array;
   hypre_ParAMGDataRArray(amg_data) = P_array;

   /*-----------------------------------------------------------------------
    * Setup Vtemp, F and U arrays
    *-----------------------------------------------------------------------*/

   Vtemp = hypre_ParAMGDataVtemp(amg_data);

   if (Vtemp != NULL)
   {
      hypre_ParVectorDestroy(Vtemp);
      Vtemp = NULL;
   }

   Vtemp = hypre_ParVectorCreate(hypre_ParCSRMatrixComm(A_array[0]),
                                 hypre_ParCSRMatrixGlobalNumRows(A_array[0]),
                                 hypre_ParCSRMatrixRowStarts(A_array[0]));
   hypre_ParVectorInitialize(Vtemp);
   hypre_ParVectorSetPartitioningOwner(Vtemp,0);
   hypre_ParAMGDataVtemp(amg_data) = Vtemp;

   F_array = hypre_ParAMGDataFArray(amg_data);
   U_array = hypre_ParAMGDataUArray(amg_data);

   if (F_array != NULL || U_array != NULL)
   {
      for (j = 1; j < old_num_levels; j++)
      {
         if (F_array[j] != NULL)
         {
            hypre_ParVectorDestroy(F_array[j]);
            F_array[j] = NULL;
         }
         if (U_array[j] != NULL)
         {
            hypre_ParVectorDestroy(U_array[j]);
            U_array[j] = NULL;
         }
      }
   }

   if (F_array == NULL)
      F_array = hypre_CTAlloc(hypre_ParVector*, max_levels);
   if (U_array == NULL)
      U_array = hypre_CTAlloc(hypre_ParVector*, max_levels);

   F_array[0] = f;
   U_array[0] = u;

   for (j = 1; j < num_levels; j++)
   {
      F_array[j] =
         hypre_ParVectorCreate(hypre_ParCSRMatrixComm(A_array[j]),
                               hypre_ParCSRMatrixGlobalNumRows(A_array[j]),
                               hypre_ParCSRMatrixRowStarts(A_array[j]));
      hypre_ParVectorInitialize(F_array[j]);
      hypre_ParVectorSetPartitioningOwner(F_array[j],0);

      U_array[j] =
         hypre_ParVectorCreate(hypre_ParCSRMatrixComm(A_array[j]),
                               hypre_ParCSRMatrixGlobalNumRows(A_array[j]),
                               hypre_ParCSRMatrixRowStarts(A_array[j]));
      hypre_ParVectorInitialize(U_array[j]);
      hypre_ParVectorSetPartitioningOwner(U_array[j],0);
   }

   hypre_ParAMGDataFArray(amg_data) = F_array;
   hypre_ParAMGDataUArray(amg_data) = U_array;

   /*-----------------------------------------------------------------------
    * Print some stuff
    *-----------------------------------------------------------------------*/

   if (amg_ioutdat == 1 || amg_ioutdat == 3)
      hypre_BoomerAMGSetupStats(amg_data,A);

#if DEBUG
   if (amg_ioutdat == -3)
   {  
      char  filename[255];

      for (j = 0; j < (num_levels - 1); j++)
      {
         sprintf(filename, "zout_A_%02d.ysmp", j);
         hypre_ParCSRMatrixPrint(A_array[j], filename);
         sprintf(filename, "zout_P_%02d.ysmp", j);
         hypre_ParCSRMatrixPrint(P_array[j], filename);
      }                         
      sprintf(filename, "zout_A_%02d.ysmp", j);
      hypre_ParCSRMatrixPrint(A_array[j], filename);
   } 
#endif

   return(Setup_err_flag);
}  
