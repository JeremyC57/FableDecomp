# prelist.py <kind: test|head|new> <addr file>: clang -fsyntax-only precheck (nf.py pre) of each row's landed test / HEAD source / new
# source (rows/<addr>.cpp); a failure is retried with -fno-wchar (NOWCHAR). Results in work/pre.log; errors in work/pre_err_<label>.txt.
import sys, os, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
kind = sys.argv[1]
for a in [l.strip() for l in open(sys.argv[2]) if l.strip()]:
    f = {'test': nf.test_of(a), 'head': nf.src_of(a), 'new': nf.A / 'rows' / (a + '.cpp')}[kind]
    if f is None or not os.path.exists(f): open(nf.W / 'pre.log', 'a').write('%s_%s NOFILE\n' % (kind, a)); continue
    lab = '%s_%s' % (kind, a)
    nf.cmd_pre(lab, str(f))
    last = open(nf.W / 'pre.log').read().splitlines()[-1]
    if ' FAIL' in last: nf.cmd_pre(lab, str(f), True)
print('PRELIST_DONE', kind)
