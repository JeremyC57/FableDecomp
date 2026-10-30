#!/usr/bin/env python3
"""regscripts.py: walk retail CScriptInfoManager::RegisterScripts (0x00CD52D0) and print, per AddScript call, the script
name string, the script Alloc (CScriptInfo +0) and the master-data Alloc (CScriptInfo +4, immediate or register value)."""
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import vt
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
md = Cs(CS_ARCH_X86, CS_MODE_32); md.detail = False
va = 0x00CD52D0; code = vt.rd(va, 0x9000)
regs = {}; stores = {}; lastpush = None; out = []
for i in md.disasm(code, va):
    m = re.match(r"(e\w\w), (0x[0-9a-f]+)$", i.op_str)
    if i.mnemonic == "mov" and m: regs[m.group(1)] = int(m.group(2), 16)
    m = re.match(r"dword ptr \[esp \+ (0x[0-9a-f]+)\], (\S+)$", i.op_str)
    if i.mnemonic == "mov" and m:
        v = m.group(2); stores[int(m.group(1), 16)] = int(v, 16) if v.startswith("0x") else ("reg", v, regs.get(v))
    if i.mnemonic == "push" and i.op_str.startswith("0x") and vt.sec_of(int(i.op_str, 16)) in (".rdata", ".data"): lastpush = int(i.op_str, 16)
    if i.mnemonic == "call" and i.op_str == "0xcb5c90":
        al = [(k, v) for k, v in stores.items() if isinstance(v, int) and vt.sec_of(v) == ".text"]
        out.append((i.address, vt.cstr(lastpush) if lastpush else "?", dict(stores)))
        stores = {}
    if i.mnemonic == "ret": break
for a, name, st in out:
    s = " ".join(f"{k:x}={('%x' % v) if isinstance(v, int) else (v[1] + ':' + ('%x' % v[2] if v[2] else '?'))}" for k, v in sorted(st.items()))
    print(f"{a:08x}\t{name}\t{s}")
