#!/usr/bin/env bash
# linkcheck.sh <tag> <addr...>: /FORCE /NODEFAULTLIB DLL link of the HEAD objects and of the new objects (response files), plus a control
# (the first new object listed twice must give LNK2005). Prints duplicates / unresolved counts and the unresolved-set difference.
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
W=$S/w22/agentNF433; tag=$1; shift; L=$W/work/link_$tag; mkdir -p $L
for side in h n c; do : > $L/$side.rsp; done
for a in "$@"; do for side in h n; do o=$(ls $W/work/wd_$tag$side/*/*/$a/$a.obj | head -1); echo "\"Z:${o//\//\\}\"" >> $L/$side.rsp; done; done
cp $L/n.rsp $L/c.rsp; f=$(head -1 $L/n.rsp | tr -d '"' | sed 's#^Z:##; s#\\#/#g'); cp "$f" $L/ctl_copy.obj; echo "\"Z:${L//\//\\}\\ctl_copy.obj\"" >> $L/c.rsp
for side in h n c; do (cd $L && LINK= timeout 900 wine "$FABLE_VC71/bin/link.exe" /nologo /dll /noentry /force /nodefaultlib /out:$side.dll @$side.rsp > $L/$side.txt 2>&1); done
for s in h n c; do echo "$s: objects $(grep -c . $L/$s.rsp) dup $(grep -cE "LNK2005|LNK4006" $L/$s.txt) unresolved $(grep -cE 'LNK2001|LNK2019' $L/$s.txt)"; 
  grep -oE 'unresolved external symbol (_[A-Za-z0-9_@]+|"[^"]*" \([^)]+\))' $L/$s.txt | sed -E 's/unresolved external symbol //; s/^"[^"]*" \(//; s/\)$//' | sort -u > $L/und_$s.txt; done
echo "unresolved only in HEAD:"; comm -23 $L/und_h.txt $L/und_n.txt; echo "unresolved only in new:"; comm -13 $L/und_h.txt $L/und_n.txt
