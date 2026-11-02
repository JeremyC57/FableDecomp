# cflags.py <ov> <addr> <flags...>: compile the overlay source of a row with the given flags (instead of the row's); prints obj path
import sys, os, subprocess, shutil
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import verify_and_land as V, ovinc
from rows import rows, ROOT
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ov, a = sys.argv[1], sys.argv[2]; fl = sys.argv[3:]
R = rows(); d = R[a]
src = os.path.join(W, ov, 'rebuild/src/compiled', d['Source'])
if not os.path.exists(src): src = os.path.join(ROOT, 'rebuild/src/compiled', d['Source'])
out = os.path.join(W, 'pw', 'cf_' + a); shutil.rmtree(out, ignore_errors=True); os.makedirs(out)
open(os.path.join(out, 'a.cpp'), 'w', encoding='utf-8', errors='surrogateescape').write(ovinc.rewrite(open(src, encoding='utf-8', errors='surrogateescape').read(), os.path.join(W, ov)))
o = os.path.join(out, 'a.obj')
r = subprocess.run(['timeout', '120', V.WINE, str(V.CL_EXE)] + V.tool_args(['/nologo', '/c'] + fl + ['/Fo' + o, os.path.join(out, 'a.cpp')]), capture_output=True, text=True, errors='replace', stdin=subprocess.DEVNULL, env=V.env(), cwd=ROOT)
if not os.path.exists(o): print(r.stdout[-2000:]); sys.exit(1)
print(o)
