#!/bin/bash
# g.sh <addr> <regex> : grep the landed source of row <addr>
cd /home/user/FableDecomp
f=$(ls rebuild/src/compiled/${1:0:2}/${1:2:2}/*_$1.cpp 2>/dev/null | head -1)
echo "## $f"; grep -nE "$2" "$f" | cut -c1-260
