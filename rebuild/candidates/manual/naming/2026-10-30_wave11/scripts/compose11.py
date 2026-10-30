# compose wave-11 changes on fresh copies of the CURRENT repo: compose10.py <out_overlay> <stage...>
# stages: sw1 (sub-wave 1: hand h10_gwf/h10_gmp + fam_isv / fam_gmin / fam_gmp), sw586, sw2, sw3 ... in the order given
import sys, os, shutil, subprocess
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); SC = os.path.join(W, 'scripts')
out = sys.argv[1]; stages = sys.argv[2:]
if os.path.exists(os.path.join(W, out)): shutil.rmtree(os.path.join(W, out))
env = dict(os.environ, HE_OV=out)
STAGES = {}
def load_extra():
    p = os.path.join(SC, 'stages.py')
    if os.path.exists(p):
        g = {}; exec(open(p).read(), g); STAGES.update(g['STAGES'])
load_extra()
for st in stages:
    hands, fams = STAGES[st]
    for h in hands:
        r = subprocess.run([sys.executable, os.path.join(SC, 'hand', h)], capture_output=True, text=True, env=env, cwd=os.path.join(SC, 'hand'))
        bad = [l for l in r.stdout.splitlines() if not (l.startswith('ok') or l.startswith('already'))]
        print(st, h, 'ok', r.stdout.count('ok '), 'already', r.stdout.count('already '), 'other', len(bad), r.stderr.strip()[-300:]); [print('   ', l) for l in bad[:5]]
    for f, lst in fams:
        r = subprocess.run([sys.executable, os.path.join(SC, f), out, os.path.join(W, lst)], capture_output=True, text=True, cwd='/home/user/FableDecomp')
        open(os.path.join(W, 'logs', 'compose_%s_%s.log' % (out, f[:-3])), 'w').write(r.stdout + r.stderr)  # famlog
        L = r.stdout.splitlines(); c = {k: sum(1 for l in L if l.startswith(k)) for k in ('edited', 'skip', 'already')}
        print(st, f, c, r.stderr.strip()[-300:]); [print('   ', l[:200]) for l in L if l.startswith('skip')][:8]
