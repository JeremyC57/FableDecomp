# pv.py <addr> <variant.cpp> [--flags ...]: like pd.py but derives the leaf (// Leaf: line, else ?Name@Module from rows.tsv) and keeps // Flags:
import sys,re,subprocess,os,difflib
S=os.environ["S"]; W=f"{S}/w22/retry10"
addr,src=sys.argv[1:3]; extra=sys.argv[3:]
txt=open(src).read()
m=re.search(r"^// Leaf:\s*(\S+)",txt,re.M)
if m: leaf=m.group(1)
else:
    for l in open(f"{W}/rows.tsv"):
        f=l.rstrip("\n").split("\t")
        if f[0]==addr: leaf=f"?{f[2]}@{f[1]}" if f[1]!="_global" else f[2]; break
fm=re.search(r"^// Flags:\s*(.*)$",txt,re.M)
flags=fm.group(1).split() if fm else []
if extra and extra[0]=="--flags": flags+=extra[1:]
ret=subprocess.run(["bash",f"{W}/g.sh",addr],capture_output=True,text=True).stdout
r=re.findall(r"--- retail.*?(?=--- built)",ret,re.S)[0].splitlines()[1:]
pb=subprocess.run(["python3",f"{W}/probe.py",src,leaf]+(["--flags"]+flags if flags else []),capture_output=True,text=True).stdout
b=pb.splitlines()[1:]
if not pb.startswith("---"): print(pb[-1500:]); sys.exit(1)
def norm(s,addr=0):
    def rel(m):
        t=int(m.group(2),16); return f"{m.group(1)} {'+' if t>=addr else '-'}{abs(t-addr):x}"
    s=re.sub(r"\b(j\w+|loop\w*)\s+(0x[0-9a-f]+)",rel,s)
    s=re.sub(r"\b(call)\s+0x[0-9a-f]+","\\1 X",s)
    s=re.sub(r"\[0x[0-9a-f]+\]","[G]",s); s=re.sub(r"\[0\]","[G]",s)
    s=re.sub(r"ptr \[0x[0-9a-f]+\]","ptr [G]",s)
    return s
def f(L):
    out=[]
    for l in L:
        if not l.strip(): continue
        parts=l.split(); addr=int(parts[0],16)
        out.append(norm(" ".join(parts[2:]),addr))
    return out
rr=f(r); bb=f(b)
print(f"retail {len(rr)} built {len(bb)} insns; {pb.splitlines()[0]}  leaf={leaf} flags={flags}")
n=0
for l in difflib.unified_diff(rr,bb,lineterm="",n=0):
    if not l.startswith(" ") and not l.startswith(("@@","---","+++")): print(l); n+=1
print("diff lines:",n)
