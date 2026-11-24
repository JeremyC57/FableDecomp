# dcalls.py <exe> <va_hex> <len>: list the direct call targets of a donor-build function (names via the repo's PDB name set when known).
import sys, struct, capstone
p, va, n = sys.argv[1], int(sys.argv[2], 16), int(sys.argv[3])
d = open(p, 'rb').read(); pe = struct.unpack_from('<I', d, 0x3c)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 24 + 28)[0]
names = {}
for l in open('/home/user/FableDecomp/ghidra_out/fablewin_pdb_names.tsv', encoding='utf-8', errors='replace'):
    x = l.rstrip('\n').split('\t')
    if len(x) >= 2: names.setdefault(x[0].lower(), x[1])
for i in range(ns):
    o = pe + 24 + osz + 40 * i; vs, sva, rs, rp = struct.unpack_from('<IIII', d, o + 8)
    if base + sva <= va < base + sva + max(vs, rs): code = d[rp + va - base - sva: rp + va - base - sva + n]
md = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
def resolve(t, depth=0):
    nm = names.get('%08x' % t)
    if nm or depth > 2: return nm
    for i in md.disasm(d[0:0], 0): pass
    return None
for i in md.disasm(code, va):
    if i.mnemonic in ('call', 'jmp') and i.op_str.startswith('0x'):
        t = int(i.op_str, 16)
        # follow an incremental-link thunk (jmp rel32) once
        nm = names.get('%08x' % t)
        if not nm:
            for sec in range(ns):
                o = pe + 24 + osz + 40 * sec; vs, sva, rs, rp = struct.unpack_from('<IIII', d, o + 8)
                if base + sva <= t < base + sva + max(vs, rs):
                    b = d[rp + t - base - sva: rp + t - base - sva + 5]
                    if b[0] == 0xE9: t2 = (t + 5 + struct.unpack_from('<i', b, 1)[0]) & 0xffffffff; nm = 'thunk->' + str(names.get('%08x' % t2))
        print('%08x %s %08x %s' % (i.address, i.mnemonic, t, nm))
