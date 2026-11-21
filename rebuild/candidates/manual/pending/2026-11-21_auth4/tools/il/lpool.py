#!/usr/bin/env python3
"""lpool.py <symlog.json> <dump2.json> : the kind-4/5 symbol pool (locals, inlined callee locals/params, field symbols) in allocation order"""
import json,sys,struct
t=json.load(open(sys.argv[1])); ev=t['events']; sy=t['syms']
d=json.load(open(sys.argv[2])); names=d.get('syms',{})
# lines per symbol from IL dump
dev=d['events']; pi=[i for i,e in enumerate(dev) if e[0]=='pipe']; ti=[i for i in pi if 'InitialiseFromFileData' in dev[i][1]][0]; prev=max(i for i in pi if i<ti)
lines={}
for e in dev[prev+1:ti]:
    if e[0]!='t': continue
    td=struct.unpack('<12I',bytes.fromhex(e[2])); ln=td[5]&0xffff
    for o in e[3]+e[4]:
        od=struct.unpack('<12I',bytes.fromhex(o[1]))
        if od[1] in (0x24f,0x250): lines.setdefault(od[6],set()).add(ln)
pi=[i for i,e in enumerate(ev) if e[0]=='pipe']; ti=[i for i in pi if 'InitialiseFromFileData' in ev[i][1]][0]; prev=max(i for i in pi if i<ti)
seg=[s for s in sy if prev<s[0]<=ti]
SITE={0x1075c331:'temp',0x107615c6:'k15',0x1076f08c:'inl',0x10769fe4:'field',0x1075ed93:'temp2'}
pos=0
for n,s in enumerate(seg):
    if s[1] in (4,5,13):
        nm=names.get('%x'%s[2]); 
        print('%4d L%-3d k%d #%-4d %-6s %-24s lines=%s  %s'%(n,pos,s[1],s[3],SITE.get(s[4][0],hex(s[4][0])),(s[5] if len(s)>5 and s[5] else '-'),sorted(lines.get(s[2],[])),' '.join('%x'%v for v in s[4][1:5])))
        pos+=1
