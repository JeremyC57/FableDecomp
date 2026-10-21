# sweep2.py <worker> <n> <list> <out>: mechanical transforms graded WITH the source's Leaf/Flags (pv.py); writes var/<a>_s7<T>.cpp
import os,re,subprocess,sys
S="/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad"; W=f"{S}/w22/retry10"
wk,nw,lst,outp=int(sys.argv[1]),int(sys.argv[2]),sys.argv[3],sys.argv[4]
IDX=re.compile(r"^([ \t]*)((?:const\s+)?[\w:<>]+(?:\s*\*)?)\s*(&?)\s*operator\[\]\(([^()]*)\)(\s*const)?\s*\{\s*return\s+(\w+)\[(\w+)\];\s*\}[ \t]*(//[^\n]*)?$",re.M)
def t_idx(t):
    k=[0]
    def rep(m):
        ind,rt,ref,params,const,ptr,idx,cm=m.groups()
        if ptr in ("this",): return m.group(0)
        k[0]+=1
        elem=rt if ref else ("const "+rt if not rt.startswith("const ") else rt)
        acc=f"{ind}{elem}* _S7Begin{k[0]}() const {{ return {ptr}; }}\n"
        return f"{acc}{ind}{rt}{ref} operator[]({params}){const or ''} {{ return *(_S7Begin{k[0]}() + {idx}); }}{(' '+cm) if cm else ''}"
    t2=IDX.sub(rep,t)
    return t2 if k[0] else None
def t_op(t):
    m=re.search(r"(\w[\w:]*\*) GetPItem\(\)(?: const)?;",t)
    if not m: return None
    rt=m.group(1)
    t2=re.sub(r"SetPItem\(((?:\([^()]*\))?)(\w+)\.GetPItem\(\)\);", lambda mm: f"{rt} item = {mm.group(2)}.GetPItem(); SetPItem({mm.group(1)}item);", t)
    return t2 if t2!=t else None
DEC=re.compile(r"--((?:\([\w\s\*]+\))?[\w>.()\[\]-]*?(?:m_refCount|RefCount|m_count|refCount|m_references|References)) == 0")
def t_dec(t):
    t2,n=DEC.subn(lambda m: f"DecRefToZero({m.group(1)})",t)
    if n==0 or "DecRefToZero(long&" in t: return None
    lines=t2.split("\n"); i=0
    while i<len(lines) and (lines[i].startswith("//") or lines[i].startswith("#") or lines[i].strip()==""): i+=1
    return "\n".join(lines[:i])+"\nstatic __forceinline bool DecRefToZero(long& count) { return --count == 0; }\n"+"\n".join(lines[i:])
T={"idx":t_idx,"op":t_op,"dec":t_dec}
rows=[l.strip() for l in open(lst) if l.strip()]
with open(outp,"a") as fo:
    for i,a in enumerate(rows):
        if i%nw!=wk: continue
        t=open(f"{W}/src/{a}.cpp",errors="replace").read()
        res=[]
        cur=t
        for name,fn in T.items():
            v=fn(t)
            if v is None: continue
            res.append((name,v))
        if len(res)>1:
            comb=t
            for name,fn in T.items():
                v=fn(comb)
                if v is not None: comb=v
            res.append(("all",comb))
        for name,v in res:
            vp=f"{W}/var/{a}_s7{name}.cpp"; open(vp,"w").write(v)
            env=dict(os.environ); env["S"]=S
            r=subprocess.run(["python3",f"{W}/var/pv.py",a,vp],capture_output=True,text=True,env=env,cwd="/home/user/FableDecomp")
            last=(r.stdout.strip().splitlines() or ["?"])[-1]
            fo.write(f"{a}\t{name}\t{last}\n"); fo.flush()
