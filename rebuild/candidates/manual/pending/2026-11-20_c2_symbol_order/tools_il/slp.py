#!/usr/bin/env python3
"""slp.py <symlog.json> [lo hi] : symbol allocations of the target function; numbers lo..hi (default field-symbol summary)"""
import json,sys,collections
t=json.load(open(sys.argv[1])); ev=t['events']; sy=t['syms']
pi=[i for i,e in enumerate(ev) if e[0]=='pipe']; ti=[i for i in pi if 'AddToBoundingSphere' in ev[i][1]][0]; prev=max(i for i in pi if i<ti)
seg=[s for s in sy if prev<s[0]<=ti]
SITE={0x1075c331:'temp',0x107615c6:'k15',0x1076f08c:'inl',0x10769fe4:'field',0x1075ed93:'temp2'}
if len(sys.argv)>3:
    lo,hi=int(sys.argv[2]),int(sys.argv[3])
    for n,s in enumerate(seg):
        if lo<=s[3]<=hi: print(n, 'k%d'%s[1], s[3], SITE.get(s[4][0],hex(s[4][0])), ' '.join('%x'%v for v in s[4][1:6]))
else:
    # first-seen order of distinct numbers per site class
    seen=set(); order=[]
    for n,s in enumerate(seg):
        if s[3] not in seen: seen.add(s[3]); order.append((n,s))
    print('distinct numbers',len(seen),'max',max(seen))
    # chunk first-allocation order
    ch=[]; 
    for n,s in order:
        c=s[3]//32
        if c not in ch: ch.append(c); print('chunk',c,'first at alloc',n,'by',SITE.get(s[4][0],hex(s[4][0])),'k%d'%s[1],'num',s[3])
