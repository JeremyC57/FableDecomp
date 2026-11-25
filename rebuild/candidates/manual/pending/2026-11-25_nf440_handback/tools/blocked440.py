#!/usr/bin/env python3
"""blocked440.py: family addresses blocked by another agent's lock, a pending RESERVED.tsv, or a newest Account B claim."""
import os,glob,re,collections,sys
R='/home/user/FableDecomp'; S='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'; W=S+'/w22/agentNF440'
rows=[l.rstrip('\n').split('\t') for l in open(R+'/rebuild/candidates/manual/pending/2026-11-25_nf428_handback/FAMILY_TABLE.tsv')][1:]
res=set()
for f in glob.glob(R+'/rebuild/candidates/manual/pending/*/RESERVED.tsv'):
    for l in open(f,errors='replace'):
        for m in re.finditer(r'\b(?:0x)?([0-9a-fA-F]{8})\b',l): res.add(m.group(1).lower())
claim={}
for l in open(R+'/rebuild/candidates/manual/COORDINATION_CLAIMS.tsv',errors='replace'):
    for m in re.finditer(r'\b(?:0x)?([0-9a-fA-F]{8})\b',l): claim[m.group(1).lower()]=l.strip()[:160]
C=collections.Counter(); out=[]
for r in rows:
    a=r[0]; o=S+'/w22/nflocks/'+a+'/owner'
    st=open(o).read().strip() if os.path.exists(o) else 'none'
    k='free' if st in ('none','released','NF440') else 'lock:'+st
    if a in res: k='RESERVED'
    cl=claim.get(a,'')
    if cl and re.search(r'\bB\b|Account B|acctb',cl,re.I): k='claimB'
    C[k]+=1; out.append((a,k,cl))
print(C)
open(W+'/evidence/blocked.tsv','w').write(''.join('\t'.join(x)+'\n' for x in out))
