# rdsweep.py <addr>...: in stand-in bodies, turn stores through const-reference (IN) params into reads; grade; install on 0 diff
import sys,re,os,subprocess
S=os.environ["S"]; W=f"{S}/w22/retry10"
matched=set(l.split("\t")[0] for l in open(f"{W}/STATUS.tsv") if re.search(r"\t(RELOCATION_)?MATCH\t",l))
def conv(src):
    names=set()
    for m in re.finditer(r"noinline\)[^\n{;]*\(([^)]*)\)",src):
        for p in m.group(1).split(","):
            p=p.strip()
            if re.match(r"const\s+[\w:<>\s\*]+&\s*\w+$",p) or re.search(r"\bconst\s*&\s*\w+$",p):
                names.add(re.findall(r"\w+",p)[-1])
    if not names: return None
    pat=r"^(\s*)\*\(void\*\*\)&(%s)\s*=\s*p;\s*$"%"|".join(map(re.escape,names))
    out,n=re.subn(pat,r"\1if (*(void**)&\2 != p)\n\1    return;",src,flags=re.M)
    return out if n else None
for a in sys.argv[1:]:
    if a in matched: continue
    best=None
    for suf in ("refnotag2","refnotag","ref","combonotag2","combo","lbnotag2","lb","def3"):
        f=f"{W}/var/{a}_{suf}.cpp"
        if not os.path.exists(f): continue
        new=conv(open(f).read())
        if new is None: continue
        nf=f"{W}/var/{a}_{suf}rd.cpp"; open(nf,"w").write(new)
        try: r=subprocess.run(["python3",f"{W}/var/pv.py",a,nf],capture_output=True,text=True,timeout=600).stdout
        except Exception as e: r=f"timeout {e}"
        m=re.search(r"diff lines: (\d+)",r); n=int(m.group(1)) if m else 999
        first=r.splitlines()[0][:90] if r else "?"
        print(f"{a} {suf}rd | {first} | diff lines: {n}",flush=True)
        if best is None or n<best[0]: best=(n,suf,nf,r)
        if n==0: break
    if best and best[0]==0:
        n,suf,nf,r=best
        src=f"{W}/src/{a}.cpp"; pre=f"{W}/var/{a}_pre_{suf}rd.cpp"
        if os.path.exists(src): os.replace(src,pre)
        open(src,"w").write("".join(l for l in open(nf) if not l.startswith("// Grade:")))
        leaf=re.search(r"leaf=(\S+)",r).group(1)
        g=subprocess.run(["bash",f"{W}/g.sh",a,leaf],capture_output=True,text=True,timeout=600).stdout.splitlines()[0]
        print(f"{a} GRADE: {g}",flush=True)
        if "MATCH" not in g and os.path.exists(pre): os.replace(pre,src)
