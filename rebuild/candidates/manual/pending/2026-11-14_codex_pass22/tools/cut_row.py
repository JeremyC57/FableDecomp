#!/usr/bin/env python3
"""cut_row.py <Fable.exe> <va-hex> <length> [--dis] [--out file]
Cut one function's bytes out of YOUR OWN retail Fable.exe (ImageBase 0x400000). Prints a hex dump, or with --dis a
disassembly (needs `objdump` with i386 support on PATH). Never put the output in anything you hand back."""
import struct, subprocess, sys, tempfile, os
if len(sys.argv) < 4: sys.exit(__doc__)
d = open(sys.argv[1], 'rb').read(); va = int(sys.argv[2], 16); ln = int(sys.argv[3], 0)
pe = struct.unpack_from('<I', d, 0x3c)[0]; nsec = struct.unpack_from('<H', d, pe + 6)[0]
optsz = struct.unpack_from('<H', d, pe + 20)[0]; base = struct.unpack_from('<I', d, pe + 24 + 28)[0]
blob = None
for i in range(nsec):
    o = pe + 24 + optsz + 40 * i
    vsz, rva, rsz, raw = struct.unpack_from('<IIII', d, o + 8)
    if base + rva <= va < base + rva + max(vsz, rsz):
        off = raw + va - base - rva; blob = d[off:off + ln]
if blob is None: sys.exit('address is in no section')
if '--out' in sys.argv: open(sys.argv[sys.argv.index('--out') + 1], 'wb').write(blob)
if '--dis' in sys.argv:
    f = tempfile.NamedTemporaryFile(delete=False, suffix='.bin'); f.write(blob); f.close()
    print(subprocess.run(['objdump', '-D', '-b', 'binary', '-mi386', '-Mintel', '--adjust-vma=0x%x' % va, f.name],
                         capture_output=True, text=True).stdout)
    os.unlink(f.name)
else:
    for i in range(0, len(blob), 16): print('%08X  %s' % (va + i, ' '.join('%02x' % b for b in blob[i:i + 16])))
