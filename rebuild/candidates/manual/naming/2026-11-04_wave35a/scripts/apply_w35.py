# apply_w35.py <W> : apply PASS rows of a wave-35 overlay (RENAMES.tsv) to the working tree
import sys,os,csv,re,subprocess,shutil
W=sys.argv[1]
ren={r['address']:r for r in csv.DictReader(open(W+'/RENAMES.tsv'),delimiter='\t')}
ok=[l.split('\t')[0] for l in open(W+'/PASS.tsv').read().splitlines()[1:] if 'PASS' in l]
ps_p='rebuild/build_candidates.ps1'; ps=open(ps_p,encoding='utf-8',newline='').read()
done=0;miss=[]
for a in ok:
    r=ren.get(a)
    if not r: miss.append((a,'no rename row')); continue
    for kind in ('source','test'):
        old=r['old_'+kind]; new=r['new_'+kind]; ov=W+'/overlay/'+new
        if not os.path.exists(ov): miss.append((a,'no overlay '+new)); break
        if old!=new and os.path.exists(old):
            subprocess.run(['git','mv','-k',old,new],check=True)
        shutil.copyfile(ov,new)
    else:
        rel_s=r['new_source'].replace('rebuild/src/compiled/',''); rel_t=r['new_test'].replace('rebuild/tests/','')
        m=re.search(r"(Address = '%s'.*?)(\n\s*\})"%a,ps,re.S)
        blk=m.group(1); nb=re.sub(r"Source = '[^']*'","Source = '%s'"%rel_s,blk,count=1)
        nb=re.sub(r"TestSource = '[^']*'","TestSource = '%s'"%rel_t,nb,count=1)
        nb=re.sub(r"Module = '[^']*'","Module = '%s'"%r['new_module'].replace("'","''"),nb,count=1)
        ps=ps.replace(blk,nb,1); done+=1
open(ps_p,'w',encoding='utf-8',newline='').write(ps)
print('applied',done,'problems',len(miss)); [print(x) for x in miss[:10]]
