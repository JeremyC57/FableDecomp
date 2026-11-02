# fam_gptr.py <family> <ov> <rows.txt>: respell a pointer global (bound stand-in spellings of one retail address) to the
# established `extern CLASS* NAME;`. The row's `extern OLDTYPE old;` goes (a global-scope declaration is replaced in place, a
# namespace-scope one is removed and the final is inserted before the outermost enclosing namespace); `class CLASS;` is
# forward-declared when the row has no CLASS. When OLDTYPE is not CLASS*, every read becomes `((OLDTYPE)NAME)` and every
# plain assignment `old = e;` becomes `NAME = (CLASS*)(e);` (a reinterpreting cast between unrelated pointer types, or a
# pointer <-> 32-bit integer one: no code change; the row's own class model stays the type its callees are bound under).
#   FAM[f] = (target, final symbol, final identifier, class)
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs, f2m
FAM = {
  'psnd': ('013b8394', '?PSoundManager@@3PAVCASoundManager@@A', 'PSoundManager', 'CASoundManager'),
  'pdisp': ('013b8390', '?PDisplayManager@@3PAVCDisplayManager@@A', 'PDisplayManager', 'CDisplayManager'),
  'pinput': ('013b8388', '?PInputManager@@3PAVCInputManager@@A', 'PInputManager', 'CInputManager'),
  'pgbm': ('013b837c', '?PGraphicBankManager@@3PAVCGraphicBankManager@@A', 'PGraphicBankManager', 'CGraphicBankManager'),
  'pfont': ('013b838c', '?PFontManager@@3PAVCFontManager@@A', 'PFontManager', 'CFontManager'),
}
fam = sys.argv[1]; T, FIN, NEWID, CLS = FAM[fam]
BT, BS = inputs.bindings(); B = BT[T]
E, IX = inputs.index()
SYM = re.compile(r'^\?(\w+)@((?:\w+@)*)@3(.+)A$')
def scopes(c, pos):
    st = []
    for m in re.finditer(r'\bnamespace\s+(\w+)\s*\{|\bnamespace\s*\{|\b(?:class|struct|union|enum)\s+\w+[^;{()]*\{|\{|\}', c[:pos]):
        g = m.group(0)
        if g == '}':
            if st: st.pop()
        elif g.startswith('namespace'): st.append(('ns', m.group(1), m.start()))
        elif g == '{': st.append(('blk', None, m.start()))
        else: st.append(('cls', None, m.start()))
    return st
def stmt_end(c, i):
    d = 0
    for j in range(i, len(c)):
        ch = c[j]
        if ch in '([{': d += 1
        elif ch in ')]}':
            d -= 1
            if d < 0: return -1
        elif ch == ';' and d == 0: return j
    return -1
def tr(t, a):
    olds = [s for s in IX.get(a, {}).get('u', []) if s in B and s != FIN]
    if not olds: return None, 'no stand-in spelling in the index'
    done = []
    for s in olds:
        m = SYM.match(s)
        if not m: return None, 'not a plain non-const global: %s' % s
        name, scope = m.group(1), [x for x in m.group(2).split('@') if x]
        c = f2m.code_only(t)
        rx = r'\bextern\s+((?:const\s+)?[\w:]+(?:\s*\*)?)\s*' + re.escape(name) + r'\s*;'
        ds = list(re.finditer(rx, c))
        if len(ds) != 1: return None, '%d declarations of %s' % (len(ds), name)
        d = ds[0]; oldty = re.sub(r'\s+', '', d.group(1)).replace('const', 'const ')
        sc = scopes(c, d.start())
        if any(k == 'blk' for k, _, _ in sc): return None, '%s declared inside a class / block' % name
        if [n for _, n, _ in sc] != scope: return None, 'scope mismatch for %s' % name
        if re.search(r'\b' + NEWID + r'\b', c): return None, '%s already named here' % NEWID
        hascls = re.search(r'\b(class|struct)\s+' + CLS + r'\b', c)
        if hascls and hascls.group(1) == 'struct': return None, 'row models %s as a struct' % CLS
        same = re.sub(r'\s', '', oldty) == CLS + '*'
        # uses (code only, outside the declaration)
        qual = '::'.join(scope)
        urx = re.compile(r'(?<![\w.])(?<!->)(?:(?:::)?' + (re.escape(qual) + r'::' if qual else '') + r')?' + re.escape(name) + r'\b' if qual else r'(?<![\w.:])(?<!->)' + re.escape(name) + r'\b')
        out = []; i = 0
        uses = [u for u in urx.finditer(c) if not (d.start() <= u.start() < d.end())]
        if qual: uses = [u for u in uses if not c[max(0, u.start() - 2):u.start()] == '::' or c[u.start():].startswith(qual)]
        nr = nw = 0
        for u in uses:
            if u.start() < i: continue
            after = c[u.end():u.end() + 3]
            if re.match(r'\s*=(?!=)', c[u.end():]):
                if same: out.append(t[i:u.start()] + NEWID); i = u.end(); nw += 1; continue
                eq = c.index('=', u.end()); e = stmt_end(c, eq + 1)
                if e < 0: return None, 'assignment of %s not a statement' % name
                rhs = t[eq + 1:e].strip()
                out.append(t[i:u.start()] + '%s = (%s*)(%s)' % (NEWID, CLS, rhs)); i = e; nw += 1; continue
            pre = c[max(0, u.start() - 2):u.start()]
            if re.match(r'\s*(\+\+|--|[-+*/|&^]=)', c[u.end():]) or pre.endswith('&') or pre.endswith('++') or pre.endswith('--'):
                if not same: return None, 'non-assignment write / address of %s' % name
            out.append(t[i:u.start()] + (NEWID if same else '((%s)%s)' % (oldty, NEWID))); i = u.end(); nr += 1
        out.append(t[i:]); t2 = ''.join(out)
        # the declaration (positions before the first use are unchanged)
        c2 = f2m.code_only(t2); d2 = list(re.finditer(rx, c2))
        if len(d2) != 1: return None, 'declaration lost'
        d2 = d2[0]
        decl = ('' if hascls or re.search(r'\bclass\s+' + CLS + r'\s*;', c2) else 'class %s;\n' % CLS) + 'extern %s* %s;' % (CLS, NEWID)
        if not sc:
            ls = t2.rfind('\n', 0, d2.start()) + 1
            pre = t2[ls:d2.start()]
            if pre.strip():
                t2 = t2[:d2.start()] + t2[d2.end():]
                t2 = t2[:ls] + decl + '   // 0x%s (naming wave 13)\n' % T.upper() + t2[ls:]
            else:
                t2 = t2[:ls] + pre + decl.replace('\n', '\n' + pre) + t2[d2.end():]
        else:
            t2 = t2[:d2.start()] + t2[d2.end():]
            st0 = t2.rfind('\n', 0, sc[0][2]) + 1
            t2 = t2[:st0] + decl + '   // 0x%s (naming wave 13)\n' % T.upper() + t2[st0:]
        t = t2; done.append('%s(%s r%d w%d)' % (name, 'same' if same else oldty, nr, nw))
    return t, '%s -> %s' % (' '.join(done), NEWID)
def ck(S0, S1, a):
    return F.diffcheck(S0, S1, lambda s: s in B, lambda s: s == FIN, [FIN])
F.run(sys.argv[2], F.rows_from(sys.argv[3]), tr, ck, 'gptr_' + fam)
