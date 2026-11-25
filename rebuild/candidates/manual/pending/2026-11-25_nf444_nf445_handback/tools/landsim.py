#!/usr/bin/env python3
"""landsim.py <batch> <simroot>: reproduce land_scripted's recheck state outside the repo. <simroot>/rebuild/ gets: include/ (repo copy +
batch hdr/engine), build_candidates.ps1 (repo copy + CATALOG_CHANGES), oracles/auto-re-candidates.tsv (link), src/compiled/<HEAD Source>
= rows/<a>.cpp and tests/<HEAD TestSource> = tests/<a>_test.cpp (else the repo HEAD test) for every row. Then recheck.py (run with cwd =
<simroot>) builds the two payloads land_scripted builds: non-S rows and option S members (UNITS kind S)."""
import os,re,shutil,subprocess,sys
B,X=sys.argv[1],sys.argv[2]; R='/home/user/FableDecomp'
if os.path.exists(X): shutil.rmtree(X)
os.makedirs(X+'/rebuild/oracles'); shutil.copytree(R+'/rebuild/include',X+'/rebuild/include')
for root,_,fs in os.walk(B+'/hdr'):
    for f in fs:
        if f.endswith('.h'):
            rel=os.path.relpath(os.path.join(root,f),B+'/hdr'); os.makedirs(os.path.dirname(X+'/rebuild/include/'+rel),exist_ok=True)
            shutil.copy(os.path.join(root,f),X+'/rebuild/include/'+rel)
os.symlink(R+'/rebuild/oracles/auto-re-candidates.tsv',X+'/rebuild/oracles/auto-re-candidates.tsv')
ps=open(R+'/rebuild/build_candidates.ps1',encoding='utf-8-sig').read()
ch=[l.split('\t') for l in open(B+'/CATALOG_CHANGES.tsv').read().splitlines()[1:] if l]
def edit(ps,a,field,new):
    m=re.search(r"(\[pscustomobject\]@\{\n        Address = '%s'\n)(.*?)(\n    \})"%a,ps,re.S)
    body=m.group(2)
    if re.search(r"^        %s = '"%field,body,re.M): body=re.sub(r"^(        %s = ')[^']*'"%field,lambda mm:mm.group(1)+new+"'",body,flags=re.M)
    else: body=body+"\n        %s = '%s'"%(field,new)
    return ps[:m.start(2)]+body+ps[m.end(2):]
for a,field,old,new,*_ in ch: ps=edit(ps,a,field,new)
open(X+'/rebuild/build_candidates.ps1','w',encoding='utf-8').write(ps)
ent={}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}",ps,re.S):
    kv=dict(re.findall(r"(\w+) = '([^']*)'",blk))
    if 'Address' in kv: ent[kv['Address'].lower()]=kv
rows=open(B+'/rows.txt').read().split()
for a in rows:
    s=X+'/rebuild/src/compiled/'+ent[a]['Source']; os.makedirs(os.path.dirname(s),exist_ok=True); shutil.copy(f'{B}/rows/{a}.cpp',s)
    t=X+'/rebuild/tests/'+ent[a]['TestSource']; os.makedirs(os.path.dirname(t),exist_ok=True)
    shutil.copy(f'{B}/tests/{a}_test.cpp' if os.path.exists(f'{B}/tests/{a}_test.cpp') else R+'/rebuild/tests/'+ent[a]['TestSource'],t)
ul=open(B+'/UNITS.tsv').read().splitlines(); k=ul[0].split('\t').index('kind')
sS={f[0] for f in (l.split('\t') for l in ul[1:] if l) if len(f)>k and f[k].strip()=='S'}
for tag,grp in (('',[a for a in rows if a not in sS]),('_s',[a for a in rows if a in sS])):
    os.makedirs(f'{X}/p{tag}',exist_ok=True)
    r=subprocess.run(['python3',R+'/tools/decomp_pipeline/linux_lane/lane_tools/recheck.py',f'{X}/p{tag}']+grp,cwd=X,capture_output=True,text=True)
    print(tag or 'main',len(grp),r.stdout.strip()[-200:],r.stderr.strip()[-300:])
print('S members',sorted(sS))
