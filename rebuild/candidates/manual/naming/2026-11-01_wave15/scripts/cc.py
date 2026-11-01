# compile a C++ text with the lane cl.exe (/c /Od /W3, the repo include path) under `timeout 120`; return [(line, message)]
import os, sys, re, subprocess
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline')
import verify_and_land as V
from pathlib import Path
PW = Path(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))) / 'pw' / 'cc'; PW.mkdir(parents=True, exist_ok=True)
_E = None
def errors(text, tag='x'):
    global _E
    if _E is None: _E = V.env()
    tp = PW / ('%s.cpp' % tag); tp.write_text(text, encoding='utf-8', errors='surrogateescape')
    args = ['/nologo', '/c', '/Od', '/W3', '/Fo%s' % (PW / ('%s.obj' % tag)), str(tp)]
    cmd = ['timeout', '120'] + ([V.WINE] if V.WINE else []) + [str(V.CL_EXE)] + V.tool_args(args)
    r = subprocess.run(cmd, capture_output=True, text=True, errors='replace', env=_E, cwd='/home/user/FableDecomp')
    if r.returncode == 124: return [(0, 'TIMEOUT')]
    out = []
    for l in r.stdout.splitlines():
        m = re.search(r'\((\d+)\) : (?:fatal )?error (C\d+)(.*)', l)
        if m: out.append((int(m.group(1)), m.group(2) + m.group(3)))
    if r.returncode and not out: out.append((0, 'RC%d %s' % (r.returncode, r.stdout[-300:])))
    return out
