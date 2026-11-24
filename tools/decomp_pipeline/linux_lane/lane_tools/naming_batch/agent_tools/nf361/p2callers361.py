# p2callers361.py <target hex>: Part 2 retail leg. Every E8 / E9 rel32 in the executable sections whose target is <target>, mapped to the
# landed catalog row whose [address, address+length) holds the site (build_candidates.ps1 x auto-re-candidates length), else to any
# catalog row, else '-'. Also counts abs32 pointers to the target in every section. Output p2/callers.tsv: site kind landed_row catalog_row.
import sys, re, struct, bisect
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF361/tools'); import nf
T = int(sys.argv[1], 16)
landed = set(m.group(1).lower() for m in re.finditer(r"Address = '([0-9a-fA-F]{8})'", nf.PS)); cat = nf.catalog(); ln = {}; allr = {}
for a, r in cat.items():
    if r.get('length', '').isdigit():
        allr[int(a, 16)] = int(r['length'])
        if a in landed: ln[int(a, 16)] = int(r['length'])
st = sorted(ln); sa = sorted(allr)
def row(site, st, ln):
    k = bisect.bisect_right(st, site) - 1
    return '%08x' % st[k] if k >= 0 and st[k] <= site < st[k] + ln[st[k]] else '-'
out = open(str(nf.A) + '/p2/callers.tsv', 'w'); n = 0; ab = 0
for nm, va, d, ch in nf.exe():
    if ch & 0x20:
        for m in re.finditer(rb'[\xe8\xe9]', d):
            p = m.start()
            if p + 5 > len(d): break
            if (va + p + 5 + struct.unpack_from('<i', d, p + 1)[0]) & 0xffffffff == T:
                site = va + p; n += 1
                print('%08x\t%s\t%s\t%s' % (site, 'call' if d[p] == 0xe8 else 'jmp', row(site, st, ln), row(site, sa, allr)), file=out)
    for m in re.finditer(re.escape(struct.pack('<I', T)), d): ab += 1; print('%08x\tabs32(%s)\t%s\t%s' % (va + m.start(), nm, row(va + m.start(), st, ln), row(va + m.start(), sa, allr)), file=out)
out.close(); print('sites', n, 'abs32', ab)
