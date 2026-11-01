# compose19.py <out_overlay> [family ...]: re-run every wave-19 stage, in order, on fresh copies of the CURRENT repo
# (hand scripts and famlib copy a file on first touch). After each stage the files it changed are copied, with their
# cumulative content, into ov_<family>/ (only when out_overlay is ov_final) and the rows whose source or test it changed are
# written to runs/<family>.rows.txt -- so the coordinator can commit family by family, in this order.
# With family names: run only the stages up to and including the last named family (for staged work).
import sys, os, shutil, subprocess, hashlib, re
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); SC = os.path.join(W, 'scripts'); RUNS = os.path.join(W, 'runs')
sys.path.insert(0, SC)
from rows import rows
R = rows()
SRC2ROW = {}
for a, d in R.items():
    SRC2ROW['rebuild/src/compiled/' + d.get('Source', '')] = a; SRC2ROW['rebuild/tests/' + d.get('TestSource', '')] = a
out = sys.argv[1]; only = sys.argv[2:]
if os.path.exists(os.path.join(W, out)): shutil.rmtree(os.path.join(W, out))
H = lambda fam, s: ('hand', fam, s)
Fm = lambda fam, args, fc=False: ('fam', fam, args, fc)
# family rows: runs/base_<fam>.rows.txt minus runs/excl_<fam>.txt (rows whose recheck failed: DEFERRED) -> runs/in_<fam>.rows.txt
def inrows(fam):
    b = os.path.join(RUNS, 'base_%s.rows.txt' % fam); x = os.path.join(RUNS, 'excl_%s.txt' % fam)
    B = [l.split()[0] for l in open(b) if l.strip()] if os.path.exists(b) else []
    X = set(l.split()[0] for l in open(x) if l.strip()) if os.path.exists(x) else set()
    open(os.path.join(RUNS, 'in_%s.rows.txt' % fam), 'w').write(''.join(a + '\n' for a in B if a not in X))
    return 'in_%s.rows.txt' % fam
STEPS = [
  H('rfail', 'h19_stl.py'),
  H('c684', 'h19_c684.py'),
  H('c685', 'h19_c685.py'),
  H('c680', 'h19_c680.py'),
]
EXTRA = os.path.join(SC, 'compose19_steps.py')
if os.path.exists(EXTRA): exec(open(EXTRA).read())
if only:
    last = max(i for i, s in enumerate(STEPS) if s[1] in only)
    STEPS = STEPS[:last + 1]
def snap(root):
    m = {}
    for dp, dn, fn in os.walk(root):
        for f in fn:
            p = os.path.join(dp, f); m[os.path.relpath(p, root)] = hashlib.md5(open(p, 'rb').read()).hexdigest()
    return m
ovr = os.path.join(W, out); os.makedirs(ovr, exist_ok=True)
if out == 'ov_final':
    for f in set(s[1] for s in STEPS):
        if os.path.exists(os.path.join(W, 'ov_' + f)): shutil.rmtree(os.path.join(W, 'ov_' + f))
env0 = dict(os.environ, HE_OV=out)
famrows = {}
for st in STEPS:
    before = snap(ovr)
    if st[0] == 'hand':
        _, fam, scr = st
        r = subprocess.run([sys.executable, os.path.join(SC, 'hand19', scr)], capture_output=True, text=True, env=env0, cwd=os.path.join(SC, 'hand19'))
        bad = [l for l in r.stdout.splitlines() if not (l.startswith('ok') or l.startswith('already'))]
        print(fam, scr, 'ok', r.stdout.count('ok '), 'already', r.stdout.count('already '), 'other', len(bad), r.stderr.strip()[-400:]); open(os.path.join(W, 'logs', 'compose_%s_%s_%s.log' % (out, fam, scr)), 'w').write(r.stdout + r.stderr); [print('   ', l) for l in bad[:5]]
    else:
        _, fam, args, fc = st
        args = [os.path.join(SC, args[0])] + [os.path.join(RUNS, x) if x.endswith(('.txt', '.tsv')) else x for x in args[1:]]
        env = dict(os.environ, QTAG='nw19_c' + fam); env.pop('FORCE_CAST', None)
        if fc: env['FORCE_CAST'] = '1'
        r = subprocess.run([sys.executable] + args, capture_output=True, text=True, cwd='/home/user/FableDecomp', env=env)
        open(os.path.join(W, 'logs', 'compose_%s_%s_%s.log' % (out, fam, os.path.basename(args[-1]))), 'w').write(r.stdout + r.stderr)
        L = r.stdout.splitlines(); c = {k: sum(1 for l in L if l.startswith(k)) for k in ('edited', 'skip', 'already')}
        print(fam, os.path.basename(args[0]), c, r.stderr.strip()[-300:])
    after = snap(ovr)
    ch = [p for p in after if before.get(p) != after[p]]
    # a file copied on first touch but unchanged vs the repo is not a change
    ch = [p for p in ch if not (p not in before and open(os.path.join(ovr, p), 'rb').read() == open(os.path.join('/home/user/FableDecomp', p), 'rb').read())]
    fr = famrows.setdefault(fam, set())
    for p in ch:
        a = SRC2ROW.get(p)
        if a: fr.add(a)
        if out == 'ov_final':
            dst = os.path.join(W, 'ov_' + fam, p); os.makedirs(os.path.dirname(dst), exist_ok=True); shutil.copyfile(os.path.join(ovr, p), dst)
    sys.stdout.flush()
if out == 'ov_final':
    for fam, rs in famrows.items():
        open(os.path.join(RUNS, fam + '.rows.txt'), 'w').write(''.join(a + '\n' for a in sorted(rs)))
    print({f: len(r) for f, r in famrows.items()})
# drop first-touch copies that equal the repo (famlib copies a file before it knows whether it will edit it)
for dp, dn, fn in os.walk(ovr):
    for f in fn:
        p = os.path.join(dp, f); rel = os.path.relpath(p, ovr); rp = os.path.join('/home/user/FableDecomp', rel)
        if os.path.exists(rp) and open(p, 'rb').read() == open(rp, 'rb').read(): os.unlink(p)
