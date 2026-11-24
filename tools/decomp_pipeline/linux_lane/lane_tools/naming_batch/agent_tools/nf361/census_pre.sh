#!/usr/bin/env bash
# census_pre.sh <cand.tsv>: per census row: clang precheck of the landed test (before the lock), claim the NF361 lock, clang precheck of
# the landed source (after the lock). Writes work/census_pre.tsv: addr test_pre lock src_pre. VC7.1 /Zs fallbacks run separately (D1).
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF361
L=/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/lock.sh
export NF_TREE=$A/work/tree
while IFS=$'\t' read a t s; do
  tp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.test_of('$a'))")
  tr=$(python3 $A/tools/nf.py pre CT_$a $tp nowchar | awk '{print $3}')
  if [ "$tr" = PASS ]; then lk=$(bash $L NF361 $a | awk '{print $1}'); else lk=NOLOCK_TEST_FAIL; fi
  sr=-
  if [ "$lk" = OK ]; then sr=$(python3 $A/tools/nf.py pre CS_$a /home/user/FableDecomp/rebuild/src/compiled/$s nowchar | awk '{print $3}'); fi
  echo -e "$a\t$tr\t$lk\t$sr"
done < "$1" > $A/work/census_pre.tsv
