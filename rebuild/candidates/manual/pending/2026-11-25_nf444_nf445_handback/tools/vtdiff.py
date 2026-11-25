#!/usr/bin/env python3
"""vtdiff.py <H.txt> <N.txt> <label>: per (row, vftable, slot) compare vt432 letters HEAD v new (a agree, W WRONG, u unknown, '-' slot not
emitted); prints every changed slot with the retail target and the new object's slot symbol, classified:
  ALLOWED   agree -> WRONG at 0x00C6D630 / 0x00C6F3E0 / 0x00C6D120 (R-NF445-1);  DEBT  unknown/absent -> WRONG;  IMPROVED  -> agree;
  OTHER     anything else (would mean HOLD)."""
import re,sys,struct,importlib.util as iu,glob
S='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad'; B=S+'/w22/agentNF445/batches/nf444_hdr'
d=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',d,0x3c)[0]; n=struct.unpack_from('<H',d,pe+6)[0]; so=struct.unpack_from('<H',d,pe+20)[0]
secs=[(struct.unpack_from('<I',d,pe+24+so+i*40+12)[0],struct.unpack_from('<I',d,pe+24+so+i*40+8)[0],struct.unpack_from('<I',d,pe+24+so+i*40+20)[0]) for i in range(n)]
def rd(va):
    r=va-0x400000
    for a,s,raw in secs:
        if a<=r<a+s: return struct.unpack_from('<I',d,raw+r-a)[0]
def parse(f):
    t={}
    for l in open(f):
        p=l.split()
        if len(p)==4 and re.fullmatch(r'[0-9a-f]{8}',p[2]) and re.fullmatch(r'[auW]+',p[3]): t[(p[0],p[1],p[2])]=p[3]
    return t
H=parse(sys.argv[1]); N=parse(sys.argv[2]); lab=sys.argv[3]
sp=iu.spec_from_file_location("ceq",S+"/w22/agentNF432/tools/coffload.py"); m=iu.module_from_spec(sp); sp.loader.exec_module(m)
def nsym(row,vt,k):
    o=B+f'/evidence/pk/N_{row}.obj'
    for v,rel in m.vftables(o):
        if v==vt:
            for off,s in rel:
                if off//4==k: return s
    return '-'
ALLOW={'00c6d630','00c6f3e0','00c6d120'}; cnt={}
for key in sorted(set(H)|set(N)):
    h=H.get(key,''); nn=N.get(key,''); L=max(len(h),len(nn))
    for k in range(L):
        a=h[k] if k<len(h) else '-'; b=nn[k] if k<len(nn) else '-'
        if a==b: continue
        tgt='%08x'%rd(int(key[2],16)+4*k)
        if b=='W' and a=='a': kind='ALLOWED' if tgt in ALLOW else 'OTHER'
        elif b=='W' and a in 'u-': kind='DEBT'
        elif b=='a': kind='IMPROVED'
        elif b in 'u-' and a=='W': kind='IMPROVED_W_to_u'
        else: kind='OTHER'
        cnt[kind]=cnt.get(kind,0)+1
        print(f'{lab}\t{key[0]}\t{key[1]}\t{k}\t{tgt}\t{a}->{b}\t{kind}\t{nsym(key[0],key[1],k)}')
print(f'{lab}\tSUMMARY\t{cnt}')
