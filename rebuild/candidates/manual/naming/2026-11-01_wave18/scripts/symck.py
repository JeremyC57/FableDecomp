# symck.py <ov> <rows.txt>: compile each listed row's overlay source; print its leaf, whether the object defines it, and the
# undefined symbols (minus ??_C / __real constants)
import sys, os, re, concurrent.futures as cf
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F
ov, rf = sys.argv[1:3]
A = [l.split()[0] for l in open(rf) if l.strip()]
def one(a):
    p = F.src_path(ov, a); t = open(p, errors='replace').read()
    leaf = ([l[9:].strip() for l in t.split('\n') if l.startswith('// Leaf: ')] or ['?'])[0]
    import coff, subprocess
    d = os.path.join(F.W, 'pw', 'symck'); os.makedirs(d, exist_ok=True); o = os.path.join(d, a + '.obj')
    if os.path.exists(o): os.unlink(o)
    fl = F.R[a].get('CompilerFlags', '').split() or ['/O2', '/Oy', '/W3']
    r = subprocess.run(['timeout', '120', F.V.WINE, str(F.V.CL_EXE)] + F.V.tool_args(['/nologo', '/c'] + fl + ['/Fo' + o, p]), capture_output=True, text=True, stdin=subprocess.DEVNULL, env=F.E)
    if not os.path.exists(o): return a, leaf, 'NOCOMPILE', [l for l in r.stdout.splitlines() if 'error' in l][:3]
    D, U = coff.syms(o)
    return a, leaf, 'DEF' if leaf in D else 'NODEF', sorted(s for s in U if not s.startswith(('??_C@', '__real')))
with cf.ThreadPoolExecutor(3) as ex:
    for a, leaf, st, u in ex.map(one, A):
        print(a, st, leaf, '| U:', ' '.join(u)[:600])
