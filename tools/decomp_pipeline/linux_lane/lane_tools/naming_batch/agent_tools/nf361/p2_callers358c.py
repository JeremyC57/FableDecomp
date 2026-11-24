# callers358c.py: callers358.py retargeted at the uncatalogued retail functions that hold copy call sites (second-level leg).
# callers358.py: NF361 census. Every retail call / jmp rel32 (E8 / E9) whose target is one of the three copies, mapped to the landed
# catalog row whose [address, address+length) holds the site (build_candidates.ps1 entries x auto-re-candidates length). Also counts
# raw 32-bit absolute pointers to the copy (none expected for a non-virtual). Output: work/callers.tsv: copy site kind row(or -) src.
import sys, re, struct, bisect, collections
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF361/tools'); import nf
T = {0x00428494:'CreateMenuEntryComponent',0x0057d23b:'FUN_0057d23b',0x00590F0B:'GetItemsListUIComponentPC',0x005AC10E:'ConstructActualBuyInventoryPC',0x005B9641:'fn_005b9641',0x005FE840:'ConstructRegionExitMarkers',0x007070D0:'UpdateGraphics',0x007FF670:'fn_007ff670',0x007FF720:'fn_007ff720',0x00973900:'SummonPillarsOld'}
landed = set(m.group(1).lower() for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'", nf.PS)); cat = nf.catalog(); ln = {}
for a in landed:
    r = cat.get(a)
    if r and r.get('length', '').isdigit(): ln[int(a, 16)] = int(r['length'])
# all catalog rows (landed or not) for the unlanded-site attribution
allr = {}
for a, r in cat.items():
    if r.get('length', '').isdigit(): allr[int(a, 16)] = int(r['length'])
st = sorted(ln); sa = sorted(allr)
def row(site, st, ln):
    k = bisect.bisect_right(st, site) - 1
    return '%08x' % st[k] if k >= 0 and st[k] <= site < st[k] + ln[st[k]] else None
out = open(nf.W / 'callers_unlanded_fns.tsv', 'w')
for nm, va, d, ch in nf.exe():
    if not ch & 0x20: continue
    for m in re.finditer(rb'[\xe8\xe9]', d):
        p = m.start()
        if p + 5 > len(d): break
        tgt = (va + p + 5 + struct.unpack_from('<i', d, p + 1)[0]) & 0xffffffff
        if tgt in T:
            r = row(va + p, st, ln); rc = row(va + p, sa, allr)
            out.write('%08x\t%08x\t%s\t%s\t%s\t%s\n' % (tgt, va + p, 'call' if d[p] == 0xE8 else 'jmp', r or '-', rc or '-', nf.field(r, 'Source') if r else '-'))
for nm, va, d, ch in nf.exe():
    for t in T:
        for m in re.finditer(re.escape(struct.pack('<I', t)), d):
            out.write('%08x\t%08x\tabs32:%s\t%s\t%s\t-\n' % (t, va + m.start(), nm, row(va + m.start(), st, ln) or '-', row(va + m.start(), sa, allr) or '-'))
out.close()
