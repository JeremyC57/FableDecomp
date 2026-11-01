# compose12.py <out_overlay>: re-run every wave-12 stage, in the order first run, on fresh copies of the CURRENT repo.
# Hand scripts copy a file on first touch; families compile before/after (famlib) and revert on any failure.
import sys, os, shutil, subprocess
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); SC = os.path.join(W, 'scripts'); RUNS = os.path.join(W, 'runs')
out = sys.argv[1]
if os.path.exists(os.path.join(W, out)): shutil.rmtree(os.path.join(W, out))
H = lambda s: ('hand', s)
Fm = lambda tag, args, fc=False: ('fam', tag, args, fc)
STEPS = [
  # ---- sub-wave 1: single-callee multi-spelling families
  H('h12_s1a.py'), H('h12_s1c.py'), H('h12_s1b.py'),
  Fm('isstop', ['fam_isstop.py', out, 'isstop.rows.txt']),
  Fm('isstop2', ['fam_base.py', 'isstop', out, 'isstop2.rows.txt']),
  H('h12_s1d.py'),
  Fm('gst', ['fam_decl.py', 'gst', out, 'gst.rows.txt']),
  Fm('gst2', ['fam_decl.py', 'gst', out, 'gst2.rows.txt']),
  Fm('gdi2', ['fam_base.py', 'gdi2', out, 'gdi2.rows.txt']),
  Fm('isat', ['fam_base.py', 'isat', out, 'isat_ok.rows.txt']),
  Fm('isatf', ['fam_free.py', 'isat', out, 'isatf.rows.txt']),
  Fm('gpvf', ['fam_free.py', 'gpv', out, 'gpv.rows.txt']),
  Fm('gpvm', ['fam_gpv.py', out, 'gpvm.rows.txt']),
  Fm('sbisat', ['fam_base.py', 'sbisat', out, 'sbisat_ok.rows.txt']),
  Fm('isat_fc', ['fam_base.py', 'isat', out, 'isat_fc.rows.txt'], True),
  H('h12_t1.py'), H('h12_s1e.py'),
  # ---- sub-wave 2: chunk-row leaf / callee fixes
  H('h12_s2a.py'), H('h12_s2b.py'), H('h12_s2c.py'), H('h12_s2d.py'), H('h12_s2e.py'),
  Fm('auto', ['fam_auto.py', out, 'auto_pairs.tsv', 'auto_ok.rows.txt']),
  H('h12_t2.py'),
]
env0 = dict(os.environ, HE_OV=out)
for st in STEPS:
    if st[0] == 'hand':
        r = subprocess.run([sys.executable, os.path.join(SC, 'hand', st[1])], capture_output=True, text=True, env=env0, cwd=os.path.join(SC, 'hand'))
        bad = [l for l in r.stdout.splitlines() if not (l.startswith('ok') or l.startswith('already'))]
        print(st[1], 'ok', r.stdout.count('ok '), 'already', r.stdout.count('already '), 'other', len(bad), r.stderr.strip()[-300:]); [print('   ', l) for l in bad[:5]]
    else:
        _, tag, args, fc = st
        args = [os.path.join(SC, args[0])] + [os.path.join(RUNS, x) if x.endswith(('.txt', '.tsv')) else x for x in args[1:]]
        env = dict(os.environ, QTAG='nw12_c' + tag); env.pop('FORCE_CAST', None)
        if fc: env['FORCE_CAST'] = '1'
        r = subprocess.run([sys.executable] + args, capture_output=True, text=True, cwd='/home/user/FableDecomp', env=env)
        open(os.path.join(W, 'logs', 'compose_%s_%s.log' % (out, tag)), 'w').write(r.stdout + r.stderr)
        L = r.stdout.splitlines(); c = {k: sum(1 for l in L if l.startswith(k)) for k in ('edited', 'skip', 'already')}
        print(tag, c, r.stderr.strip()[-300:]); [print('   ', l[:200]) for l in L if l.startswith('skip')][:0]
    sys.stdout.flush()
