# esb: entity-script ??_G rows whose body is ~CEntityScriptBase + delete; class from the landed Alloc that stores the vtable
import sys, re, os, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import *
ent=ps1_entries(); cat=catalog(); E,F=pdb()
outdir=sys.argv[1]; only=set(sys.argv[2:]); MEM=os.environ.get('ESB_MEMBERS')=='1'
os.makedirs(outdir,exist_ok=True)
res=[]
for l in open(os.environ['S']+'/wave33/famA_tr.tsv'):
    a=l.rstrip('\n').split('\t')
    addr,vt,calls,st=a
    PLAIN='calls=00f35b40=CEntityScriptBase::~CEntityScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete'
    if not MEM and calls!=PLAIN: continue
    if MEM and (calls==PLAIN or not calls.endswith('00f35b40=CEntityScriptBase::~CEntityScriptBase ; 00bfe9bc=MSVCR71.DLL::operator_delete')): continue
    if only and addr not in only: continue
    allocs=[x.split('=',1) for x in st.split(' ; ') if x.endswith('::Alloc')] if st else []
    if len(allocs)!=1:
        res.append((addr,'DEFER','storer: '+(st or 'none'))); continue
    aa,an=allocs[0]
    e=ent.get(aa)
    if not e: res.append((addr,'DEFER','Alloc %s not landed'%aa)); continue
    src=open(ROOT+'/rebuild/src/compiled/'+e['Source'],encoding='utf-8').read()
    m=re.search(r'^//\s*Leaf:\s*(\?Alloc@(\S+?)@@SI\S*)',src,re.M)
    if not m: res.append((addr,'DEFER','Alloc %s leaf unparsed'%aa)); continue
    allocsym=m.group(1); q=m.group(2)  # Entity@Script@NScript
    parts=q.split('@')
    if len(parts)!=3 or parts[2]!='NScript': res.append((addr,'DEFER','Alloc %s qual %s'%(aa,q))); continue
    entc,scr=parts[0],parts[1]
    qual='NScript::%s::%s'%(scr,entc)
    if an!=qual+'::Alloc': res.append((addr,'DEFER','Alloc catalog name %s != leaf %s'%(an,qual))); continue
    gsym='??_G%s@@UAEPAXI@Z'%q
    pd=[]
    for s,tag in (('??_G%s@@UAEPAXI@Z'%q,'??_G'),('??_E%s@@UAEPAXI@Z'%q,'??_E'),('??1%s@@UAE@XZ'%q,'??1')):
        if s in E: pd.append('Ego_r.pdb '+s)
        if s in F: pd.append('FableWin.pdb '+s)
    allocpdb=[n for n,S_ in (('Ego_r',E),('FableWin',F)) if allocsym in S_]
    if not pd: res.append((addr,'DEFER','no PDB dtor/deleting dtor for %s'%q)); continue
    if not allocpdb: res.append((addr,'DEFER','Alloc symbol not in PDB name sets')); continue
    # strip the Alloc header comment block (leading // lines)
    lines=src.split('\n'); i=0
    while i<len(lines) and lines[i].startswith('//'): i+=1
    body='\n'.join(lines[i:])
    # remove the user-declared entity destructor
    pat=re.compile(r'^[ \t]*virtual ~%s\(\);[^\n]*\n'%re.escape(entc),re.M)
    nd=len(pat.findall(body))
    if nd!=1: res.append((addr,'DEFER','Alloc source declares ~%s %d times'%(entc,nd))); continue
    body=pat.sub('',body)
    if re.search(r'~%s\s*\('%re.escape(entc),body): res.append((addr,'DEFER','another ~%s in Alloc source'%entc)); continue
    ln=int(cat[addr][2])
    hdr=[
"// %s::`scalar deleting destructor'(unsigned int flags) -- the implicit ~%s (%s), then free the entity when bit 0 is set"%(qual,entc,'no members with destructors, i.e. ~CEntityScriptBase' if not MEM else 'its members\' destructors, then ~CEntityScriptBase'),
"// Leaf: %s"%gsym,
"// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %d bytes, thiscall, `ret 4`; calls %s."%(addr.upper(),ln,calls.replace('calls=','')),
"// Naming wave 33 (family esb): slot 0 of vtable 0x%s (no RTTI: NScript TU), the vtable the landed Alloc 0x%s stores"%(vt.upper(),aa.upper()),
"//   (%s, %s); %s."%(allocsym,' / '.join(n+'.pdb' for n in allocpdb),'; '.join(pd)),
"//   Ego_r.pdb (release) has no out-of-line ??1 for the entity, and retail calls ~CEntityScriptBase directly: the entity destructor is implicit.",
"//   Was a free __fastcall CLandscapeBackgroundPatch_vector_deleting_destructor stand-in (catalog CVtable_%s)."%vt,
"// Shape: the deleting destructor is emitted with the vtable, i.e. in the TU that constructs the entity: the class model and the PDB Alloc",
"//   of the landed row 0x%s (rebuild/src/compiled/%s), its user-declared destructor dropped. The Alloc body is not part of this graded body."%(aa.upper(),e['Source']),
]
    new='\n'.join(hdr)+'\n'+body
    d=os.path.dirname(cat_src:=ent[addr]['Source'])
    nsrc=d+'/'+fname(qual)+'_scalardeletingdestructor_'+addr+'.cpp'
    ntest=os.path.dirname(ent[addr]['TestSource'])+'/'+fname(qual)+'_scalardeletingdestructor_'+addr+'_test.cpp'
    p=os.path.join(outdir,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(new)
    res.append((addr,'OK',json.dumps(dict(qual=qual,name=qual+"::`scalar deleting destructor'",module=qual,sym=gsym,old_src=ent[addr]['Source'],new_src=nsrc,old_test=ent[addr]['TestSource'],new_test=ntest,vt=vt,alloc=aa,allocsym=allocsym,pdb=pd,allocpdb=allocpdb,old_module=ent[addr]['Module'],old_name=cat[addr][1]))))
with open(os.path.join(outdir,'gen.tsv'),'w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
import collections
print(collections.Counter(r[1] for r in res))
