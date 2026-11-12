#!/usr/bin/env python3
"""vt.py <addr> <test.cpp> [row.obj] -- build and run a behaviour test with the LANDER's own check (VC7.1 under Wine).
Prints `<addr> PASS|FAIL|TCC_FAIL|LINK_FAIL` (+ the first compiler errors on TCC_FAIL). Always exits 0."""
import os, sys

def _sweep_orphan_compilers(max_age=300):
    """Kill Wine cl.exe / link.exe older than max_age seconds: orphans of timed-out graders spin at 100% CPU for hours."""
    try:
        import subprocess as _sp, os as _os, signal as _sig
        out = _sp.run(['ps', '-eo', 'pid,etimes,comm'], capture_output=True, text=True, timeout=10).stdout
        for line in out.split('\n')[1:]:
            c = line.split()
            if len(c) >= 3 and c[2] in ('cl.exe', 'link.exe') and c[1].isdigit() and int(c[1]) > max_age:
                try: _os.kill(int(c[0]), _sig.SIGKILL)
                except Exception: pass
    except Exception:
        pass
_sweep_orphan_compilers()

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
