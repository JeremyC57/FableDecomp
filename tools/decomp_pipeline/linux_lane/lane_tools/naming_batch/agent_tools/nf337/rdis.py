#!/usr/bin/env python3
"""rdis.py <addr> [maxbytes]: disassemble retail Fable.exe from addr until the first ret / unconditional jmp (evidence only; never
copy the output into hand-back files)."""
import os, struct, sys
import capstone
EXE = os.environ.get("FABLE_EXE")
def load():
    d = open(EXE, "rb").read()
    pe = struct.unpack_from("<I", d, 0x3c)[0]
    nsec = struct.unpack_from("<H", d, pe + 6)[0]; opt = struct.unpack_from("<H", d, pe + 20)[0]
    ib = struct.unpack_from("<I", d, pe + 24 + 28)[0]
    secs = []
    for i in range(nsec):
        o = pe + 24 + opt + 40 * i
        va, vs, raw, rs = struct.unpack_from("<IIII", d, o + 8)[1], struct.unpack_from("<I", d, o + 8)[0], struct.unpack_from("<I", d, o + 20)[0], struct.unpack_from("<I", d, o + 16)[0]
        secs.append((ib + va, vs, raw, rs))
    return d, secs
def read(d, secs, va, n):
    for s, vs, raw, rs in secs:
        if s <= va < s + max(vs, rs):
            off = raw + (va - s); return d[off:off + n]
    raise SystemExit("unmapped %x" % va)
if __name__ == "__main__":
    d, secs = load()
    a = int(sys.argv[1], 16); n = int(sys.argv[2]) if len(sys.argv) > 2 else 256
    full = len(sys.argv) > 3
    md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    for ins in md.disasm(read(d, secs, a, n), a):
        print("%08x %-16s %s %s" % (ins.address, ins.bytes.hex(), ins.mnemonic, ins.op_str))
        if not full and (ins.mnemonic.startswith("ret") or ins.mnemonic == "jmp"):
            break
