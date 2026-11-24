#!/usr/bin/env bash
# locscan.sh <addr...>: for HEAD objects of rows with a same-TU GetSubDef body: every DEF symbol naming GetSubDef (PDB spelling or stand-in),
# and pk_all / pvr verdicts of the four PDB copies when defined. Output work/loc_scan.tsv lines: addr kind symbol verdict(pk_all) pvr size
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF357
cd $A/work
for a in "$@"; do
  python3 $A/tools/nf.py syms obj/H_$a.obj | grep "^DEF .*GetSubDef" | while read k s; do
    if echo "$s" | grep -qE '^\?\?\$GetSubDef@V(CInventoryItemDef|CWeaponDef|CEntitySoundDef|CAppearanceDef)@@@CDefClassBase@@QBE_NPAV'; then
      [ -f pkc/H_$a.pkall.tsv ] || python3 $A/tools/pkrun352.py $A/work/pkc $a H=$A/work/obj/H_$a.obj > /dev/null
      v=$(awk -F'\t' -v s="$s" '$10==s{print $1"\t"$5"\t"$6}' pkc/H_$a.pkall.tsv | head -1); pv=$(grep -F " $s" pkc/H_$a.pvr.txt | awk '{print $1}' | head -1)
      echo -e "$a\tPDB\t$s\t$v\tpvr=$pv"
    else echo -e "$a\tOTHER\t${s:0:120}"; fi
  done
done
