# gen_requal.py <outdir> [addr...]: MEMBER / NSWRONG / ALREADY / FREE(Main) rows -> the proven NScript member, the row's own layout model kept
import sys,re,json,os
from common35 import *
from requal import requalify, Fail
out=os.path.abspath(sys.argv[1]); only=set(sys.argv[2:]); os.makedirs(out,exist_ok=True)
ent=ps1_entries(); cat=catalog(); C=cats(); EV=evidence()
res=[]
def header(s,name,t,a,sym,notes,kind):
    lines=s.split('\n'); i=0
    while i<len(lines) and lines[i].startswith('//'): i+=1
    hdr=lines[:i]; rest=lines[i:]
    l1=hdr[0] if hdr else ''
    desc=l1.split(' -- ',1)[1] if ' -- ' in l1 else re.sub(r'^//\s*','',l1)
    newl1='// %s()%s'%(name,(' -- '+desc) if desc else '')
    hdr=[h for h in hdr[1:] if not h.startswith('// Leaf:')]
    evf,evs=EV.get(a,('',''))
    ln=int(cat[a][2])
    add=['// Leaf: %s'%t]+hdr+[
      '// Naming wave 35: identity %s (%s); PDB %s in %s.'%(short_ev(evs),'/'.join(evf.split('/')[-2:]),t,pdbwhere(t)),
      '//   Was %s; this source now defines the PDB member (%s); same bytes.'%({'ALREADY':'already this symbol, with a bare `// Leaf:` line and the old file name','FREE':'the free __fastcall stand-in `%s`'%sym}.get(kind,'`%s`'%sym),'; '.join(notes) if notes else 'class model unchanged')]
    return '\n'.join([newl1]+add+rest)
for a,(addr,k,sym,t,name) in sorted(C.items()):
    if only and a not in only: continue
    if k not in('MEMBER','NSWRONG','ALREADY') or name.endswith('::Alloc'): continue
    classes,meth=split_target(name)
    acc='private' if t.endswith('EAEXXZ') else 'public'
    s=rowsrc(ent[a]['Source'])
    try:
        if k=='ALREADY': ns,notes=s,[]
        elif k=='FREE':
            m=re.match(r'^\?(\w+)@@YIXPA[UV](\w+)@@@Z$',sym)
            if not m: raise Fail('free symbol shape %s'%sym)
            ns,notes=requalify(s,m.group(2),m.group(1),classes,meth,acc,free=True,pdbsym=t)
        else:
            parts=sym.split('@@')[0][1:].split('@')
            cm,cn=parts[0],parts[1]
            if k=='NSWRONG' and len(parts)==4 and parts[1]!=classes[-1]: raise Fail('NSWRONG wrong inner %s (manual)'%sym)
            ns,notes=requalify(s,cn,cm,classes,meth,acc,pdbsym=t)
    except Fail as e:
        res.append((a,'DEFER',str(e))); continue
    ns=header(ns,name,t,a,sym,notes,k)
    newmod='NScript::'+'::'.join(classes)
    base=fname(newmod)[:80]+'_'+meth+'_'+a
    nsrc=os.path.dirname(ent[a]['Source'])+'/'+base+'.cpp'; ntest=os.path.dirname(ent[a]['TestSource'])+'/'+base+'_test.cpp'
    p=os.path.join(out,'src',nsrc); os.makedirs(os.path.dirname(p),exist_ok=True)
    open(p,'w').write(ns)
    res.append((a,'OK',json.dumps(dict(path=p,leaf=t,module=newmod,new_src=nsrc,new_test=ntest,old_src=ent[a]['Source'],old_test=ent[a]['TestSource'],old_sym=sym,fam='requal-'+k,notes=notes))))
with open(out+'/gen.tsv','w') as f:
    for r in res: f.write('\t'.join(r)+'\n')
import collections; print(collections.Counter(r[1] for r in res))
print(collections.Counter(r[2].split(' ')[0]+' '+' '.join(r[2].split(' ')[2:4]) for r in res if r[1]!='OK').most_common(20))
