#!/usr/bin/env bash
# census.sh <tag> <rowlist>: HEAD recheck payload for the rows, vland --recheck-landed, then per-row (symbol, target) pairs.
set -u
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; A=$S/w22/agentNF430; t=$1; L=$2
source $S/wine/env.sh; export S; unset TMPDIR TMP
cd $A/work/rsroot; LT=tools/decomp_pipeline/linux_lane/lane_tools
mkdir -p $A/work/p_$t; python3 $LT/recheck.py $A/work/p_$t $(cat $L) > $A/work/p_$t.log 2>&1
timeout 5400 env LANDVERIFY_WORK=$A/work/wd_$t python3 -u $LT/vland.py $A/work/p_$t/payload.json $A/work/p_$t/oracle.tsv --recheck-landed > $A/work/vland_$t.log 2>&1
PAIRS_OUT=$A/work/pairs_$t.json python3 $S/w22/nftools/pairs.py $A/work/p_$t/payload.json $A/work/p_$t/oracle.tsv $A/work/wd_$t > $A/work/pairs_$t.log 2>&1
echo DONE > $A/work/census_$t.done
