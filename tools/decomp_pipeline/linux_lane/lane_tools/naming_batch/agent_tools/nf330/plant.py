# plant.py: build work/tree_ctl = work/tree + one `typedef int nf330_planted_<X>;` per hdr/ header, placed on a path every existing includer
# reads (generated _VIRTUAL_MODEL headers: first line of the default #else branch; CCombatSequenceBase: inside the widened PASS27 block;
# the add-only opt-in headers: the line just above the inserted #ifdef). equiv_check(repo, tree_ctl) must flag the includers.
import os, glob, shutil, re, subprocess
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF330'
W = A + '/work'; T = W + '/tree_ctl'
subprocess.run(['python3', '/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py', A, T], capture_output=True)
shutil.copytree(W + '/tree', T, symlinks=True)
anchors = {'CAIStateGroup_Gaze': '#ifdef FABLE_RETAIL_CAISTATEGROUP_GAZE_RTTI_DECLS',   # NF330: inside the landed Gaze model branch
           'CEngineInternalPrimitiveBase': '#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_SETCHILDPRIMITIVE_ORDER',
           'CEngineInternalPrimitiveParticleGroup': '#ifdef FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEGROUP_RTTI_SLOTS',
           'CAIStateGroup_EnemyCreatureBase': '#ifdef FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_OVERRIDES',
           'CAInputProcess': '#ifdef FABLE_RETAIL_CAINPUTPROCESS_INIT',
           'CInputProcessManager': '#ifdef FABLE_RETAIL_CINPUTPROCESSMANAGER_FRIEND_CAINPUTPROCESS'}
n = 0
for f in sorted(glob.glob(A + '/hdr/engine/*.h')):
    X = os.path.basename(f)[:-2]; p = T + '/engine/' + X + '.h'; L = open(p).read().split('\n'); pl = 'typedef int nf330_planted_%s;' % X
    if X == 'CCombatSequenceBase':
        i = L.index('#include "stlport/fable_containers.h"'); L.insert(i + 1, pl)
    elif X in anchors:
        i = next(k for k, l in enumerate(L) if l.startswith(anchors[X])); L.insert(i, pl)
    elif '#ifdef FABLE_RETAIL_%s_VIRTUAL_MODEL' % X.upper() in L:
        i0 = L.index('#ifdef FABLE_RETAIL_%s_VIRTUAL_MODEL' % X.upper()); d = 0   # NF330: the #else at depth 0 (nested opt-ins skipped)
        for i in range(i0, len(L)):
            t = L[i].strip()
            if t.startswith('#if'): d += 1
            elif t.startswith('#endif'): d -= 1
            elif t.startswith('#else') and d == 1: break
        L.insert(i + 1, pl)
    elif '#ifdef FABLE_RETAIL_%s_RTTI_DECLS' % X.upper() in L:   # NF330: a base model's add-only RTTI_DECLS block: plant just above it
        i = L.index('#ifdef FABLE_RETAIL_%s_RTTI_DECLS' % X.upper()); L.insert(i, pl)
    else:
        print('NO ANCHOR', X); continue
    open(p, 'w').write('\n'.join(L)); n += 1
print('planted', n)
