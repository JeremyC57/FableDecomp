#!/usr/bin/env bash
# linkcheck.sh <tag> <file of accepted addresses> <run dir>...  (NF324, 2026-11-24): a /FORCE DLL link of every HEAD object and, separately,
# every new object of the accepted rows (objects taken from <run dir>/c*/{head,new}/wd), with LINK= cleared (GOTCHAS nf80). Writes
# work/link_<tag>_{head,new}.txt and prints the LNK2005 (duplicate) counts and whether the sorted LNK2001 / LNK2019 unresolved-symbol sets
# are equal. Lane env sourced; run in the background.
set -u
A=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF335
tag=$1; list=$2; shift 2
W=$A/work/link_$tag; mkdir -p $W
for side in head new; do
  : > $W/$side.rsp
  while read -r a; do
    for r in "$@"; do
      o=$(ls $r/c*/$side/wd/${a:0:2}/${a:2:2}/$a/$a.obj 2>/dev/null | head -1)
      [ -n "$o" ] && { echo "\"Z:${o//\//\\}\"" >> $W/$side.rsp; break; }
    done
  done < "$list"
  (cd $W && LINK= timeout 900 wine "$FABLE_VC71/bin/link.exe" /nologo /dll /noentry /force /nodefaultlib /out:$side.dll @$side.rsp > $A/work/link_${tag}_$side.txt 2>&1)
done
for side in head new; do
  echo "$side: objects $(grep -c . $W/$side.rsp) duplicates(LNK2005) $(grep -c LNK2005 $A/work/link_${tag}_$side.txt) unresolved $(grep -cE 'LNK2001|LNK2019' $A/work/link_${tag}_$side.txt)"
done
for s in head new; do   # the unresolved symbol's mangled name (the first parenthesised name), not the referencing function's
  grep -oE 'unresolved external symbol (_[A-Za-z0-9_@]+|"[^"]*" \([^)]+\))' $A/work/link_${tag}_$s.txt | sed -E 's/unresolved external symbol //; s/^"[^"]*" \(//; s/\)$//' | sort -u > $W/und_$s.txt
done
cmp -s $W/und_head.txt $W/und_new.txt && echo "unresolved sets EQUAL ($(wc -l < $W/und_head.txt))" || { echo "unresolved sets DIFFER"; diff $W/und_head.txt $W/und_new.txt | head -20; }
echo LINKCHECK_DONE
