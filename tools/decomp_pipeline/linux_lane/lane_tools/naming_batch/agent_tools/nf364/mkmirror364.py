# mkmirror364.py <root> <addr...>: a read-only mirror of the repo for equiv_check (which reads rebuild/build_candidates.ps1, rebuild/src/compiled/<Source>,
# rebuild/tests/<TestSource> relative to its cwd, and -idirafter rebuild/include). Everything is a symlink into the repo EXCEPT the directories on the
# path of each changed row, which are real directories of per-entry symlinks, and the changed rows themselves, which are real copies of
# rows/<addr>.cpp written at creation (no write ever goes through a symlink).
import sys, os, shutil
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364')
sys.path.insert(0, str(A / 'tools')); import nf
R = Path('/home/user/FableDecomp'); root = Path(sys.argv[1]); assert str(root.resolve()).startswith(str(A)) and not root.exists()
srcs = {nf.src_of(a): A / 'rows' / (a + '.cpp') for a in sys.argv[2:]}
real_dirs = set()
for s in srcs:
    p = s.parent
    while p != R: real_dirs.add(p); p = p.parent
def build(repo_dir, mdir):
    mdir.mkdir(parents=True)
    for e in sorted(os.listdir(repo_dir)):
        rp = repo_dir / e; mp = mdir / e
        if rp in real_dirs: build(rp, mp)
        elif rp in srcs: shutil.copyfile(srcs[rp], mp)
        else: os.symlink(rp, mp)
build(R / 'rebuild', root / 'rebuild')
n = 0
for s, new in srcs.items():
    m = root / s.relative_to(R); assert m.is_file() and not m.is_symlink() and m.read_bytes() == new.read_bytes(); n += 1
print('MIRROR', root, 'real rows', n, 'real dirs', len(real_dirs))
