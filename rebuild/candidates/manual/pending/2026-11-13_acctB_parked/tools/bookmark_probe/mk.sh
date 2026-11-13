#!/bin/bash
# mk.sh <base.cpp> <out.cpp> DEF1 DEF2 ...: copy base with extra #defines after OUTLINE_INLINE
b=$1; o=$2; shift 2; add=""
for d in "$@"; do add="$add\\n#define FABLE_RETAIL_$d"; done
sed "s/^#define FABLE_RETAIL_CRENDERSTATEMANAGER_OUTLINE_INLINE\$/&$add/" $b > $o
