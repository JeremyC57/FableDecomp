#!/bin/bash
# runpair.sh <tag> [overlay]: vland --recheck-landed of work/<tag>h then work/<tag>n, BOTH compiled under the same LANDVERIFY_WORK path
# (work/wd_cur, renamed to wd_<tag>h / wd_<tag>n after each run) so ?A0x anonymous-namespace hashes agree (GOTCHAS 315).
# -> work/recheck_<tag>h.log, work/recheck_<tag>n.log, work/rp_<tag>.done. Each vland has its own `timeout 900`.
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF323; W=$A/work; cd $W
OVL=$2; HOVL=$3   # runpair2: HEAD side on the frozen old tree <3>, new side on <2>
P=/home/user/FableDecomp/tools/decomp_pipeline:/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools
[ -e $W/wd_cur ] && { echo "wd_cur exists" > $W/rp_$1.done; exit 1; }
FABLE_HDR_OVERLAY=$HOVL PYTHONPATH=$P LANDVERIFY_WORK=$W/wd_cur timeout 900 python3 -u /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/vland.py $W/$1h_payload.json $W/$1h_oracle.tsv --recheck-landed > $W/recheck_$1h.log 2>&1
mv $W/wd_cur $W/wd_$1h
export FABLE_HDR_OVERLAY=$OVL
PYTHONPATH=$P LANDVERIFY_WORK=$W/wd_cur timeout 900 python3 -u /home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/vland.py $W/$1n_payload.json $W/$1n_oracle.tsv --recheck-landed > $W/recheck_$1n.log 2>&1
mv $W/wd_cur $W/wd_$1n
echo done > $W/rp_$1.done
