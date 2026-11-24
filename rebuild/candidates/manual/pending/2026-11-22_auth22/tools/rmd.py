import signal; signal.signal(signal.SIGPIPE, signal.SIG_DFL)
import sys,re,difflib
# rmd.py <q.out> [ctx]: register-masked diff (callee-saved esi/edi/ebp/ebx -> R, bl/bh -> rl; scratch eax/ecx/edx -> S when 'm2'), stack offsets kept
t=open(sys.argv[1]).read(); n=int(sys.argv[2]) if len(sys.argv)>2 else 0
m2=len(sys.argv)>3
r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)[0].splitlines()[1:]; b=re.findall(r"--- built.*",t,re.S)[0].splitlines()[1:]
def norm(l):
    p=l.split(None,2); s=p[2] if len(p)>2 else l
    s=re.sub(r"^(call|j\w+) 0x[0-9a-f]+",r"\1 X",s)
    s=re.sub(r"0x[0-9a-f]{6,8}","IMM",s); s=re.sub(r"\[(IMM|0x[0-9a-f]+|\d+)\]","[ABS]",s)
    s=re.sub(r"(, |push )(IMM|0)$",r"\1K0",s)
    s=re.sub(r"\b(esi|edi|ebp|ebx)\b","R",s); s=re.sub(r"\b(bl|bh)\b","rl",s)
    if m2: s=re.sub(r"\b(eax|ecx|edx)\b","S",s); s=re.sub(r"\[esp \+ 0x[0-9a-f]+\]","[SP]",s)
    return s
R=[norm(l) for l in r]; B=[norm(l) for l in b]
sm=difflib.SequenceMatcher(None,R,B,autojunk=False)
d=sum(max(i2-i1,j2-j1) for op,i1,i2,j1,j2 in sm.get_opcodes() if op!='equal'); h=sum(1 for op,*_ in sm.get_opcodes() if op!='equal')
print("masked residue_lines=%d hunks=%d (retail %d instrs, ours %d)"%(d,h,len(R),len(B)))
for op,i1,i2,j1,j2 in sm.get_opcodes():
    if op=='equal': continue
    print("@@ retail %s"%(r[i1].split()[0] if i1<len(r) else 'end'))
    for l in R[i1:i2]: print("-",l)
    for l in B[j1:j2]: print("+",l)
