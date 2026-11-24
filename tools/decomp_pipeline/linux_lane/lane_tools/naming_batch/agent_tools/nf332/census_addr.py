# census_addr.py <addr...>: nf.py census for arbitrary retail addresses (not ENTRIES lines): landed rows whose retail body calls /
# jumps to / holds each address (E8/E9 rel32 + absolute dword scan) -> stdout "addr<TAB>rows"
import sys, re, struct, bisect
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF332/tools'); import nf
secs = nf.exe(); cat = nf.catalog(); ln = {}
for a in set(m.group(1).lower() for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'", nf.PS)):
    r = cat.get(a)
    if r and r.get('length', '').isdigit(): ln[int(a, 16)] = int(r['length'])
st = sorted(ln)
def row(site):
    k = bisect.bisect_right(st, site) - 1
    return '%08x' % st[k] if k >= 0 and st[k] <= site < st[k] + ln[st[k]] else None
T = {int(x, 16) for x in sys.argv[1:]}; refs = {t: set() for t in T}
for nm, va, d, ch in secs:
    if ch & 0x20:
        for m in re.finditer(rb'[\xe8\xe9]', d):
            p = m.start()
            if p + 5 > len(d): break
            tgt = (va + p + 5 + struct.unpack_from('<i', d, p + 1)[0]) & 0xffffffff
            if tgt in T:
                rw = row(va + p)
                if rw: refs[tgt].add(rw)
    for t in T:
        for m in re.finditer(re.escape(struct.pack('<I', t)), d):
            rw = row(va + m.start())
            if rw: refs[t].add(rw)
for t in sorted(T): print('%08x\t%s' % (t, ','.join(sorted(refs[t] - {'%08x' % t}))))
