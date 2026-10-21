import os,re,subprocess,shutil
S=os.environ["S"]; W=f"{S}/w22/retry10"
rows=[l.strip() for l in open(f"{W}/var/todo.txt") if l.strip()]
out=open(f"{W}/var/sweep_idx_result.tsv","a")
pat=re.compile(r"^([ \t]+)((?:const )?[\w:<>]+)(&?) operator\[\]\((?:const )?(?:unsigned )?(?:long|int|short|size_t)&? (\w+)\)( const)? \{ return (\w+)\[(\w+)\]; \}[ \t]*(//[^\n]*)?$",re.M)
for a in rows:
    p=f"{W}/src/{a}.cpp"; t=open(p).read()
    k=[0]
    def rep(m):
        ind,rt,ref,param,const,ptr,idx,cm=m.groups()
        k[0]+=1
        elem=rt if ref else ("const "+rt if not rt.startswith("const ") else rt)
        acc=f"{ind}{elem}* _SweepBegin{k[0]}() const {{ return {ptr}; }}\n"
        return f"{acc}{ind}{rt}{ref} operator[]({m.group(0).split('operator[](')[1].split(')')[0]}){const or ''} {{ return *(_SweepBegin{k[0]}() + {idx}); }}{(' '+cm) if cm else ''}"
    t2=pat.sub(rep,t)
    if k[0]==0: continue
    shutil.copy(p,f"{W}/var/sweep_bak/{a}.cpp"); open(p,"w").write(t2)
    r=subprocess.run(["bash",f"{W}/g.sh",a],capture_output=True,text=True).stdout.splitlines()
    line=r[0] if r else "?"
    ok=("RELOCATION_MATCH" in line) or (" MATCH" in line)
    out.write(f"{a}\t{'MATCH' if ok else 'no'}\t{line}\n"); out.flush()
    if not ok: shutil.copy(f"{W}/var/sweep_bak/{a}.cpp",p)
