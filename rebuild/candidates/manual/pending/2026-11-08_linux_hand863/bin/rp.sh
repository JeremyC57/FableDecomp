#!/bin/bash
# rp.sh <addr> <tag> : relocations of A/w/<addr>_<tag> with the retail target and the reloc_bindings spellings of that target
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh >/dev/null 2>&1; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
A=$S/w22/agent863; a=$1; t=$2; src=$A/w/${a}_$t/$a.cpp
leaf=$(grep -o "^// Leaf: .*" $src | sed 's|// Leaf: ||')
python3 $A/bin/relpair.py $a "$leaf" $A/w/${a}_$t/$a.obj | sort -k4 | awk '{print $3, $4}' | sort -u > $A/tmp/rp_$a.txt
python3 - $A/tmp/rp_$a.txt <<'PY'
import sys,collections
b=collections.defaultdict(set)
for l in open('/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>=2: b[p[1]].add(p[0])
for l in open(sys.argv[1]):
    t,sym=l.split()
    bs=b.get(t,set())
    flag='OK ' if sym in bs else ('NEW' if not bs else 'DIF')
    print(flag,t,sym, '' if flag=='OK ' else ('| bound: '+' '.join(sorted(bs))[:300]))
PY
