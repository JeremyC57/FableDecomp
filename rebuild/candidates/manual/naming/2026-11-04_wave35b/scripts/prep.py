# prep.py <famdir> <nparts> [addr...]: split OK rows of famdir/gen.tsv into rk_part_NN payloads
import sys,json,os,subprocess,re
fam=os.path.abspath(sys.argv[1]); n=int(sys.argv[2]); only=set(sys.argv[3:])
ok=[]
for l in open(fam+'/gen.tsv'):
    a=l.rstrip('\n').split('\t')
    if a[1]=='OK' and (not only or a[0] in only):
        j=json.loads(a[2]); t=open(j['path']).read()
        t2=re.sub(r'"(?:[^"\\\n]|\\.)*"|\'(?:[^\'\\\n]|\\.)*\'','""',re.sub(r'//[^\n]*','',re.sub(r'/\*.*?\*/','',t,flags=re.S)))
        if t2.count('{')!=t2.count('}') or t2.count('(')!=t2.count(')'): print('UNBALANCED (skipped)',a[0]); continue
        ok.append((a[0],j))
import shutil
for d in os.listdir(fam):
    if d.startswith('rk_part_'): shutil.rmtree(fam+'/'+d,ignore_errors=True) if os.path.isdir(fam+'/'+d) else os.remove(fam+'/'+d)
for i in range(n):
    part=ok[i::n]
    if not part: continue
    d=fam+'/rk_part_%02d'%i; os.makedirs(d,exist_ok=True)
    with open(d+'/gen.tsv','w') as f:
        for a,j in part: f.write('\t'.join([a,j['path'],j['leaf'],j['module'],j.get('test_path','')])+'\n')
    subprocess.run(['python3',os.path.dirname(os.path.abspath(__file__))+'/mkpay.py',d,d+'/gen.tsv'],check=True)
