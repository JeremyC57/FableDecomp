#!/usr/bin/env bash
# genhdr429.sh <family> <class list> (NF429; NF427 genhdr427.sh repointed to the AI state-group family): per class, keep the repo original in
# orig/hdr_<X>.h, run genmodel.py with the family's base options into hdr/engine/<X>.h, pin the moved asserts (lane_merge/pin_asserts.py).
#   AB       base CAIStateGroupBase (its FABLE_RETAIL_CAISTATEGROUPBASE + _MEMBERS polymorphic branch), sizeof 0x14
#   HB:<B>:<size>:<needs>  base <B> is a hand-written shared polymorphic header (no flat model), its switches given
#   CH:<B>   base <B>'s VIRTUAL_MODEL: a model made here (hdr/engine/<B>.h, its Needs line) or a landed NF322/NF325/NF330 model in the repo
#            header (no Needs line there: the chain's switches are the CAIStateGroupBase pair plus every VIRTUAL_MODEL macro up the chain)
# Run from the repo root.
set -u
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF429
fam=$1; list=$2
BASEN=FABLE_RETAIL_CAISTATEGROUPBASE,FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
case $fam in
  AB) OPTS=(--base CAIStateGroupBase --basehdr engine/CAIStateGroupBase.h --basesize 0x14 --needs $BASEN);;
  CH:*) B=${fam#CH:}
        if [ -f $A/hdr/engine/$B.h ]; then BH=$A/hdr/engine/$B.h; else BH=rebuild/include/engine/$B.h; fi
        grep -q "FABLE_RETAIL_${B^^}_VIRTUAL_MODEL" $BH || { echo "$B BASE_HAS_NO_MODEL"; exit 1; }
        BS=$(sed -n "/#ifdef FABLE_RETAIL_${B^^}_VIRTUAL_MODEL/,/#else/p" $BH | grep -oP "FABLE_STATIC_ASSERT\(sizeof\($B\) == \K0x[0-9a-fA-F]+" | head -1)
        BN=$(sed -n "/#ifdef FABLE_RETAIL_${B^^}_VIRTUAL_MODEL/,/#else/p" $BH | grep -m1 -oP "// Needs \(defined by the including row, above its first #include\): \K[^.]*" | tr -d ' ')
        if [ -z "$BN" ]; then
          # landed model with no Needs line: walk its `class B : public P` chain up to CAIStateGroupBase
          BN=$BASEN; P=$B
          while :; do
            PH=rebuild/include/engine/$P.h; [ -f $A/hdr/engine/$P.h ] && PH=$A/hdr/engine/$P.h
            PP=$(sed -n "/#ifdef FABLE_RETAIL_${P^^}_VIRTUAL_MODEL/,/#else/p" $PH | grep -m1 -oP "^class $P : public \K\w+")
            [ -z "$PP" ] && { echo "$X CHAIN_BROKEN at $P"; exit 1; }
            [ "$PP" = CAIStateGroupBase ] && break
            BN=$BN,FABLE_RETAIL_${PP^^}_VIRTUAL_MODEL; P=$PP
          done
        fi
        OPTS=(--base $B --basehdr engine/$B.h --basesize $BS --needs $BN,FABLE_RETAIL_${B^^}_VIRTUAL_MODEL);;
  HB:*) # a hand-written shared polymorphic base header: HB:<B>:<sizeof hex>:<needs,...> (the base's switches as its landed rows define them)
        IFS=: read -r _ B BS BN <<< "$fam"
        OPTS=(--base $B --basehdr engine/$B.h --basesize $BS --needs $BN);;
  *) echo "bad family"; exit 2;;
esac
while read -r X; do
  [ -z "$X" ] && continue
  h=rebuild/include/engine/$X.h
  [ -f "$h" ] || { echo "$X NOHDR"; continue; }
  grep -q "FABLE_RETAIL_${X^^}_VIRTUAL_MODEL" "$h" && { echo "$X HAS_MODEL"; continue; }
  [ -f $A/orig/hdr_$X.h ] || cp "$h" $A/orig/hdr_$X.h
  cmp -s "$h" $A/orig/hdr_$X.h || { echo "$X STALE_ORIG"; continue; }
  PU=$(python3 $A/tools/pure429.py "$X" | cut -d' ' -f2-)
  case "$PU" in *MISSING*) echo "$X PURE_MISSING $PU"; continue;; esac
  EX=(); [ -f $A/work/extra_$X.txt ] && while read -r m; do EX+=(--extra "$m"); done < $A/work/extra_$X.txt   # non-virtual members rows call
  out=$(python3 $A/tools/genmodel.py "$X" "${OPTS[@]}" $PU "${EX[@]}" --src $A/orig/hdr_$X.h 2>&1 | tail -1)
  echo "$out"
  case "$out" in *" OK "*) ;; *) continue;; esac
  python3 tools/decomp_pipeline/linux_lane/lane_tools/lane_merge/pin_asserts.py rebuild/include $A/hdr engine/$X.h | grep -E "pinned [0-9]|unusual" | grep -v "^total" | sed "s/^/  pin: /"
done < "$list"
