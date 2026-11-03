import os,re,subprocess,sys
S=os.environ['S']; D=S+'/w22/chunk732'
stub=D+'/stubinc'
macs=set()
for root,_,fs in os.walk(D+'/hdr'):
    for f in fs:
        macs|=set(re.findall(r'FABLE_RETAIL_\w+',open(os.path.join(root,f),errors='replace').read()))
hdrs=[os.path.relpath(os.path.join(r,f),D+'/hdr') for r,_,fs in os.walk(D+'/hdr') for f in fs]
names=set(os.path.basename(h) for h in hdrs)
cands=[]
for root,_,fs in os.walk('rebuild/src/compiled'):
    for f in fs:
        if not f.endswith('.cpp'): continue
        p=os.path.join(root,f); t=open(p,errors='replace').read()
        if any(n in t for n in names) or any(('define '+m) in t for m in macs): cands.append(p)
print('candidates',len(cands),file=sys.stderr)
def cpp(inc,p):
    while True:
        r=subprocess.run(['cpp','-P','-nostdinc','-undef','-D_MSC_VER=1310','-D_M_IX86=600','-D_WIN32','-I',inc,'-I',stub,p],capture_output=True,text=True)
        m=re.search(r"fatal error: ([^:]+): No such file",r.stderr)
        if not m: return r.stdout
        q=os.path.join(stub,m.group(1)); os.makedirs(os.path.dirname(q),exist_ok=True); open(q,'w').close()
norm=lambda s:[re.sub(r'fable_static_assert_\d+','SA',l) for l in s.splitlines() if l.strip()]
for p in cands:
    a=norm(cpp(D+'/base',p)); b=norm(cpp(D+'/mirror',p))
    if a!=b: print(p)
