import signal; signal.signal(signal.SIGPIPE, signal.SIG_DFL)
import sys,re,difflib
# normalised diff of a q.py output: call/jmp targets and large immediates masked; prints hunks + residue count
t=open(sys.argv[1]).read()
n=int(sys.argv[2]) if len(sys.argv)>2 else 1
head=t.splitlines()[0]
r=re.findall(r"--- retail.*?(?=--- built)",t,re.S)
if not r: print(head); sys.exit()
r=r[0].splitlines()[1:]; b=re.findall(r"--- built.*",t,re.S)[0].splitlines()[1:]
def norm(l):
    p=l.split(None,2)
    s=p[2] if len(p)>2 else l
    s=re.sub(r"^(call|j\w+) 0x[0-9a-f]+",r"\1 X",s)
    s=re.sub(r"0x[0-9a-f]{6,8}","IMM",s)
    s=re.sub(r"\[(IMM|0x[0-9a-f]+|\d+)\]","[ABS]",s)
    s=re.sub(r"(, |push )(IMM|0)$",r"\1K0",s)
    return s
R=[norm(l) for l in r]; B=[norm(l) for l in b]
sm=difflib.SequenceMatcher(None,R,B,autojunk=False)
diffs=sum(max(i2-i1,j2-j1) for op,i1,i2,j1,j2 in sm.get_opcodes() if op!='equal')
print(head, "| residue_lines=%d"%diffs)
for l in difflib.unified_diff(R,B,lineterm="",n=n):
    if l.startswith(('---','+++')): continue
    print(l)
