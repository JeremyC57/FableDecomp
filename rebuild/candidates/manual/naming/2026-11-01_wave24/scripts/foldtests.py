# foldtests.py: after compose15.py ov_final -- the 'tests' stage only respells test stubs of rows another family edited; copy
# each such test file into that family's overlay (ov_<fam>) so every family commit links on its own, then drop ov_tests /
# runs/tests.rows.txt. Writes runs/ov_final.files.txt (every file of ov_final) and runs/family_files.tsv.
import os, sys, shutil
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); RUNS = os.path.join(W, 'runs')
sys.path.insert(0, os.path.join(W, 'scripts'))
from rows import rows
R = rows()
FAMS = ['bcn', 'bip', 'cab', 'cmb', 'scb', 'sai', 'anong', 'dec', 'newline', 'w14def', 'queue', 'leaf']
fr = {f: set(l.strip() for l in open(os.path.join(RUNS, f + '.rows.txt')) if l.strip()) for f in FAMS if os.path.exists(os.path.join(RUNS, f + '.rows.txt'))}
T = [l.strip() for l in open(os.path.join(RUNS, 'tests.rows.txt')) if l.strip()]
for a in T:
    owner = [f for f in FAMS if a in fr.get(f, ())]
    if not owner: raise SystemExit('no owner family for ' + a)
    f = owner[-1]   # the last family that edited the row's source (its committed state includes the earlier ones)
    rel = 'rebuild/tests/' + R[a]['TestSource']
    dst = os.path.join(W, 'ov_' + f, rel); os.makedirs(os.path.dirname(dst), exist_ok=True)
    shutil.copyfile(os.path.join(W, 'ov_final', rel), dst); print('test', a, '->', f)
shutil.rmtree(os.path.join(W, 'ov_tests'), ignore_errors=True); os.unlink(os.path.join(RUNS, 'tests.rows.txt'))
files = []
for dp, dn, fn in os.walk(os.path.join(W, 'ov_final')):
    for x in fn: files.append(os.path.relpath(os.path.join(dp, x), os.path.join(W, 'ov_final')))
open(os.path.join(RUNS, 'ov_final.files.txt'), 'w').write(''.join(x + '\n' for x in sorted(files)))
with open(os.path.join(RUNS, 'family_files.tsv'), 'w') as o:
    for f in FAMS:
        d = os.path.join(W, 'ov_' + f)
        for dp, dn, fn in os.walk(d):
            for x in fn: o.write('%s\t%s\n' % (f, os.path.relpath(os.path.join(dp, x), d)))
print(len(files), 'files in ov_final')
