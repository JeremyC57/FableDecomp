# gen_opfmd.py <outdir>: entity OnPredicateFail bodies that set one CQ_SunnyvaleMasterData flag (PMasterData +0x18), named from engine/CQ_SunnyvaleMasterData.h
import sys,os,re,json,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
from capstone import *
ent=ps1_entries(); cat=catalog(); EV=evidence(); P=pdbmap(); md=Cs(CS_ARCH_X86,CS_MODE_32)
outdir=sys.argv[1]; os.makedirs(outdir,exist_ok=True)
MD=open(ROOT+'/rebuild/include/engine/CQ_SunnyvaleMasterData.h').read()
mdoff={int(o,16):(t,n) for t,n,o in re.findall(r'^\s*(bool|long)\s+(\w+);\s*//\s*\+0x([0-9a-fA-F]+)',MD,re.M)}
d=json.load(open(W+'/classify.json'))
res=[]
for r in d:
    if r.get('M')!='OnPredicateFail' or int(r['len']) not in (8,61): continue
    a=r['addr']; ins=[(i.mnemonic,i.op_str) for i in md.disasm(bytes.fromhex(cat[a][3]),int(a,16))]
    lds=[o for m_,o in ins if m_=='mov' and re.match(r'eax, dword ptr \[e[cs][xi] \+ 0x18\]',o)]
    st=[re.match(r'byte ptr \[eax \+ (0x[0-9a-f]+)\], ([01])',o) for m_,o in ins if m_=='mov' and o.startswith('byte ptr [eax')]
    if len(lds)!=1 or len(st)!=1 or not st[0]: continue
    off=int(st[0].group(1),16); val=st[0].group(2)
    O,E=r['O'],r['E']; qual='NScript::%s::%s'%(O,E)
    if off not in mdoff or mdoff[off][0]!='bool': res.append((a,'DEFER','CQ_SunnyvaleMasterData.h has no bool at +0x%x'%off)); continue
    mem=mdoff[off][1]
    sym=r['pdb'][0] if len(r['pdb'])==1 else None
    al=[x for x,l in r['alloc'] if l]
    if not sym or len(al)!=1: res.append((a,'DEFER','sym/Alloc')); continue
    asrc=open(ROOT+'/rebuild/src/compiled/'+ent[al[0]]['Source'],errors='replace').read()
    _,tu=strip_head(asrc); tu,cpf=fix_cp(tu)
    if not re.search(r'CQ_SunnyvaleMasterData\*\s*PMasterData;\s*//\s*\+0x18',tu): res.append((a,'DEFER','Alloc model: no PMasterData +0x18')); continue
    stx=re.search(r'\bclass\s+(?:\w+::)?%s\s*(:[^{;]*)?\{'%re.escape(E),tu); end=block(tu,stx.end()-1); ls=tu.rfind('\n',0,end)+1
    tu=tu[:ls]+'private:\n    virtual void OnPredicateFail();   // PDB %s (slot 5)\n'%sym+tu[ls:]
    q61=int(r['len'])==61
    body=('    if (Me.MsgIsKilledBy(""))\n        PMasterData->%s = %s;\n' if q61 else '    PMasterData->%s = %s;\n')%(mem,'true' if val=='1' else 'false')
    pre=('#define FABLE_RETAIL_CCHARSTRING\n' if q61 else '')+'#define FABLE_RETAIL_NSCRIPT\n#include "engine/CQ_SunnyvaleMasterData.h"   // the master data the entity writes (members at their retail offsets)\n'
    txt=pre+tu+('\n' if not tu.endswith('\n') else '')+'\nvoid %s::OnPredicateFail()\n{\n%s}\n'%(qual,body)
    s_,_=qcheck(a,txt,sym)
    if not s_.startswith(('MATCH','RELOCATION')): res.append((a,'DEFER','model TU: '+s_)); continue
    ev=EV[a]; e=ent[a]
    hdr=['// %s::OnPredicateFail() -- %sset CQ_SunnyvaleMasterData::%s'%(qual,'the entity was killed (Me.MsgIsKilledBy("")): ' if q61 else '',mem),
         '// Leaf: %s'%sym,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %s bytes%s; PMasterData +0x18 -> %s +0x%x.'%(a.upper(),cat[a][2],', CScriptThing slot 18 MsgIsKilledBy (+0x48) on Me with a temporary CCharString("") (0x0099EBF0 / ~ 0x0099EAE0)' if q61 else '',mem,off),
         '// Identity (naming wave 35, family opfmd): %s.'%ev[1].replace('; LANDED row (respell source)',''),
         '//   %s in %s (%s). The flag is named by engine/CQ_SunnyvaleMasterData.h (bool %s; // +0x%x).'%(sym,pdbs(sym),ev[2],mem,off),
         '// Was: %s (catalog stand-in %s).'%(first_comment(open(ROOT+'/rebuild/src/compiled/'+e['Source'],errors='replace').read()),e['Module']),
         '// Class: the class model of the landed Alloc row 0x%s (rebuild/src/compiled/%s); the PDB member declaration added%s.'%(al[0].upper(),ent[al[0]]['Source'],'; its local CCountedPointer stand-in replaced by engine/CCountedPointer.h (wave 33 D4)' if cpf else '')]
    new='\n'.join(hdr)+'\n'+txt
    dd=os.path.dirname(e['Source']); base=fname(qual)+'_OnPredicateFail_'+a
    nsrc=dd+'/'+base+'.cpp'; ntest=os.path.dirname(e['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(outdir,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(new)
    res.append((a,'OK',json.dumps(dict(qual=qual,name='OnPredicateFail',module=qual,sym=sym,path='A',old_src=e['Source'],new_src=nsrc,old_test=e['TestSource'],new_test=ntest,old_module=e['Module'],cat=r['cn']))))
with open(os.path.join(outdir,'gen.tsv'),'w') as f:
    for x in res: f.write('\t'.join(x)+'\n')
print(collections.Counter(x[1] for x in res)); [print(x) for x in res if x[1]=='DEFER']
