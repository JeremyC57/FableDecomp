#!/usr/bin/env python3
"""ids.py <dump2.json> : table of operand ids (operand +0x10 >> 5 & 0x7ff..) -> symbol, kind, lines"""
import json,struct,sys
t=json.load(open(sys.argv[1])); ev=t['events']; syms=t.get('syms',{})
pi=[i for i,e in enumerate(ev) if e[0]=='pipe']
ti=[i for i in pi if 'AddToBoundingSphere' in ev[i][1]][0]
prev=max([i for i in pi if i<ti] or [-1])
seg=[e for e in ev[prev+1:ti] if e[0]=='t']
tab={}
for e in seg:
    d=struct.unpack('<12I',bytes.fromhex(e[2])); line=d[5]&0xffff
    for o in e[3]+e[4]:
        od=struct.unpack('<12I',bytes.fromhex(o[1]))
        if od[1] not in (0x24f,0x250): continue
        idn=(od[4]&0xffff)>>5
        if not idn: continue
        s=syms.get('%x'%od[6]); nm=(s[2] or 'k%d'%s[0]) if s else '?'
        r=tab.setdefault(idn,[od[6],nm,od[1],od[2]>>16,set(),od[4]>>16, od[5]]); r[4].add(line)
for idn in sorted(tab):
    r=tab[idn]; print('%4d sym=%07x %-22s %s ty=%04x hi=%x f14=%x lines=%s'%(idn,r[0],r[1],'M' if r[2]==0x250 else 'S',r[3],r[5],r[6],sorted(r[4])))
