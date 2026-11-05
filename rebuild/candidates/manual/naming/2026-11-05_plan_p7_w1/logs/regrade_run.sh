export S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad; source $S/wine/env.sh; export S; cd /home/user/FableDecomp
run(){ LANDVERIFY_WORK=$S/rg7/lv$1 timeout 7000 python3 -u tools/decomp_pipeline/linux_lane/lane_tools/vland.py $S/rg7/r$1/payload.json $S/rg7/r$1/oracle.tsv --recheck-landed > $S/rg7/vland$1.log 2>&1; rm -rf $S/rg7/lv$1; }
(run 00; run 02; run 04; run 06) & (run 01; run 03; run 05; run 07) & wait; echo REGRADE_DONE
