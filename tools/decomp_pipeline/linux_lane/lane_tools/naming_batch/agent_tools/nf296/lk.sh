#!/bin/bash
# lk.sh <tag> <watch-regex> <HEAD objs...> -- <new objs...>: two links (HEAD set, new set) -> evidence/link_<tag>.txt (run in background)
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; source $S/wine/env.sh >/dev/null 2>&1; export S
A=$S/w22/agentNF296; tag=$1; w=$2; shift 2; h=(); n=(); side=h
for x in "$@"; do if [ "$x" = -- ]; then side=n; continue; fi; [ $side = h ] && h+=("$x") || n+=("$x"); done
mkdir -p $A/work/lk; o=$A/evidence/link_$tag.txt
{ echo "## HEAD set: ${h[*]##*/}"; timeout 900 python3 $A/tools/lnk2.py $A/work/lk/h_$tag "$w" "${h[@]}"; echo "## new set: ${n[*]##*/}"; timeout 900 python3 $A/tools/lnk2.py $A/work/lk/n_$tag "$w" "${n[@]}"; echo LK_DONE; } > $o 2>&1
