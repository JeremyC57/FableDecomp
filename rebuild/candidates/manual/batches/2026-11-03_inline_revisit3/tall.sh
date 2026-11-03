#!/bin/bash
I=${S}/pragma3
for T in "$@"; do for d in $I/rows/*/; do a=$(basename $d); echo "$a $T"; done; done | xargs -P4 -n2 sh -c 'cp '$I'/rows/$0/plain.cpp '$I'/rows/$0/T$1.cpp; r=$(QQTAG=T$1 '$I'/qq.sh $0 '$I'/rows/$0/T$1.cpp --flags /d2inlT$1 | head -1 | sed "s/leaf.*//"); echo "T$1 $r"'
