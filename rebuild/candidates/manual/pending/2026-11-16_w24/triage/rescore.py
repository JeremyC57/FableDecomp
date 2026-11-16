#!/usr/bin/env python3
"""rescore.py <outdir> <all.tsv> <excl_hits> : aligned residue (difflib) per DIFFER row: changed instruction count, hunks"""
import sys,os,re,difflib
d=sys.argv[1]; ex=set(open(sys.argv[3]).read().split())
def nn(x):
    x=re.sub(r'\[(0x)?[0-9a-f]+\]','[A]',x); x=re.sub(r'0x[0-9a-f]{6,}','A',x)
    if x.startswith(('call','jmp','j')): x=re.sub(r'0x[0-9a-f]+','T',x)
    return x
print('addr\tretail\tbuilt\taligned_changed\thunks\tclass\tlen_src')
for l in open(sys.argv[2]).read().splitlines()[1:]:
    f=l.split('\t')
    if f[1]!='DIFFER' or f[0] in ex: continue
    t=open(os.path.join(d,f[0]+'.txt'),errors='replace').read()
    A=[nn(x.split(None,2)[2]) for x in t.split('--- retail')[1].split('--- built')[0].splitlines()[1:] if len(x.split(None,2))==3]
    B=[nn(x.split(None,2)[2]) for x in t.split('--- built')[1].splitlines()[1:] if len(x.split(None,2))==3]
    sm=difflib.SequenceMatcher(None,A,B,autojunk=False)
    ch=0; h=0
    for op,i1,i2,j1,j2 in sm.get_opcodes():
        if op!='equal': h+=1; ch+=max(i2-i1,j2-j1)
    print(f[0],f[2],f[3],ch,h,f[5],f[6],sep='\t')
