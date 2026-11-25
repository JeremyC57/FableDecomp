#!/usr/bin/env python3
"""vtstores.py: every catalog function's `mov dword ptr [reg(+disp)], imm32` stores whose imm32 lies in .rdata (vtable-pointer stores).
Writes r2/work/vtstores.tsv: imm32 (vtable VA) <TAB> function address <TAB> store offset <TAB> disp. No bytes."""
import os, sys, capstone, struct
R = "/home/user/FableDecomp"
A = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/"
sys.path.insert(0, A + "tools"); import nf
secs = nf.exe(); rdata = [s for s in secs if s[0] == ".rdata"][0]
lo, hi = rdata[1], rdata[1] + len(rdata[2])
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32); md.detail = True
out = open(A + "r2/work/vtstores.tsv", "w")
n = 0
for l in open(R + "/rebuild/oracles/auto-re-candidates.tsv"):
    q = l.rstrip("\n").split("\t")
    if q[0] == "address": continue
    try: b = bytes.fromhex(q[3]); va = int(q[0], 16)
    except Exception: continue
    if b"\xc7" not in b: continue
    for i in md.disasm(b, va):
        if i.mnemonic != "mov" or i.bytes[0] != 0xC7 or len(i.operands) != 2: continue
        o0, o1 = i.operands
        if o0.type != capstone.x86.X86_OP_MEM or o1.type != capstone.x86.X86_OP_IMM or o0.size != 4: continue
        v = o1.imm & 0xffffffff
        if lo <= v < hi:
            out.write("%08x\t%s\t%x\t%d\n" % (v, q[0].lower(), i.address - va, o0.mem.disp)); n += 1
out.close(); print("stores", n)
