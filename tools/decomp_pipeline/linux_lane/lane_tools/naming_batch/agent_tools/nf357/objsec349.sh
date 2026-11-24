#!/usr/bin/env bash
# objsec349.sh <addr...>: objsec346 HEAD v new (evidence/objsec/<a>.txt) and REREAD-only v new (evidence/objsec_RN/<a>.txt; pk files of R
# copied as H_ into evidence/pkRN) for each row. Leaf from the landed source.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF357
export S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S; unset TMPDIR TMP
mkdir -p $A/evidence/objsec $A/evidence/objsec_RN $A/evidence/pkRN
for a in "$@"; do
  leaf=$(python3 -c "import sys; sys.path.insert(0,'$A/tools'); import nf; print(nf.leaf_of(nf.src_of('$a').read_text(encoding='utf-8'),'$a'))")
  python3 $A/tools/objsec346.py $A/work/obj/H_$a.obj $A/work/obj/$a.obj $A/evidence/pk $a "$leaf" > $A/evidence/objsec/$a.txt 2>&1
  for e in pkall.tsv pvr.txt; do cp $A/evidence/pk/R_$a.$e $A/evidence/pkRN/H_$a.$e; cp $A/evidence/pk/N_$a.$e $A/evidence/pkRN/N_$a.$e; done
  python3 $A/tools/objsec346.py $A/work/obj/${a}_r.obj $A/work/obj/$a.obj $A/evidence/pkRN $a "$leaf" > $A/evidence/objsec_RN/$a.txt 2>&1
  head -1 $A/evidence/objsec/$a.txt; echo "  RN: $(head -1 $A/evidence/objsec_RN/$a.txt)"
done
