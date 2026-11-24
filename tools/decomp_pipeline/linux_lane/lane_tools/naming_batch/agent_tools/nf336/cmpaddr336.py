#!/usr/bin/env python3
"""cmpaddr336.py <obj> <symbol> <retail addr>: the COMDAT defining <symbol> v Fable.exe at <addr> over its length, relocation fields
masked on both sides (pickany_vs_retail.py's rule); prints EQ / NEQ, the length and the copy's relocation symbols, plus each relocation's
retail target address read from the exe (rel32 for call/jmp, absolute otherwise). No bytes printed."""
import sys, struct, os, importlib.util
spec = importlib.util.spec_from_file_location('pk', '/home/user/FableDecomp/rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py')
pk = importlib.util.module_from_spec(spec); spec.loader.exec_module(pk)
obj, sym, addr = sys.argv[1], sys.argv[2], int(sys.argv[3], 16)
exe = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', exe, 0x3c)[0]; nsec = struct.unpack_from('<H', exe, pe + 6)[0]; opt = struct.unpack_from('<H', exe, pe + 20)[0]
base = struct.unpack_from('<I', exe, pe + 24 + 28)[0]; sh = pe + 24 + opt; xs = []
for k in range(nsec):
    o = sh + 40 * k; vs, va, rs, rp = struct.unpack_from('<IIII', exe, o + 8); xs.append((va, max(vs, rs), rp))
def rd(va, n):
    rva = va - base
    for s, sz, rp in xs:
        if s <= rva < s + sz: return exe[rp + rva - s:rp + rva - s + n]
syms, secs, body = pk.coff(obj)
hit = [s for s in syms if s[1] == sym and s[3] > 0]
if not hit: print('NOSYM', sym); sys.exit(1)
raw, rel = body(hit[0][3]); r = bytearray(rd(addr, len(raw)))
tg = []
for off, n in rel:
    v = struct.unpack_from('<I', r, off)[0]
    call = off > 0 and r[off - 1] in (0xE8, 0xE9)
    tg.append('%s@%08X' % (n, (addr + off + 4 + v) & 0xffffffff if call else v))
    r[off:off + 4] = b'\0\0\0\0'
print('EQ ' if bytes(r) == bytes(raw) else 'NEQ', '%08X' % addr, len(raw), sym, '| relocs:', ' '.join(tg))
