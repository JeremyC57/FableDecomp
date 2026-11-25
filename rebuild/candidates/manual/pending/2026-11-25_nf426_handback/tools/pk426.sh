#!/usr/bin/env bash
# pk426.sh <batch>: pk_all on every row object, HEAD (old leaf) and new (new leaf), with --addrs=work/newaddrs.tsv (the batch names'
# own addresses, authoritative). Output <batch>/evidence/pk_<side>.tsv (address then pk_all's --tsv lines).
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF426; B=$A/batches/$1
cd $A/work/rsroot; PK=tools/decomp_pipeline/linux_lane/lane_tools/pk_all/pk_all.py
: > $B/evidence/pk_H.tsv; : > $B/evidence/pk_N.tsv
while IFS=$'\t' read a mem t old new obj sh; do
  python3 -I -B $PK $B/obj/H_$a.obj $a --leaf="$old" --addrs=$A/work/newaddrs.tsv --tsv 2>&1 | sed "s/^/$a\t/" >> $B/evidence/pk_H.tsv
  python3 -I -B $PK $B/obj/$a.obj $a --leaf="$new" --addrs=$A/work/newaddrs.tsv --tsv 2>&1 | sed "s/^/$a\t/" >> $B/evidence/pk_N.tsv
done < $B/PLAN.tsv
echo PK_DONE $1
