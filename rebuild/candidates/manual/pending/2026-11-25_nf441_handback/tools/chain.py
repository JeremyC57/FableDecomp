#!/usr/bin/env python3
"""chain.py <rvt.json>: per retail VA: slot0 (sdd) -> dtor -> ordered vftable VAs the dtor chain stores (own first, then bases)."""
import sys, json, bisect
sys.path.insert(0, __import__("os").path.dirname(__file__))
from feat import *
rv = json.load(open(sys.argv[1]))
VAS = {int(k, 16) for k in rv}
DEL = {0x00C3C370, 0x00BFE9BC}
def body(va):
    res = []  # ('store', vt) / ('call', tgt)
    for ins in MD.disasm(rd(va, extent(va)), va):
        if ins.mnemonic == "mov" and ins.op_str.startswith("dword ptr [") and "], 0x" in ins.op_str:
            v = int(ins.op_str.split("], ")[1], 16)
            if v in VAS: res.append(("store", v, ins.op_str.split("]")[0] + "]"))
        if ins.mnemonic in ("call", "jmp") and ins.op_str.startswith("0x"):
            res.append(("call", int(ins.op_str, 16), ""))
        if ins.mnemonic == "ret": break
    return res
def chain(f, depth=0, seen=None):
    seen = seen or set()
    if f in seen or depth > 4: return []
    seen.add(f); out = []
    for k, x, where in body(f):
        if k == "store" and where in ("dword ptr [ecx]", "dword ptr [esi]", "dword ptr [eax]", "dword ptr [edi]", "dword ptr [ebx]", "dword ptr [edx]", "dword ptr [ebp]"):
            if x not in out: out.append(x)
        elif k == "call" and is_text(x) and x not in DEL:
            for y in chain(x, depth + 1, seen):
                if y not in out: out.append(y)
    return out
res = {}
for va, r in sorted(rv.items()):
    if not r["slots"]: continue
    s0 = int(r["slots"][0], 16)
    b = body(s0)
    calls = [x for k, x, w in b if k == "call" and x not in DEL and is_text(x)]
    dt = calls[0] if calls else s0
    ch = chain(dt)
    res[va] = {"sdd": "%08x" % s0, "dtor": "%08x" % dt, "chain": ["%08x" % x for x in ch]}
    print(va, res[va]["sdd"], res[va]["dtor"], " ".join(res[va]["chain"]))
json.dump(res, open("work/chain.json", "w"))
