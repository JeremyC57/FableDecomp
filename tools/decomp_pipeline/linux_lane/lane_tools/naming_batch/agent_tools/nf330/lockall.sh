#!/usr/bin/env bash
# lockall.sh <claim|release> <file of addresses>: run the repo's naming_batch/lock.sh for NF330 on each address; prints only non-OK lines
# and a count. Needs S exported.
set -u
L=/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/lock.sh
mode=$1; n=0; bad=0
while read -r a; do [ -z "$a" ] && continue; n=$((n+1))
  if [ "$mode" = release ]; then r=$(bash $L NF330 "$a" release); else r=$(bash $L NF330 "$a"); fi
  case "$r" in OK*|RELEASED*) ;; *) echo "$r"; bad=$((bad+1));; esac
done < "$2"
echo "lockall $mode: $n addresses, $bad not OK"
