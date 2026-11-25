# cc.py <src> <obj> [flags...]: one VC7.1 compile with verify_and_land's env (60 s limit), prints cl output.
import sys, subprocess, os
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline')
import verify_and_land as V
src, obj = sys.argv[1], sys.argv[2]; fl = sys.argv[3:] or ['/O2', '/Oy', '/W3']
e = V.env(); cmd = ([V.WINE] if V.WINE else []) + [str(V.CL_EXE)] + V.tool_args(['/nologo', '/c'] + fl + ['/Fo' + obj, src])
try:
    r = subprocess.run(cmd, capture_output=True, text=True, errors='replace', env=e, timeout=int(os.environ.get('CCT', '60')))
    print('RC', r.returncode); print(r.stdout[-4000:]); print(r.stderr[-2000:])
except subprocess.TimeoutExpired: print('TIMEOUT')
