#!/usr/bin/env bash
# lock_pre_src.sh: phase C. For every row whose test passed clang (pre_tests.tsv PASS) or VC7.1 /Zs (pre_zs.log ZS_PASS ZT_<a>), claim the
# NF358 lock (skipping a lock held by anyone else), then clang-precheck the landed source. Writes work/lock_pre.tsv: addr testpre lock srcpre.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF358
L=/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/lock.sh
cd $A/work
while IFS=$'\t' read a r tp; do
  [ "$a" = PRE_TESTS_DONE ] && continue
  t=$r; if [ "$r" = FAIL ]; then grep -q "ZS_PASS ZT_$a " pre_zs.log && t=ZS_PASS || t=ZS_FAIL; fi
  if [ "$t" = PASS ] || [ "$t" = ZS_PASS ]; then lk=$(bash $L NF358 $a | tr ' ' ':'); else lk=NOLOCK_TEST_FAIL; fi
  sr=-
  if [ "${lk%%:*}" = OK ]; then sp=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.src_of('$a'))"); sr=$(python3 $A/tools/nf.py pre CS_$a $sp nowchar | awk '{print $3}'); fi
  echo -e "$a\t$t\t$lk\t$sr"
done < pre_tests.tsv > lock_pre.tsv
echo LOCK_PRE_DONE >> lock_pre.tsv
