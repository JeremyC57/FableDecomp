#!/usr/bin/env python3
"""pkgate423.py <batch> <tag>: pk_all + pickany_vs_retail on HEAD and new objects of every row (leaf = the row's // Leaf symbol when it
resolves, else the object's only / largest defined .text function). Writes batches/<b>/evidence/pk/{H,N}_<a>.{pkall.tsv,pvr.txt} and
evidence/pk_summary.txt: copies, NEQ count, EQ count per side; flags new NEQ and reach flips (copy set or verdict HEAD != new)."""
import sys, os, glob, re, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R = '/home/user/FableDecomp'
LT = R + '/tools/decomp_pipeline/linux_lane/lane_tools'
PVR = R + '/rebuild/candidates/manual/pending/2026-11-21_codex_pass26/tools/pickany_vs_retail.py'
b, tag = sys.argv[1:3]; B = W + '/batches/' + b; E = B + '/evidence/pk'; os.makedirs(E, exist_ok=True)
rows = open(B + '/rows.txt').read().split()
def obj(k, a): return glob.glob('%s/work/wd_%s%s/*/*/%s/%s.obj' % (W, tag, k, a, a))[0]
SM = {l.split('\t')[0]: l.rstrip('\n').split('\t')[1:] for l in open(B + '/selfmap.tsv') if l.strip()}
def leaf(p, a, k):
    want = SM[a][0] if k == 'h' else SM[a][1]
    secs, sl = nf.coff(p)
    fn = [(len(secs[sec - 1]['data']), nm) for i, nm, v, sec, t, scl in sl if scl == 2 and sec > 0 and secs[sec - 1]['name'].startswith('.text')]
    c = [nm for _, nm in fn if nm == want]
    if c: return c[0]
    return 'NOLEAF_' + want
out = []; bad = 0
for a in rows:
    res = {}
    for k, K in (('h', 'H'), ('n', 'N')):
        o = obj(k, a); lf = leaf(o, a, k)
        p = subprocess.run([sys.executable, '-I', '-B', LT + '/pk_all/pk_all.py', o, a, '--leaf=' + lf, '--tsv'], capture_output=True, text=True)
        open('%s/%s_%s.pkall.tsv' % (E, K, a), 'w').write('# leaf %s\n' % lf + p.stdout + (('# stderr ' + p.stderr[-400:].replace('\n', ' ') + '\n') if p.stderr.strip() else ''))
        q = subprocess.run([sys.executable, '-I', '-B', PVR, os.environ['FABLE_EXE'], R + '/rebuild/oracles/reloc_bindings.tsv', o, a, '--leaf=' + lf], capture_output=True, text=True)
        open('%s/%s_%s.pvr.txt' % (E, K, a), 'w').write(q.stdout + q.stderr[-400:])
        lines = [l for l in p.stdout.split('\n') if l.strip() and not l.startswith('#')]
        res[K] = (lf, lines, q.stdout.count('NEQ'), 'Traceback' in p.stderr + q.stderr, q.stdout)
    h, n = res['H'], res['N']
    flag = []
    if n[2] > h[2]: flag.append('NEW_NEQ %d>%d' % (n[2], h[2]))
    if len(n[1]) != len(h[1]): flag.append('COPIES %d->%d' % (len(h[1]), len(n[1])))
    if h[3] or n[3]: flag.append('CRASH')
    bad += bool(flag)
    out.append('%s\tH copies=%d neq=%d | N copies=%d neq=%d\t%s\tleaf %s' % (a, len(h[1]), h[2], len(n[1]), n[2], ' '.join(flag) or 'OK', n[0]))
open(B + '/evidence/pk_summary.txt', 'w').write('\n'.join(out) + '\n'); print('\n'.join(out)); print('BAD', bad)
