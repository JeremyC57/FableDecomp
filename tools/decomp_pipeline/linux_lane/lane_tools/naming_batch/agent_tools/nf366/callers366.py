# callers366.py <addr...>: retail E8 / E9 rel32 sites whose target is <addr> (a byte scan of .text; candidates, not a disassembly), with the
# enclosing catalog row. NF366.
import sys, os, struct, bisect
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
cat = nf.catalog(); starts = sorted(int(k, 16) for k in cat)
def owner(va):
    i = bisect.bisect_right(starts, va) - 1
    a = starts[i]; r = cat['%08x' % a]
    return '%08x%s %s' % (a, '' if va < a + int(r['length']) else '?', r['name'])
for nm, base, d, ch in nf.exe():
    if nm != '.text': continue
    T = {int(a, 16): a for a in sys.argv[1:]}; hits = {a: [] for a in sys.argv[1:]}
    for i in range(len(d) - 5):
        if d[i] in (0xE8, 0xE9):
            t = (base + i + 5 + struct.unpack_from('<i', d, i + 1)[0]) & 0xffffffff
            if t in T: hits[T[t]].append((base + i, d[i]))
    for a in sys.argv[1:]:
        print('##', a, cat.get(a, {}).get('name'), len(hits[a]))
        for va, op in hits[a]: print('  %08x %s %s' % (va, 'call' if op == 0xE8 else 'jmp', owner(va)))
