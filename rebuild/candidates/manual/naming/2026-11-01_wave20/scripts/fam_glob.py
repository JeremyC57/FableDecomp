# fam_glob.py <family> <ov> <rows.txt>: respell an extern scalar global (a bound stand-in spelling of one retail address) to the
# established spelling: the row's `extern [const] T old;` declaration becomes FINAL_DECL (at global scope: a namespace-scope
# stand-in is removed and the final declaration is inserted before the outermost enclosing namespace), and every use of the old
# identifier (NS::old or old) is renamed. Only stand-ins with the final's type code are taken (const-ness may differ: A -> B).
#   FAM[f] = (target, final symbol, final identifier, final declaration, mangled type code)
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs, f2m
FAM = {
  'eps': ('0129ba3c', '?EPSILON@@3MB', 'EPSILON', 'extern const float EPSILON;', 'M'),
  'twopi': ('0124f2b8', '?TWO_PI@@3NB', 'TWO_PI', 'extern const double TWO_PI;', 'N'),
}
fam = sys.argv[1]; T, FIN, NEWID, DECL, TY = FAM[fam]
BT, BS = inputs.bindings(); B = BT[T]
E, IX = inputs.index()
SYM = re.compile(r'^\?(\w+)@((?:\w+@)*)@3(.+)([AB])$')
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
def tr(t, a):
    olds = [s for s in IX.get(a, {}).get('u', []) if s in B and s != FIN]
    if not olds: return None, 'no stand-in spelling in the index'
    done = []
    for s in olds:
        m = SYM.match(s)
        if not m: return None, 'not a plain global: %s' % s
        name, scope, ty = m.group(1), [x for x in m.group(2).split('@') if x], m.group(3)
        if ty != TY: return None, 'type %s of %s' % (ty, s)
        c = f2m.code_only(t)
        rx = r'(?m)^[ \t]*extern\s+(?:const\s+)?[\w:]+\s+(?:const\s+)?' + re.escape(name) + r'\s*;[^\n]*\n?'
        ds = list(re.finditer(rx, c)); inline = False
        if not ds:
            # a declaration that shares its line with other code: `...; extern const T name;`
            ds = list(re.finditer(r'\bextern\s+(?:const\s+)?[\w:]+\s+(?:const\s+)?' + re.escape(name) + r'\s*;', c)); inline = True
        if not ds: return None, 'no declaration of %s' % name
        scs = [scopes(c, d.start()) for d in ds]
        if any(k == 'cls' for sc in scs for k, _, _ in sc): return None, '%s declared inside a class' % name
        nss = [[n for k, n, _ in sc if k == 'ns'] for sc in scs]
        if any(x != scope for x in nss): return None, 'scope %s != %s' % (nss, scope)
        if len(ds) > 1 and (inline or scope): return None, '%d declarations of %s' % (len(ds), name)
        have = re.search(r'\b' + NEWID + r'\b', c)
        if have and not re.search(r'\bextern\s+' + re.escape(DECL[7:]), c): return None, '%s already used here' % NEWID
        if not scope and not inline:
            # global-scope or block-scope extern declarations: each is replaced in place
            for d in reversed(ds):
                line = t[d.start():d.end()]; cm = re.search(r'//.*', line)
                ind = re.match(r'[ \t]*', line).group(0)
                new = ind + DECL + ((' ' * max(1, len(line.split('//')[0].rstrip()) - len(ind) - len(DECL)) + cm.group(0)) if cm else '') + ('\n' if line.endswith('\n') else '')
                t = t[:d.start()] + new + t[d.end():]
        else:
            d = ds[0]; sc = scs[0]
            t = t[:d.start()] + t[d.end():]
            if not have:
                nsl = [x for x in sc if x[0] == 'ns']
                st0 = t.rfind('\n', 0, (nsl[0][2] if nsl else d.start())) + 1
                t = t[:st0] + DECL + '   // 0x%s (established spelling, naming wave 13)\n' % T.upper() + t[st0:]
        qual = '::'.join(scope)
        if qual: t = re.sub(r'(?<![\w:])(?:::)?' + re.escape(qual) + r'::' + re.escape(name) + r'\b', NEWID, t)
        t = re.sub(r'(?<![\w.])(?<!->)(?<!::)' + re.escape(name) + r'\b', NEWID, t)
        done.append(name + ('@' + qual if qual else ''))
    return t, '%s -> %s' % (' '.join(done), NEWID)
def ck(S0, S1, a):
    return F.diffcheck(S0, S1, lambda s: s in B, lambda s: s == FIN, [FIN])
F.run(sys.argv[2], F.rows_from(sys.argv[3]), tr, ck, 'glob_' + fam)
