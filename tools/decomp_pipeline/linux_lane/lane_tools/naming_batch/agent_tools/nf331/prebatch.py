# prebatch.py (NF328, copied by NF331) <tag> <listfile>: clang -fsyntax-only precheck (tools/nf.py pre, NOWCHAR) of each listed row's HEAD (landed) source,
# in sequence -> work/pre.log (one line per source), work/pre_<tag>.done when finished. Background, timeout 900 on the launch line.
import sys, subprocess
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf
tag, lst = sys.argv[1], sys.argv[2]
for a in open(lst).read().split():
    p = nf.src_of(a)
    subprocess.run(['python3', str(nf.A / 'tools/nf.py'), 'pre', 'H_' + a, str(p), 'nowchar'], capture_output=True, cwd=str(nf.W))
open(nf.W / ('pre_%s.done' % tag), 'w').write('done\n')
