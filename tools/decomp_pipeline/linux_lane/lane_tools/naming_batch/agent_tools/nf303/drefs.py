# drefs.py <function-mangled-or-prefix*> <target-mangled>...: donor FableWin.exe: does the named function's body hold the address of the
# target (or of its incremental-link jmp thunk) as an imm32 / rel32 operand (an address taken, e.g. a thread proc passed to _beginthread)?
# Names only; never copy donor bytes.
import sys,struct,bisect
D='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/pdb/debug tlc/FableWin.exe'
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
    if len(p)>=2: a=int(p[0],16); nm.setdefault(a,p[1]); byname.setdefault(p[1],[]).append(a)
st=sorted(nm)
q=sys.argv[1]; fs=[(s,a) for s,v in byname.items() if s==q or (q.endswith('*') and s.startswith(q[:-1])) for a in v]
for t in sys.argv[2:]:
    ta=set(byname.get(t,[]))
    if not ta: print('## target %s NOT IN FableWin'%t); continue
    # thunks: any E9 jmp in code to the target
    for s,e,rp in secs:
        d=X[rp:rp+(e-s)]
    for s,a in fs:
        k=bisect.bisect_right(st,a); end=st[k] if k<len(st) else a+0x4000; body=rd(a,end-a); hit=[]
        for i in range(len(body)-3):
            v=struct.unpack_from('<I',body,i)[0]
            cand={v,(a+i+4+struct.unpack_from('<i',body,i)[0])&0xffffffff}
            for c in cand:
                r=c if c in ta else None
                if r is None:
                    b=rd(c,5)
                    if b[:1]==b'\xe9' and ((c+5+struct.unpack('<i',b[1:5])[0])&0xffffffff) in ta: r=c
                if r is not None: hit.append(i)
        print('%s %s in %s'%('YES' if hit else 'NO',t,s))
