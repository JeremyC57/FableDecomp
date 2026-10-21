#!/bin/bash
# rsw.sh <listfile> <tag>: refsweep -> tagsweep2 -> rdsweep over the list (logs in s7/)
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
W=$S/w22/retry10; L=$(cat $1)
python3 $W/var/refsweep.py $L > $W/s7/refsweep_$2.log 2>&1
python3 $W/var/tagsweep2.py $L > $W/s7/tagsweep2_$2.log 2>&1
python3 $W/var/rdsweep.py $L > $W/s7/rdsweep_$2.log 2>&1
echo DONE >> $W/s7/rdsweep_$2.log
