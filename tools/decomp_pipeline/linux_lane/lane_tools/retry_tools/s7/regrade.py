# regrade.py <worker> <nworkers> <listfile> <outfile>: grade src/<addr>.cpp for each addr, output addr status ndiff rN bN
import sys,os,re,subprocess,difflib
S=os.environ["S"]; W=f"{S}/w22/retry10"
wk,nw,lst,out=int(sys.argv[1]),int(sys.argv[2]),sys.argv[3],sys.argv[4]
addrs=[l.split()[0] for l in open(lst) if l.strip()]
leafs={}
for l in open(f"{W}/rows.tsv"):
    f=l.rstrip("\n").split("\t"); leafs[f[0]]=f[2]
env=dict(os.environ); env["FABLE_VC71"]=f"{S}/wine/vc71rtm"; env["LANDVERIFY_WORK"]=f"{S}/wine/landverify_rtm_w{wk}"; env["LINK"]="bufferoverflowu.lib"
def norm(s,addr):
    def rel(m):
        t=int(m.group(2),16); return f"{m.group(1)} {'+' if t>=addr else '-'}{abs(t-addr):x}"
    s=re.sub(r"\b(j\w+|loop\w*)\s+(0x[0-9a-f]+)",rel,s)
    s=re.sub(r"\b(call)\s+0x[0-9a-f]+","\\1 X",s)
    return s
def f(L):
    o=[]
    for l in L:
        p=l.split()
        if len(p)<3: continue
        try: a=int(p[0],16)
        except: continue
        o.append(norm(" ".join(p[2:]),a))
    return o
with open(out,"a") as fo:
    for i,a in enumerate(addrs):
        if i%nw!=wk: continue
        src=f"{W}/src/{a}.cpp"
        if not os.path.exists(src): fo.write(f"{a}\tNOSRC\n"); continue
        txt=open(src,errors="replace").read()
        m=re.search(r"^// Leaf:\s*(\S+)",txt,re.M); leaf=m.group(1) if m else leafs.get(a,"")
        fm=re.search(r"^// Flags:\s*(.*)$",txt,re.M); fl=fm.group(1).split() if fm else []
        cmd=["python3",f"{S}/tools/q.py",a,src,leaf]+(["--flags"]+fl if fl else [])
        try: r=subprocess.run(cmd,capture_output=True,text=True,env=env,timeout=300,cwd="/home/user/FableDecomp").stdout
        except Exception as e: fo.write(f"{a}\tTIMEOUT\n"); fo.flush(); continue
        l1=r.splitlines()[0] if r else "EMPTY"
        st=l1.split()[2] if len(l1.split())>2 else l1
        nd=""
        if "--- retail" in r and "--- built" in r:
            rr=f(re.findall(r"--- retail.*?(?=--- built)",r,re.S)[0].splitlines()[1:])
            bb=f(re.findall(r"--- built.*",r,re.S)[0].splitlines()[1:])
            nd=sum(1 for l in difflib.unified_diff(rr,bb,lineterm="",n=0) if l[:1] in "+-" and not l.startswith(("---","+++")))
            nd=f"{nd}\t{len(rr)}\t{len(bb)}"
        fo.write(f"{a}\t{st}\t{nd}\n"); fo.flush()
