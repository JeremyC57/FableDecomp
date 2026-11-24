# xsites366.py <addr...>: every retail .text/.rdata site holding the 4-byte address; prints VA, the 3 preceding opcode bytes
# classified (a DeleteFunc store `mov dword ptr [eax+4], imm32`, a call operand, or other; no bytes printed), the enclosing catalog row (address, name) and its reloc_bindings self spellings. NF366.
import sys, os, struct, bisect
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
cat = nf.catalog(); starts = sorted(int(k, 16) for k in cat)
def owner(va):
    i = bisect.bisect_right(starts, va) - 1
    if i < 0: return None
    a = starts[i]; r = cat['%08x' % a]
    return ('%08x' % a, r['name'], int(r['length'])) if va < a + int(r['length']) else ('%08x?' % a, r['name'], int(r['length']))
secs = nf.exe()
for a in sys.argv[1:]:
    t = struct.pack('<I', int(a, 16)); print('##', a, cat.get(a.lower(), {}).get('name'), cat.get(a.lower(), {}).get('length'))
    for nm, base, d, ch in secs:
        i = d.find(t)
        while i >= 0:
            va = base + i; pre = d[max(0, i - 3):i]
            kind = 'store mov [eax+4],imm32' if pre == bytes((0xC7, 0x40, 0x04)) else ('call-rel32 operand (coincidental)' if pre[-1:] == bytes((0xE8,)) else 'other')
            print('  %s %08x %s owner=%s' % (nm, va, kind, owner(va) if nm == '.text' else '-'))
            i = d.find(t, i + 1)
