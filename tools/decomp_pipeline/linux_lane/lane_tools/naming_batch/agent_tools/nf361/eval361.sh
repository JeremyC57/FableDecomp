#!/usr/bin/env bash
# eval361.sh <addr> <objtag>: H v variant object (objtag N = work/obj/<a>.obj, else work/obj/<a>_<tag>.obj): objsec352 verdict + the
# changed / added / dropped section keys, leafrel, both pick-any checkers (pkrun352 -> work/pk/<tag>_<a>.*), the Reset / ReleaseData /
# GetDef<CUIMiscThingsDef> / GetDef<CThingObjectDef> verdicts (pk_all: verdict size/retail leafref reach), and cmpcomdat of each such COMDAT
# against the owner object (work/owner_ref/*). Needs FABLE_EXE / FABLE_ROOT.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF361
cd $A/work; a=$1; t=$2; if [ "$t" = N ]; then o=obj/$a.obj; else o=obj/${a}_$t.obj; fi
[ -f pk/H_$a.pkall.tsv ] || python3 $A/tools/pkrun352.py $A/work/pk $a H=$A/work/obj/H_$a.obj > /dev/null
python3 $A/tools/pkrun352.py $A/work/pk $a $t=$A/work/$o > /dev/null
leaf=$(head -1 pk/H_$a.pkall.tsv | awk '{print $3}')
mkdir -p ev/objsec ev/leafrel; python3 $A/tools/objsec352.py obj/H_$a.obj $o "$leaf" > ev/objsec/${a}_$t.txt; python3 $A/tools/leafrel.py obj/H_$a.obj $o "$leaf" > ev/leafrel/${a}_$t.txt
os=$(tail -1 ev/objsec/${a}_$t.txt); chg=$(grep "^CHANGED\|^ADDED\|^DROPPED\|^SYM" ev/objsec/${a}_$t.txt | cut -c1-90 | tr '\n' ';'); lr=$(head -1 ev/leafrel/${a}_$t.txt)
v(){ awk -F'\t' -v s="$2" 'index($10,s)==1 {printf "%s:%s/%s:l%s:r%s ", $1, $6, $7, $8, $9}' pk/$1_$a.pkall.tsv; }
RS='?Reset@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXPAVCComponent@NUISystem@@@Z'; RD='?ReleaseData@?$CCountedPointer@VCComponent@NUISystem@@@@QAEXXZ'
GU='??$GetDef@VCUIMiscThingsDef@NUISystem@@@CDefinitionManager@@'; GT='??$GetDef@VCThingObjectDef@@@CDefinitionManager@@'
cmp(){ r=$(python3 $A/tools/cmpcomdat.py "$1" "$2" $o | tail -1 | awk '{print $1, $2, $3}'); echo "$r"; }
gsym(){ python3 $A/tools/nf.py syms $o | awk -v p="$1" '$1=="DEF" && index($2,p)==1 {print $2}' | head -1; }
gu=$(gsym "$GU"); gt=$(gsym "$GT")
echo -e "$a\t$t\t$os\tH[Reset=$(v H "$RS")RD=$(v H "$RD")GU=$(v H "$GU")GT=$(v H "$GT")]\t$t[Reset=$(v $t "$RS")RD=$(v $t "$RD")GU=$(v $t "$GU")GT=$(v $t "$GT")]\t$lr\townerRS=$(cmp "$RS" owner_ref/nf362_H_00429c6f.obj)|ownerRD=$(cmp "$RD" obj/H_004291de.obj)|ownerGU=$( [ -n "$gu" ] && cmp "$gu" owner_ref/nf362_H_0042b312.obj)|ownerGT=$( [ -n "$gt" ] && cmp "$gt" owner_ref/nf362_H_0042b040.obj)\t$chg"
