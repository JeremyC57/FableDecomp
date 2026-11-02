# fam_stools19.py <ov> <rows.txt> [tag]: CAIStateGroupBase::PeekThingSearchTools -- the PDB member is the protected const reference getter
# ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ (Ego_r / FableWin); the landed rows declare a public
# `CThingSearchTools* PeekThingSearchTools();` on CAIStateGroupBase (or on the row's own derived stand-in class). Name-only respell:
#   - the declaration becomes `const CThingSearchTools& PeekThingSearchTools() const;` under protected: in the row's CAIStateGroupBase
#     (moved there from a derived class that derives from CAIStateGroupBase in the row text);
#   - `PeekThingSearchTools()->m(...)` becomes `PeekThingSearchTools().m(...)`, `*PeekThingSearchTools()` the plain reference, a pointer use
#     `&PeekThingSearchTools()`;
#   - the row's CThingSearchTools model members become const (the PDB members called through the const reference are QBE: the row-local
#     stand-in spellings follow, QAE -> QBE), as do their same-TU noinline bodies.
# check: the stand-in is gone, the final is named, every other symbol change is a QAE -> QBE respell of a CThingSearchTools member.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib2 as F, inputs, acclib
import fam_dtor_set as FS
from memlib import depth_at
ov, rf = sys.argv[1], sys.argv[2]; TAG = sys.argv[3] if len(sys.argv) > 3 else 'stools'
T = '008fd000'; FIN = '?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ'
BT, BS = inputs.bindings(); B = set(BT.get(T, ()))
DECL = 'const CThingSearchTools& PeekThingSearchTools() const;   // 0x008FD000 (Ego_r.pdb ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ, protected)'
def stmt_span(c, pos):
    j = pos
    while j > 0 and c[j - 1] not in ';{}:': j -= 1
    while j < pos and c[j] in ' \t\n': j += 1
    k = c.index(';', pos) + 1
    return j, k
def enclosing_class(c, pos):
    best = None
    for m in re.finditer(r'\b(class|struct)\s+(\w+)\b(\s*:[^;{]*)?\s*\{', c):
        b = m.end() - 1; e = F.blk_end(c, b)
        if b < pos < e and (best is None or b > best[2]): best = (m.group(1), m.group(2), b, e, m.start(), m.group(3) or '')
    return best
def const_members(t):
    # every member function declaration / inline definition of the row's CThingSearchTools model (and its out-of-line bodies) becomes const
    c = F.code_only(t); d = F.class_defs(c, 'CThingSearchTools')
    if len(d) != 1: return t, 0
    kw, s, b, e = d[0]; n = 0; out = []; last = 0
    for m in re.finditer(r'\)\s*(const\s*)?([;{])', c):
        if not (b < m.start() < e) or depth_at(c, m.start()) != depth_at(c, b) + 1: continue
        if m.group(1): continue
        # the statement must be a function declaration (a name and '(' before the ')')
        j = m.start(); dep = 0
        while j > b:
            if c[j] == ')': dep += 1
            elif c[j] == '(':
                dep -= 1
                if dep == 0: break
            j -= 1
        head = c[max(b + 1, stmt_span(c, j)[0]):j]
        if not re.search(r'\w\s*$', head) or re.search(r'\b(return|if|while|for|switch)\b', head) or '=' in head: continue
        if re.search(r'\b(static|friend|typedef)\b', head): continue
        if re.search(r'\bCThingSearchTools\s*$', head.strip()) and '~' not in head: continue   # a constructor
        out.append(t[last:m.start() + 1]); out.append(' const'); last = m.start() + 1; n += 1
    out.append(t[last:]); t = ''.join(out)
    # out-of-line bodies
    c = F.code_only(t); out = []; last = 0
    for m in re.finditer(r'\bCThingSearchTools::\w+\s*\(', c):
        k = m.end() - 1; dep = 0
        while k < len(c):
            if c[k] == '(': dep += 1
            elif c[k] == ')':
                dep -= 1
                if dep == 0: break
            k += 1
        mm = re.match(r'\s*\{', c[k + 1:])
        if not mm: continue
        out.append(t[last:k + 1]); out.append(' const'); last = k + 1; n += 1
    out.append(t[last:]); return ''.join(out), n
def tr(t, a):
    c = F.code_only(t)
    ms = [m for m in re.finditer(r'\bCThingSearchTools\s*\*\s*PeekThingSearchTools\s*\(\s*\)\s*;', c)]
    if len(ms) != 1: return None, '%d pointer declarations of PeekThingSearchTools' % len(ms)
    m = ms[0]; cls = enclosing_class(c, m.start())
    if not cls: return None, 'declaration outside a class'
    kw, S, b, e, s0, bases = cls
    ds, de = stmt_span(c, m.start())
    if S == 'CAIStateGroupBase':
        t = FS.set_stmt(t, c, kw, b, ds, de, DECL, 'protected'); mode = 'inplace'
    else:
        bl = [re.sub(r'\b(public|protected|private|virtual)\b', '', x).strip() for x in bases.lstrip(' :').split(',')] if bases else []
        if 'CAIStateGroupBase' not in bl: return None, 'declared on %s, which does not derive from CAIStateGroupBase in the row text' % S
        d2 = F.class_defs(c, 'CAIStateGroupBase')
        if len(d2) != 1: return None, '%d definitions of CAIStateGroupBase' % len(d2)
        t = FS.del_stmt(t, ds, de)
        c = F.code_only(t); kw2, s2, b2, e2 = F.class_defs(c, 'CAIStateGroupBase')[0]
        acc0 = 'private' if kw2 == 'class' else 'public'
        t = t[:b2 + 1] + '\nprotected:\n    ' + DECL + '\n' + acc0 + ':' + t[b2 + 1:]; mode = 'moved from %s' % S
        if not re.search(r'\b(class|struct)\s+CThingSearchTools\b', c[:s2]):
            ls = t.rfind('\n', 0, s2) + 1
            t = t[:ls] + 'class CThingSearchTools;\n' + t[ls:]
    # call sites
    c = F.code_only(t); out = []; last = 0; n = 0
    for cm in re.finditer(r'(?<![\w$])PeekThingSearchTools\s*\(\s*\)', c):
        if cm.start() < last: continue
        pre = cm.start() - 1
        while pre >= 0 and c[pre] in ' \t': pre -= 1
        # the declaration itself
        if re.search(r'&\s*$', c[max(0, pre - 1):pre + 1]) or re.search(r'const\s*$', c[cm.start() - 7:cm.start()]): continue
        k = cm.end(); nxt = c[k:k + 3]
        if nxt.startswith('->'): out.append(t[last:k]); out.append('.'); last = k + 2; n += 1
        elif c[pre] == '*' and (pre == 0 or c[pre - 1] not in ')]' and not c[pre - 1].isalnum()):   # a dereference: drop the star
            out.append(t[last:pre]); out.append(t[pre + 1:k]); last = k; n += 1
        elif re.match(r'\s*\.', c[k:]): continue
        else: out.append(t[last:cm.start()]); out.append('(CThingSearchTools*)&'); out.append(t[cm.start():k]); last = k; n += 1   # a pointer use (the row's free helpers take the pointer)
    out.append(t[last:]); t = ''.join(out)
    t, nc = const_members(t)
    t = re.sub(r'PeekThingSearchTools\(\)->', 'PeekThingSearchTools().', t)   # comments
    return t, '%s; %d call(s) rewritten, %d CThingSearchTools member(s) made const' % (mode, n, nc)
def ck(D0, U0, D1, U1, a):
    st = {s for s in U0 if s in B and s != FIN}
    if st & U1: return 'stand-in still named'
    if FIN not in U1: return 'final not named'
    hz = lambda s: '?A0x' in s
    rem = [s for s in (U0 | D0) - (U1 | D1) if s not in st and not hz(s)]
    add = [s for s in (U1 | D1) - (U0 | D0) if s != FIN and not hz(s)]
    conv = lambda s: s.replace('@@QBE', '@@QAE').replace('@@ABE', '@@AAE').replace('@@IBE', '@@IAE')
    radd = [s for s in add if not ('@CThingSearchTools@@' in s and conv(s) in rem)]
    rrem = [s for s in rem if not ('@CThingSearchTools@@' in s and s in [conv(x) for x in add])]
    if rrem or radd: return 'diff -%s +%s' % (rrem[:3], radd[:3])
    return None
F.run(ov, [l.split()[0] for l in open(rf) if l.strip()], tr, ck, TAG)
