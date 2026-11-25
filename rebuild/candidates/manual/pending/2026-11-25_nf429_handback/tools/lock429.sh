#!/usr/bin/env bash
# lock429.sh lock|release <addr...|file>: NF429 row locks under $S/w22/nflocks/<addr>/owner (lock only when missing or `released`).
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; op=$1; shift
L="$@"; [ $# -eq 1 ] && [ -f "$1" ] && L=$(cat "$1")
for a in $L; do
  f=$S/w22/nflocks/$a/owner; o=$(cat $f 2>/dev/null || echo released)
  if [ $op = lock ]; then
    if [ "$o" = released ] || [ "$o" = NF429 ]; then mkdir -p $S/w22/nflocks/$a; echo NF429 > $f; echo "LOCKED $a"; else echo "BUSY $a $o"; fi
  else
    if [ "$o" = NF429 ]; then echo released > $f; echo "RELEASED $a"; else echo "NOT_OWNER $a $o"; fi
  fi
done
