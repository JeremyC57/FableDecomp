#!/bin/bash
# usage: gr.sh <file.cpp> [extra flags...]  -- grades a source whose basename is the address; reads // Leaf: and // Flags:;
# sets the chunk-856 header overlay for the ten chunk-856 addresses. Prints the q.py summary + diff lines.
export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad
source $S/wine/env.sh; export S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; cd /home/user/FableDecomp
A=$S/w22/research861
f=$1; shift
a=$(basename $f .cpp); a=${a%%_*}; a=${a%%.*}
case $a in 009d8cf0|005aecc0|00a8b990|00a4efc0|0049a0b0) export FABLE_HDR_OVERLAY=$A/hdr860;; 0050a650|005253f0|00aad650|00d33730|00d3a4a0|00ddae40|00e72630|00e799f0|00ec4a00|00ef4b40) export FABLE_HDR_OVERLAY=$A/hdr856;; esac
lf=$(grep -o "^// Leaf: .*" $f | head -1 | sed 's|// Leaf: ||')
fl=$(grep -o "^// Flags: .*" $f | head -1 | sed 's|// Flags: ||;s|none||')
allfl="$fl $*"; allfl=$(echo $allfl)
timeout 300 python3 $S/tools/q.py $a $f $lf ${allfl:+--flags $allfl} > $A/tmp/$a.last.txt 2>&1
head -1 $A/tmp/$a.last.txt
python3 - $A/tmp/$a.last.txt <<'PY'
import sys,re
t=open(sys.argv[1]).read().split('\n')
try:
    i=[k for k,l in enumerate(t) if l.startswith('--- retail')][0]; j=[k for k,l in enumerate(t) if l.startswith('--- built')][0]
except IndexError: sys.exit()
def norm(ls):
    out=[]
    for l in ls:
        m=re.match(r'^([0-9a-f]{8})\s+([0-9a-f]+)\s+(.*)$',l)
        if not m: continue
        out.append(re.sub(r'ptr \[0\]','ptr [IMM]',re.sub(r'(push|mov \w+,|cmp \w+,) 0$',r'\1 IMM',re.sub(r'0x[0-9a-f]{5,8}','IMM',m.group(3)))))
    return out
r=norm(t[i+1:j]); b=norm(t[j+1:])
import difflib
d=[l for l in difflib.unified_diff(r,b,lineterm='',n=0) if not l.startswith(('---','+++','@@'))]
import hashlib
print(f'textdiff lines: {len(d)}  builtmd5 {hashlib.md5("\n".join(b).encode()).hexdigest()[:10]}')
for l in d[:40]: print('  ',l)
PY
