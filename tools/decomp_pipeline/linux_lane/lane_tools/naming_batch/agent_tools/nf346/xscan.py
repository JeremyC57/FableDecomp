# xscan.py <addr...>: exe scan of Fable.exe code for rel32 calls / jumps to, and dword references of, each address; prints each site with the
# catalog row (landed or not) that contains it. Adapted from nf.py census (no ENTRIES file).
import sys, os, re, struct, bisect, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
T = [int(x.replace('0x', ''), 16) for x in sys.argv[1:]]
cat = nf.catalog(); landed = set(m.group(1).lower() for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'", nf.PS))
ln = {int(a, 16): int(r['length']) for a, r in cat.items() if r.get('length', '').isdigit()}
st = sorted(ln)
def row(site):
    k = bisect.bisect_right(st, site) - 1
    return '%08x' % st[k] if k >= 0 and st[k] <= site < st[k] + ln[st[k]] else None
for nm, va, d, ch in nf.exe():
    if not ch & 0x20: continue
    for m in re.finditer(rb'[\xe8\xe9]', d):
        p = m.start()
        if p + 5 > len(d): break
        tgt = (va + p + 5 + struct.unpack_from('<i', d, p + 1)[0]) & 0xffffffff
        if tgt in T:
            r = row(va + p); print('%08x <- %s %08x row=%s %s' % (tgt, 'call' if d[p] == 0xe8 else 'jmp', va + p, r, ('LANDED ' + cat[r]['name']) if r in landed else ('notlanded ' + (cat[r]['name'] if r else ''))))
    for t in T:
        for m in re.finditer(re.escape(struct.pack('<I', t)), d):
            r = row(va + m.start()); print('%08x <- dword %08x row=%s %s' % (t, va + m.start(), r, 'LANDED' if r in landed else ''))
