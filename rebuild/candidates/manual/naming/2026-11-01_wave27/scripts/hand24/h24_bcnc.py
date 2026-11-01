# h24_bcnc.py: naming wave 24, family bcnc -- runs fam_bcnc24.py on runs/in_bcnc.rows.txt (the rows naming the public-virtual
# ??1CBaseClassNonCopyable@@UAE@XZ at 0x0099A300: runs/base_bcnc.rows.txt minus runs/excl_bcnc.txt).
import os, sys, subprocess
W = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
R = os.path.join(W, 'runs')
B = [l.split()[0] for l in open(os.path.join(R, 'base_bcnc.rows.txt')) if l.strip()]
xf = os.path.join(R, 'excl_bcnc.txt'); X = set(l.split()[0] for l in open(xf) if l.strip()) if os.path.exists(xf) else set()
open(os.path.join(R, 'in_bcnc.rows.txt'), 'w').write(''.join(a + '\n' for a in B if a not in X))
r = subprocess.run([sys.executable, os.path.join(W, 'scripts', 'fam_bcnc24.py'), os.environ.get('HE_OV', 'ov_hand'), os.path.join(R, 'in_bcnc.rows.txt')], capture_output=True, text=True, cwd=W)
for l in r.stdout.splitlines():
    print(l.replace('edited', 'ok'))
sys.stderr.write(r.stderr)
