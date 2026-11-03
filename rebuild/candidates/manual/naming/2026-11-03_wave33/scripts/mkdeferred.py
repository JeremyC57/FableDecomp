import sys, os, json, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import *
W=os.environ['S']+'/wave33'
cat=catalog()
applied=set(l.split('\t')[1] for l in open(W+'/applied_all.tsv'))
ctx={l.split('\t')[0]:l.rstrip('\n').split('\t') for l in open(W+'/famA_tr.tsv')}
reason={}; prop={}
def gen(fam):
    for l in open(W+'/'+fam+'/gen.tsv'):
        a=l.rstrip('\n').split('\t')
        if a[1]=='DEFER' and a[0] not in reason: reason[a[0]]='%s: %s'%(fam.replace('ctorB','ctor'),a[2])
        elif a[1]=='OK': prop.setdefault(a[0],json.loads(a[2])['sym'])
for fam in ('esb','esbm','ctorB','sf'): gen(fam)
for fam in ('ctorB','esbm','esb','sf'):
    p=W+'/'+fam+'/pass.tsv'
    for l in open(p):
        s,f,a,g=l.rstrip('\n').split('\t')
        if s=='FAIL' and a not in applied:
            why=g
            if 'DIFFER(30v40)' in g: why+=' -- the constructor row\'s class model lacks the member destructor(s) retail runs (retail calls a member dtor before the base dtor)'
            elif 'DIFFER' in g: why+=' -- the storer source does not emit the ??_G (leaf matches no symbol / different model)'
            elif 'RELOC_NAME' in g: why+=' -- the source declares the class dtor out of line where retail expands it (binding would give a second name to the base dtor)'
            reason[a]='%s recheck: %s'%(fam.replace('ctorB','ctor'),why)
out=['row\ttarget\tfinal_symbol\tstill_emitted\treason']
C=collections.Counter()
for a,q in sorted(cat.items()):
    n=q[1]
    if not (re.match(r"^CVtable_[0-9a-fA-F]+::`scalar deleting destructor'$",n) or re.match(r'^_global::ScalarDeletingDtor_[0-9a-f]+$',n)): continue
    if a in applied: continue
    c=ctx.get(a,['','','',''])
    vt,calls,st=c[1],c[2],c[3]
    r=reason.get(a)
    if not r:
        if 'CScriptThing::~CScriptThing ; 00ce1000' in calls and '00cdd4c0' in calls:
            r='aes HOLD: CActiveEntityScript<T> ??_G (DECISIONS_W29_RESOLVED D1: retail expands the implicit ~CActiveEntityScriptBase, VC7.1 RTM calls it; the wave 28 stand-in stays byte-identical)'; k='aes'
        elif not vt:
            r='noref: no vtable or data reference and no PDB-named caller (the non-virtual ??_G QAE form); identity rests on the called destructor alone (one source) and a trivial-dtor derived class would give the same bytes'; k='noref'
        elif 0x00c08ae0<=int(a,16)<0x00ca0000:
            r='libpk / GameCODA / Vorbis (no RTTI, no PDB): no second source for a class name'; k='libpk'
        else:
            r='not reached: no unique PDB-named storer (Alloc / ctor / spawning row) for vtable %s; storers: %s'%(vt,st[:200] or 'none'); k='other'
    else: k=r.split(':')[0].split()[0]
    C[k]+=1
    out.append('\t'.join([a,vt or '-',prop.get(a,'-'),n,r]))
open(sys.argv[1],'w').write('\n'.join(out)+'\n')
print(C, sum(C.values()))
