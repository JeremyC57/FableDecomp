# ipra_scan.py: find retail listings where ecx/edx is read after a call before being written (IPRA-dependent code)
import re,glob,os
st={}
for l in open("../STATUS.tsv"):
    f=l.rstrip("\n").split("\t")
    if len(f)>1 and f[1].startswith(("MATCH","RELOC")): st[f[0]]=1
def regs_read_written(ins):
    p=ins.split(None,1); mn=p[0]; ops=p[1] if len(p)>1 else ""
    parts=[x.strip() for x in ops.split(",")] if ops else []
    read=set(); write=set()
    def regs(s): return set(re.findall(r"\b(e[abcd]x|e[sd]i|e[bs]p|[abcd][lh]|[abcd]x)\b",s))
    def full(r):
        m={"al":"eax","ah":"eax","ax":"eax","bl":"ebx","bh":"ebx","bx":"ebx","cl":"ecx","ch":"ecx","cx":"ecx","dl":"edx","dh":"edx","dx":"edx"}
        return m.get(r,r)
    if mn in ("mov","movzx","movsx","lea") and len(parts)==2:
        d,s=parts
        if "[" in d: read|=regs(d)
        else: write|={full(r) for r in regs(d)}
        read|=regs(s)
    elif mn in ("xor","sub") and len(parts)==2 and parts[0]==parts[1] and "[" not in parts[0]:
        write|={full(r) for r in regs(parts[0])}
    elif mn=="call":
        read|=regs(ops)
    elif mn in ("push",):
        read|=regs(ops)
    elif mn=="pop":
        write|={full(r) for r in regs(ops)}
    else:
        for x in parts: read|=regs(x)
        if parts and "[" not in parts[0] and mn not in ("cmp","test","push"): write|={full(r) for r in regs(parts[0])}
    return {full(r) for r in read}, write
hits=[]
for p in sorted(glob.glob("lst/*.txt")):
    a=os.path.basename(p)[:8]
    if a in st: continue
    t=open(p).read()
    r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)
    if not r: continue
    L=[" ".join(l.split()[2:]) for l in r[0].splitlines()[1:] if len(l.split())>2]
    for i,ins in enumerate(L):
        if not ins.startswith("call"): continue
        if ins.startswith("call dword ptr"): pass
        # scan forward until a write of ecx/edx or a branch/call
        for reg in ("ecx","edx"):
            for j in range(i+1,min(i+12,len(L))):
                nx=L[j]
                if re.match(r"(j\w+|ret|call)",nx):
                    break
                rd,wr=regs_read_written(nx)
                if reg in rd and reg not in wr:
                    hits.append((a,i,reg,ins,nx)); break
                if reg in wr: break
            else: continue
            break
seen=set()
for h in hits:
    if h[0] in seen: continue
    seen.add(h[0]); print(*h)
print(len(seen))
