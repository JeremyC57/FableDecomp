# dl.py <addr>...: print differing lines (call targets/rel jumps normalised) of g.sh output
import sys,re,subprocess,os,difflib
S=os.environ.get("S","/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad"); W=f"{S}/w22/retry10"
def norm(s,addr):
    def rel(m):
        t=int(m.group(2),16); return f"{m.group(1)} {'+' if t>=addr else '-'}{abs(t-addr):x}"
    s=re.sub(r"\b(j\w+|loop\w*)\s+(0x[0-9a-f]+)",rel,s); s=re.sub(r"\b(call)\s+0x[0-9a-f]+","\\1 X",s); return s
def f(L):
    o=[]
    for l in L:
        p=l.split()
        if len(p)<3: continue
        o.append((p[0],norm(" ".join(p[2:]),int(p[0],16))))
    return o
for a in sys.argv[1:]:
    g=subprocess.run(["bash",f"{W}/g.sh",a],capture_output=True,text=True).stdout
    print("=====",g.splitlines()[0] if g else a)
    r=re.findall(r"--- retail.*?(?=--- built)",g,re.S)
    if not r: continue
    rr=f(r[0].splitlines()[1:]); bb=f(re.findall(r"--- built.*",g,re.S)[0].splitlines()[1:])
    sm=difflib.SequenceMatcher(None,[x[1] for x in rr],[x[1] for x in bb],autojunk=False)
    for op,i1,i2,j1,j2 in sm.get_opcodes():
        if op=="equal": continue
        for i in range(i1,i2): print("  -",rr[i][0],rr[i][1])
        for j in range(j1,j2): print("  +",bb[j][0],bb[j][1])
