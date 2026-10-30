#!/usr/bin/env python3
"""stb_sites.py: per landed row, count STB ctor/dtor sites in retail bytes: calls to 0x007E72A0 / 0x007E74D0 / 0x007E7520
(out-of-line) vs inline expansions (ctor: call CBaseObject() 0x0099A380 followed within 3 insns by a store of the STB
vtable 0x0127094C; dtor: store of the GROBase vtable 0x0126008C followed within 3 insns by call/jmp ~CBaseObject 0x0099A430
after a PImp release). Prints addr size ncall_ctor ninl_ctor ncall_dtor ninl_dtor src."""
import os, re, sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
R = os.environ["FABLE_ROOT"]
ps = open(R + "/rebuild/build_candidates.ps1", encoding="utf-8-sig").read()
ent = {}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if "Address" in kv: ent[kv["Address"].lower().replace("0x", "")] = kv
md = Cs(CS_ARCH_X86, CS_MODE_32)
for l in open(R + "/rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q = l.split("\t"); a = q[0].lower()
    if a not in ent: continue
    b = bytes.fromhex(q[3]); va = int(a, 16)
    ins = list(md.disasm(b, va))
    cc = ic = cd = idd = 0
    for i, x in enumerate(ins):
        if x.mnemonic in ("call", "jmp") and x.op_str in ("0x7e72a0",): cc += 1
        if x.mnemonic in ("call", "jmp") and x.op_str in ("0x7e74d0", "0x7e7520"): cd += 1
        if x.mnemonic == "call" and x.op_str == "0x99a380":
            if any("0x127094c" in y.op_str for y in ins[i+1:i+4]): ic += 1
        if x.mnemonic == "mov" and x.op_str.endswith("0x126008c"):
            if any(y.op_str == "0x99a430" for y in ins[i+1:i+4]): idd += 1
    if cc or ic or cd or idd:
        print(a, len(b), cc, ic, cd, idd, ent[a]["Source"], sep="\t")
