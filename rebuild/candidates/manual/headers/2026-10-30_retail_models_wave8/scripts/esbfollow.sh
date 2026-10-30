#!/bin/bash
# esbfollow.sh <tag>: ESB2 on every row whose CScriptThing is now shared (wave 7 NEXT: ESB follows each ST batch):
# dry-run, skip rows with a reverted family edit, apply, mcheck, revert failures, privmain on leafsym renames, re-mcheck
export S=/tmp/claude-0/-home-user-FableDecomp/a013f3ec-d440-5b47-87a1-ed975cb80dfb/scratchpad
source $S/wine/env.sh >/dev/null; export FABLE_ROOT=$S/mig8/repo; cd $S/mig8/repo
t=$1; L=$S/mig8/logs; W=$S/mig8/work; SC=$S/mig8/scripts
python3 $SC/count.py -l CEntityScriptBase | tail -n +2 > $W/esbf_$t.all
python3 $SC/mig_esb2.py --dry $(cat $W/esbf_$t.all) | grep ^edit | awk '{print $2}' | grep -v -F -f <(cut -f1 $W/rev.tsv | sort -u | sed 's/$/.cpp/') > $W/esbf_$t.txt
echo "candidates $(wc -l < $W/esbf_$t.txt)"
[ -s $W/esbf_$t.txt ] || exit 0
python3 $SC/fam.py apply ESB2 mig_esb2.py -- $(cat $W/esbf_$t.txt)
timeout 3000 python3 $SC/mcheck.py -j 3 $(cat $W/esbf_$t.txt) > $L/mc_esb2_$t.txt 2>&1
cut -f2 $L/mc_esb2_$t.txt | grep -v WARN | sort | uniq -c
python3 $SC/fam.py revert ESB2 $L/mc_esb2_$t.txt
python3 $SC/bind_new.py $L/mc_esb2_$t.txt | tail -3
python3 $SC/privmain.py $L/mc_esb2_$t.txt > $L/privmain_$t.txt
P=$(grep ^edit $L/privmain_$t.txt | awk '{print $2}')
if [ -n "$P" ]; then timeout 2000 python3 $SC/mcheck.py -j 3 $P > $L/mc_esb2_${t}p.txt 2>&1; echo "privmain: $(grep -c EAEXXZ $L/mc_esb2_${t}p.txt) EAE of $(echo $P | wc -w)"; python3 $SC/fam.py revert ESB2 $L/mc_esb2_${t}p.txt; fi
