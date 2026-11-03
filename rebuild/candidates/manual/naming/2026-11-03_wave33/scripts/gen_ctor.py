# ctor family: vtable stored (last) by exactly one landed PDB-named constructor; the ??_G source is that ctor's TU
# usage: gen_ctor.py <outdir> <variant A|B> [addr...]
import sys, os, re, json, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import *
from pe import PE
p=PE(); rd=[s for s in p.secs if s[0]=='.rdata'][0]
ent=ps1_entries(); cat=catalog(); E,F=pdb()
outdir=sys.argv[1]; var=sys.argv[2]; only=set(sys.argv[3:])
os.makedirs(outdir,exist_ok=True)
res=[]
def splitq(n):
    out=[];d=0;cur=''
    i=0
    while i<len(n):
        c=n[i]
        if c=='<': d+=1
        elif c=='>': d-=1
        if d==0 and n[i:i+2]=='::': out.append(cur); cur=''; i+=2; continue
        cur+=c; i+=1
    out.append(cur); return out
def lastvt(st):
    b=bytes.fromhex(cat[st][3]); vts=[]
    for i in range(2,len(b)-3):
        v=struct.unpack_from('<I',b,i)[0]
        if rd[1]<=v<rd[1]+rd[4] and p.is_text(p.u32(v) or 0) and b[i-2]==0xc7 and (b[i-1]&0xC0)==0 and (b[i-1]&7) not in (4,5): vts.append(v)
    return vts
for l in open(os.environ['S']+'/wave33/ctor_scan.tsv'):
    addr,vt,calls,ct=l.rstrip('\n').split('\t'); ct=json.loads(ct)
    if only and addr not in only: continue
    if len(ct)!=1: continue
    st,lf=ct[0]
    if '@@' not in lf:
        cs=[x for x in (E|F) if x.startswith(lf) and re.match(r'^\?\?0(.+?)@@[QIA]AE',x)]
        qs=set(re.match(r'^\?\?0(.+?)@@',x).group(1) for x in cs)
        if len(qs)==1: lf=sorted(cs)[0]; lfnote='(Leaf prefix completed from the PDB name set)'
    m=re.match(r'^\?\?0(.+?)@@[QIA]AE',lf)
    if not m: res.append((addr,'DEFER','ctor leaf %s'%lf)); continue
    q=m.group(1)
    if q.startswith('?$'):
        pass
    cname=cat[st][1]
    if '::' not in cname: res.append((addr,'DEFER','ctor catalog name %s'%cname)); continue
    sp=splitq(cname)
    qual='::'.join(sp[:-1])
    last=sp[-2].split('<')[0] if len(sp)>1 else ''
    if not q.startswith('?$'):
        lq='::'.join(reversed(q.split('@')))
        if lq!=qual: catnote=' (the ctor row is catalogued as %s; its Leaf is the PDB symbol)'%cname
        else: catnote=''
        qual=lq; last=q.split('@')[0]
    else:
        catnote=''
        if sp[-1].split('<')[0]!=last or not q.startswith('?$'+last+'@'): res.append((addr,'DEFER','template ctor catalog name %s vs leaf %s'%(cname,lf))); continue
    vts=lastvt(st)
    if not vts or vts[-1]!=int(vt,16): res.append((addr,'DEFER','vtable %s not the last store of ctor %s'%(vt,st))); continue
    pd=[]
    for s in ('??_G%s@@UAEPAXI@Z'%q,'??_E%s@@UAEPAXI@Z'%q,'??1%s@@UAE@XZ'%q,'??1%s@@MAE@XZ'%q):
        if s in E: pd.append('Ego_r.pdb '+s)
        if s in F: pd.append('FableWin.pdb '+s)
    ctpdb=[n for n,S_ in (('Ego_r',E),('FableWin',F)) if lf in S_]
    if not pd: res.append((addr,'DEFER','no PDB dtor/deleting dtor for %s'%q)); continue
    if not ctpdb: res.append((addr,'DEFER','ctor symbol %s not in PDB name sets'%lf)); continue
    e=ent[st]
    src=open(ROOT+'/rebuild/src/compiled/'+e['Source'],encoding='utf-8').read()
    lines=src.split('\n'); i=0
    while i<len(lines) and lines[i].startswith('//'): i+=1
    body='\n'.join(lines[i:])
    note='its own source unchanged'
    if '#include "engine/CLevelScriptBase.h"' in body and 'FABLE_RETAIL_CLEVELSCRIPTBASE' not in body:
        body=body.replace('#include "engine/CLevelScriptBase.h"','#define FABLE_RETAIL_CLEVELSCRIPTBASE   // retail\'s implicit ~CLevelScriptBase<T> (chunk 713 opt-in): the ??_G calls ~CScriptBase 0x00CBD510\n#include "engine/CLevelScriptBase.h"',1)
        note='its own source with the FABLE_RETAIL_CLEVELSCRIPTBASE opt-in added'
    if var=='B':
        pat=re.compile(r'^[ \t]*(virtual )?~%s\(\)( throw\(\))?;[^\n]*\n'%re.escape(last),re.M)
        nd=len(pat.findall(body))
        if nd!=1: res.append((addr,'DEFER','variant B: ~%s declared %d times'%(last,nd))); continue
        body=pat.sub('',body); note='its user-declared destructor dropped (retail expands the destructor into the ??_G)'
    ln=int(cat[addr][2])
    gsym='??_G%s@@UAEPAXI@Z'%q
    callsdesc=calls.replace('calls=','')
    hdr=[
"// %s::`scalar deleting destructor'(unsigned int flags) -- the destructor (expanded), then free the object when bit 0 is set"%qual,
"// Leaf: %s"%gsym,
"// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %d bytes, thiscall, `ret 4`; calls %s."%(addr.upper(),ln,callsdesc),
"// Naming wave 33 (family %s): slot 0 of vtable 0x%s (no RTTI), the vtable the landed constructor 0x%s stores last"%(os.path.basename(outdir.rstrip('/')),vt.upper(),st.upper()),
"//   (%s, %s); %s."%(lf+catnote,' / '.join(n+'.pdb' for n in ctpdb),'; '.join(pd)),
"//   Was a free __fastcall stand-in (catalog %s)."%cat[addr][1],
"// Shape: the deleting destructor is emitted with the vtable, i.e. in the constructor's TU: the class model and the PDB constructor of",
"//   the landed row 0x%s (rebuild/src/compiled/%s), %s. The constructor body is not part of this graded body."%(st.upper(),e['Source'],note),
]
    new='\n'.join(hdr)+'\n'+body
    nsrc=os.path.dirname(ent[addr]['Source'])+'/'+fname(qual)+'_scalardeletingdestructor_'+addr+'.cpp'
    ntest=os.path.dirname(ent[addr]['TestSource'])+'/'+fname(qual)+'_scalardeletingdestructor_'+addr+'_test.cpp'
    pth=os.path.join(outdir,'src',nsrc); os.makedirs(os.path.dirname(pth),exist_ok=True); open(pth,'w').write(new)
    res.append((addr,'OK',json.dumps(dict(qual=qual,name=qual+"::`scalar deleting destructor'",module=qual,sym=gsym,old_src=ent[addr]['Source'],new_src=nsrc,old_test=ent[addr]['TestSource'],new_test=ntest,vt=vt,ctor=st,ctorsym=lf,pdb=pd,ctorpdb=ctpdb,old_module=ent[addr]['Module'],old_name=cat[addr][1],variant=var))))
import collections
cnt=collections.Counter(json.loads(r[2])['sym'] for r in res if r[1]=='OK')
res=[(r[0],'DEFER','symbol %s would name %d retail addresses (twin ctors share one Leaf: the storers are misnamed)'%(json.loads(r[2])['sym'],cnt[json.loads(r[2])['sym']])) if r[1]=='OK' and cnt[json.loads(r[2])['sym']]>1 else r for r in res]
with open(os.path.join(outdir,'gen.tsv'),'w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
import collections
print(collections.Counter(r[1] for r in res))
for r in res:
    if r[1]=='DEFER': print(r[0],r[2])
