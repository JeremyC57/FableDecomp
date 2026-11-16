# resolve_pick.py <in> <out> <choices>: choices like "t,o,t" (theirs/ours per conflict, in order) on a --diff3 merge file
import sys
lines=open(sys.argv[1]).read().split('\n'); ch=sys.argv[3].split(','); out=[]; i=0; k=0
while i<len(lines):
    l=lines[i]
    if not l.startswith('<<<<<<<'):
        out.append(l); i+=1; continue
    o=[];b=[];t=[]; i+=1
    while not lines[i].startswith('|||||||'): o.append(lines[i]); i+=1
    i+=1
    while not lines[i].startswith('======='): b.append(lines[i]); i+=1
    i+=1
    while not lines[i].startswith('>>>>>>>'): t.append(lines[i]); i+=1
    i+=1
    out += t if ch[k]=='t' else o; k+=1
assert k==len(ch), (k,ch)
open(sys.argv[2],'w').write('\n'.join(out))
