# foldscan358.py: folded-TU leg. Every landed row whose source includes (quoted, transitively, inside rebuild/src/compiled) the Source file
# of a census row that DEFs one of the copies; prints those NOT in work/cand.tsv (they would emit the same copy without being censused).
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
C = '/home/user/FableDecomp/rebuild/src/compiled'; E = nf.A / 'evidence'
emit = {}
for c in ('00429c6f', '0042b312', '0042b040'):
    for l in open(E / ('CENSUS_%s.tsv' % c)):
        p = l.rstrip('\n').split('\t')
        if len(p) > 5 and p[5].startswith('DEF'): emit.setdefault(os.path.normpath(os.path.join(C, nf.field(p[0], 'Source'))), set()).add((p[0], c))
cand = set(l.split('\t')[0] for l in open(nf.W / 'cand.tsv'))
def incs(p, seen):
    if p in seen or not os.path.exists(p): return
    seen.add(p)
    for m in re.finditer(r'^\s*#\s*include\s+"([^"]+)"', open(p, encoding='utf-8', errors='replace').read(), re.M):
        q = os.path.normpath(os.path.join(os.path.dirname(p), m.group(1)))
        if not os.path.exists(q): q = os.path.normpath(os.path.join('/home/user/FableDecomp/rebuild/include', m.group(1)))   # `../src/compiled/..` resolves via -I rebuild/include
        if q.startswith(C): incs(q, seen)
rows = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", nf.PS, re.S):
    a = re.search(r"\bAddress = '([0-9A-Fa-f]{8})'", m.group(1)); s = re.search(r"\bSource = '([^']*)'", m.group(1))
    if a and s: rows.setdefault(a.group(1).lower(), os.path.normpath(os.path.join(C, s.group(1))))
hits = 0; folded_any = 0
with open(nf.W / 'foldscan.tsv', 'w') as o:
    for a, s in sorted(rows.items()):
        seen = set(); incs(s, seen); seen.discard(s)
        if seen: folded_any += 1
        for f in seen:
            if f in emit:
                new = a not in cand
                o.write('%s\t%s\t%s\t%s\n' % (a, 'NEW' if new else 'in_census', os.path.basename(f), ','.join('%s:%s' % x for x in sorted(emit[f]))))
                hits += new
print('landed rows with folded includes:', folded_any, 'NEW hits:', hits)
