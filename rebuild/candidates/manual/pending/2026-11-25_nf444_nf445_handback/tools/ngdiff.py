#!/usr/bin/env python3
"""ngdiff.py <batch>: per row, FAIL lines of name_gate new that HEAD does not have (offsets '+N' dropped), classified by whether the
address is owned by the batch (its rows, vftable 0x012BEDA4) or is a RELOC_NAME / LOCAL_CLASS line. Output: evidence/name_gate_diff.txt"""
import re,sys
B=sys.argv[1]; own=set(open(B+'/rows.txt').read().split())|{'012beda4'}
def parse(f):
    d={}; cur=None
    for l in open(f):
        l=l.rstrip('\n')
        if l.startswith('## '): cur=l.split()[1]; d[cur]=set(); continue
        if cur and l.startswith('FAIL'): d[cur].add(re.sub(r' \+[0-9a-f]+ ',' ',re.sub(r'\s+',' ',l)))
    return d
H=parse(B+'/evidence/name_gate_head.txt'); N=parse(B+'/evidence/name_gate_new.txt')
out=[]; foreign=[]
for a in N:
    for l in sorted(N[a]-H.get(a,set())):
        m=re.match(r'FAIL ([0-9a-f]{8}) ',l); addr=m.group(1) if m else None
        tag='OWN' if addr in own else ('NOADDR' if addr is None else 'FOREIGN')
        out.append(f'{a}\tNEW\t{tag}\t{l}')
        if tag=='FOREIGN': foreign.append(f'{a}\t{l}')
    for l in sorted(H.get(a,set())-N[a]): out.append(f'{a}\tGONE\t-\t{l}')
open(B+'/evidence/name_gate_diff.txt','w').write('\n'.join(out)+'\n')
print('new FAIL lines',sum(1 for x in out if '\tNEW\t' in x),'gone',sum(1 for x in out if '\tGONE\t' in x),'foreign',len(foreign))
for f in foreign: print('FOREIGN',f)
for x in out:
    if '\tNEW\tNOADDR' in x: print(x[:230])
