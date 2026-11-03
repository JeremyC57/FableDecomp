import sys, os, struct, re, collections
sys.path.insert(0, sys.argv[0].rsplit('/',1)[0])
from pe import PE
p=PE()
cat={}
for l in open('rebuild/oracles/auto-re-candidates.tsv'):
    a=l.rstrip('\n').split('\t')
    if len(a)>=3 and a[0]!='address': cat[int(a[0],16)]=a[1]
man={}
for l in open('rebuild/manifest/functions.tsv'):
    a=l.rstrip('\n').split('\t')
    try: man[int(a[0],16)]=a
    except: pass
def nm(x):
    if x in cat: return cat[x]
    if x in man: return 'M:'+'|'.join(man[x][1:3])
    return '?'
def calls(ad,n):
    b=p.bytes(ad,n); r=[]
    for i in range(n-4):
        if b[i]==0xe8:
            t=(ad+i+5+struct.unpack_from('<i',b,i+1)[0])&0xffffffff
            if p.is_text(t): r.append((i,t))
    return r
fam=[l.rstrip('\n').split('\t') for l in open(sys.argv[1])][1:]
out=open(sys.argv[2],'w')
for a in fam:
    ad=int(a[0],16); n=int(a[2])
    cs=calls(ad,n)
    vt=a[4].split(',')[0] if a[4] else ''
    slots=[]
    if vt:
        v=int(vt,16)
        for k in range(0,40):
            f=p.u32(v+4*k)
            if f is None or not p.is_text(f): break
            if k>0 and p.u32(v+4*k-4+4) is None: break
            slots.append('%d:%08x=%s'%(k,f,nm(f)))
            # stop at zero
    out.write('%s\t%s\t%s\tcalls=%s\tslots=%s\n'%(a[0],n,vt,' ; '.join('%08x=%s'%(t,nm(t)) for i,t in cs),' | '.join(slots[:14])))
