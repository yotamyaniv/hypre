/*
 * File:          bHYPRE_StructStencil_Impl.h
 * Symbol:        bHYPRE.StructStencil-v1.0.0
 * Symbol Type:   class
 * Babel Version: 1.0.0
 * Description:   Server-side implementation for bHYPRE.StructStencil
 * 
 * WARNING: Automatically generated; only changes within splicers preserved
 * 
 */

#ifndef included_bHYPRE_StructStencil_Impl_h
#define included_bHYPRE_StructStencil_Impl_h

#ifndef included_sidl_header_h
#include "sidl_header.h"
#endif
#ifndef included_bHYPRE_StructStencil_h
#include "bHYPRE_StructStencil.h"
#endif
#ifndef included_sidl_BaseClass_h
#include "sidl_BaseClass.h"
#endif
#ifndef included_sidl_BaseInterface_h
#include "sidl_BaseInterface.h"
#endif
#ifndef included_sidl_ClassInfo_h
#include "sidl_ClassInfo.h"
#endif
#ifndef included_sidl_RuntimeException_h
#include "sidl_RuntimeException.h"
#endif

/* DO-NOT-DELETE splicer.begin(bHYPRE.StructStencil._includes) */
/* Put additional include files here... */

/*BHEADER**********************************************************************
 * Copyright (c) 2006   The Regents of the University of California.
 * Produced at the Lawrence Livermore National Laboratory.
 * Written by the HYPRE team <hypre-users@llnl.gov>, UCRL-CODE-222953.
 * All rights reserved.
 *
 * This file is part of HYPRE (see http://www.llnl.gov/CASC/hypre/).
 * Please see the COPYRIGHT_and_LICENSE file for the copyright notice, 
 * disclaimer and the GNU Lesser General Public License.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License (as published by the Free
 * Software Foundation) version 2.1 dated February 1999.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the terms and conditions of the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * $Revision$
 ***********************************************************************EHEADER*/

#include "struct_mv.h"
/* DO-NOT-DELETE splicer.end(bHYPRE.StructStencil._includes) */

/*
 * Private data for class bHYPRE.StructStencil
 */

struct bHYPRE_StructStencil__data {
  /* DO-NOT-DELETE splicer.begin(bHYPRE.StructStencil._data) */
  /* Put private data members here... */
   HYPRE_StructStencil  stencil;
   int32_t dim;
   int32_t size;
  /* DO-NOT-DELETE splicer.end(bHYPRE.StructStencil._data) */
};

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Access functions for class private data and built-in methods
 */

extern struct bHYPRE_StructStencil__data*
bHYPRE_StructStencil__get_data(
  bHYPRE_StructStencil);

extern void
bHYPRE_StructStencil__set_data(
  bHYPRE_StructStencil,
  struct bHYPRE_StructStencil__data*);

extern
void
impl_bHYPRE_StructStencil__load(
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_bHYPRE_StructStencil__ctor(
  /* in */ bHYPRE_StructStencil self,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_bHYPRE_StructStencil__ctor2(
  /* in */ bHYPRE_StructStencil self,
  /* in */ void* private_data,
  /* out */ sidl_BaseInterface *_ex);

extern
void
impl_bHYPRE_StructStencil__dtor(
  /* in */ bHYPRE_StructStencil self,
  /* out */ sidl_BaseInterface *_ex);

/*
 * User-defined object methods
 */

extern
bHYPRE_StructStencil
impl_bHYPRE_StructStencil_Create(
  /* in */ int32_t ndim,
  /* in */ int32_t size,
  /* out */ sidl_BaseInterface *_ex);

extern struct bHYPRE_StructStencil__object* 
  impl_bHYPRE_StructStencil_fconnect_bHYPRE_StructStencil(const char* url,
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct bHYPRE_StructStencil__object* 
  impl_bHYPRE_StructStencil_fcast_bHYPRE_StructStencil(void* bi,
  sidl_BaseInterface* _ex);
extern struct sidl_BaseClass__object* 
  impl_bHYPRE_StructStencil_fconnect_sidl_BaseClass(const char* url,
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_BaseClass__object* 
  impl_bHYPRE_StructStencil_fcast_sidl_BaseClass(void* bi,
  sidl_BaseInterface* _ex);
extern struct sidl_BaseInterface__object* 
  impl_bHYPRE_StructStencil_fconnect_sidl_BaseInterface(const char* url,
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_BaseInterface__object* 
  impl_bHYPRE_StructStencil_fcast_sidl_BaseInterface(void* bi,
  sidl_BaseInterface* _ex);
extern struct sidl_ClassInfo__object* 
  impl_bHYPRE_StructStencil_fconnect_sidl_ClassInfo(const char* url,
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_ClassInfo__object* 
  impl_bHYPRE_StructStencil_fcast_sidl_ClassInfo(void* bi,
  sidl_BaseInterface* _ex);
extern struct sidl_RuntimeException__object* 
  impl_bHYPRE_StructStencil_fconnect_sidl_RuntimeException(const char* url,
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_RuntimeException__object* 
  impl_bHYPRE_StructStencil_fcast_sidl_RuntimeException(void* bi,
  sidl_BaseInterface* _ex);
extern
void
impl_bHYPRE_StructStencil_Destroy(
  /* in */ bHYPRE_StructStencil self,
  /* out */ sidl_BaseInterface *_ex);

extern
int32_t
impl_bHYPRE_StructStencil_SetDimension(
  /* in */ bHYPRE_StructStencil self,
  /* in */ int32_t dim,
  /* out */ sidl_BaseInterface *_ex);

extern
int32_t
impl_bHYPRE_StructStencil_SetSize(
  /* in */ bHYPRE_StructStencil self,
  /* in */ int32_t size,
  /* out */ sidl_BaseInterface *_ex);

extern
int32_t
impl_bHYPRE_StructStencil_SetElement(
  /* in */ bHYPRE_StructStencil self,
  /* in */ int32_t index,
  /* in rarray[dim] */ int32_t* offset,
  /* in */ int32_t dim,
  /* out */ sidl_BaseInterface *_ex);

extern struct bHYPRE_StructStencil__object* 
  impl_bHYPRE_StructStencil_fconnect_bHYPRE_StructStencil(const char* url,
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct bHYPRE_StructStencil__object* 
  impl_bHYPRE_StructStencil_fcast_bHYPRE_StructStencil(void* bi,
  sidl_BaseInterface* _ex);
extern struct sidl_BaseClass__object* 
  impl_bHYPRE_StructStencil_fconnect_sidl_BaseClass(const char* url,
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_BaseClass__object* 
  impl_bHYPRE_StructStencil_fcast_sidl_BaseClass(void* bi,
  sidl_BaseInterface* _ex);
extern struct sidl_BaseInterface__object* 
  impl_bHYPRE_StructStencil_fconnect_sidl_BaseInterface(const char* url,
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_BaseInterface__object* 
  impl_bHYPRE_StructStencil_fcast_sidl_BaseInterface(void* bi,
  sidl_BaseInterface* _ex);
extern struct sidl_ClassInfo__object* 
  impl_bHYPRE_StructStencil_fconnect_sidl_ClassInfo(const char* url,
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_ClassInfo__object* 
  impl_bHYPRE_StructStencil_fcast_sidl_ClassInfo(void* bi,
  sidl_BaseInterface* _ex);
extern struct sidl_RuntimeException__object* 
  impl_bHYPRE_StructStencil_fconnect_sidl_RuntimeException(const char* url,
  sidl_bool ar, sidl_BaseInterface *_ex);
extern struct sidl_RuntimeException__object* 
  impl_bHYPRE_StructStencil_fcast_sidl_RuntimeException(void* bi,
  sidl_BaseInterface* _ex);
#ifdef __cplusplus
}
#endif
#endif
