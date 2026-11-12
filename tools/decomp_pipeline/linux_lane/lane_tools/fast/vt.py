#!/usr/bin/env python3
"""vt.py <addr> <test.cpp> [row.obj] -- build and run a behaviour test with the LANDER's own check (VC7.1 under Wine).
Prints `<addr> PASS|FAIL|TCC_FAIL|LINK_FAIL` (+ the first compiler errors on TCC_FAIL). Always exits 0."""
import os, sys
try:
    sys.path.insert(0, 'tools/decomp_pipeline'); import verify_and_land as V
    from pathlib import Path
    a = sys.argv[1].lower().replace('0x', ''); t = sys.argv[2]; sobj = sys.argv[3] if len(sys.argv) > 3 else None
    w = Path(os.environ['S']) / 'wine' / 'vt' / a; w.mkdir(parents=True, exist_ok=True)
    e = V.env(); r = V.behaviour_of(open(t, encoding='utf-8').read(), a, a + '_TEST PASS', w, e, sobj)
    print(a, r)
    if r == 'TCC_FAIL':
        tc = V.cl(['/nologo', '/c', '/Od', '/W3', f"/Fo{w / (a + '.t.obj')}", str(w / (a + '.test.cpp'))], e)
        print('\n'.join(l[-200:] for l in (tc.stdout or '').splitlines() if 'error' in l)[:1500])
except Exception as ex:
    print('vt.py could not run:', type(ex).__name__, str(ex)[:200])
sys.exit(0)
