# ann.py <rdis-file>: annotate call targets / absolute addresses with reloc_bindings symbols, manifest names, strings, floats
import sys,os,re,struct,csv,collections
S=os.environ['S']; R='/home/user/FableDecomp'
b=open(S+'/dl/game/Fable The Lost Chapters/Fable.exe','rb').read()
pe=struct.unpack_from('<I',b,0x3c)[0]; n=struct.unpack_from('<H',b,pe+6)[0]; oh=struct.unpack_from('<H',b,pe+20)[0]
secs=[struct.unpack_from('<8sIIII',b,pe+24+oh+40*i) for i in range(n)]
def off(va):
    for nm,vs,v,rs,rp in secs:
        if 0x400000+v<=va<0x400000+v+rs: return rp+va-0x400000-v,nm
    return None,None
bind=collections.defaultdict(collections.Counter)
for l in open(R+'/rebuild/oracles/reloc_bindings.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>=3: bind[p[1].lower()][p[0]]+=1
man={}
for l in open(R+'/rebuild/manifest/functions.tsv'):
    p=l.split('\t',3)
    if len(p)>2: man[p[0].lower()]=p[2]+'::'+p[1]
def desc(va):
    k='%08x'%va; out=[]
    if k in bind: out.append(bind[k].most_common(1)[0][0])
    elif k in man: out.append('M:'+man[k])
    o,nm=off(va)
    if o is not None and nm and nm.startswith((b'.rdata',b'.data')):
        s=b[o:o+64]
        z=s.split(b'\0')[0]
        if len(z)>=3 and all(32<=c<127 for c in z): out.append('"%s"'%z.decode())
        else:
            ws=b[o:o+80]; w=''
            for i in range(0,80,2):
                c=struct.unpack_from('<H',ws,i)[0]
                if c==0: break
                if 32<=c<127: w+=chr(c)
                else: w=''; break
            if len(w)>=3: out.append('L"%s"'%w)
            else:
                f=struct.unpack_from('<f',b,o)[0]; d=struct.unpack_from('<d',b,o)[0]
                out.append('f=%g d=%g'%(f,d))
    return ' | '.join(out)
for l in open(sys.argv[1]):
    l=l.rstrip('\n'); ad=[]
    for m in re.finditer(r'0x([0-9a-f]{6,8})',l):
        v=int(m.group(1),16)
        if v>=0x400000 and v<0x1500000 and not re.search(r'\bj[a-z]+ ',l): 
            d=desc(v)
            if d: ad.append(d)
    print(l+('    ; '+' ; '.join(ad) if ad else ''))
