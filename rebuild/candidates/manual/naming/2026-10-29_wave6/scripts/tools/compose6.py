# compose every wave-6 change on fresh copies of the CURRENT repo: compose6.py <out_overlay>
# 1. hand scripts (exact replacements / whole-file rewrites; he.py copies a file from the repo on first touch), in order
# 2. scripted families on their verified file lists, in order (a file not yet in the overlay is copied from the repo)
# Prints per-step edit counts; a family printing a `skip` for a listed file is reported (it must be 0).
import sys, os, shutil, subprocess
sys.path.insert(0, os.path.dirname(__file__)); import rows
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
out = os.path.join(W, sys.argv[1])
HAND = ['upm6.py', 'own6.py', 'conf6.py', 'getters6.py']
FAMS = ['xfer6', 'xtext6', 'ccs6', 'dos6', 'glr6', 'tbi6', 'gmp6', 'ref6', 'gpi6']
if os.path.exists(out): shutil.rmtree(out)
env = dict(os.environ, HE_OV=os.path.relpath(out, W))
for h in HAND:
    r = subprocess.run([sys.executable, os.path.join(W, 'hand', h)], capture_output=True, text=True, env=env, cwd=W)
    bad = [l for l in r.stdout.splitlines() if not (l.startswith('ok') or l.startswith('already'))]
    print(h, 'ok', r.stdout.count('ok '), 'already', r.stdout.count('already '), 'other', len(bad), r.stderr.strip()[-300:])
    for l in bad[:5]: print('   ', l)
for f in FAMS:
    rels = [l.strip() for l in open(os.path.join(W, f + '.files.txt')) if l.strip()]
    for rel in rels:
        d = os.path.join(out, rel)
        if not os.path.exists(d):
            os.makedirs(os.path.dirname(d), exist_ok=True); shutil.copyfile(os.path.join(rows.ROOT, rel), d)
    paths = [os.path.join(out, r) for r in rels]
    n = 0; skips = []
    for i in range(0, len(paths), 300):
        r = subprocess.run([sys.executable, os.path.join(W, 'fam_%s.py' % f)] + paths[i:i + 300], capture_output=True, text=True, cwd=W)
        n += r.stdout.count('edited '); skips += [l for l in r.stdout.splitlines() if l.startswith('skip')]
        sys.stderr.write(r.stderr)
    print(f, 'files', len(rels), 'edited', n, 'skipped', len(skips))
    for l in skips[:8]: print('   ', l.replace(out + '/', ''))
