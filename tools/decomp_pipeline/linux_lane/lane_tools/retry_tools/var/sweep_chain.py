import os,re,subprocess,shutil
S=os.environ["S"]; W=f"{S}/w22/retry10"
rows=[l.strip() for l in open(f"{W}/var/sweep_chain.txt") if l.strip()]
out=open(f"{W}/var/sweep_chain_result.tsv","a")
pat=re.compile(r"^([ \t]+)([^\n]*?)((?:\w+(?:->|\.))*)(\w+)\(\)->(\w+)->(\w+)\(",re.M)
for a in rows:
    p=f"{W}/src/{a}.cpp"; t=open(p).read()
    n=[0]
    def rep(m):
        ind,pre,objpre,fn,mem,call=m.groups()
        if pre.strip().startswith("//"): return m.group(0)
        d=re.search(r"(\w[\w:<>]*\s*\*)\s*%s\(\)(?: const)?\s*(?:;|\{)"%re.escape(fn),t)
        if not d: return m.group(0)
        rt=d.group(1).replace(" ","")
        n[0]+=1; name=f"result{n[0]}"
        return f"{ind}{rt} {name} = {objpre}{fn}();\n{ind}{pre}{name}->{mem}->{call}("
    t2=pat.sub(rep,t)
    if n[0]==0: out.write(f"{a}\tSKIP\n"); out.flush(); continue
    shutil.copy(p,f"{W}/var/sweep_bak/{a}.cpp"); open(p,"w").write(t2)
    r=subprocess.run(["bash",f"{W}/g.sh",a],capture_output=True,text=True).stdout.splitlines()
    line=r[0] if r else "?"
    ok=("RELOCATION_MATCH" in line) or (" MATCH" in line)
    out.write(f"{a}\t{'MATCH' if ok else 'no'}\t{line}\n"); out.flush()
    if not ok: shutil.copy(f"{W}/var/sweep_bak/{a}.cpp",p)
