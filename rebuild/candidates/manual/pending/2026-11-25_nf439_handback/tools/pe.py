#!/usr/bin/env python3
"""pe.py: minimal PE32 reader + capstone disassembly. Usage: pe.py <exe> <va hex> <len> [--raw]
Imported by triage tools (read_va)."""
import struct, sys, functools
import capstone

S = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad"
EXES = {"retail": S + "/dl/game/Fable The Lost Chapters/Fable.exe",
        "egor": "/tmp/claude-0/egod_pdb_dl/debug tlc/ego_r.exe",
        "fw": "/tmp/claude-0/egod_pdb_dl/debug tlc/FableWin.exe"}

@functools.lru_cache(None)
def load(path):
    d = open(path, "rb").read()
    pe = struct.unpack_from("<I", d, 0x3c)[0]
    nsec = struct.unpack_from("<H", d, pe + 6)[0]
    opt = struct.unpack_from("<H", d, pe + 20)[0]
    base = struct.unpack_from("<I", d, pe + 24 + 28)[0]
    secs = []
    o = pe + 24 + opt
    for i in range(nsec):
        name, vs, va, rs, ro = struct.unpack_from("<8sIIII", d, o + 40 * i)
        secs.append((va, max(vs, rs), ro, rs))
    return d, base, secs

def read_va(exe, va, n):
    d, base, secs = load(EXES.get(exe, exe))
    rva = va - base
    for sva, sz, ro, rs in secs:
        if sva <= rva < sva + sz:
            off = ro + rva - sva
            return d[off:off + n]
    return b""

def dis(exe, va, n):
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    return [(i.address, i.mnemonic, i.op_str, i.size) for i in md.disasm(read_va(exe, va, n), va)]

if __name__ == "__main__":
    exe, va, n = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3])
    for a, m, o, s in dis(exe, va, n):
        print("%08x  %-6s %s" % (a, m, o))
