#!/usr/bin/env python3
"""shape427.py: retail body shape of each worklist row (catalog bytes, int3/nop tail trimmed) -> evidence/shapes.tsv
(address, member, class, shape, length, detail). No bytes are written, only the shape name and the decoded constant / target."""
import csv,sys,os,collections,struct
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__))); import nf
cat=nf.catalog()
rows=list(csv.DictReader(open('evidence/classify.tsv'),delimiter='\t'))
def shape(a):
    r=cat.get(a)
    if not r or not r.get('bytes'): return 'NOBYTES',0,''
    b=bytes.fromhex(r['bytes'])
    while len(b)>1 and b[-1] in (0xcc,0x90): b=b[:-1]
    n=len(b)
    if b==b'\xc3': return 'EMPTY',n,''
    if b==b'\xc2\x04\x00': return 'EMPTY_RET4',n,''
    if n==3 and b[0]==0xc2 and b[2]==0: return 'EMPTY_RETN',n,'%d'%b[1]          # NF429: `ret N` (N/4 stack arguments, nothing done)
    if b==b'\x33\xc0\x40\xc3': return 'RET_LONG',n,'1'                          # NF429: size-mode `return 1` (xor eax,eax; inc eax)
    if n==4 and b[0]==0x6a and b[2:]==b'\x58\xc3': return 'RET_LONG',n,'%d'%struct.unpack('<b',b[1:2])[0]   # NF429: size-mode push imm8; pop eax
    if b==b'\xb0\x01\xc3': return 'RET_TRUE',n,''
    if b==b'\x32\xc0\xc3': return 'RET_FALSE',n,''
    if b==b'\x33\xc0\xc3': return 'RET_ZERO',n,''
    if n==6 and b[0]==0xb8 and b[5]==0xc3: return 'RET_LONG',n,'%d'%struct.unpack('<i',b[1:5])[0]
    if n==7 and b[:2]==b'\xd9\x05' and b[6]==0xc3: return 'RET_FLOAT_GLOBAL',n,'0x%08X'%struct.unpack('<I',b[2:6])[0]
    if n==5 and b[0]==0xe9: return 'JMP',n,'0x%08X'%((int(a,16)+5+struct.unpack('<i',b[1:5])[0])&0xffffffff)
    if n<=8 and b[-1]==0xc3 and b[0] in (0x8a,0x8b,0xd9,0x0f): return 'GET_MEMBER',n,''
    return 'OTHER',n,''
w=csv.writer(open('evidence/shapes.tsv','w'),delimiter='\t'); w.writerow(['addr','member','class','shape','len','detail','status','self_form'])
c=collections.Counter()
for r in rows:
    s,n,d=shape(r['addr'])
    w.writerow([r['addr'],r['member'],r['class'],s,n,d,r['status'],r['self_form']])
    c[(r['member'] if r['member'] in ('Clone','?0','?1','?_G','?_E') else 'VIRT', s)]+=1
for k,v in sorted(c.items(),key=lambda x:-x[1]): print(v,k)
