#!/usr/bin/env python3
# dd.py last.txt [-v] : normalised instruction diff of q.py output (retail vs built)
import sys, re, difflib
t = open(sys.argv[1]).read().splitlines()
def grab(tag):
    out=[]; on=False
    for l in t:
        if l.startswith("--- retail"): on = (tag=="r"); continue
        if l.startswith("--- built"): on = (tag=="b"); continue
        if on and re.match(r"^[0-9a-f]{8}  ", l): out.append(l)
    return out
R=grab("r"); B=grab("b")
def norm(l):
    a=l[:8]; parts=l[10:].split(None,1); hx=parts[0]; rest=parts[1] if len(parts)>1 else ""
    if rest.startswith("call ") and not "ptr" in rest: rest="call X"
    elif "00000000" in hx and len(hx)>=10: rest=re.sub(r"(?<![0-9a-fx])0(?![0-9a-fx])","X",rest)
    rest=re.sub(r"0x[0-9a-f]{6,}", "X", rest)
    if "-m" in sys.argv: rest=re.sub(r"\[esp \+ 0x[0-9a-f]+\]", "[esp+?]", rest); rest=re.sub(r"(sub|add) esp, 0x[0-9a-f]+","\\1 esp, ?",rest)
    return rest.strip()
nr=[norm(l) for l in R]; nb=[norm(l) for l in B]
sm=difflib.SequenceMatcher(None, nr, nb, autojunk=False)
same=sum(b.size for b in sm.get_matching_blocks())
print(f"same_insns={same}/{len(nr)} built_insns={len(nb)} ratio={sm.ratio():.3f}")
if "-v" in sys.argv:
    ctx = int(sys.argv[sys.argv.index("-v")+1]) if len(sys.argv)>sys.argv.index("-v")+1 else 2
    for op,i1,i2,j1,j2 in sm.get_opcodes():
        if op=="equal": continue
        print(f"== {op} retail[{i1}:{i2}] built[{j1}:{j2}]")
        for k in range(max(0,i1-ctx),i1): print("   ", R[k])
        for k in range(i1,i2): print(" R ", R[k])
        for k in range(j1,j2): print(" B ", B[k])
