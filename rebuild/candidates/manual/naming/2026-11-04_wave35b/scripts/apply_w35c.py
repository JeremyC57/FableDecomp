import sys,os,csv,re,subprocess,shutil
W=sys.argv[1]; dry=len(sys.argv)>2
def fs(p): return p if p.startswith('rebuild/') else 'rebuild/src/compiled/'+p
def ft(p): return p if p.startswith('rebuild/') else 'rebuild/tests/'+p
ren={}
for r in csv.DictReader(open(W+'/RENAMES.tsv'),delimiter='\t'):
    r={**r,'old_source':fs(r['old_source']),'new_source':fs(r['new_source']),'old_test':ft(r['old_test']),'new_test':ft(r['new_test'])}
    ren[r['address']]=r
ok=[l.split('\t')[0] for l in open(W+'/PASS.tsv').read().splitlines()[1:] if 'PASS' in l]
ps_p='rebuild/build_candidates.ps1'; ps=open(ps_p,encoding='utf-8',newline='').read()
prob=[];done=0;cov=set()
for a in ok:
    r=ren.get(a)
    if not r: continue
    for kind in ('source','test'):
        old=r['old_'+kind]; new=r['new_'+kind]; ov=W+'/overlay/'+new; cov.add(new)
        if not os.path.exists(ov): prob.append((a,'no overlay',new)); continue
        if not os.path.exists(old) and not os.path.exists(new): prob.append((a,'no repo file',old)); continue
        if dry: continue
        if old!=new and os.path.exists(old): subprocess.run(['git','mv','-k',old,new],check=True)
        shutil.copyfile(ov,new)
    m=re.search(r"(Address = '%s'.*?)(\n\s*\})"%a,ps,re.S)
    if not m: prob.append((a,'no ps1 block')); continue
    blk=m.group(1)
    nb=re.sub(r"Source = '[^']*'","Source = '%s'"%r['new_source'].replace('rebuild/src/compiled/',''),blk,count=1)
    nb=re.sub(r"TestSource = '[^']*'","TestSource = '%s'"%r['new_test'].replace('rebuild/tests/',''),nb,count=1)
    nb=re.sub(r"Module = '[^']*'","Module = '%s'"%r['new_module'].replace("'","''"),nb,count=1)
    ps=ps.replace(blk,nb,1); done+=1
# in-place files (callers, headers)
inplace=0
for d,_,fl in os.walk(W+'/overlay/rebuild'):
    for f in fl:
        p=os.path.relpath(os.path.join(d,f),W+'/overlay')
        if p in cov: continue
        if not os.path.exists(p): prob.append(('-','overlay-only file',p)); continue
        inplace+=1
        if not dry: shutil.copyfile(os.path.join(d,f),p)
if not dry: open(ps_p,'w',encoding='utf-8',newline='').write(ps)
print('renamed',done,'inplace',inplace,'problems',len(prob)); [print(x) for x in prob[:40]]
