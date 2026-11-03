#!/bin/bash
# sibrun.sh <batch-index>: vland --recheck-landed one sibling batch on the full mirror
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
D=$S/w22/chunk733; cd /home/user/FableDecomp
LANDVERIFY_WORK=$D/lv/sib$1 FABLE_HDR_OVERLAY=$D/mirror timeout 115 python3 tools/decomp_pipeline/linux_lane/lane_tools/vland.py $D/sib/b$1.json $D/sib/oracle.tsv --recheck-landed > $D/sib/out_b$1.txt 2>&1; echo "b$1 rc=$?"
grep -E "^[0-9a-f]{8} +(MATCH|RELOCATION_MATCH|DIFFER|SRC_FAIL|[A-Z_]+)" $D/sib/out_b$1.txt | grep -v WARN
