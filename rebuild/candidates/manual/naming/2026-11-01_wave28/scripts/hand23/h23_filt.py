# h23_filt.py: naming wave 23, family filt (DECISIONS_W22_RESOLVED item 2) -- runs fam_filt23.py on runs/in_filt.rows.txt
# (runs/base_filt.rows.txt minus runs/excl_filt.txt) with the re-identifications in runs/filt_rename.tsv.
import os, sys, subprocess
W = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
R = os.path.join(W, 'runs')
B = [l.split()[0] for l in open(os.path.join(R, 'base_filt.rows.txt')) if l.strip()]
xf = os.path.join(R, 'excl_filt.txt'); X = set(l.split()[0] for l in open(xf) if l.strip()) if os.path.exists(xf) else set()
open(os.path.join(R, 'in_filt.rows.txt'), 'w').write(''.join(a + '\n' for a in B if a not in X))
r = subprocess.run([sys.executable, os.path.join(W, 'scripts', 'fam_filt23.py'), os.path.join(R, 'in_filt.rows.txt'), os.path.join(R, 'filt_rename.tsv')], capture_output=True, text=True)
for l in r.stdout.splitlines():
    print(l.replace('edited', 'ok'))
sys.stderr.write(r.stderr)
