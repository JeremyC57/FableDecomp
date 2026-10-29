# LABELS.tsv for wave 8: proven names only. For every wave-8 final (targets.FINAL, MANUAL_PROVEN) that a rechecked row now
# emits, the verifier's RELOC_NAME warning names the catalog label at that address; where the label's qualified name differs
# from the final's, the label is respelled. usage: mklabels8.py <run tag>...
import os, sys, re, json, collections
W = os.path.dirname(os.path.abspath(__file__)); os.chdir(W); sys.path.insert(0, 'tools')
import und, qn
from targets import FINAL, CONF, MANUAL_PROVEN
lab = collections.defaultdict(set); seen = collections.defaultdict(set)
pat = re.compile(r"^(\w{8})\s+WARN RELOC_NAME: (\S+) -> (\w{8}) labelled '(.*)'$")
for tag in sys.argv[1:]:
    for l in open(os.path.join('runs', tag + '.log'), errors='replace'):
        m = pat.match(l.rstrip('\n'))
        if m: lab[m.group(3)].add(m.group(4)); seen[m.group(3)].add((m.group(2), m.group(1)))
U = und.get(list(FINAL.values()))
out = [('address', 'name', 'note')]
for a in sorted(FINAL):
    if a not in MANUAL_PROVEN or a not in lab: continue
    s = FINAL[a]; q = qn.qname(U.get(s, s))
    if s not in {x for x, _ in seen[a]}: continue
    labels = sorted(lab[a]); heads = {re.split(r'[ (]', L, 1)[0] for L in labels}
    if heads == {q}: continue
    rows = sorted({r for x, r in seen[a] if x == s})
    out.append((a, q, 'label says %s; %s; emitted as %s by %d rechecked row(s) (%s)' % (' / '.join(labels), CONF.get(a, ''), s, len(rows), ','.join(rows[:8]))))
open('LABELS.tsv', 'w').write(''.join('\t'.join(r).replace('\n', ' ') + '\n' for r in out)); print(len(out) - 1, 'labels')
