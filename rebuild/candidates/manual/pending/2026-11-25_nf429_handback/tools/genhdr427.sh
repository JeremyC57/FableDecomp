#!/usr/bin/env bash
# genhdr427.sh <family> <class list> (NF427; NF329 genhdr.sh repointed): per class, keep the repo original in orig/hdr_<X>.h, run genmodel.py
# with the family's base options into hdr/engine/<X>.h, pin the moved asserts (lane_merge/pin_asserts.py). Repo root.
set -u
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF429
fam=$1; list=$2
case $fam in
  CA) OPTS=(--base CCombatActionBase --basehdr engine/CCombatActionBase.h --basesize 0x114 --needs FABLE_RETAIL_BASE_OBJECT,FABLE_RETAIL_DEF_CLASS_BASE,FABLE_RETAIL_CCREATUREACTIONBASE,FABLE_RETAIL_KRAKEN_ROAR --prefwd "class CSoundPair; class CAnimationEntry");;
  CB) OPTS=(--base CCreatureActionBase --basehdr engine/CCreatureActionBase.h --basesize 0xa8 --needs FABLE_RETAIL_BASE_OBJECT,FABLE_RETAIL_CCREATUREACTIONBASE);;
  CS) OPTS=(--base CCombatSequenceBase --basehdr engine/CCombatSequenceBase.h --basesize 0x28 --needs FABLE_RETAIL_CCOMBATSEQUENCEBASE);;
  CH:*) B=${fam#CH:}; BS=$(grep -oP "FABLE_STATIC_ASSERT\(sizeof\($B\) == \K0x[0-9a-fA-F]+" $A/hdr/engine/$B.h | head -1)
        BN=$(grep -m1 -oP "// Needs \(defined by the including row, above its first #include\): \K[^.]*" $A/hdr/engine/$B.h | tr -d ' ')
        OPTS=(--base $B --basehdr engine/$B.h --basesize $BS --needs $BN,FABLE_RETAIL_$(echo $B | tr a-z A-Z)_VIRTUAL_MODEL --prefwd "class CSoundPair; class CAnimationEntry");;
  *) echo "bad family"; exit 2;;
esac
while read -r X; do
  [ -z "$X" ] && continue
  h=rebuild/include/engine/$X.h
  [ -f "$h" ] || { echo "$X NOHDR"; continue; }
  grep -q "FABLE_RETAIL_${X^^}_VIRTUAL_MODEL" "$h" && { echo "$X HAS_MODEL"; continue; }
  [ -f $A/orig/hdr_$X.h ] || cp "$h" $A/orig/hdr_$X.h
  cmp -s "$h" $A/orig/hdr_$X.h || { echo "$X STALE_ORIG"; continue; }
  out=$(python3 $A/tools/genmodel.py "$X" "${OPTS[@]}" --src $A/orig/hdr_$X.h 2>&1 | tail -1)
  echo "$out"
  case "$out" in *" OK "*) ;; *) continue;; esac
  python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py rebuild/include $A/hdr engine/$X.h | grep -E "pinned [0-9]|unusual" | grep -v "^total" | sed "s/^/  pin: /"
done < "$list"
