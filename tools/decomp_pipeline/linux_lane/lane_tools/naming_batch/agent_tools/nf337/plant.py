# plant.py (NF337): work/tree_ctl = work/tree + one `typedef int nf337_planted_<X>;` per hdr/engine header, on the line just above the
# inserted `#ifdef FABLE_RETAIL_<X>_COPY_DECLS` (inside the class body, a path every includer of that class model reads). equiv_check(repo,
# tree_ctl) must flag the includers; equiv_check(repo, tree) must flag none.
import os, glob, shutil, subprocess
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF337'
W = A + '/work'; T = W + '/tree_ctl'
subprocess.run(['python3', '/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py', A, T], capture_output=True)
shutil.copytree(W + '/tree', T, symlinks=True); n = 0
for f in sorted(glob.glob(A + '/hdr/engine/*.h')):
    X = os.path.basename(f)[:-2]; p = T + '/engine/' + X + '.h'; L = open(p).read().split('\n')
    i = L.index('#ifdef FABLE_RETAIL_%s_COPY_DECLS' % X.upper()); L.insert(i, '    typedef int nf337_planted_%s;' % X)
    open(p, 'w').write('\n'.join(L)); n += 1
print('planted', n)
