# count346.py: today's-HEAD D3 recount and the NF346 outcome per row, from evidence/pk/{H,N}_<a>.pkall.tsv (pk_all) and work/eval.tsv.
# Per row: HEAD leaf-called NEQ GetDef<T> copies (and GetSubDef, listed apart), how many the define fixes (EQ in new), how many are left.
# Writes evidence/d3_recount.tsv and prints totals by class.
import os, re, glob, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); P = A + '/evidence/pk/'
ev = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A + '/work/eval.tsv')}
def load(p):
    d = {}
    if not os.path.exists(p): return None
    for l in open(p):
        f = l.rstrip('\n').split('\t')
        if l.startswith('#') or len(f) < 10: continue
        d[(f[9], f[2])] = (f[0], f[7] == 'y' or f[8] == 'y')
    return d
rows = sorted(set(re.search(r'H_([0-9a-f]{8})\.pkall', p).group(1) for p in glob.glob(P + 'H_*.pkall.tsv')))
out = ['addr\tclass\thead_neq_getdef\tfixed\tleft\thead_neq_subdef\tleft_getdef_copies']
tot = {}
for a in rows:
    h, n = load(P + 'H_%s.pkall.tsv' % a), load(P + 'N_%s.pkall.tsv' % a)
    hg = [k for k, v in h.items() if v[0].startswith('NEQ') and v[1] and k[0].startswith('??$GetDef@')]
    hs = [k for k, v in h.items() if v[0].startswith('NEQ') and v[1] and k[0].startswith('??$GetSubDef@')]
    c = ev[a][1] if a in ev else 'NOT_PORTED'
    fx = [k for k in hg if n and n.get(k, ('',))[0].startswith('EQ')] if c in ('ACCEPT', 'PARTIAL') else []
    left = [k for k in hg if k not in fx]
    out.append('%s\t%s\t%d\t%d\t%d\t%d\t%s' % (a, c, len(hg), len(fx), len(left), len(hs), ' '.join('%s:%s' % (k[1], re.match(r'\?\?\$GetDef@V?(.*?)@@@', k[0]).group(1)) for k in left)))
    t = tot.setdefault(c, [0, 0, 0, 0, 0]); t[0] += 1; t[1] += len(hg); t[2] += len(fx); t[3] += len(left); t[4] += len(hs)
open(A + '/evidence/d3_recount.tsv', 'w').write('\n'.join(out) + '\n')
for c, t in sorted(tot.items()): print('%-11s rows %3d  head NEQ GetDef %3d  fixed %3d  left %3d  (GetSubDef NEQ %d, untouched)' % (c, *t))
s = [sum(t[i] for t in tot.values()) for i in range(5)]
print('%-11s rows %3d  head NEQ GetDef %3d  fixed %3d  left %3d  (GetSubDef NEQ %d, untouched)' % ('TOTAL', *s))
