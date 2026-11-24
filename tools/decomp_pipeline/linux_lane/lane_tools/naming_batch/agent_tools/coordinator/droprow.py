"""droprow.py HEADDIR NEWDIR addr...: coordinator drop test on objects built by the v-scripts (wd/<aa>/<bb>/<addr>/<addr>.obj).
For each DEF- / UND- external symbol: reloc_bindings lines naming it (and whether this row is the witness), and the HEAD pk_all verdict line."""
R='/home/user/FableDecomp/rebuild/oracles/reloc_bindings.tsv'
import sys
rb={}
for l in open(R):
    f=l.rstrip('\n').split('\t')
    if len(f)>=3: rb.setdefault(f[0],[]).append((f[1],f[2]))
import struct
def ext(p):
    d=open(p,'rb').read(); so,nsym=struct.unpack_from('<II',d,8); st=so+nsym*18; und=set(); dfn=set(); i=0
    while i<nsym:
        e=d[so+i*18:so+i*18+18]
        n=(e[:8].split(b'\0')[0] if e[:4]!=b'\0\0\0\0' else d[st+struct.unpack_from('<I',e,4)[0]:].split(b'\0')[0]).decode('latin1')
        sec=struct.unpack_from('<h',e,12)[0]
        if e[16]==2: (und if sec==0 else dfn).add(n)
        i+=1+e[17]
    return dfn,und
H,N=sys.argv[1],sys.argv[2]
for a in sys.argv[3:]:
    p=f'{a[:2]}/{a[2:4]}/{a}/{a}.obj'
    hd,hu=ext(f'{H}/wd/{p}'); nd,nu=ext(f'{N}/wd/{p}')
    pk={l.rstrip().split()[-1]:l.split()[0]+' '+l.split()[2] for l in open(f'{H}/pk/N_{a}.pkall.tsv') if not l.startswith('#') and l.strip()}
    for kind,s in [('DEF-',x) for x in sorted(hd-nd)]+[('UND-',x) for x in sorted(hu-nu)]:
        if kind=='DEF-' and s in nu: continue
        b=rb.get(s,[]); wit=[t for t in b if t[1]==a]
        print(a,kind,s[:110],'bind=%d'%len(b),'witness_here=%d'%len(wit),'pkH=',pk.get(s,'-'))
