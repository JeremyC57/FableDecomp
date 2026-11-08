#!/bin/bash
# grade a variant + summarise: gv.sh <file.cpp> <label>  (session 4)
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
A=$S/w22/agent885
f=$(realpath $1); lab=$2
bash $A/gr.sh $f > $A/probe/gv_$lab.txt 2>&1
first=$(head -1 $A/probe/gv_$lab.txt)
strict=$(grep -o "difflib matched .*" $A/probe/gv_$lab.txt | head -1)
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
cd /home/user/FableDecomp
loose=$(python3 $A/tools/cmp.py $S/wine/q/00cbfb7d/00cbfb7d.obj --leaf "$(grep -o '// Leaf: .*' $f | sed 's|// Leaf: ||')" --loose --hunks 0 2>/dev/null | grep -o "difflib matched .*" | head -1)
cd $A
python3 tools/calls.py --counts --ctx 0 --max 400 > probe/calls_$lab.txt 2>&1
calls=$(head -1 probe/calls_$lab.txt)
python3 tools/sites.py probe/calls_$lab.txt > probe/sites_$lab.txt
echo "$first"; echo "strict: $strict"; echo "loose: $loose"; echo "$calls"; cat probe/sites_$lab.txt
echo "$(date +%F) s4 $lab: $first | $strict | loose $loose | $calls | sites: $(grep -c BAD probe/sites_$lab.txt) bad" >> ATTEMPTS.log
