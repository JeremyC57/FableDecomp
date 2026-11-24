# zsbatch.py (NF331) <tag> <listfile>: D1 VC7.1 `cl /Zs` precheck (tools/nf.py zs) of each listed row's HEAD (landed) source, WITH the row's own
# CompilerFlags (nf.flags_of; NF328's zsbatch.sh passed none), in sequence -> work/pre_zs.log; work/zs_<tag>.done when finished.
# Background, timeout 900 on the launch line.
import sys, subprocess
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf
tag, lst = sys.argv[1], sys.argv[2]
for a in open(lst).read().split():
    p = nf.src_of(a); fl = nf.flags_of(a, p.read_text(encoding='utf-8'))
    subprocess.run(['python3', str(nf.A / 'tools/nf.py'), 'zs', 'H_' + a, str(p), 'src'] + fl, capture_output=True, cwd=str(nf.W))
open(nf.W / ('zs_%s.done' % tag), 'w').write('done\n')
