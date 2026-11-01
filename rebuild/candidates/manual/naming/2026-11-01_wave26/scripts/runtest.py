# runtest.py <ov> <addr>: compile the row's source (row flags) and test (/Od), link both, run under wine; print rc + stdout
import sys, os, subprocess, shutil
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import verify_and_land as V
from rows import rows, ROOT
from pathlib import Path
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ov, a = sys.argv[1], sys.argv[2].lower().replace('0x', '').zfill(8)
R = rows(); d = R[a]
def pick(rel):
    p = os.path.join(W, ov, rel); return p if os.path.exists(p) else os.path.join(ROOT, rel)
src = pick('rebuild/src/compiled/' + d['Source']); tst = pick('rebuild/tests/' + d['TestSource'])
out = os.path.join(W, 'pw', 'rt_' + a); shutil.rmtree(out, ignore_errors=True); os.makedirs(out)
shutil.copyfile(src, out + '/a.cpp'); shutil.copyfile(tst, out + '/t.cpp')
e = V.env(); fl = d.get('CompilerFlags', '').split() or ['/O2', '/Oy', '/W3']
r = subprocess.run(['timeout', '120', V.WINE, str(V.CL_EXE)] + V.tool_args(['/nologo', '/c'] + fl + ['/Fo' + out + '/a.obj', out + '/a.cpp']), capture_output=True, text=True, errors='replace', stdin=subprocess.DEVNULL, env=e, cwd=ROOT)
print('SRC', [l for l in r.stdout.splitlines() if 'error' in l][:5])
r = subprocess.run(['timeout', '120', V.WINE, str(V.CL_EXE)] + V.tool_args(['/nologo', '/c', '/Od', '/W3', '/Fo' + out + '/t.obj', out + '/t.cpp']), capture_output=True, text=True, errors='replace', stdin=subprocess.DEVNULL, env=e, cwd=ROOT)
print('TST', [l for l in r.stdout.splitlines() if 'error' in l][:5])
exe = out + '/t.exe'
lk = V.run_tool(V.VC / 'bin' / 'link.exe', ['/nologo', '/out:' + exe, out + '/t.obj'], e)
if lk.returncode != 0 or not os.path.exists(exe):
    print('self link failed:', lk.stdout[-600:])
    lk = V.run_tool(V.VC / 'bin' / 'link.exe', ['/nologo', '/out:' + exe, out + '/a.obj', out + '/t.obj'], e)
    if lk.returncode != 0: print('LINK_FAIL', lk.stdout[-1500:]); sys.exit(1)
rn = subprocess.run(['timeout', '60', V.WINE, exe], capture_output=True, text=True, errors='replace', env=e)
print('rc', rn.returncode, rn.stdout[-500:], rn.stderr[-300:])
