#!/usr/bin/env python3
"""regmap.py: for each RegisterScripts entry (work/regscripts.txt), read the script Alloc's vtable store and name the script
class from the landed rows that cite that vtable (NScript<Class>_ file names). Writes work/regmap.tsv: alloc, vtable, data alloc, classes."""
import os, re, sys, subprocess, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import vt
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
S = os.environ["S"]; R = os.environ["FABLE_ROOT"]
md = Cs(CS_ARCH_X86, CS_MODE_32)
files = subprocess.run(["git", "-C", R, "ls-files", "rebuild/src/compiled"], capture_output=True, text=True).stdout.split()
out = []
for l in open(S + "/mig7/work/regscripts.txt"):
    q = l.rstrip("\n").split("\t"); kv = dict(x.split("=", 1) for x in q[2].split())
    keys = sorted(kv, key=lambda k: int(k, 16))
    alloc = int(kv[keys[0]], 16); data = kv[keys[1]]
    vtab = None
    for i in md.disasm(vt.rd(alloc, 0x80), alloc):
        m = re.match(r"dword ptr \[(?:esi|eax|edi|ebx)\], (0x[0-9a-f]+)$", i.op_str)
        if i.mnemonic == "mov" and m: vtab = int(m.group(1), 16); break
        if i.mnemonic == "ret": break
    cls = set()
    if vtab:
        pat = "%08x" % vtab
        g = subprocess.run(["git", "-C", R, "grep", "-il", "0x" + pat, "--", "rebuild/src/compiled"], capture_output=True, text=True).stdout.split()
        for f in g:
            m = re.match(r"NScript(C\w+?)_", os.path.basename(f))
            if m: cls.add(m.group(1))
    out.append(f"{alloc:08x}\t{vtab and '%08x' % vtab}\t{data}\t{','.join(sorted(cls))}")
open(S + "/mig7/work/regmap.tsv", "w").write("\n".join(out) + "\n")
