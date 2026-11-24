# dcalls.py <mangled-or-prefix>...: in the donor FableWin.exe (debug, unfolded), list the direct call/jmp targets of each named function
# (body = from its address to the next named symbol), resolving incremental-link jmp thunks; prints the callee names. Evidence only:
# never copy donor bytes into a hand-back file.
import sys,struct,bisect
D='/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/pdb/debug tlc/FableWin.exe'
X=open(D,'rb').read(); pe=struct.unpack_from('<I',X,0x3C)[0]; ns=struct.unpack_from('<H',X,pe+6)[0]; osz=struct.unpack_from('<H',X,pe+20)[0]
base=struct.unpack_from('<I',X,pe+24+28)[0]; sh=pe+24+osz; secs=[]
for i in range(ns):
    o=sh+i*40; vs,va,rs,rp=struct.unpack_from('<IIII',X,o+8); secs.append((base+va,base+va+max(vs,rs),rp))
def rd(a,n):
    for s,e,rp in secs:
        if s<=a<e: return X[rp+a-s:rp+a-s+n]
    return b''
nm={}; byname={}
for l in open('/home/user/FableDecomp/ghidra_out/fablewin_pdb_names.tsv',encoding='utf-8',errors='replace'):
    p=l.rstrip('\n').split('\t')
    if len(p)<2: continue
    a=int(p[0],16); nm.setdefault(a,p[1]); byname.setdefault(p[1],[]).append(a)
st=sorted(nm)
def resolve(t,depth=0):
    if t in nm: return nm[t]
    b=rd(t,5)
    if b[:1]==b'\xe9' and depth<3: return resolve((t+5+struct.unpack('<i',b[1:5])[0])&0xffffffff,depth+1)
    return '?%08x'%t
for q in sys.argv[1:]:
    hits=[(s,a) for s,v in byname.items() if s==q or (q.endswith('*') and s.startswith(q[:-1])) for a in v]
    if not hits: print('## %s NOT IN FableWin set'%q); continue
    for s,a in hits:
        k=bisect.bisect_right(st,a); end=st[k] if k<len(st) else a+0x4000
        body=rd(a,min(end-a,0x20000)); out=[]
        i=0
        while i+5<=len(body):
            if body[i] in (0xe8,0xe9):
                t=(a+i+5+struct.unpack_from('<i',body,i+1)[0])&0xffffffff
                if any(s0<=t<e0 for s0,e0,_ in secs):
                    r=resolve(t)
                    if not r.startswith('?0') and not r.startswith('?1') or True: out.append(r)
            i+=1
        print('## %s @%08x len=%d'%(s,a,end-a))
        for r in sorted(set(out)):
            if r.startswith('?') and r[1:9].isalnum() and len(r)==9: continue
            print('   ',r)
