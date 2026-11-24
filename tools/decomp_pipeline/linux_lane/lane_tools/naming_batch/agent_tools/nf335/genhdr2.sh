#!/usr/bin/env bash
# genhdr2.sh <family> <class list file> (NF330): genhdr.sh (NF329) with the family's base options read from tools/families.tsv
# (family base basehdr basesize needs prefwd [preinc]). Per class: orig copy, genmodel.py into hdr/engine/<X>.h, pin_asserts, copy to work/tree.
set -u
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF335
fam=$1; list=$2
line=$(awk -F'\t' -v f="$fam" '$1==f' $A/tools/families.tsv)
[ -z "$line" ] && { echo "bad family"; exit 2; }
IFS=$'\t' read -r _ base basehdr basesize needs prefwd preinc <<< "$line"
OPTS=(--base "$base" --basehdr "$basehdr" --basesize "$basesize")
[ "$needs" != "-" ] && OPTS+=(--needs "$needs")
[ "$prefwd" != "-" ] && OPTS+=(--prefwd "$prefwd")
[ -n "${preinc:-}" ] && OPTS+=(--preinc "$preinc")
while read -r X; do
  [ -z "$X" ] && continue
  h=rebuild/include/engine/$X.h
  [ -f "$h" ] || { echo "$X NOHDR"; continue; }
  [ -f $A/orig/hdr_$X.h ] || cp "$h" $A/orig/hdr_$X.h
  cmp -s "$h" $A/orig/hdr_$X.h || { echo "$X STALE_ORIG"; continue; }
  out=$(python3 $A/tools/genmodel.py "$X" "${OPTS[@]}" --src $A/orig/hdr_$X.h 2>&1 | tail -1)
  echo "$out"
  case "$out" in *" OK "*) ;; *) continue;; esac
  python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py rebuild/include $A/hdr engine/$X.h | grep -E "pinned [0-9]|unusual" | grep -v "^total" | sed "s/^/  pin: /"
  cp $A/hdr/engine/$X.h $A/work/tree/engine/$X.h
done < "$list"
