#!/usr/bin/env bash
# gate361.sh <tag> <addr...>: stage work/pk/H_<a>.* and work/pk/<tag>_<a>.* as work/gate/<tag>/{H,N}_<a>.*, then pkcmp346 (both checkers,
# HEAD v variant), its reversed control, and the reach-flip scan (NEQ copies whose reach goes n -> y). Output work/gate/<tag>/{pk_gate,pk_gate_rev,reachflip}.txt
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF365
t=$1; shift; D=$A/work/gate/$t; mkdir -p $D
for a in "$@"; do for e in pkall.tsv pvr.txt; do cp $A/work/pk/H_$a.$e $D/H_$a.$e; cp $A/work/pk/${t}_$a.$e $D/N_$a.$e; done; done
python3 $A/tools/pkcmp346.py $D "$@" > $D/pk_gate.txt
PKCMP_REVERSE=1 python3 $A/tools/pkcmp346.py $D "$@" > $D/pk_gate_rev.txt
python3 $A/tools/reachflip_coordinator.py $D > $D/reachflip.txt
python3 $S/tools/reachflip.py $D > $D/reachflip_S.txt 2>&1
