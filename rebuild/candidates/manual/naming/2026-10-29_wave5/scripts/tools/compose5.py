# compose every wave-5 family on fresh repo copies: compose5.py <out_overlay>
# the scripted families run only on their verified file lists (in this order), then the hand scripts (exact replacements
# / whole-file rewrites; they copy a file from the repo only when no family wrote it). Prints per-family edit counts.
import sys, os, shutil, subprocess
sys.path.insert(0, os.path.dirname(__file__)); import rows
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
out = os.path.join(W, sys.argv[1])
FAMS = ['lb5', 'lbint5', 'uimgr5']
HAND = ['cb5.py', 'rw5.py', 'cvt5.py', 'fs5.py', 'own5.py']
if os.path.exists(out): shutil.rmtree(out)
lists = {f: [l.strip() for l in open(os.path.join(W, 'fam_%s.files.txt' % f)) if l.strip()] for f in FAMS}
for rel in sorted(set(x for v in lists.values() for x in v)):
    d = os.path.join(out, rel); os.makedirs(os.path.dirname(d), exist_ok=True)
    shutil.copyfile(os.path.join(rows.ROOT, rel), d)
for f in FAMS:
    paths = [os.path.join(out, r) for r in lists[f]]
    n = 0
    for i in range(0, len(paths), 400):
        r = subprocess.run([sys.executable, os.path.join(W, 'fam_%s.py' % f)] + paths[i:i + 400], capture_output=True, text=True)
        n += r.stdout.count('edited '); sys.stderr.write(r.stderr)
    print(f, 'files', len(lists[f]), 'edited', n)
env = dict(os.environ, HE_OV=os.path.relpath(out, W))
for h in HAND:
    r = subprocess.run([sys.executable, os.path.join(W, 'hand', h)], capture_output=True, text=True, env=env)
    print(h, r.stdout.count('ok '), 'edits', r.stdout.count('already '), 'already', r.stderr.strip()[-300:])
