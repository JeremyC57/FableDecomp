#!/usr/bin/env python3
"""objdis.py <obj> <symbol>: disassemble one function COMDAT of a COFF object with its relocation targets."""
import sys, capstone
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF424/tools"); import nf
secs, sl = nf.coff(sys.argv[1])
for i, n, v, sec, typ, scl in sl:
    if n == sys.argv[2] and sec > 0:
        s = secs[sec - 1]; rel = {r[0]: r[1] for r in s["rel"]}
        md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
        for ins in md.disasm(s["data"], 0):
            r = [rel[k] for k in rel if ins.address <= k < ins.address + ins.size]
            print("%4x %s %s %s" % (ins.address, ins.mnemonic, ins.op_str, ("; " + r[0]) if r else ""))
