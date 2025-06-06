#!/bin/bash
# Copyright (c) 1998 Lawrence Livermore National Security, LLC and other
# HYPRE Project Developers. See the top-level COPYRIGHT file for details.
#
# SPDX-License-Identifier: (Apache-2.0 OR MIT)

TNAME=`basename $0 .sh`
RTOL=$1
ATOL=$2

#=============================================================================
# compare with baseline case
#=============================================================================

FILES="\
 ${TNAME}.out.0\
 ${TNAME}.out.1\
 ${TNAME}.out.2\
 ${TNAME}.out.3\
 ${TNAME}.out.4\
"

for i in $FILES
do
  echo "# Output file: $i"
  tail -1 $i
done > ${TNAME}.out

# Make sure that the output file is reasonable
RUNCOUNT=`echo $FILES | wc -w`
OUTCOUNT=`grep "error code" ${TNAME}.out | wc -l`
if [ "$OUTCOUNT" != "$RUNCOUNT" ]; then
  echo "Incorrect number of runs in ${TNAME}.out" >&2
fi

# Make sure that the output file has no errors
for i in $FILES
do
  echo "# Output file: $i"
  echo "Test error code = 0"
done > ${TNAME}.testdata

diff ${TNAME}.testdata ${TNAME}.out >&2

#=============================================================================
# remove temporary files
#=============================================================================

rm -f ${TNAME}.testdata
