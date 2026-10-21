# tagsweep.py <addr>...: for each row's ref/lb/combo/def3 variants, drop stores through empty-struct tag params, grade
import sys,re,os,subprocess,glob
S=os.environ["S"]; W=f"{S}/w22/retry10"
matched=set(l.split("\t")[0] for l in open(f"{W}/STATUS.tsv") if re.search(r"\t(RELOCATION_)?MATCH\t",l))
def strip_tags(src):
    names=set()
    for m in re.finditer(r"noinline\)[^\n{;]*\(([^)]*)\)",src):
        for p in m.group(1).split(","):
            p=p.strip()
            if re.search(r"Tag\s*&\s*\w+$",p) or re.search(r"\bS\w*Tag\s*&",p):
                names.add(p.split()[-1].lstrip("&*"))
    if not names: return None
    out=re.sub(r"^\s*\*\(void\*\*\)&(%s)\s*=\s*p;\s*\n"%"|".join(map(re.escape,names)),"",src,flags=re.M)
    return out if out!=src else None
for a in sys.argv[1:]:
    if a in matched: continue
    best=None
    for suf in ("ref","combo","lb","def3"):
        f=f"{W}/var/{a}_{suf}.cpp"
        if not os.path.exists(f): continue
        new=strip_tags(open(f).read())
        if new is None: continue
        nf=f"{W}/var/{a}_{suf}notag.cpp"; open(nf,"w").write(new)
        r=subprocess.run(["python3",f"{W}/var/pv.py",a,nf],capture_output=True,text=True,timeout=600).stdout
        m=re.search(r"diff lines: (\d+)",r); n=int(m.group(1)) if m else 999
        first=r.splitlines()[0][:90] if r else "?"
        print(f"{a} {suf}notag | {first} | diff lines: {n}",flush=True)
        if best is None or n<best[0]: best=(n,suf,nf)
        if n==0: break
    if best and best[0]==0:
        n,suf,nf=best
        src=f"{W}/src/{a}.cpp"; pre=f"{W}/var/{a}_pre_{suf}notag.cpp"
        os.replace(src,pre) if os.path.exists(src) else None
        open(src,"w").write("".join(l for l in open(nf) if not l.startswith("// Grade:")))
        leaf=re.search(r"leaf=(\S+)",r).group(1)
        g=subprocess.run(["bash",f"{W}/g.sh",a,leaf],capture_output=True,text=True,timeout=600).stdout.splitlines()[0]
        print(f"{a} GRADE: {g}",flush=True)
        if "MATCH" not in g and os.path.exists(pre): os.replace(pre,src)
