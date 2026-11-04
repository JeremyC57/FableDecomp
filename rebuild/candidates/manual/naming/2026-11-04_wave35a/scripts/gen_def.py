# gen_def.py <outdir>: CDefClassBase-derived slots 19 Copy / 20 GetSizeofClass / 0 ??_G as the real members
import sys,os,re,json,collections
sys.path.insert(0,os.path.dirname(os.path.abspath(__file__)))
from lib35 import *
ent=ps1_entries(); cat=catalog(); EV=evidence(); P=pdbmap()
outdir=sys.argv[1]; only=set(sys.argv[2:]); os.makedirs(outdir,exist_ok=True)
byname={}
for a,q in cat.items(): byname.setdefault(q[1],[]).append(a)
def landed_tu(name):
    xs=[x for x in byname.get(name,[]) if x in ent]
    if len(xs)!=1: return None
    s=open(ROOT+'/rebuild/src/compiled/'+ent[xs[0]]['Source'],errors='replace').read()
    m=re.search(r'^//\s*Leaf:\s*(\S+)',s,re.M)
    return xs[0],(m.group(1) if m else None),strip_head(s)[1]
res=[]
for l in list(open(W+'/ROWS.tsv'))[1:]:
    a,cn,leaf,src=l.rstrip('\n').split('\t')
    m=re.match(r"^(C\w+Def)::(Copy|GetSizeofClass|`scalar deleting destructor')$",cn)
    if not m or (only and a not in only): continue
    C,M=m.groups()
    sym={'Copy':'?Copy@%s@@UAEXPBVCDefClassBase@@@Z'%C,'GetSizeofClass':'?GetSizeofClass@%s@@UAEKXZ'%C}.get(M,'??_G%s@@UAEPAXI@Z'%C)
    pd=[s for s in (sym,'??_E%s@@UAEPAXI@Z'%C,'??1%s@@UAE@XZ'%C) if s in P]
    if not pd: res.append((a,'DEFER','PDB lacks %s'%sym)); continue
    defn={'Copy':'void %s::Copy(const CDefClassBase* other)\n{\n    *this = *static_cast<const %s*>(other);\n}\n'%(C,C),
          'GetSizeofClass':'unsigned long %s::GetSizeofClass()\n{\n    return sizeof(%s);\n}\n'%(C,C)}.get(M,'')
    cands=[]
    if M!='Copy' and M!='GetSizeofClass':
        for nm,what in (('%s::%s'%(C,C),'constructor'),):
            t=landed_tu(nm)
            if t and t[1] and t[1].startswith('??0%s@@'%C): cands.append(('the landed %s row 0x%s (%s, rebuild/src/compiled/%s)'%(what,t[0].upper(),t[1],ent[t[0]]['Source']),t[2],ent[t[0]].get('CompilerFlags','')))
    else:
        h='engine/%s.h'%C
        if os.path.exists(ROOT+'/rebuild/include/'+h):
            for pl,s_ in prelude_for(h)[:4]: cands.append(('the shared header %s (includes as the landed includer rebuild/src/compiled/%s)'%(h,s_.split('rebuild/src/compiled/')[-1]),'\n'.join(pl)+'\n',''))
        for nm,what in (('%s::operator='%C,'operator='),('%s::%s'%(C,C),'constructor')):
            t=landed_tu(nm)
            if t: cands.append(('the class model of the landed %s row 0x%s (rebuild/src/compiled/%s)'%(what,t[0].upper(),ent[t[0]]['Source']),t[2],ent[t[0]].get('CompilerFlags','')))
    done=None; why=[]
    for desc,tu,fl in cands:
        extra=[f for f in fl.split() if f not in ('/O2','/Oy','/W3')]
        tu2=tu; added=''
        decl={'Copy':'virtual void Copy(const CDefClassBase* other);','GetSizeofClass':'virtual unsigned long GetSizeofClass();'}.get(M)
        nb=nested(tu2,C)
        if decl and nb and not re.search(r'\b%s\s*\('%M,nb[1]):
            st=re.search(r'\bclass\s+%s\s*(:[^{;]*)?\{'%re.escape(C),tu2); end=block(tu2,st.end()-1); ls=tu2.rfind('\n',0,end)+1
            tu2=tu2[:ls]+'public:\n    %s   // PDB %s\n'%(decl,sym)+tu2[ls:]; added='; the PDB member declaration added'
        mo=re.search(r'^%s& %s::operator=\('%(C,C),tu2,re.M)
        if M=='Copy' and mo:   # retail places Copy (slot 19) just before operator=: defined first, so operator= stays a call (tail jmp)
            txt=tu2[:mo.start()]+defn+'\n'+tu2[mo.start():]
        else:
            txt=tu2+('\n' if not tu2.endswith('\n') else '')+defn
        desc_=desc+added
        st_,_=qcheck(a,txt,sym,extra)
        if st_.startswith(('MATCH','RELOCATION')): done=(desc_,txt,fl); break
        why.append(st_)
    if not done: res.append((a,'DEFER','no class model reproduces it (%s)'%(' / '.join(why) or 'no landed operator= / ctor row, no header'))); continue
    desc,txt,fl=done; ev=EV[a]; e=ent[a]
    short={'Copy':'Copy(const CDefClassBase* other) -- slot 19: *this = the other def (tail call to operator=)','GetSizeofClass':'GetSizeofClass() -- slot 20: sizeof(%s) = 0x%s'%(C,cat[a][3][2:4])}.get(M,"`scalar deleting destructor'(unsigned int flags) -- slot 0")
    hdr=['// %s::%s'%(C,short),'// Leaf: %s'%sym,
         '// Retail: Fable.exe (TLC, VC7.1) @ 0x%s, %s bytes: %s.'%(a.upper(),cat[a][2],cat[a][3]),
         '// Identity (naming wave 35, family def): %s.'%ev[1],
         '//   %s in %s (%s); PDB also: %s.'%(sym,pdbs(sym),ev[2],', '.join(x for x in pd if x!=sym) or '-'),
         '// Was: %s (catalog stand-in %s).'%(first_comment(open(ROOT+'/rebuild/src/compiled/'+e['Source'],errors='replace').read()),e['Module']),
         '// Class: %s; only this member is graded.'%desc]
    new='\n'.join(hdr)+'\n'+txt
    mm={'Copy':'Copy','GetSizeofClass':'GetSizeofClass'}.get(M,'scalardeletingdestructor')
    dd=os.path.dirname(e['Source']); base=fname(C)+'_'+mm+'_'+a
    nsrc=dd+'/'+base+'.cpp'; ntest=os.path.dirname(e['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(outdir,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'w').write(new)
    res.append((a,'OK',json.dumps(dict(qual=C,name=M,module=C,sym=sym,path='def',flags=fl,old_src=e['Source'],new_src=nsrc,old_test=e['TestSource'],new_test=ntest,old_module=e['Module'],cat=cn))))
with open(os.path.join(outdir,'gen.tsv'),'w') as f:
    for x in res: f.write('\t'.join(x)+'\n')
print(collections.Counter(x[1] for x in res))
for x in res:
    if x[1]=='DEFER': print(x[0],x[2][:200])
