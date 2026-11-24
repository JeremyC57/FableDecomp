# declsum.py <decl output>: compact view: per entry, the rows whose local declaration lines carry a pointer (`Class*`) where the PDB
# spelling has a reference, plus every row flagged OFF / LOCK and the no-local-decl count
import sys, re
cur = None; row = None
for l in open(sys.argv[1]):
    l = l.rstrip('\n')
    if l.startswith('#'): print(l[:150]); continue
    if l.startswith('  no-local'): print('   ' + l.strip()[:120]); continue
    m = re.match(r'  (\w+)(\(self\))?( OFF)?( LOCK=\S+)?', l)
    if m and not l.startswith('      '): row = m.group(1) + (m.group(2) or '') + (m.group(3) or '') + (m.group(4) or ''); continue
    if l.startswith('      L') and re.search(r'\w\s*\*\s*\w*\s*[,)]', l): print('   %s %s' % (row, l.strip()[:170]))
