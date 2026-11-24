#!/usr/bin/env bash
# gate365.sh <dest> <addr...>: gate361.sh with a named destination: stage work/pk/H_<a>.* and work/pk/g_<a>.* (the swap objects) as
# work/gate/<dest>/{H,N}_<a>.*, then pkcmp346 (both checkers, HEAD v new), the reversed control, and both reach-flip scans.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF365
t=$1; shift; D=$A/work/gate/$t; mkdir -p $D
for a in "$@"; do for e in pkall.tsv pvr.txt; do cp $A/work/pk/H_$a.$e $D/H_$a.$e; cp $A/work/pk/g_$a.$e $D/N_$a.$e; done; done
python3 $A/tools/pkcmp346.py $D "$@" > $D/pk_gate.txt
PKCMP_REVERSE=1 python3 $A/tools/pkcmp346.py $D "$@" > $D/pk_gate_rev.txt
python3 $A/tools/reachflip_coordinator.py $D > $D/reachflip.txt
python3 $S/tools/reachflip.py $D > $D/reachflip_S.txt 2>&1
