# rd.py <q.out>: residue (differing insns, reloc-like operands masked) between retail and built
import sys,re,difflib
t=open(sys.argv[1]).read()
hdr=t.splitlines()[0]
r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)[0].splitlines()[1:]; b=re.findall(r"--- built.*",t,re.S)[0].splitlines()[1:]
def f(L):
    out=[]
    for l in L:
        p=l.split(None,2)
        if len(p)<3: continue
        a,ins=p[0],p[2]
        m=ins.split()[0]
        if m in("call","jmp") and "0x" in ins and "[" not in ins: ins=m+" REL"
        else:
            ins=re.sub(r"0x[0-9a-f]{6,}","ADDR",ins)
            if m=="mov" and re.search(r", 0$",ins): ins=re.sub(r", 0$",", ADDR",ins)
            if m=="push" and ins in("push 0",): ins="push ADDR"
            ins=re.sub(r"\[(0)\]","[ADDR]",ins)
        out.append((a,ins))
    return out
R=f(r);B=f(b)
sm=difflib.SequenceMatcher(None,[x[1] for x in R],[x[1] for x in B],autojunk=False)
n=0;show="-q" not in sys.argv
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op=="equal": continue
    n+=max(i2-i1,j2-j1)
    if show:
        print("@@ %s"%(R[i1][0] if i1<len(R) else "end"))
        for x in R[i1:i2]: print("-",x[1])
        for x in B[j1:j2]: print("+",x[1])
print(hdr.split()[2] if len(hdr.split())>2 else hdr, "residue",n)
