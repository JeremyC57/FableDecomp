#!/bin/bash
# vland_chain.sh <tag:payload:oracle>... : vland --recheck-landed per payload, sequentially, timeout 900 each; logs -> evidence/vland_<tag>.log (agent NF354, from NF350's)
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF354
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
cd /home/user/FableDecomp
for x in "$@"; do
  t=${x%%:*}; r=${x#*:}; p=${r%%:*}; o=${r#*:}
  LANDVERIFY_WORK=$A/work/vl_$t timeout 900 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $p $o --recheck-landed > $A/evidence/vland_$t.log 2>&1
  echo "VLAND_DONE $t rc=$?" >> $A/work/vland_chain.log
done
