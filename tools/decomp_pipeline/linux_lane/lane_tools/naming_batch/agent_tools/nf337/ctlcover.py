# ctlcover.py (NF337) <equiv ctl tsv>: which planted headers each flagged row reads (preprocess against work/tree_ctl, grep the planted
# typedef names) -> per-header row counts; every changed header must be seen by at least one landed includer.
import re, subprocess, sys, collections, glob, os
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF337'; T = A + '/work/tree_ctl'
c = collections.Counter()
for l in open(sys.argv[1]):
    f = l.rstrip('\n').split('\t')[-1]
    r = subprocess.run(['cpp', '-P', '-nostdinc', '-undef', '-D_MSC_VER=1310', '-D_M_IX86=600', '-D_WIN32', '-x', 'c++', '-I', T, '-I', A + '/work/eqS/stubinc_equiv', '-idirafter', os.path.abspath('rebuild/include'), f], capture_output=True, text=True, errors='replace')
    for x in set(re.findall(r'nf337_planted_(\w+)', r.stdout)): c[x] += 1
for f in sorted(glob.glob(A + '/hdr/engine/*.h')):
    X = os.path.basename(f)[:-2]; print('%-45s %d' % (X, c[X]))
