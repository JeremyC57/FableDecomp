# compose every wave-9 change on fresh copies of the CURRENT repo: compose9.py <out_overlay>
# 1. hand scripts (exact replacements; he.py copies a file from the repo on first touch), in order
# 2. scripted families on their verified row lists, in order (each family compiles and keeps only verified edits)
import sys, os, shutil, subprocess
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); SC = os.path.join(W, 'scripts')
out = sys.argv[1]
if os.path.exists(os.path.join(W, out)): shutil.rmtree(os.path.join(W, out))
env = dict(os.environ, HE_OV=out)
for h in ('h9_opeq.py', 'h9_self.py', 'h9_ren.py'):
    r = subprocess.run([sys.executable, os.path.join(SC, 'hand', h)], capture_output=True, text=True, env=env, cwd=os.path.join(SC, 'hand'))
    bad = [l for l in r.stdout.splitlines() if not (l.startswith('ok') or l.startswith('already'))]
    print(h, 'ok', r.stdout.count('ok '), 'already', r.stdout.count('already '), 'other', len(bad), r.stderr.strip()[-300:]); [print('   ', l) for l in bad[:5]]
FAMS = [('fam_acc9.py', 'runs/acc9.final.tasks.tsv'), ('fam_cita9.py', 'runs/cita9.rows.txt'), ('fam_xfl9.py', 'runs/xfl9.rows.txt'),
        ('fam_gror9.py', 'runs/gror9.rows.txt'), ('fam_aia9.py', 'runs/aia9f.rows.txt'), ('fam_rtc9.py', 'runs/rtc9f.rows.txt'),
        ('fam_gws9.py', 'runs/gws9.rows.txt'), ('fam_nga9.py', 'runs/nga9.rows.txt')]
for f, lst in FAMS:
    r = subprocess.run([sys.executable, os.path.join(SC, f), out, os.path.join(W, lst)], capture_output=True, text=True, cwd='/home/user/FableDecomp')
    L = r.stdout.splitlines(); c = {k: sum(1 for l in L if l.startswith(k)) for k in ('edited', 'skip', 'already')}
    print(f, c, r.stderr.strip()[-300:]); [print('   ', l[:200]) for l in L if l.startswith('skip')][:8]
