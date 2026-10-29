# respell() and helpers of fam_acc9.py, importable (no side effects beyond the env)
# fam_acc9.py <ov_root> <tasks.tsv> : access / const respelling of member declarations (wave 9).
# tasks.tsv: row, wrong symbol, final symbol (final = Ego_r.pdb + FableWin.pdb spelling; the two demangle to the same
# signature except the access level and/or the method const). In the row source: the member's single declaration in its
# class body is wrapped in the final access label (restoring the access in effect after it), and ` const` is added /
# removed after its parameter list and on an out-of-line definition `Class::Member(...)`. Access and const do not change
# code. Each edited file is compiled (row flags): kept only if the object now names the final and no longer the wrong
# spelling, else restored (reported `skip`). Idempotent (a row already emitting the final is reported `already`).
import sys, os, re, subprocess, collections
sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline'); sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import verify_and_land as V, coff, und
from rows import rows, ROOT
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
R = rows(); E = V.env()
def objsyms(a, path):
    d = os.path.join(W, 'pw', 'acc9'); os.makedirs(d, exist_ok=True); o = os.path.join(d, a + '.obj')
    if os.path.exists(o): os.unlink(o)
    fl = R[a].get('CompilerFlags', '').split() or ['/O2', '/Oy', '/W3']
    lf = open(o + '.log', 'w')
    subprocess.run(['timeout', '120', V.WINE, str(V.CL_EXE)] + V.tool_args(['/nologo', '/c'] + fl + ['/Fo' + o, path]), stdout=lf, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL, env=E); lf.close()
    if not os.path.exists(o): return None, [l for l in open(o + '.log', errors='replace').read().splitlines() if 'error' in l][:3]
    D, U = coff.syms(o); return D | U, []
def match_close(t, i, o='(', c=')'):
    d = 0
    for j in range(i, len(t)):
        if t[j] == o: d += 1
        elif t[j] == c:
            d -= 1
            if d == 0: return j
    return -1
def class_bodies(t, cname):
    out = []
    for m in re.finditer(r'\b(class|struct)\s+' + re.escape(cname) + r'\b\s*(?::[^;{]*)?\{', t):
        b = m.end() - 1; e = match_close(t, b, '{', '}')
        if e > 0: out.append((m.group(1), b, e))
    return out
ACC = re.compile(r'\b(public|protected|private)\s*:(?!:)')
def depth1_positions(t, b, e):
    # positions inside the class body at depth 1 (not inside nested braces/parens)
    d = 0; ok = bytearray(len(t))
    for j in range(b + 1, e):
        c = t[j]
        if c in '{(': d += 1
        elif c in '})': d -= 1
        elif d == 0: ok[j] = 1
        if d == 0 and c in '})': ok[j] = 1
    return ok
def respell(t, wrong_d, final_d, cname, member):
    acc_f = final_d.split(':')[0]; const_f = final_d.endswith(') const'); const_w = wrong_d.endswith(') const')
    bodies = class_bodies(t, cname)
    if len(bodies) != 1: return None, 'class %s defined %d times' % (cname, len(bodies))
    key, b, e = bodies[0]; ok = depth1_positions(t, b, e)
    pat = re.compile((r'~\s*' + re.escape(member[1:]) if member.startswith('~') else r'(?<![\w~])' + re.escape(member)) + r'\s*\(')
    ms = [m for m in pat.finditer(t, b, e) if ok[m.start()] and not re.search(r'(\.|->|::)\s*$', t[max(b, m.start() - 3):m.start()])]
    if len(ms) != 1: return None, 'member %s declared %d times in the class' % (member, len(ms))
    m = ms[0]
    # statement start
    s = m.start()
    while s > b + 1:
        c = t[s - 1]
        if ok[s - 1] and c in ';{}': break
        if ok[s - 1] and c == ':' and ACC.search(t, max(b, s - 12), s) and ACC.search(t, max(b, s - 12), s).end() == s: break
        s -= 1
    p0 = t.index('(', m.start()); p1 = match_close(t, p0)
    j = p1 + 1
    while j < e and t[j] not in ';{': j += 1
    if t[j] == '{': j = match_close(t, j, '{', '}') + 1
    else: j += 1
    if j < len(t) and t[j] == ';': j += 1
    stmt = t[s:j]; dp1 = p1 - s
    # const
    if const_f != const_w:
        after = stmt[dp1 + 1:]
        if const_f:
            if not re.match(r'\s*const\b', after): stmt = stmt[:dp1 + 1] + ' const' + after
        else: stmt = stmt[:dp1 + 1] + re.sub(r'^\s*const\b', '', after, count=1)
    # access
    cur = 'public' if key == 'struct' else 'private'
    for am in ACC.finditer(t, b, s):
        if ok[am.start()]: cur = am.group(1)
    if acc_f != cur:
        lead = stmt[:len(stmt) - len(stmt.lstrip())]
        stmt = lead + acc_f + ': ' + stmt.lstrip() + ' ' + cur + ':'
    t2 = t[:s] + stmt + t[j:]
    # out-of-line definition
    if const_f != const_w:
        for dm in re.finditer(r'\b' + re.escape(cname) + r'\s*::\s*' + (r'~\s*' + re.escape(member[1:]) if member.startswith('~') else re.escape(member)) + r'\s*\(', t2):
            q0 = t2.index('(', dm.start()); q1 = match_close(t2, q0)
            rest = t2[q1 + 1:]
            nxt = re.match(r'\s*(const\s*)?(\{|:)', rest)
            if not nxt: continue
            if const_f and not nxt.group(1): t2 = t2[:q1 + 1] + ' const' + rest
            elif not const_f and nxt.group(1): t2 = t2[:q1 + 1] + rest.replace(nxt.group(1), '', 1)
    return t2, 'ok'
