#!/usr/bin/env python3
"""pp2.py <dump2.json> [line-lo line-hi] [-a] : pretty-print the pre-lowering IL of the target function"""
import json,struct,sys
t=json.load(open(sys.argv[1])); ev=t['events']; syms=t.get('syms',{})
args=[a for a in sys.argv[2:] if not a.startswith('-')]; ALL='-a' in sys.argv
lo,hi=(int(args[0]),int(args[1])) if len(args)>=2 else (0,10**9)
pi=[i for i,e in enumerate(ev) if e[0]=='pipe']
ti=[i for i in pi if 'InitialiseFromFileData' in ev[i][1]][0]
prev=max([i for i in pi if i<ti] or [-1])
seg=[e for e in ev[prev+1:ti] if e[0]=='t']
OPN={0x263:'=',0x27a:'fmul',0x266:'fadd',0x288:'cmp',0x291:'jcc',0x2c4:'LABEL',0x2c3:'BLK',0x12:'lea?',0x278:'call',0x2d:'add'}
def opd(o):
    d=struct.unpack('<12I',bytes.fromhex(o[1])); k=d[1]; ty=d[2]>>16; s=syms.get('%x'%d[6])
    nm=(s[2] or 'k%d@%x'%(s[0],d[6])) if s else ('%x'%d[6] if d[6] else '')
    off=d[4]
    if k==0x24f: return 'S(%s%s)'%(nm, '+%x'%off if off else '')
    if k==0x250: return 'M(%s+%x)'%(nm,off)
    if k==0x24b: return 'I(%x)'%d[6]
    if k==0x24d: return 'F(%08x%08x)'%(d[7],d[6])
    if k==0x252: return 'IND[%x]'%off
    if k==0x25a: return 'L(%x)'%d[6]
    return 'K%x(%x,%x)'%(k,d[4],d[6])
for e in seg:
    d=struct.unpack('<12I',bytes.fromhex(e[2])); line=d[5]&0xffff
    if not(lo<=line<=hi): continue
    op=d[1]
    print('%06x L%-3d %-6s ty=%04x  %s  <-  %s'%(e[1],line,OPN.get(op,'%x'%op),d[2]>>16,' '.join(opd(o) for o in e[4]),' , '.join(opd(o) for o in e[3])))
