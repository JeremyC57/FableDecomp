#!/bin/bash
# gradev.sh <glob-variant-prefix> : grade rows/*/<prefix>*.cpp, 4 parallel, lines: addr variant grade dl
I=${S}/pragma3
ls $I/rows/*/$1*.cpp | while read f; do a=$(basename $(dirname $f)); echo "$a $f"; done | xargs -P4 -n2 sh -c 'r=$('$I'/qq.sh $0 $1 2>&1 | head -1 | sed "s/leaf=.*//"); echo "$(basename $1 .cpp) $r"'
