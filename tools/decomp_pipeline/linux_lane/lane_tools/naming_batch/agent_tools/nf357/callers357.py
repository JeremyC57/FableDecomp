# callers357.py: retail call / jmp rel32 sites to the four GetSubDef<T> owners, mapped to landed rows (catalog address + length);
# prints per target: landed caller rows, and those outside work/census_cand.tsv (the 85-candidate recount set).
import sys, re, struct, bisect, collections
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF357/tools'); import nf
T = {0x005BADE1: 'CInventoryItemDef', 0x0042B1A6: 'CWeaponDef', 0x0065F604: 'CEntitySoundDef', 0x0042B0A2: 'CAppearanceDef'}
landed = set(m.group(1).lower() for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'", nf.PS)); cat = nf.catalog(); ln = {}
for a in landed:
    r = cat.get(a)
    if r and r.get('length', '').isdigit(): ln[int(a, 16)] = int(r['length'])
st = sorted(ln)
def row(site):
    k = bisect.bisect_right(st, site) - 1
    return '%08x' % st[k] if k >= 0 and st[k] <= site < st[k] + ln[st[k]] else None
cand = set(l.split('\t')[0] for l in open(nf.W / 'census_cand.tsv'))
refs = collections.defaultdict(set); unl = collections.Counter()
for nm, va, d, ch in nf.exe():
    if not ch & 0x20: continue
    for m in re.finditer(rb'[\xe8\xe9]', d):
        p = m.start()
        if p + 5 > len(d): break
        tgt = (va + p + 5 + struct.unpack_from('<i', d, p + 1)[0]) & 0xffffffff
        if tgt in T:
            r = row(va + p)
            if r: refs[tgt].add(r)
            else: unl[tgt] += 1
for t, n in T.items():
    out = sorted(refs[t] - cand - {'%08x' % t})
    print('%08x %s landed_callers=%d outside_census=%d unlanded_sites=%d :: %s' % (t, n, len(refs[t]), len(out), unl[t], ' '.join(out)))
