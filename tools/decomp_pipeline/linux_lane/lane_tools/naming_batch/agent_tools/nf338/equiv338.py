"""equiv3.py <old-tree> <new-tree> <ctl-tree> <out.tsv> [addr-file]: NF319 variant of lane_merge/equiv_check.py (same cpp command, stub dir,
-idirafter and TREE path normalisation, exact token text). Per landed row source and test: DIFF when old v new preprocessed text differs
(the proof: must be 0); CTL <tokens> when the control tree (new tree + one planted `typedef int nf323_planted_<Header>;` per changed header,
outside every macro) shows planted tokens: the per-header includer census that a planted control must flag. Never raises."""
import os,re,subprocess,sys,hashlib
from concurrent.futures import ThreadPoolExecutor
S=os.environ['S']; old,new,ctl,out=sys.argv[1:5]; only=set(open(sys.argv[5]).read().split()) if len(sys.argv)>5 else None
stub=os.environ['NF_STUB']; os.makedirs(stub,exist_ok=True)
bc=open('rebuild/build_candidates.ps1',encoding='utf-8',errors='replace').read(); rows=[]
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}",bc,re.S):
    d=dict(re.findall(r"(\w+) = '([^']*)'",m.group(1)))
    if 'Address' in d and 'Source' in d:
        a=d['Address'].lower()
        if only is None or a in only: rows.append((a,'rebuild/src/compiled/'+d['Source'],'rebuild/tests/'+d['TestSource'] if d.get('TestSource') else None))
def pp(tree,f):
    args=['cpp','-P','-nostdinc','-undef','-D_MSC_VER=1310','-D_M_IX86=600','-D_WIN32','-x','c++','-I',tree,'-I',stub,'-idirafter',os.path.abspath('rebuild/include'),f]
    for _ in range(60):
        r=subprocess.run(args,capture_output=True,text=True,errors='replace')
        m=re.search(r"fatal error: ([^:\n]+): No such file",r.stderr)
        if not m: break
        p=os.path.join(stub,m.group(1).strip())
        if '..' in p: break
        os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'a').close()
    err=re.sub(r'^.*?(error|warning)','\\1',r.stderr,flags=re.M).replace(tree,'TREE')
    body=' '.join(r.stdout.split()).replace(tree,'TREE')
    return hashlib.sha1((body+'\n##ERR##\n'+'\n'.join(sorted(set(l for l in err.split('\n') if 'error' in l)))).encode()).hexdigest(), ('error' in r.stderr), body
def one(row):
    a,src,tst=row; res=[]
    try:
        for kind,f in (('src',src),('test',tst)):
            if not f or not os.path.exists(f): continue
            ho,eo,_=pp(old,f); hn,en,_=pp(new,f); _,_,bc_=pp(ctl,f)
            if ho!=hn: res.append(f'{a}\t{kind}\tDIFF\t{"cpp-error-new" if en and not eo else ""}\t{f}')
            tk=sorted(set(re.findall(r'nf323_planted_(\w+)',bc_)))
            if tk: res.append(f'{a}\t{kind}\tCTL\t{",".join(tk)}\t{f}')
    except Exception as e: res.append(f'{a}\t?\tERROR\t{type(e).__name__}\t{src}')
    return res
n=0
with open(out,'w') as o, ThreadPoolExecutor(3) as ex:
    for res in ex.map(one,rows,chunksize=16):
        n+=1
        for l in res: o.write(l+'\n'); o.flush()
    o.write('#DONE rows %d\n'%n)
print('rows checked',n,'->',out)
