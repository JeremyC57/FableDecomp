import os,re,subprocess,shutil
S=os.environ["S"]; W=f"{S}/w22/retry10"
rows=[l.strip() for l in open(f"{W}/var/sweep_dec.txt") if l.strip()]
out=open(f"{W}/var/sweep_dec_result.tsv","a")
pat=re.compile(r"--((?:\([\w\s\*]+\))?[\w>.()\[\]-]*?(?:m_refCount|RefCount|m_count|refCount|m_references)) == 0")
helper="static __forceinline bool DecRefToZero(long& count) { return --count == 0; }\n"
for a in rows:
    p=f"{W}/src/{a}.cpp"; t=open(p).read()
    t2,n=pat.subn(lambda m: f"DecRefToZero({m.group(1)})",t)
    if n==0: out.write(f"{a}\tSKIP\n"); out.flush(); continue
    # insert helper after the leading comment/pragma/include block
    lines=t2.split("\n"); i=0
    while i<len(lines) and (lines[i].startswith("//") or lines[i].startswith("#") or lines[i].strip()==""): i+=1
    t2="\n".join(lines[:i])+"\n"+helper+"\n".join(lines[i:])
    shutil.copy(p,f"{W}/var/sweep_bak/{a}.cpp"); open(p,"w").write(t2)
    r=subprocess.run(["bash",f"{W}/g.sh",a],capture_output=True,text=True).stdout.splitlines()
    line=r[0] if r else "?"
    ok=("RELOCATION_MATCH" in line) or (" MATCH" in line)
    out.write(f"{a}\t{'MATCH' if ok else 'no'}\t{line}\n"); out.flush()
    if not ok: shutil.copy(f"{W}/var/sweep_bak/{a}.cpp",p)
