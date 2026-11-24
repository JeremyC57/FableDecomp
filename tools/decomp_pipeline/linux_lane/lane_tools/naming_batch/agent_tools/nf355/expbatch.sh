#!/bin/bash
# expbatch.sh <addr> <overlay> <tag>...: sequential exp.py runs of work/exp/<tag>.cpp as row <addr> (each with its own timeout 900) (NF355)
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF355; a=$1; ov=$2; shift 2
for t in "$@"; do
  FABLE_HDR_OVERLAY=$ov timeout 900 python3 $A/tools/exp.py $a $A/work/exp/$t.cpp $t
done
echo EXPBATCH_DONE
