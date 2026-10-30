#!/usr/bin/env python3
"""mbsites.py: for every landed row whose retail bytes touch the MovieBase dtor (call 0x006E7B80 = whole ~MovieBase COMDAT,
call 0x006E7AB0 = PImp CCountedPointer dtor with the MovieBase dtor expanded, or an inline release of [x+0xc] followed by
the GROBase vtable store 0x0126008C) print the ordered site kinds, the MovieBase ctor stores (0x01260EF4), the row length
and its EH (fs:0) use."""
import os, re, struct, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
R = os.environ["FABLE_ROOT"]
md = Cs(CS_ARCH_X86, CS_MODE_32)
ps = open(R + "/rebuild/build_candidates.ps1", encoding="utf-8-sig").read()
ent = {}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if "Address" in kv: ent[kv["Address"].lower().replace("0x", "")] = kv
for l in open(R + "/rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q = l.split("\t"); a = q[0].lower()
    if a not in ent: continue
    b = bytes.fromhex(q[3]); va = int(a, 16)
    if b"\xf4\x0e\x26\x01" not in b and b"\x8c\x00\x26\x01" not in b and b"\xb0\x7a\x6e\x00" not in b: pass
    ins = list(md.disasm(b, va))
    sites = []; ctor = 0
    for k, i in enumerate(ins):
        if i.mnemonic == "call" and i.op_str.startswith("0x"):
            t = int(i.op_str, 16)
            if t == 0x006E7B80: sites.append("B80")
            elif t == 0x006E7AB0: sites.append("AB0")
        if "0x1260ef4" in i.op_str and i.mnemonic == "mov": ctor += 1
        if "0x126008c" in i.op_str and i.mnemonic == "mov":
            back = " ".join(x.mnemonic + " " + x.op_str for x in ins[max(0, k - 14):k])
            if "0x6e7ab0" not in back and "operator" not in back and "call dword ptr [eax + 4]" in back: sites.append("INL")
    if not sites: continue
    eh = "EH" if b"\x64\xa1\x00\x00\x00\x00" in b else "--"
    print(a, len(b), eh, "ctor%d" % ctor, " ".join(sites), ent[a].get("CompilerFlags", ""), ent[a]["Source"].split("/")[-1], sep="\t")
