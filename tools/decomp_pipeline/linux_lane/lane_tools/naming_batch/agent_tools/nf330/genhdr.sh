#!/usr/bin/env bash
# genhdr.sh <family> <class list file> (NF329): per class, keep the repo original in orig/hdr_<X>.h, run genmodel.py (NF325 style) with
# the family's base options into hdr/engine/<X>.h, pin the moved asserts (lane_merge/pin_asserts.py), copy into work/tree. Repo root.
# family CA: base CCombatActionBase (retail 0x114, the FABLE_RETAIL_KRAKEN_ROAR branch NF323 proved 81-slot)
# family CS: base CCombatSequenceBase (0x28, NF325's widened model under FABLE_RETAIL_CCOMBATSEQUENCEBASE)
set -u
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF330
fam=$1; list=$2
case $fam in
  CA) OPTS=(--base CCombatActionBase --basehdr engine/CCombatActionBase.h --basesize 0x114 --needs FABLE_RETAIL_BASE_OBJECT,FABLE_RETAIL_DEF_CLASS_BASE,FABLE_RETAIL_CCREATUREACTIONBASE,FABLE_RETAIL_KRAKEN_ROAR --prefwd "class CSoundPair; class CAnimationEntry");;
  CS) OPTS=(--base CCombatSequenceBase --basehdr engine/CCombatSequenceBase.h --basesize 0x28 --needs FABLE_RETAIL_CCOMBATSEQUENCEBASE);;
  *) echo "bad family"; exit 2;;
esac
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
