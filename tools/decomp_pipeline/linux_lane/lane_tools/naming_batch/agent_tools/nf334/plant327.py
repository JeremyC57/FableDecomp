# plant327.py (NF327, from NF325's plant.py): work/eq_ctl = work/eq_new + one `typedef int nf327_planted_<X>;` per hdr/ header, on a path every
# existing includer reads: add-only opt-in headers -> the line just above the inserted #ifdef; _VIRTUAL_MODEL headers -> first line of the
# default #else branch. equiv_check(eq_old, eq_ctl) must flag the includers (new headers have none).
import os, glob, shutil, subprocess
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF334'
W = A + '/work'; T = W + '/eq_ctl'
subprocess.run(['python3', '/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py', A, T], capture_output=True)
shutil.copytree(W + '/eq_new', T, symlinks=True)
anchors = {'CGameScriptInterface': '#ifdef FABLE_RETAIL_CGAMESCRIPTINTERFACE_ENDCUTFADE_CALLEES',
           'CCreatureAction_Tankard': '#ifdef FABLE_RETAIL_CCREATUREACTION_PICKUPTHINGATTABLE_COPYCTOR_DECL',
           'CIPhysicsFacesSegmentBase': '#ifdef FABLE_RETAIL_CIPHYSICSFACESSEGMENTBASE_FRIEND_BRANCH'}
for f in sorted(glob.glob(A + '/hdr/engine/*.h')):
    X = os.path.basename(f)[:-2]; p = T + '/engine/' + X + '.h'; L = open(p).read().split('\n'); pl = 'typedef int nf327_planted_%s;' % X
    if X in anchors:
        i = next(k for k, l in enumerate(L) if l.startswith(anchors[X])); L.insert(i, pl)
    elif '#ifdef FABLE_RETAIL_%s_VIRTUAL_MODEL' % X.upper() in L:
        i = L.index('#else', L.index('#ifdef FABLE_RETAIL_%s_VIRTUAL_MODEL' % X.upper())); L.insert(i + 1, pl)
    else:
        print('NO ANCHOR', X); continue
    open(p, 'w').write('\n'.join(L)); print('planted', X)
