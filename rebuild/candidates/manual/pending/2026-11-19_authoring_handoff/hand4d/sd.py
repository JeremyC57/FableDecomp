import sys,difflib,re
t=open(sys.argv[1]).read().split("\n")
hd=[l for l in t if l.strip() and not l.startswith("  WARN")]
print(hd[0] if hd else "")
if "--- retail" not in "\n".join(t): print("\n".join(t[:60])); sys.exit()
i=[k for k,l in enumerate(t) if l.startswith("--- retail")][0]
j=[k for k,l in enumerate(t) if l.startswith("--- built")][0]
def norm(l):
    p=l.split(None,2)
    if len(p)<3: return l
    ins=p[2]
    ins=re.sub(r"0x[0-9a-f]{5,8}","ADDR",ins)
    ins=re.sub(r"\[0\]","[ADDR]",ins); ins=re.sub(r"^push 0$","push ADDR",ins); ins=re.sub(r", 0$",", ADDR",ins)
    ins=re.sub(r"^(call|jmp) 0x[0-9a-f]+","\\1 X",ins); ins=re.sub(r"^(call|jmp) [0-9a-f]+$","\\1 X",ins)
    return ins
a=[l for l in t[i+1:j] if l.strip()]; b=[l for l in t[j+1:] if l.strip()]
na=[norm(l) for l in a]; nb=[norm(l) for l in b]
sm=difflib.SequenceMatcher(None,na,nb,autojunk=False)
nd=0
for op,a1,a2,b1,b2 in sm.get_opcodes():
    if op=="equal": continue
    nd+=max(a2-a1,b2-b1)
    for k in range(max(a2-a1,b2-b1)):
        l=a[a1+k] if a1+k<a2 else ""
        r=b[b1+k] if b1+k<b2 else ""
        print("%-62s | %s"%(l[:62],r[10:] if r else ""))
    print("-----")
print("ndiff",nd, "lens",len(a),len(b))
