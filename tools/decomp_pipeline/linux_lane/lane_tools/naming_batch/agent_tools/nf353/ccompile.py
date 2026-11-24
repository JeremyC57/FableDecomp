#!/usr/bin/env python3
"""ccompile.py <addr>: census compile of a landed row's HEAD source (catalog flags, its Leaf), read-only, into work/cobj/<addr>.obj;
grade appended to work/census_grades.log. Lock must be NF353's. One `timeout 900` per call (runner ccensus.sh). Agent NF353."""
import os, sys, shutil, subprocess
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF353'
R = '/home/user/FableDecomp'
os.environ['FABLE_ROOT'] = R; os.environ.pop('FABLE_HDR_OVERLAY', None)
sys.path.insert(0, A + '/tools'); sys.path.insert(0, R + '/tools/decomp_pipeline'); sys.dont_write_bytecode = True
from pathlib import Path
import nf
import verify_and_land as V
a = sys.argv[1].lower().replace('0x', '')
log = open(A + '/work/census_grades.log', 'a')
if nf.lock_owner(a) != 'NF353': print('NOLOCK', a, file=log); sys.exit(1)
src = nf.src_of(a); text = src.read_text(encoding='utf-8')
retail = bytes.fromhex(nf.catalog()[a]['bytes'])
while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
leaf = nf.leaf_of(text, a); fl = nf.flags_of(a, text)
wd = Path(A) / 'work' / 'g' / a; wd.mkdir(parents=True, exist_ok=True)
st, built, sec = V.parity_of(V.vc71(text), a, leaf, retail, wd, V.env(), extra=tuple(fl))
obj = wd / (a + '.obj'); dst = Path(A) / 'work' / 'cobj' / (a + '.obj'); dst.parent.mkdir(exist_ok=True)
if obj.exists(): shutil.copy(obj, dst)
print('C %s %s leaf=%s flags=%s obj=%s' % (a, st, leaf, ' '.join(fl) or '-', 'y' if obj.exists() else 'n'), file=log)
subprocess.run(['python3', R + '/tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/clean.py', A, str(wd)], capture_output=True)
