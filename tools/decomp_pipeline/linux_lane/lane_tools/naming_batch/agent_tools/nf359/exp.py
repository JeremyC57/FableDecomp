# exp.py <addr> <src> <tag>: one experimental VC7.1 compile + grade of <src> as row <addr> (lock required; FABLE_HDR_OVERLAY from env) -> work/exp/obj/<tag>.obj (NF355)
import sys, shutil, subprocess
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF359/tools'); import nf
a, src, tag = sys.argv[1:4]
if nf.lock_owner(a) != 'NF359': print('NOLOCK', a); sys.exit(1)
st, leaf, fl, obj, wd = nf.compile_one(a, src, tag)
if obj.exists(): shutil.copy(obj, nf.W / 'exp' / 'obj' / (tag + '.obj'))
print('EXP', tag, a, st, leaf, ' '.join(fl), flush=True)
subprocess.run(['python3', str(nf.R / 'tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py'), str(nf.A), str(wd)], capture_output=True)
