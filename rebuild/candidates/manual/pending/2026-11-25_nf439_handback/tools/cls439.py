#!/usr/bin/env python3
"""cls439.py: classify every wrong spelling of the B-list T1 targets (tracker spellings column) by what differs from the PDB spelling.
Writes work/cls.tsv: target, wrong, witnesses, code, pdb."""
import csv, re, collections, sys
csv.field_size_limit(10**9)
S='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'; W=S+'/w22/agentNF439'
B={r['target']:r for r in csv.DictReader(open(S+'/t1split/T1_B.tsv'),delimiter='\t')}
def split(m):
    x = re.match(r'^\?(\w+)@((?:[\w$?]+@)*?)@([A-Z][A-Z0-9_]*.*)$', m)
    return x.groups() if x else None
out=[]; c=collections.Counter()
for t,r in B.items():
    P=r['pdb_spelling']
    for sp in r['spellings'].split(' || '):
        m=re.match(r'^(\S+) \(([^)]*)\)$', sp.strip())
        if not m: continue
        s,wit=m.groups()
        if s==P: continue
        a=split(s); b=split(P)
        if re.sub(r'\?A0x[0-9a-f]{8}','?A0x',s)==re.sub(r'\?A0x[0-9a-f]{8}','?A0x',P): k='A0X'
        elif not s.startswith('?') or not P.startswith('?'): k='C_NAME'
        elif s.startswith('??') or P.startswith('??'): k='SPECIAL'
        elif not a or not b: k='SPECIAL'
        else:
            k=('N' if a[0]==b[0] else 'n')+('S' if a[1]==b[1] else 's')+('T' if a[2]==b[2] else 't')
        c[k]+=1; out.append((t,s,wit,k,P))
open(W+'/work/cls.tsv','w').write(''.join('\t'.join(x)+'\n' for x in out))
print(c.most_common())
