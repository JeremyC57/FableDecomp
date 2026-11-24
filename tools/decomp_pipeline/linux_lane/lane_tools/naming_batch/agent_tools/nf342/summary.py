# summary.py (NF342): count tables for SUMMARY.md from work/verdicts.tsv -> work/summary_tables.md (pasted into SUMMARY.md)
import os, sys, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import common as C
V = [l.rstrip('\n').split('\t') for l in open(C.W + '/verdicts.tsv')]
h = V[0]; V = [dict(zip(h, r)) for r in V[1:]]
ORDER = ['STALE', 'PARTIAL', 'OPEN_CHECKED', 'OPEN', 'UNPROVEN', 'NO_P', 'OTHER']
out = []
def table(key, title):
    c = collections.defaultdict(collections.Counter)
    for v in V: c[v[key]][v['verdict']] += 1
    out.append('### ' + title + '\n')
    out.append('| %s | lines | ' % key + ' | '.join(ORDER) + ' |')
    out.append('|---|---:|' + '---:|' * len(ORDER))
    for k in sorted(c, key=lambda k: (-sum(c[k].values()), k)):
        out.append('| %s | %d | ' % (k, sum(c[k].values())) + ' | '.join(str(c[k][o]) if c[k][o] else '' for o in ORDER) + ' |')
    tot = collections.Counter(v['verdict'] for v in V)
    out.append('| **total** | **%d** | ' % len(V) + ' | '.join('**%d**' % tot[o] for o in ORDER) + ' |\n')
table('kind', 'By kind')
table('source', 'By source tag')
open(C.W + '/summary_tables.md', 'w').write('\n'.join(out) + '\n')
print('\n'.join(out[:14]))
