#!/usr/bin/env python3
"""slotdiff427.py: per family class, retail vftable v its RTTI direct base's retail vftable, dword by dword.
Every differing slot (or slot past the base count) must be declared by the class model; its RTTI-port label must name the class
(owner) and be in a PDB name set. -> evidence/slotdiff.tsv (class, base, nslots, nbase, slot, va, label, owner_ok, pdb) and
evidence/slotclass.tsv (class, verdict, n_diff, n_bad, bad slots)."""
import os,sys,re,csv,collections,struct
sys.path.insert(0,os.path.dirname(__file__)); import pe
R='/home/user/FableDecomp'; EXE=os.environ['FABLE_EXE']
VT=collections.defaultdict(set)
for r in csv.DictReader(open(R+'/rebuild/manifest/rtti-vtables.tsv'),delimiter='\t'): VT[r['class']].add(int(r['vtable_va'],16))
B={}
for l in open(R+'/ghidra_out/rtti_bases.tsv'):
    p=l.rstrip('\n').split('\t')
    if len(p)>3 and '?A0x' not in p[1]: B[p[1]]=p[3].split('@')[0]
L={}
for f in ('labels_rtti_port.tsv','labels_rtti_port_dtors.tsv'):
    for l in open(R+'/ghidra_out/'+f):
        p=l.rstrip('\n').split('\t')
        if len(p)>1: L.setdefault(p[0].lower(),p[1])
PS=collections.defaultdict(set)
for fn in ('egor','fablewin'):
    for l in open(R+'/ghidra_out/%s_pdb_names.tsv'%fn,errors='replace'):
        q=l.rstrip('\n').split('\t')
        if len(q)>1: PS[q[1]].add(fn)
def slots(C):
    v=sorted(VT.get(C,()))
    if len(v)!=1: return None,None
    va=v[0]; out=[]
    while True:
        d=int.from_bytes(pe.read_va(EXE,va+4*len(out),4),'little')
        if not (0x401000<=d<0x01200000) or len(out)>300: break
        out.append(d)
        # stop at the next vftable's COL dword (next class's vftable begins after a COL pointer in .rdata)
    return va,out
def owner(lab):
    m=re.match(r'\?\?_[EG](\w+)@@',lab)
    if m: return m.group(1)
    m=re.match(r'\?[^@?]+@(\w+)@@',lab)
    return m.group(1) if m else None
fam=[l.split('\t')[0] for l in open('evidence/classes.tsv').read().splitlines()[1:]]
extra=sys.argv[1:]
w=open('evidence/slotdiff.tsv','w'); w.write('class\tbase\tnslots\tnbase\tslot\tva\tlabel\towner_ok\tpdb\n')
wc=open('evidence/slotclass.tsv','w'); wc.write('class\tbase\tverdict\tnslots\tnbase\tn_decl\tn_bad\tbad\n')
cnt=collections.Counter()
for C in fam+extra:
    b=B.get(C,'?'); va,sl=slots(C); bva,bsl=slots(b)
    if sl is None or bsl is None:
        wc.write('%s\t%s\tNO_VT\t\t\t\t\t%s\n'%(C,b,'class' if sl is None else 'base')); cnt['NO_VT']+=1; continue
    nd=0; bad=[]
    for j,t in enumerate(sl):
        if j<len(bsl) and bsl[j]==t: continue
        lab=L.get('%08x'%t,'-'); own=owner(lab) if lab!='-' else None
        ok = own==C
        pdb='+'.join(sorted(PS.get(lab,()))) or 'NONE'
        if lab.startswith('??_'): pdb=pdb if pdb!='NONE' else 'dtor'
        w.write('%s\t%s\t%d\t%d\t%d\t%08x\t%s\t%s\t%s\n'%(C,b,len(sl),len(bsl),j,t,lab,'y' if ok else 'n',pdb))
        nd+=1
        if not ok or pdb=='NONE': bad.append('%d:%s'%(j,lab[:60]))
    v='OK' if not bad else 'BAD'
    if len(sl)<len(bsl): v='SHORT'; bad.append('shorter than base')
    cnt[v]+=1
    wc.write('%s\t%s\t%s\t%d\t%d\t%d\t%d\t%s\n'%(C,b,v,len(sl),len(bsl),nd,len(bad),' '.join(bad)))
print(cnt)
