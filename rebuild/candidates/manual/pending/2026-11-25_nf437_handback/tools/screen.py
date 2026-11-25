#!/usr/bin/env python3
# screen.py <addr...>: lock owner, newest claim account, reserved, T3 col2/3 lines per address.
import sys, re, glob, collections, os
R='/home/user/FableDecomp'; S='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'
claims={}
L=open(R+'/rebuild/candidates/manual/COORDINATION_CLAIMS.tsv',encoding='utf-8').read().splitlines()
hdr=L[0].split('\t')
for l in L[1:]:
    q=l.split('\t')
    for x in re.findall(r'(?:0x)?([0-9a-fA-F]{8})\b', l): claims[x.lower()]=(q[1]+'/'+q[4]+'/'+q[0]) if len(q)>4 else l[:40]
res=collections.defaultdict(list)
for f in glob.glob(R+'/rebuild/candidates/manual/pending/*/RESERVED.tsv'):
    for l in open(f):
        for x in re.findall(r'(?:0x)?([0-9a-fA-F]{8})\b', l): res[x.lower()].append(f.split('/')[-2])
t3=collections.defaultdict(list)
for l in open(R+'/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv').read().splitlines()[1:]:
    q=l.split('\t')
    for x in re.findall(r'0x([0-9a-fA-F]{8})', ' '.join(q[1:3])): t3[x.lower()].append(q[0]+':'+' '.join(q[1:3])[:80])
for a in sys.argv[1:]:
    a=a.lower().replace('0x','')
    p=S+'/w22/nflocks/%s/owner'%a
    lk=open(p).read().strip() if os.path.exists(p) else '-'
    print('\t'.join([a, 'lock='+lk, 'claim='+claims.get(a,'-').replace('\t',' '), 'res='+','.join(res.get(a,[]) ) , 't3='+' | '.join(t3.get(a,[]))[:300].replace('\t',' ')]))
