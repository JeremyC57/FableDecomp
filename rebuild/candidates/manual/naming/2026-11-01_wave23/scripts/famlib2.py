# famlib2: famlib.run with defined/undefined split: transform(text, row) -> (new|None, why); check(D0,U0,D1,U1,row) -> None|reason
import sys, os, subprocess, shutil, re
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import verify_and_land as V, coff, inputs
from rows import rows, ROOT
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
R = rows(); E = V.env()
def objsyms(a, path, tag='fam'):
    d = os.path.join(W, 'pw', tag); os.makedirs(d, exist_ok=True); o = os.path.join(d, a + '.obj')
    if os.path.exists(o): os.unlink(o)
    fl = R[a].get('CompilerFlags', '').split() or ['/O2', '/Oy', '/W3']
    lf = open(o + '.log', 'w')
    subprocess.run(['timeout', '120', V.WINE, str(V.CL_EXE)] + V.tool_args(['/nologo', '/c'] + fl + ['/Fo' + o, path]), stdout=lf, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL, env=E); lf.close()
    if not os.path.exists(o): return None, None, [l for l in open(o + '.log', errors='replace').read().splitlines() if 'error' in l][:3]
    D, U = coff.syms(o); return D, U, []
def src_path(ov, a, k='src'):
    rel = ('rebuild/src/compiled/' + R[a]['Source']) if k == 'src' else ('rebuild/tests/' + R[a]['TestSource'])
    p = os.path.join(W, ov, rel)
    if not os.path.exists(p): os.makedirs(os.path.dirname(p), exist_ok=True); shutil.copyfile(os.path.join(ROOT, rel), p)
    return p
def rd(p): return open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
def wr(p, t): open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t)
def run(ov, rowlist, transform, check, tag='fam', post=None):
    for a in rowlist:
        if a not in R: print('skip', a, 'not a landed row'); continue
        p = src_path(ov, a); t0 = rd(p)
        D0, U0, _ = objsyms(a, p, tag)
        if D0 is None: print('skip', a, 'compile fails before'); continue
        try: t1, why = transform(t0, a)
        except Exception as ex: t1, why = None, 'transform error %r' % ex
        if t1 is None: print('skip', a, why); sys.stdout.flush(); continue
        if t1 == t0: print('already', a, why); continue
        wr(p, t1); D1, U1, err = objsyms(a, p, tag)
        bad = ('compile: ' + ' | '.join(x[-170:] for x in err)) if D1 is None else check(D0, U0, D1, U1, a)
        if not bad and post:
            t2 = post(t1, D1, U1, a)
            if t2 is None: bad = 'leaf symbol lost'
            elif t2 != t1: wr(p, t2)
        if bad: wr(p, t0); print('skip', a, why, '::', bad)
        else: print('edited', a, why, '+u', sorted(U1 - U0)[:5], '-u', sorted(U0 - U1)[:5])
        sys.stdout.flush()
_PDB = None
def pdb():
    global _PDB
    if _PDB is None:
        N = inputs.names(); _PDB = N['Ego_r.pdb'] | N['FableWin.pdb']
    return _PDB
def code_only(t):
    # mask // and /* */ comments and string literals with spaces (same length)
    out = list(t); i = 0; n = len(t)
    while i < n:
        c = t[i]
        if t.startswith('//', i):
            j = t.find('\n', i); j = n if j < 0 else j
            for k in range(i, j): out[k] = ' '
            i = j
        elif t.startswith('/*', i):
            j = t.find('*/', i + 2); j = n if j < 0 else j + 2
            for k in range(i, j):
                if t[k] != '\n': out[k] = ' '
            i = j
        elif c == '"' or c == "'":
            j = i + 1
            while j < n and t[j] != c:
                j += 2 if t[j] == '\\' else 1
            for k in range(i + 1, min(j, n)): out[k] = ' '
            i = j + 1
        else: i += 1
    return ''.join(out)
def blk_end(c, i):
    d = 0
    for j in range(i, len(c)):
        if c[j] == '{': d += 1
        elif c[j] == '}':
            d -= 1
            if d == 0: return j
    return -1
def class_defs(c, name):
    # definitions (not forward decls) of class/struct `name`: list of (kw, start, body_open, body_close)
    out = []
    for m in re.finditer(r'\b(class|struct)\s+' + re.escape(name) + r'\b(\s*:[^;{]*)?\s*\{', c):
        b = m.end() - 1; e = blk_end(c, b)
        out.append((m.group(1), m.start(), b, e))
    return out
def rename_code(t, old, new):
    c = code_only(t); out = []; last = 0
    for m in re.finditer(r'(?<![\w$])' + re.escape(old) + r'(?![\w$])', c):
        out.append(t[last:m.start()]); out.append(new); last = m.end()
    out.append(t[last:]); return ''.join(out)
