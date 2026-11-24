# ctlcover.py (NF325): per planted header, the flagged rows (work/eq4c.tsv) whose tree_ctl preprocessed text carries its
# `nf330_planted_<X>` token -> evidence/equiv_control_per_header.tsv (every header with a landed includer must have >= 1).
import subprocess, re, collections, glob, os
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF330'; W = A + '/work'
T = W + '/tree_ctl'; R = '/home/user/FableDecomp'
cov = collections.defaultdict(set)
for l in open(W + '/equiv_ctl2.tsv'):
    a, kind, _, _, f = l.rstrip('\n').split('\t')
    r = subprocess.run(['cpp', '-P', '-nostdinc', '-undef', '-D_MSC_VER=1310', '-D_M_IX86=600', '-D_WIN32', '-x', 'c++', '-I', T, '-I', W + '/eqS/stubinc_equiv',
                        '-idirafter', R + '/rebuild/include', R + '/' + f], capture_output=True, text=True, errors='replace')
    for x in set(re.findall(r'nf330_planted_(\w+)', r.stdout)): cov[x].add(a + ':' + kind)
o = open(A + '/evidence/equiv_control_per_header.tsv', 'w'); o.write('header\tflagged_rows\texamples\n')
for f in sorted(glob.glob(A + '/hdr/engine/*.h')):
    X = os.path.basename(f)[:-2]; s = sorted(cov.get(X, ()))
    o.write('%s\t%d\t%s\n' % (X, len(s), ' '.join(s[:6])))
o.close(); print(open(A + '/evidence/equiv_control_per_header.tsv').read())
