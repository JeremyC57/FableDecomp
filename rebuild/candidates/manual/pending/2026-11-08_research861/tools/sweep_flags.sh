#!/bin/bash
# sweep_flags.sh <file.cpp> : grade one source under a list of extra flag sets; print status/diff/md5 per set
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
A=$S/w22/research861; f=$1
sets=("" "/G5" "/G6" "/G7" "/GB" "/Os" "/Ot" "/Op" "/Oi-" "/Ob1" "/Gy" "/GF" "/GR-" "/Zp1" "/Zp4" "/Zp8" "/GS" "/Oa" "/Ow" "/Oi" "/Ox" "/O1" "/EHsc" "/QIfist" "/G7 /Os" "/G5 /Os" "/GB /Os" "/Gs" "/Gs0" "/Ge" "/Gh" "/GX-" "/Zi" "/Z7" "/Gf" "/Gz" "/Gd" "/Gr" "/QIfdiv" "/Op-" "/Og-")
for s in "${sets[@]}"; do r=$($A/gr.sh $f $s 2>&1); echo "[$s] $(echo "$r" | head -1 | awk '{print $3}') $(echo "$r" | grep textdiff)"; done
