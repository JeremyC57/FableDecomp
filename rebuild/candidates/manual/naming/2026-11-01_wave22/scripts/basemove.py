# basemove: a stand-in member `RET OLD(PARAMS);` declared in a row-local class X is really the base accessor BASE::NEW (PDB).
# move(t, old_rx, new_name, base, base_decl_line) -> (text, how) | (None, reason)
#   old_rx: regex for the one declaration inside X (whole declaration incl ';'), group 'name' = the old method name
#   base_decl_line: the member declaration to put in BASE (e.g. 'bool IsActiveThreadTerminating() const;   // 0x00F35B30 ...')
# Strategy: (1) X derives (directly) from BASE and BASE has a body here: move the declaration into BASE's body;
#           (2) X has no base and the row has no BASE: insert `class BASE { public: <decl> };` before X and derive X from it
#               (an empty base at offset 0; X's layout is unchanged);
#           (3) otherwise: declare nothing in X and call through `((const BASE*)this)->NEW(...)` (a fresh BASE when absent).
#   Calls of OLD( in X's member bodies (unqualified or this->) are renamed to NEW(.
import re, acclib, f2m
def _body(t, name):
    m = re.search(r'\b(class|struct)\s+' + re.escape(name) + r'\b\s*(:[^{;]*)?\{', t)
    if not m: return None
    d = 0
    for j in range(m.end() - 1, len(t)):
        d += {'{': 1, '}': -1}.get(t[j], 0)
        if d == 0: return m, j
    return None
def move(t, old_rx, new_name, base, base_decl, cast_const=True, base_acc='public', force_cast=False):
    c = f2m.code_only(t)
    ms = list(re.finditer(old_rx, c))
    if len(ms) != 1: return None, '%d declarations' % len(ms)
    m = ms[0]; old = m.group('name')
    cs = acclib.class_span(t, m.start())
    if not cs: return None, 'declaration not in a class'
    kw, X, bs, be = cs
    if X == base: return None, 'declared in %s itself' % base
    # the declaration line goes
    ls = t.rfind('\n', 0, m.start()) + 1; le = t.find('\n', m.end())
    line = t[ls:le]
    if t[ls:m.start()].strip() or re.sub(r'//.*', '', t[m.end():le]).strip(): return None, 'declaration shares its line'
    calls_rx = r'(?<![\w.>:])(?:this\s*->\s*)?' + re.escape(old) + r'\s*\('
    other = [x for x in re.finditer(r'(?:\.|->|::)\s*' + re.escape(old) + r'\s*\(', c) if not re.match(r'->\s*', x.group(0)) or c[max(0, x.start() - 4):x.start()] != 'this']
    if re.search(r'\b' + re.escape(old) + r'\s*\([^()]*\)\s*(const\s*)?\{', c): return None, '%s defined here' % old
    hm = re.search(r'\b(class|struct)\s+' + re.escape(X) + r'\b\s*(:[^{;]*)?\{', t)
    bases = hm.group(2)
    bb = _body(t, base)
    t2 = t[:ls] + t[le + 1:]
    how = None
    if force_cast: mode = 3
    elif bases and re.search(r'\b' + re.escape(base) + r'\b', bases) and bb: mode = 1
    elif not bases and not re.search(r'\b(class|struct)\s+' + re.escape(base) + r'\b', c): mode = 2
    else: mode = 3
    if mode == 1:
        bm, bj = _body(t2, base)
        ins = t2.rfind('\n', 0, bj) + 1
        acc = acclib.access_at(t2, bm.group(1), bm.end(), bj)
        lab = '' if acc == base_acc else base_acc + ':\n'
        t2 = t2[:ins] + lab + '    ' + base_decl + '\n' + t2[ins:]
        how = 'moved into %s' % base
        t2 = re.sub(calls_rx, lambda x: new_name + '(', t2)
    elif mode == 2:
        hm2 = re.search(r'\b(class|struct)\s+' + re.escape(X) + r'\b\s*\{', t2)
        st = t2.rfind('\n', 0, hm2.start()) + 1
        t2 = t2[:st] + 'class %s { %s:   // the PDB base the accessor belongs to (empty here: offset 0)\n    %s\n};\n' % (base, base_acc, base_decl) + t2[st:]
        hm2 = re.search(r'\b(class|struct)\s+' + re.escape(X) + r'\b\s*\{', t2)
        t2 = t2[:hm2.end() - 1].rstrip() + ' : public %s ' % base + t2[hm2.end() - 1:]
        how = 'derived from a new %s' % base
        t2 = re.sub(calls_rx, lambda x: new_name + '(', t2)
    else:
        if not bb and not re.search(r'\b(class|struct)\s+' + re.escape(base) + r'\b', c):
            hm2 = re.search(r'\b(class|struct)\s+' + re.escape(X) + r'\b[^{;]*\{', t2)
            st = t2.rfind('\n', 0, hm2.start()) + 1
            t2 = t2[:st] + 'class %s { public:   // the PDB class the accessor belongs to\n    %s\n};\n' % (base, base_decl) + t2[st:]
        elif bb:
            return None, '%s defined here but not a direct base of %s' % (base, X)
        else:
            return None, '%s declared (incomplete) here' % base
        if other: return None, 'called on another object (%d)' % len(other)
        q = 'const ' if cast_const else ''
        t2 = re.sub(calls_rx, lambda x: '((%s%s*)this)->%s(' % (q, base, new_name), t2)
        how = 'called through a %s cast' % base
    return t2, '%s::%s -> %s::%s (%s)' % (X, old, base, new_name, how)
