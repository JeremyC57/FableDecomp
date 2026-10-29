# qc.py <ov_root> <row>...: compile each row's overlay source (row flags), print symbols added/removed vs the repo index
import sys, os, subprocess, json, shutil
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import verify_and_land as V, coff, inputs
from rows import rows, ROOT
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
ov = os.path.join(W, sys.argv[1]); A = sys.argv[2:]
R = rows(); IDX = {}
for f in ('idx/full.jsonl', 'idx/miss.jsonl'):
    if os.path.exists(os.path.join(W, f)):
        for l in open(os.path.join(W, f)):
            r = json.loads(l)
            if 'err' not in r: IDX[r['a']] = set(r['d']) | set(r['u'])
e = V.env(); d = os.path.join(W, 'pw', 'qc'); os.makedirs(d, exist_ok=True)
for a in A:
    rel = 'rebuild/src/compiled/' + R[a]['Source']; s = os.path.join(ov, rel)
    if not os.path.exists(s): s = os.path.join(ROOT, rel)
    fl = R[a].get('CompilerFlags', '').split() or ['/O2', '/Oy', '/W3']
    o = os.path.join(d, a + '.obj')
    if os.path.exists(o): os.unlink(o)
    lf = open(o + '.log', 'w'); r = subprocess.run(['timeout', '120', V.WINE, str(V.CL_EXE)] + V.tool_args(['/nologo', '/c'] + fl + ['/Fo' + o, s]), stdout=lf, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL, env=e); lf.close(); r.stdout = open(o + '.log').read()
    if not os.path.exists(o):
        print(a, 'COMPILE FAIL', '\n'.join(l for l in r.stdout.splitlines() if 'error' in l)[:1500]); continue
    D, U = coff.syms(o); N = D | U; B = IDX.get(a, set())
    print(a, 'ok', '+', sorted(N - B), '-', sorted(B - N))
