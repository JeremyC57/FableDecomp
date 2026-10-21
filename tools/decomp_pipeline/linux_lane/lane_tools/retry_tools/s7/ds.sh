#!/bin/bash
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
python3 $S/w22/retry10/var/defsweep.py $(cat $S/w22/retry10/s7/def_cands.txt) > $S/w22/retry10/s7/defsweep_s7.log 2>&1
echo DONE >> $S/w22/retry10/s7/defsweep_s7.log
