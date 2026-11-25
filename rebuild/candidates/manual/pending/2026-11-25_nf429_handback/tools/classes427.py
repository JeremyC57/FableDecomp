#!/usr/bin/env python3
"""per-class table for the family: direct base, header state, open rows by member, VTHOLD count -> evidence/classes.tsv"""
import csv,collections,re,os
R='/home/user/FableDecomp'; S=os.environ['S']
B={}
for l in open(R+'/ghidra_out/rtti_bases.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>3 and '?A0x' not in p[1]: B[p[1]]=p[3].split('@')[0]
rows=list(csv.DictReader(open('evidence/classify.tsv'),delimiter='\t'))
cl=collections.defaultdict(list)
for r in rows: cl[r['class']].append(r)
w=csv.writer(open('evidence/classes.tsv','w'),delimiter='\t')
w.writerow(['class','base','hdr','nopen','nvth','members'])
cc=collections.Counter()
for c,rs in sorted(cl.items()):
    hdr=rs[0]['hdr']; base=B.get(c,'?')
    nv=sum(1 for r in rs if r['vth'])
    w.writerow([c,base,hdr,len(rs),nv,','.join(sorted(r['member'] for r in rs))])
    cc[(base if base in('CCombatActionBase','CCreatureActionBase','CCombatSequenceBase') else 'other',hdr)]+=len(rs)
for k,v in cc.most_common(): print(v,k)
