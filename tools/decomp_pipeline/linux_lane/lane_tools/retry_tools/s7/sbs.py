# sbs.py <addr> [alt-src] : side-by-side retail v built (src/<addr>.cpp via g.sh with its Leaf/Flags, or an alt src via probe.py)
import sys,re,subprocess,os,itertools
S=os.environ.get("S","/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad"); W=f"{S}/w22/retry10"
a=sys.argv[1]
def leafflags(p):
    txt=open(p).read()
    m=re.search(r"^// Leaf:\s*(\S+)",txt,re.M)
    leaf=m.group(1) if m else [l.split("\t")[2] for l in open(f"{W}/rows.tsv") if l.startswith(a)][0]
    fm=re.search(r"^// Flags:\s*(.*)$",txt,re.M); fl=fm.group(1).split() if fm else []
    return leaf,fl
leaf,fl=leafflags(f"{W}/src/{a}.cpp")
g=subprocess.run(["bash",f"{W}/g.sh",a,leaf]+(["--flags"]+fl if fl else []),capture_output=True,text=True).stdout
print(g.splitlines()[0] if g else "EMPTY")
r=re.findall(r"--- retail.*?(?=--- built)",g,re.S)
if not r: sys.exit()
r=r[0].splitlines()[1:]
if len(sys.argv)>2:
    src=sys.argv[2]; leaf,fl=leafflags(src)
    pb=subprocess.run(["python3",f"{W}/probe.py",src,leaf]+(["--flags"]+fl if fl else []),capture_output=True,text=True).stdout
    print(pb.splitlines()[0] if pb else "PROBE FAIL")
    base=int(a,16)
    def fix(l):
        p=l.split(); s=" ".join(p[1:])
        s=re.sub(r"\b(j\w+|loop\w*|call)\s+0x([0-9a-f]+)",lambda m: f"{m.group(1)} 0x{int(m.group(2),16)+base:x}",s)
        return f"{int(p[0],16)+base:08x} "+s
    b=[fix(l) for l in pb.splitlines()[1:] if l.strip()]
else:
    b=re.findall(r"--- built.*",g,re.S)[0].splitlines()[1:]
def n(x):
    if not x: return ""
    p=x.split(); s=" ".join(p[2:]); a=int(p[0],16)
    def rel(m):
        tg=int(m.group(2),16); return f"{m.group(1)} {'+' if tg>=a else '-'}{abs(tg-a):x}"
    s=re.sub(r"\b(j\w+|loop\w*)\s+(0x[0-9a-f]+)",rel,s); s=re.sub(r"\b(call)\s+0x[0-9a-f]+","\\1 X",s)
    s=re.sub(r"\[0x[0-9a-f]+\]","[G]",s); s=re.sub(r"\[0\]","[G]",s)
    return s
for x,y in itertools.zip_longest(r,b,fillvalue=""):
    xs=n(x); ys=n(y); mk=" " if xs==ys else "*"
    try: print(f"{x[:8]:8} {mk} {xs[:52]:52} | {ys[:52]}")
    except BrokenPipeError: break
