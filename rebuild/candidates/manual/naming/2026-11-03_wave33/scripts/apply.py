# apply.py <pass.tsv> : pass.tsv = fam<TAB>addr<TAB>grade. Writes the respelled sources into the working tree (new file names),
# moves the tests, updates build_candidates.ps1 (Source/TestSource/Module), and writes the overlay copy + record rows.
import sys, os, re, json, shutil
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from common import *
W=os.environ['S']+'/wave33'
REC=ROOT+'/rebuild/candidates/manual/naming/2026-11-03_wave33'
rows=[l.rstrip('\n').split('\t') for l in open(sys.argv[1]) if l.strip()]
gen={}
for fam in set(r[0] for r in rows):
    for l in open(W+'/'+fam+'/gen.tsv'):
        a=l.rstrip('\n').split('\t')
        if a[1]=='OK': gen[(fam,a[0])]=json.loads(a[2])
ps=open(ROOT+'/rebuild/build_candidates.ps1',encoding='utf-8').read()
FAMNAME={'esb':'esb','esbm':'esbm','ctorA':'ctor','ctorB':'ctor','sf':'sf'}
def evidence(fam,d):
    if fam in ('esb','esbm'):
        return "naming wave 33 (%s): slot 0 of vtable %s (no RTTI), stored last by the landed PDB-named Alloc %s (%s in %s); %s; rechecked byte-identical"%(FAMNAME[fam],d['vt'],d['alloc'],d['allocsym'],'/'.join(d['allocpdb']),'; '.join(d['pdb']))
    if fam.startswith('ctor'):
        return "naming wave 33 (ctor): slot 0 of vtable %s (no RTTI), stored last by the landed PDB-named constructor %s (%s in %s); %s; rechecked byte-identical"%(d['vt'],d['ctor'],d['ctorsym'],'/'.join(d['ctorpdb']),'; '.join(d['pdb']))
    if fam=='sf':
        st=', '.join(s for s,_ in d['storers'][:6])+(' +%d'%(len(d['storers'])-6) if len(d['storers'])>6 else '')
        return "naming wave 33 (sf): slot 0 of vtable %s (no RTTI), stored by the landed spawning rows %s whose sources construct %s; %s; rechecked byte-identical"%(d['vt'],st,d['qual'],'; '.join(d['pdb']))
ren=[];lab=[]
for fam,a,grade in rows:
    d=gen[(fam,a)]
    blk=re.search(r"(\[pscustomobject\]@\{\n        Address = '%s'\n.*?\n    \})"%a,ps,re.S)
    assert blk, a
    b=blk.group(1); nb=b
    assert "Source = '%s'"%d['old_src'] in nb and "TestSource = '%s'"%d['old_test'] in nb, a
    nb=nb.replace("Source = '%s'"%d['old_src'],"Source = '%s'"%d['new_src'],1)
    nb=nb.replace("TestSource = '%s'"%d['old_test'],"TestSource = '%s'"%d['new_test'],1)
    nb=re.sub(r"Module = '[^']*'","Module = '%s'"%d['module'].replace("'","''"),nb,1)
    ps=ps.replace(b,nb,1)
    src=open(W+'/'+fam+'/src/'+d['new_src']).read()
    os_=ROOT+'/rebuild/src/compiled/'+d['old_src']; ns=ROOT+'/rebuild/src/compiled/'+d['new_src']
    ot=ROOT+'/rebuild/tests/'+d['old_test']; nt=ROOT+'/rebuild/tests/'+d['new_test']
    if os.path.exists(os_) and os_!=ns: os.remove(os_)
    open(ns,'w').write(src)
    if os.path.exists(ot) and ot!=nt: shutil.move(ot,nt)
    for base,rel,content in ((W+'/ov_final/rebuild/src/compiled/',d['new_src'],src),(W+'/ov_final/rebuild/tests/',d['new_test'],open(nt).read())):
        os.makedirs(os.path.dirname(base+rel),exist_ok=True); open(base+rel,'w').write(content)
    ev=evidence(fam,d)
    ren.append('\t'.join([a,d['old_name'],d['name'],d['old_module'],d['module'],'rebuild/src/compiled/'+d['old_src'],'rebuild/src/compiled/'+d['new_src'],'rebuild/tests/'+d['old_test'],'rebuild/tests/'+d['new_test'],'%s; the row now defines %s (was a free __fastcall stand-in); %s'%(ev.split(':')[0],d['sym'],grade)]))
    lab.append('\t'.join([a,d['name'],ev+' (%s + PASS)'%grade]))
open(ROOT+'/rebuild/build_candidates.ps1','w',encoding='utf-8').write(ps)
os.makedirs(REC,exist_ok=True)
with open(W+'/renames.rows','a') as f: f.write(''.join(r+'\n' for r in ren))
with open(W+'/labels.rows','a') as f: f.write(''.join(r+'\n' for r in lab))
print('applied',len(rows))
