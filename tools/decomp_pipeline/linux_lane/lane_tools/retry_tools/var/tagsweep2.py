# tagsweep2.py <addr>...: drop stand-in stores through params whose type is an empty struct in the TU (tags, __false_type ...), grade
import sys,re,os,subprocess
S=os.environ["S"]; W=f"{S}/w22/retry10"
matched=set(l.split("\t")[0] for l in open(f"{W}/STATUS.tsv") if re.search(r"\t(RELOCATION_)?MATCH\t",l))
def empty_types(src):
    t=set()
    for m in re.finditer(r"struct\s+(\w+)\s*\{((?:[^{}]|\{[^{}]*\})*)\}\s*;",src):
        body=re.sub(r"//.*","",m.group(2))
        body=re.sub(r"\b%s\s*\(\s*\)\s*\{\s*\}"%re.escape(m.group(1)),"",body)
        if not body.strip(): t.add(m.group(1))
    return t
def strip(src):
    et=empty_types(src)
    if not et: return None
    names=set()
    for m in re.finditer(r"noinline\)[^\n{;]*\(([^)]*)\)",src):
        for p in m.group(1).split(","):
            p=p.strip(); w=re.findall(r"\w+",p)
            if len(w)>=2 and any(x in et for x in w[:-1]): names.add(w[-1])
    if not names: return None
    out=re.sub(r"^\s*\*\(void\*\*\)&?(%s)\s*=\s*p;\s*\n"%"|".join(map(re.escape,names)),"",src,flags=re.M)
    return out if out!=src else None
for a in sys.argv[1:]:
    if a in matched: continue
    best=None
    for suf in ("ref","combo","lb","def3","def"):
        f=f"{W}/var/{a}_{suf}.cpp"
        if not os.path.exists(f): continue
        new=strip(open(f).read())
        if new is None: continue
        nf=f"{W}/var/{a}_{suf}notag2.cpp"; open(nf,"w").write(new)
        try: r=subprocess.run(["python3",f"{W}/var/pv.py",a,nf],capture_output=True,text=True,timeout=600).stdout
        except Exception as e: r=f"timeout {e}"
        m=re.search(r"diff lines: (\d+)",r); n=int(m.group(1)) if m else 999
        first=r.splitlines()[0][:90] if r else "?"
        print(f"{a} {suf}notag2 | {first} | diff lines: {n}",flush=True)
        if best is None or n<best[0]: best=(n,suf,nf,r)
        if n==0: break
    if best and best[0]==0:
        n,suf,nf,r=best
        src=f"{W}/src/{a}.cpp"; pre=f"{W}/var/{a}_pre_{suf}notag2.cpp"
        if os.path.exists(src): os.replace(src,pre)
        open(src,"w").write("".join(l for l in open(nf) if not l.startswith("// Grade:")))
        leaf=re.search(r"leaf=(\S+)",r).group(1)
        g=subprocess.run(["bash",f"{W}/g.sh",a,leaf],capture_output=True,text=True,timeout=600).stdout.splitlines()[0]
        print(f"{a} GRADE: {g}",flush=True)
        if "MATCH" not in g and os.path.exists(pre): os.replace(pre,src)
