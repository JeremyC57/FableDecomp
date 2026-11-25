#!/usr/bin/env bash
# release423.sh <batch...>: keep NF439 locks only on the ACCEPT rows of the given batches; release every other NF439 lock
W=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF439; L=$(dirname $W)/nflocks
cat $(for b in "$@"; do echo $W/batches/$b/rows.txt; done) | sort -u > $W/work/accept_rows.txt
n=0; for d in $L/*/; do a=$(basename $d); if [ "$(cat $d/owner 2>/dev/null)" = NF439 ] && ! grep -qx $a $W/work/accept_rows.txt; then echo released > $d/owner; n=$((n+1)); fi; done; echo released $n
