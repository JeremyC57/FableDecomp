#!/usr/bin/env bash
# eval365.sh <addr>: H v g (work/obj/H_<a>.obj v work/obj/<a>_g.obj): objsec352 verdict + changed/added/dropped keys, leafrel on the
# row's leaf, both pick-any checkers (pkrun352 -> work/pk/{H,g}_<a>.*), every GetDef<T> DEF in g compared (cmpcomdat: bytes + relocs)
# with the owner object of T (my own read-only HEAD builds: 0x0042B312, 0x0042B040, 0x00817B40), and fpcheck365 (every changed / added
# function COMDAT v the landed reference rows 0x006096E0 0x005E5F30 0x006D1250 0x005E4BE0). Needs FABLE_EXE / FABLE_ROOT.
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF365
cd $A/work; a=$1; o=obj/${a}_g.obj
[ -f pk/H_$a.pkall.tsv ] || python3 $A/tools/pkrun352.py $A/work/pk $a H=$A/work/obj/H_$a.obj > /dev/null
python3 $A/tools/pkrun352.py $A/work/pk $a g=$A/work/$o > /dev/null
leaf=$(head -1 pk/H_$a.pkall.tsv | awk '{print $3}')
mkdir -p ev/objsec ev/leafrel ev/fp ev/own
python3 $A/tools/objsec352.py obj/H_$a.obj $o "$leaf" > ev/objsec/$a.txt
python3 $A/tools/leafrel.py obj/H_$a.obj $o "$leaf" > ev/leafrel/$a.txt
python3 $A/tools/fpcheck365.py obj/H_$a.obj $o obj/H_006096e0.obj obj/H_005e5f30.obj obj/H_006d1250.obj obj/H_005e4be0.obj > ev/fp/$a.txt
: > ev/own/$a.txt
for s in $(python3 $A/tools/nf.py syms $o | awk '$1=="DEF" && index($2,"??$GetDef@")==1 {print $2}'); do
  case "$s" in
    '??$GetDef@VCUIMiscThingsDef@NUISystem@@@'*) ow=obj/H_0042b312.obj;;
    '??$GetDef@VCThingObjectDef@@@'*) ow=obj/H_0042b040.obj;;
    '??$GetDef@VCArmourDef@@@'*) ow=obj/H_00817b40.obj;;
    *) ow=NONE;;
  esac
  if [ "$ow" = NONE ]; then echo "NOOWNER $s" >> ev/own/$a.txt; else echo "$(python3 $A/tools/cmpcomdat.py "$s" $ow $o | tail -1 | awk '{print $1,$2,$3,$NF}') $ow $s" >> ev/own/$a.txt; fi
done
fpbad=$(grep -c "^DIFF" ev/fp/$a.txt); fpnodef=$(awk -F'\t' '$1=="NODEF"{print $3}' ev/fp/$a.txt | sort -u | while read s; do grep -q -P "^EQUAL\t[^\t]*\t\Q$s\E\t" ev/fp/$a.txt || echo "$s"; done | tr '\n' ' ')
echo -e "$a\t$(tail -1 ev/objsec/$a.txt)\t$(head -1 ev/leafrel/$a.txt)\tfpDIFF=$fpbad\tfpNOREF=[${fpnodef}]\towner=[$(awk '{print $1}' ev/own/$a.txt | sort | uniq -c | tr -s ' ' | tr '\n' ';')]"
