#!/usr/bin/env bash
# gates434.sh <batch> <defs-file>: full NF434 gate run for a port batch (new root rs<b> from the batch hdr): vland H/N, name_gate --test,
# fwchk, vt432 (plain / g1o selves / g1o selves + landed row leaves), objsec, slot-0 + home scans, pk, /FORCE link, mutation (mut434).
set -u
S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad; W=$S/w22/agentNF434; b=$1; D=$2; B=$W/batches/$b; E=$B/evidence
mkdir -p $E; bash $W/tools/mkroots.sh rs$b - $B/hdr > /dev/null; bash $W/tools/runpair432.sh $b rs$b > /dev/null
source $S/wine/env.sh > /dev/null; export S; cd $W
for k in h n; do cp work/vland_$b$k.log $E/; echo "vland $k: $(grep -E '^[0-9a-f]{8} +[A-Z_]+ +(PASS|FAIL)' work/vland_$b$k.log | awk '{print $2,$3}' | sort | uniq -c | tr '\n' ' ') warns $(grep -c WARN work/vland_$b$k.log) literal $(grep -c LITERAL work/vland_$b$k.log)"; done
NG_OVERLAY=$B/hdr bash tools/ngate432.sh $b $W/work/tab_head.tsv > /dev/null 2>&1; echo "name_gate: $(cut -f2 $E/name_gate.tsv | sort | uniq -c | tr '\n' ' ')"
python3 tools/fwchk434.py $W/work/wd_${b}n $B/rows.txt > $E/fwchk.txt; echo "fwchk: $(tail -1 $E/fwchk.txt)"
python3 tools/vt432.py $W/work/wd_${b}n $B/rows.txt work/tab_head.tsv > $E/vt_plain.txt; echo "vt plain: $(tail -1 $E/vt_plain.txt)"
python3 tools/vt432.py $W/work/wd_${b}h $B/rows.txt work/tab_head.tsv > $E/vt_head.txt; echo "vt head: $(tail -1 $E/vt_head.txt)"
VT_SELFMAP=work/selfmap_all.tsv python3 tools/vt432.py $W/work/wd_${b}n $B/rows.txt work/tab_head.tsv > $E/vt_leafmap.txt; echo "vt g1o+leaves: $(tail -1 $E/vt_leafmap.txt)"
python3 tools/gate425.py $b $b > /dev/null 2>&1; echo "objsec: $(cut -f2 $E/objgate_summary.tsv | sort | uniq -c | tr '\n' ' ') LEAF-diff lines: $(grep -c "LEAF" $E/objgate_summary.tsv)"
python3 tools/slot0chk.py $W/work/wd_${b}n gen/$b.tsv > $E/slot0.tsv 2>&1; echo "slot0 EQ: $(grep -c 'bytes EQ relocs EQ' $E/slot0.tsv) of $(wc -l < $E/slot0.tsv)"
python3 tools/home434.py $W/work/wd_${b}n $B/rows.txt > $E/homes.tsv; echo "homes NONE: $(grep -c 'EQ_home=NONE' $E/homes.tsv) of $(wc -l < $E/homes.tsv)"
timeout 2400 python3 tools/pkgate425.py $b $b > $E/pk.log 2>&1; echo "pk: $(grep -c NEW_NEQ $E/pk.log) rows NEW_NEQ flag; reached NEQ new: $(cat $E/pk/N_*.pkall.tsv | awk -F'\t' '$1=="NEQ" && $8=="y"' | wc -l) (head $(cat $E/pk/H_*.pkall.tsv | awk -F'\t' '$1=="NEQ" && $8=="y"' | wc -l))"
python3 tools/linkgate425.py $b $b $(cat $D) 2>&1 | tail -1
bash tools/mut432.sh $b rs$b tools/mut434.py > /dev/null 2>&1; cp work/vland_${b}m.log $E/; echo "mutants: $(grep -cE '^[0-9a-f]{8} +[A-Z_()0-9v]+ +FAIL' work/vland_${b}m.log) FAIL of $(wc -l < $B/rows.txt)"
