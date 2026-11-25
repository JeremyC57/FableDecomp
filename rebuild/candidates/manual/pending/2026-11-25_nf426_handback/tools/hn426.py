#!/usr/bin/env python3
"""hn426.py <batch dir> <addr...>: VC7.1 HEAD (repo row) and new (batch rows/) objects -> <batch>/obj/H_<a>.obj, <a>.obj; grade line per side;
then the new test (/Od) linked with the new row object and run (PASS/FAIL), and the leaf COMDAT selection of each side."""
import sys, os, shutil, subprocess, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf, coff
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); import verify_and_land as V
B = sys.argv[1]; os.makedirs(B + '/obj', exist_ok=True)
def leafsec(obj, leaf):
    for s in coff.sections(obj):
        if s['name'].startswith('.text') and s['sym'] and s['sym'].startswith(leaf): return s
def test(a, obj, src_test, tag):
    wd = nf.W / ('t_' + tag + a); wd.mkdir(parents=True, exist_ok=True); e = V.env()
    tp = wd / 't.cpp'; tp.write_text(open(src_test).read()); to = wd / 't.obj'; ex = wd / 't.exe'
    for f in (to, ex):
        if f.exists(): f.unlink()
    r = V.run_tool(V.CL_EXE, ['/nologo', '/c', '/Od', '/W3', '/Fo%s' % to, str(tp)], e)
    if not to.exists(): return 'TCC_FAIL ' + r.stdout[-300:]
    r0 = V.run_tool(V.VC / 'bin' / 'link.exe', ['/nologo', '/out:%s' % ex, str(to)], e)
    if r0.returncode == 0: return 'SELF_CONTAINED_LINK(not linking row)'
    r = V.run_tool(V.VC / 'bin' / 'link.exe', ['/nologo', '/out:%s' % ex, str(obj), str(to)], e)
    if not ex.exists(): return 'LINK_FAIL ' + (r.stdout or '')[-300:]
    rn = subprocess.run(([V.WINE] if V.WINE else []) + [str(ex)], capture_output=True, text=True, env=e, timeout=60)
    return ('PASS' if rn.returncode == 0 and 'PASS' in rn.stdout else 'FAIL rc=%d' % rn.returncode)
for a in sys.argv[2:]:
    out = []
    for t, p in (('H', nf.src_of(a)), ('N', B + '/rows/%s.cpp' % a)):
        st, leaf, fl, obj, wd = nf.compile_one(a, p, t)
        dst = B + '/obj/' + ('H_' if t == 'H' else '') + a + '.obj'
        if obj.exists(): shutil.copy(obj, dst)
        s = leafsec(dst, leaf) if os.path.exists(dst) else None
        out.append('%s=%s sel=%s len=%s' % (t, getattr(st, 'name', st), s and s['sel'], s and s['size']))
        if t == 'H': hb = s and s['raw']
        else: nb = s and s['raw']
    tr = test(a, B + '/obj/%s.obj' % a, B + '/tests/%s_test.cpp' % a, 'N') if os.path.exists(B + '/tests/%s_test.cpp' % a) else '-'
    print(a, ' '.join(out), 'leafbytes', 'SAME' if hb == nb and hb else 'DIFF', 'test', tr, flush=True)
