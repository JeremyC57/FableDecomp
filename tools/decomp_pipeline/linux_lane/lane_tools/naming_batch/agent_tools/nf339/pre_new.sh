#!/bin/bash
# pre_new.sh <changed list> <controls list> <donefile>: clang prechecks: changed rows' new source (N_) + test (NT_ if rows/<a>_test.cpp) + HEAD source (H_) + landed test (T_); controls' HEAD source + test
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
A=$S/w22/agentNF339
for a in $(cat $1); do
  timeout 120 python3 $A/tools/nf.py pre N_$a $A/rows/$a.cpp > /dev/null
  [ -f $A/rows/${a}_test.cpp ] && timeout 120 python3 $A/tools/nf.py pre NT_$a $A/rows/${a}_test.cpp > /dev/null
done
cat $1 $2 > $A/work/.pre_h.$$; $A/tools/pre_rows.sh $A/work/.pre_h.$$ $3
