#!/usr/bin/env bash
# eval357.sh <addr> <objtag>: H v variant object: objsec352 verdict, leafrel, pk (both checkers) on the variant, copy verdicts of the 4 types.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF357
cd $A/work; a=$1; t=$2; o=obj/${a}_$t.obj
leaf=$(head -1 pkc/H_$a.pkall.tsv | awk '{print $3}')
os=$(python3 $A/tools/objsec352.py obj/H_$a.obj $o "$leaf" | tail -1)
nch=$(python3 $A/tools/objsec352.py obj/H_$a.obj $o "$leaf" | grep -c "^CHANGED\|^ADDED\|^DROPPED")
chg=$(python3 $A/tools/objsec352.py obj/H_$a.obj $o "$leaf" | grep "^CHANGED\|^ADDED\|^DROPPED" | grep -o "GetSubDef@V[A-Za-z]*\|GetDef@V[A-Za-z]*" | tr '\n' ',')
lr=$(python3 $A/tools/leafrel.py obj/H_$a.obj $o "$leaf" | head -1)
python3 $A/tools/pkrun352.py $A/work/pkv $a $t=$A/work/$o > /dev/null
cp pkc/H_$a.pkall.tsv pkc/H_$a.pvr.txt pkv/ 2>/dev/null
v=$(awk -F'\t' '$10 ~ /^\?\?\$GetSubDef@V(CInventoryItemDef|CWeaponDef|CEntitySoundDef|CAppearanceDef)@@@CDefClassBase/ {split($10,x,"@@@"); printf "%s:%s:%s ", substr(x[1],14), $1, $6}' pkv/${t}_$a.pkall.tsv)
echo -e "$a\t$t\t$os\tnchanged=$nch\t$chg\t$lr\t$v"
