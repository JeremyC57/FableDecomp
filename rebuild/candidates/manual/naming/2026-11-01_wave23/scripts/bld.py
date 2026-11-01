# bld.py <ov> <addr> [outdir]: compile the overlay (or repo) source of a row with its flags; prints the obj path or the errors
import sys, os, subprocess, shutil
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import verify_and_land as V
from rows import rows, ROOT
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ov, a = sys.argv[1], sys.argv[2].lower().replace('0x', '').zfill(8)
R = rows(); d = R[a]
src = os.path.join(W, ov, 'rebuild/src/compiled', d['Source'])
if not os.path.exists(src): src = os.path.join(ROOT, 'rebuild/src/compiled', d['Source'])
out = sys.argv[3] if len(sys.argv) > 3 else os.path.join(W, 'pw', 'dd_' + a)
shutil.rmtree(out, ignore_errors=True); os.makedirs(out)
shutil.copyfile(src, os.path.join(out, 'a.cpp'))
fl = d.get('CompilerFlags', '').split() or ['/O2', '/Oy', '/W3']
o = os.path.join(out, 'a.obj')
r = subprocess.run(['timeout', '120', V.WINE, str(V.CL_EXE)] + V.tool_args(['/nologo', '/c'] + fl + ['/Fo' + o, os.path.join(out, 'a.cpp')]), capture_output=True, text=True, errors='replace', stdin=subprocess.DEVNULL, env=V.env(), cwd=ROOT)
if not os.path.exists(o): print('\n'.join(l for l in r.stdout.splitlines() if 'error' in l or 'warning' in l)[:3000]); sys.exit(1)
print(o)
