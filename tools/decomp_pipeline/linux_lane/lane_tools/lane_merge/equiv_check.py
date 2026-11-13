"""equiv_check.py <old-include-tree> <new-include-tree> <out.tsv> [addr-file]: for every landed row, preprocess its source and its test with
gcc cpp against both include trees and report the rows whose preprocessed text differs. Identical text means the compiler sees the same
input, so the row's bytes cannot have changed; only the differing rows need a real VC7.1 recheck. Never raises."""
import os,re,subprocess,sys,hashlib
from concurrent.futures import ThreadPoolExecutor
S=os.environ['S']; old,new,out=sys.argv[1:4]; only=set(open(sys.argv[4]).read().split()) if len(sys.argv)>4 else None
stub=S+'/stubinc_equiv'; os.makedirs(stub,exist_ok=True)
bc=open('rebuild/build_candidates.ps1',encoding='utf-8',errors='replace').read(); rows=[]
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}",bc,re.S):
    d=dict(re.findall(r"(\w+) = '([^']*)'",m.group(1)))
    if 'Address' in d and 'Source' in d:
        a=d['Address'].lower()
        if only is None or a in only: rows.append((a,'rebuild/src/compiled/'+d['Source'],'rebuild/tests/'+d['TestSource'] if d.get('TestSource') else None))
def pp(tree,f):
    args=['cpp','-P','-nostdinc','-undef','-D_MSC_VER=1310','-D_M_IX86=600','-D_WIN32','-x','c++','-I',tree,'-I',stub,f]
    for _ in range(60):
        r=subprocess.run(args,capture_output=True,text=True,errors='replace')
        m=re.search(r"fatal error: ([^:\n]+): No such file",r.stderr)
        if not m: break
        p=os.path.join(stub,m.group(1).strip())
        if '..' in p: break
        os.makedirs(os.path.dirname(p),exist_ok=True); open(p,'a').close()
    err=re.sub(r'^.*?(error|warning)','\\1',r.stderr,flags=re.M)
    err=err.replace(tree,'TREE')
    body=re.sub(r'fable_static_assert_\d+','fable_static_assert_N',' '.join(r.stdout.split()))   # the assert typedef name carries __LINE__      # token text only: blank lines and spacing differ when unrelated opt-in blocks are added
    return hashlib.sha1((body+'\n##ERR##\n'+'\n'.join(sorted(set(l for l in err.split('\n') if 'error' in l)))).encode()).hexdigest(), ('error' in r.stderr)
def one(row):
    a,src,tst=row; res=[]
    try:
        for kind,f in (('src',src),('test',tst)):
            if not f or not os.path.exists(f): continue
            ho,eo=pp(old,f); hn,en=pp(new,f)
            if ho!=hn: res.append(f'{a}\t{kind}\tDIFF\t{"cpp-error-new" if en and not eo else ""}\t{f}')
    except Exception as e: res.append(f'{a}\t?\tERROR\t{type(e).__name__}\t{src}')
    return res
n=0
with open(out,'w') as o, ThreadPoolExecutor(4) as ex:
    for res in ex.map(one,rows,chunksize=16):
        n+=1
        for l in res: o.write(l+'\n'); o.flush()
print('rows checked',n,'-> differing lines in',out)
