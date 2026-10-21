#!/bin/bash
# rt.sh <addr>...: build test from s7/t/<addr>.cpp tail and run it; exit 1 if any fails
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; cd /home/user/FableDecomp
W=$S/w22/retry10; rc=0
for a in "$@"; do
python3 $W/s7/mkt.py $a $W/s7/t/$a.cpp
r=$(python3 $W/tall.py $a | tail -1); echo "$r"
case "$r" in *PASS*) ;; *) rc=1; python3 $W/var/trun.py $W/test/$a.cpp 2>&1 | grep -v "^$" | tail -15;; esac
done
exit $rc
