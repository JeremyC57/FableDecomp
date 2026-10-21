import os,re,subprocess,shutil,sys
S=os.environ["S"]; W=f"{S}/w22/retry10"
rows=[l.strip() for l in open(f"{W}/var/sweep_op.txt") if l.strip()]
out=open(f"{W}/var/sweep_op_result.tsv","a")
for a in rows:
    p=f"{W}/src/{a}.cpp"; t=open(p).read()
    m=re.search(r"(\w[\w:]*\*) GetPItem\(\)(?: const)?;",t)
    if not m: out.write(f"{a}\tSKIP no GetPItem decl\n"); out.flush(); continue
    rt=m.group(1)
    t2=re.sub(r"SetPItem\(((?:\([^()]*\))?)(\w+)\.GetPItem\(\)\);", lambda mm: f"{rt} item = {mm.group(2)}.GetPItem(); SetPItem({mm.group(1)}item);", t)
    if t2==t: out.write(f"{a}\tSKIP no site\n"); out.flush(); continue
    shutil.copy(p,f"{W}/var/sweep_bak/{a}.cpp")
    open(p,"w").write(t2)
    r=subprocess.run(["bash",f"{W}/g.sh",a],capture_output=True,text=True).stdout.splitlines()
    line=r[0] if r else "?"
    ok=("RELOCATION_MATCH" in line) or (" MATCH" in line)
    out.write(f"{a}\t{'MATCH' if ok else 'no'}\t{line}\n"); out.flush()
    if not ok: shutil.copy(f"{W}/var/sweep_bak/{a}.cpp",p)
