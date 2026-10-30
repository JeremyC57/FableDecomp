# shared driver for wave-10 families: famlib.run(ov, rows, transform, check)
#   transform(text, row) -> (new_text | None, reason); check(before_syms, after_syms, row) -> None (ok) | reason
# Each row's source is copied into the overlay on first touch (from the CURRENT repo), transformed, compiled with the row's
# flags; kept only when it compiles and check() passes, else restored. Prints edited / skip / already lines.
import sys, os, subprocess, shutil
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import verify_and_land as V, coff
from rows import rows, ROOT
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
R = rows(); E = V.env()
def objsyms(a, path, tag='fam'):
    d = os.path.join(W, 'pw', tag); os.makedirs(d, exist_ok=True); o = os.path.join(d, a + '.obj')
    if os.path.exists(o): os.unlink(o)
    fl = R[a].get('CompilerFlags', '').split() or ['/O2', '/Oy', '/W3']
    lf = open(o + '.log', 'w')
    subprocess.run(['timeout', '120', V.WINE, str(V.CL_EXE)] + V.tool_args(['/nologo', '/c'] + fl + ['/Fo' + o, path]), stdout=lf, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL, env=E); lf.close()
    if not os.path.exists(o): return None, [l for l in open(o + '.log', errors='replace').read().splitlines() if 'error' in l][:3]
    D, U = coff.syms(o); return D | U, []
def src_path(ov, a, k='src'):
    rel = ('rebuild/src/compiled/' + R[a]['Source']) if k == 'src' else ('rebuild/tests/' + R[a]['TestSource'])
    p = os.path.join(W, ov, rel)
    if not os.path.exists(p): os.makedirs(os.path.dirname(p), exist_ok=True); shutil.copyfile(os.path.join(ROOT, rel), p)
    return p
def rd(p): return open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
def wr(p, t): open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t)
def run(ov, rowlist, transform, check, tag='fam'):
    for a in rowlist:
        if a not in R: print('skip', a, 'not a landed row'); continue
        p = src_path(ov, a); t0 = rd(p)
        S0, _ = objsyms(a, p, tag)
        if S0 is None: print('skip', a, 'compile fails before'); continue
        t1, why = transform(t0, a)
        if t1 is None: print('skip', a, why); continue
        if t1 == t0: print('already', a, why); continue
        wr(p, t1); S1, err = objsyms(a, p, tag)
        bad = ('compile: ' + ' | '.join(x[-170:] for x in err)) if S1 is None else check(S0, S1, a)
        if bad: wr(p, t0); print('skip', a, bad)
        else: print('edited', a, why, '+', sorted(S1 - S0)[:6], '-', sorted(S0 - S1)[:6])
        sys.stdout.flush()
def hashy(s): return '?A0x' in s
def diffcheck(S0, S1, rem_ok, add_ok, need=None):
    """rem_ok(sym)/add_ok(sym) predicates; need: symbols that must be present after. Returns None or reason."""
    rem = [s for s in S0 - S1 if not hashy(s) and not rem_ok(s)]
    add = [s for s in S1 - S0 if not hashy(s) and not add_ok(s)]
    miss = [s for s in (need or ()) if s not in S1]
    if rem or add or miss: return 'diff: -%s +%s missing %s' % (rem[:3], add[:3], miss[:2])
    return None
def rows_from(path): return [l.split()[0] for l in open(path) if l.strip() and not l.startswith('#')]
