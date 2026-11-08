#!/usr/bin/env python3
# sites.py <ours.txt|retail-a2-extract> : list expanded ~vector (call [edx] loop) and expanded ~CScriptThing (vptr store) blocks
# with the jumps that reach each block start.
import sys, re
L = [l.rstrip("\n") for l in open(sys.argv[1]) if re.match(r"^[0-9a-f]{8} ", l)]
addr = [int(l[:8], 16) for l in L]
txt = [l[9:] for l in L]
jumps = {}
for a, t in zip(addr, txt):
    m = re.match(r"j\w+ 0x([0-9a-f]+)", t)
    if m: jumps.setdefault(int(m.group(1), 16), []).append(a)
def start_of(i):
    # walk back to the nearest jump target or the instruction after an unconditional jmp/ret
    j = i
    while j > 0:
        if addr[j] in jumps: return j
        if txt[j-1].startswith(("jmp", "ret")): return j
        j -= 1
    return 0
for i, t in enumerate(txt):
    kind = None
    if t == "call dword ptr [edx]" and "push 0" in txt[i-2]: kind = "VEC"
    if re.search(r"mov dword ptr \[esp \+ 0x[0-9a-f]+\], 0x(abcdef|1238c8c)$", t): kind = "THING"
    if kind:
        s = start_of(i)
        print(f"{kind:5} {addr[i]:08x} block {addr[s]:08x} from {[hex(x) for x in jumps.get(addr[s], [])]} prev: {txt[s-1] if s else ''}")
