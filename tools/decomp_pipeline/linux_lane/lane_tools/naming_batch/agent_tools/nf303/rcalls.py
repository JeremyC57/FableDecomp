# rcalls.py <mangled>...: donor FableWin.exe reverse lookup: the named functions whose bodies call / jump to the given function (directly or
# through its incremental-link jmp thunk). Names only; evidence for which caller binds which callee. Never copy donor bytes.
import sys,struct,bisect,re
D='/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/pdb/debug tlc/FableWin.exe'
X=open(D,'rb').read(); pe=struct.unpack_from('<I',X,0x3C)[0]; ns=struct.unpack_from('<H',X,pe+6)[0]; osz=struct.unpack_from('<H',X,pe+20)[0]
base=struct.unpack_from('<I',X,pe+24+28)[0]; sh=pe+24+osz; secs=[]
for i in range(ns):
    o=sh+i*40; vs,va,rs,rp=struct.unpack_from('<IIII',X,o+8); ch=struct.unpack_from('<I',X,o+36)[0]; secs.append((base+va,rs,rp,ch))
nm={}; byname={}
for l in open('/home/user/FableDecomp/ghidra_out/fablewin_pdb_names.tsv',encoding='utf-8',errors='replace'):
    p=l.rstrip('\n').split('\t')
    if len(p)>=2: a=int(p[0],16); nm.setdefault(a,p[1]); byname.setdefault(p[1],[]).append(a)
st=sorted(nm)
code=[(va,X[rp:rp+rs]) for va,rs,rp,ch in secs if ch&0x20]
def refs(tset):
    out=[]
    for va,d in code:
        for m in re.finditer(rb'[\xe8\xe9]',d):
            p=m.start()
            if p+5>len(d): break
            t=(va+p+5+struct.unpack_from('<i',d,p+1)[0])&0xffffffff
            if t in tset: out.append((va+p,d[p]))
    return out
for q in sys.argv[1:]:
    tg=set(byname.get(q,[]))
    if not tg: print('## %s NOT IN FableWin set'%q); continue
    thunks=set(s for s,op in refs(tg) if op==0xe9 and s not in nm)
    callers=set()
    for s,op in refs(tg|thunks):
        if s in thunks: continue
        k=bisect.bisect_right(st,s)-1; callers.add(nm[st[k]] if k>=0 else '?')
    print('## %s callers=%d'%(q,len(callers)))
    for c in sorted(callers): print('   ',c)
