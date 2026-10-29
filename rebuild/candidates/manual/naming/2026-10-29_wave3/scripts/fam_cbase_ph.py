#!/usr/bin/env python3
"""Respell the file-scope `CBase` placeholder roots (wave-2A deferrals) to their PDB classes.

Retail roots (Ego_r.pdb + FableWin.pdb, RTTI; naming/CONFLICTS.md):
  A = CBaseClass             ctor 0x0099A2D0 ??0CBaseClass@@QAE@XZ, dtor 0x0099A2E0 ??1CBaseClass@@UAE@XZ (public)
  N = CBaseClassNonCopyable  ctor 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ, dtor 0x0099A300
                             ??1CBaseClassNonCopyable@@MAE@XZ (protected virtual)
In both, vtable slot 0 is the virtual destructor. The rows this script takes model the root as
    struct CBase { virtual void v00(); [CBase();] [~CBase();] };        (v00 / v0 / VSlot0 / VTableAnchor)
i.e. a placeholder virtual for slot 0 next to a ctor (PLACEHOLDER_ROOT) or a NON-virtual dtor (NOVIRT ~CBase).
ROLE (cbase_role.py, the row's relocation target) says which root it is.

Edits (all or nothing per file):
  * the root becomes `class <NEW> {\\n<access>:` (public for A, protected for N; access labels dropped);
  * the placeholder statement goes: with a dtor statement the dtor becomes `virtual ~<NEW>()`, without one the
    placeholder is replaced by `virtual ~<NEW>()` (one vptr either way, slot 0 = the destructor as in retail);
  * a derived class's redeclaration of the placeholder (`[virtual] void v00();` or with an empty inline body, a
    depth-1 statement of a class that names CBase as a direct base) is deleted: the derived class's implicit
    (or declared) destructor now overrides slot 0, so later virtual slots keep their indices;
  * every code use of `CBase` (base clauses, init lists, `CBase::~CBase`) becomes <NEW>; comments
    `CBase::CBase`, `CBase::~CBase` and `CBase` right before a root address follow.
Blockers (no edit): any other use of the placeholder name (a call, an out-of-line definition, a qualified use),
another member in the root, a base clause on the root, an existing <NEW> token, more than one CBase body.
Behaviour tests are edited only when they define CBase members out of line (they link the row object); they keep
the root public. Idempotent (a second run finds no `CBase`).
usage: fam_cbase_ph.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import (mask, scopes, apply_edits, tokens, prev_sig, prev_word, comment_spans, run, drop_nested, del_span,
                    global_classes, row_of, innermost, statements)
from cbase_role import ROLE, A, N

PLACEHOLDER = re.compile(r'^\s*virtual\s+void\s+(v0+|v00|VSlot0|VTableAnchor)\s*\(\s*(?:void)?\s*\)\s*;\s*$')
ADDR = re.compile(r'0x0099A(?:2D0|2E0|2F0|300)\b', re.I)
SKIP = {}
DTOR = {A: '0x0099A2E0', N: '0x0099A300'}


def transform(text, path):
    a, is_test = row_of(path)
    if a not in ROLE or a in SKIP:
        return text, 0, []
    return respell(text, a, is_test, 'CBase', ROLE[a])


def respell(text, a, is_test, old, new, plain=False):
    """Root `old` -> PDB root `new` (A or N). plain: also take a root with a virtual dtor and no placeholder."""
    m = mask(text)
    if is_test and not re.search(r'\b%s\s*::\s*~?\s*%s\s*\(' % (old, old), m):
        return text, 0, []
    acc = 'public' if (new == A or is_test) else 'protected'
    sc = scopes(m)
    cl = global_classes(m, sc, old)
    if not cl:
        return text, 0, []
    if len(cl) != 1:
        return text, 0, ['MULTI %s' % old]
    c = cl[0]
    if tokens(m, new):
        return text, 0, ['EXISTS %s' % new]
    hs = m.rfind(c.key, 0, c.lb)
    if ':' in m[hs:c.lb]:
        return text, 0, ['BASES']
    edits = []
    ctor = dtor = ph = None
    for st, en in statements(m, c.lb, c.rb):
        s = m[st:en]
        if re.match(r'(public|private|protected)\s*:', s):
            edits.append(del_span(text, m, st, en)); continue
        if PLACEHOLDER.match(s):
            if ph is not None: return text, 0, ['TWO_PLACEHOLDERS']
            ph = (st, en); continue
        if re.match(r'^\s*(?:virtual\s+)?~\s*%s\s*\(\s*(?:void)?\s*\)\s*;\s*$' % old, s):
            dtor = (st, en); continue
        if re.match(r'^\s*(?:inline\s+)?%s\s*\(\s*(?:void)?\s*\)\s*(?:;|\{\s*\}\s*;?)\s*$' % old, s):
            ctor = (st, en); continue
        return text, 0, ['BODY %s' % re.sub(r'\s+', ' ', s)[:60]]
    vdtor = dtor is not None and re.match(r'^\s*virtual\b', m[dtor[0]:dtor[1]])
    if ph is None and not (plain and vdtor):
        return text, 0, []          # not a placeholder root (wave-2A shapes)
    if ph is not None and vdtor:
        return text, 0, ['PLACEHOLDER_AND_VIRTUAL_DTOR']
    phn = PLACEHOLDER.match(m[ph[0]:ph[1]]).group(1) if ph is not None else None
    # derived redeclarations of the placeholder
    bases = {}
    for k in sc:
        if k.kind == 'class' and k.name:
            hd = m[m.rfind(k.key, 0, k.lb):k.lb]
            bl = hd.split(':', 1)[1] if ':' in hd else ''
            bases.setdefault(k.name, set()).update(re.findall(r'\b([A-Za-z_]\w*)\b', bl))

    def derives(name, seen=()):
        if name in seen: return False
        bs = bases.get(name, set())
        return old in bs or any(derives(b, seen + (name,)) for b in bs)
    for p in (tokens(m, phn) if phn else []):
        if ph[0] <= p < ph[1]:
            continue
        s = innermost(sc, p)
        line = text.count('\n', 0, p) + 1
        if s is None or s.kind != 'class' or s is c:
            # an out-of-line empty placeholder body `void X::v00() { }` (tests: it backs a vtable)
            vs = m.rfind('void', 0, p)
            od = re.match(r'void\s+(\w+)\s*::\s*%s\s*\(\s*(?:void)?\s*\)\s*\{\s*\}' % phn, m[vs:])
            if od and vs + od.start(1) < p < vs + od.end() and (s is None or s.kind != 'class'):
                owner = od.group(1)
                if owner == old and dtor is None:
                    edits.append((vs, p + len(phn), '%s::~%s' % (new, new)))      # it becomes the root's virtual dtor body
                    continue
                if owner == old or derives(owner):
                    edits.append(del_span(text, m, vs, vs + od.end()))   # its declaration goes too
                    continue
                if owner in bases:
                    continue        # an unrelated class's own placeholder body
            return text, 0, ['PH_USE %s line %d' % (phn, line)]
        if not derives(s.name):
            continue            # an unrelated class's own placeholder (a second, independent base)
        hit = None
        for st, en in statements(m, s.lb, s.rb):
            if st <= p < en:
                hit = (st, en)
        if hit is None or not re.match(r'^\s*(?:virtual\s+)?void\s+%s\s*\(\s*(?:void)?\s*\)\s*(?:;|\{\s*\}\s*;?)\s*$' % phn,
                                         m[hit[0]:hit[1]]):
            return text, 0, ['PH_USE %s line %d' % (phn, line)]
        edits.append(del_span(text, m, hit[0], hit[1]))
    # the root: head, placeholder, dtor
    edits.append((hs, c.lb + 1, 'class %s {\n%s:%s' % (new, acc, '' if m[c.lb + 1] == '\n' else ' ')))
    if ph is None:
        pass                        # plain: the dtor is already virtual
    elif dtor is not None:
        edits.append(del_span(text, m, ph[0], ph[1]))
        k = dtor[0]
        while m[k] in ' \t\r\n': k += 1
        edits.append((k, k, 'virtual '))
    else:
        k = ph[0]
        while m[k] in ' \t\r\n': k += 1
        eol = text.find('\n', ph[1]); eol = len(text) if eol < 0 else eol
        cm = '' if text[ph[1]:eol].strip() else '   // %s' % DTOR[new]
        edits.append((k, ph[1], 'virtual ~%s();%s' % (new, cm)))
    for p in tokens(m, old):
        j = prev_sig(m, p)
        if j >= 1 and m[j] == ':' and m[j - 1] == ':':
            q = prev_word(m, j - 1)
            if q and q != old:
                return text, 0, ['QUALIFIED %s::%s' % (q, old)]
        edits.append((p, p + len(old), new))
    for ca, cb in comment_spans(text, m):
        seg = text[ca:cb]
        for mm in re.finditer(r'\b%s::~%s\b|\b%s::%s\b|\b%s\b(?=[^\n]{0,24}%s)' % (old, old, old, old, old, ADDR.pattern), seg):
            g = mm.group(0)
            edits.append((ca + mm.start(), ca + mm.end(), g.replace(old, new) if '::' in g else new))
    if not is_test:
        lf = leaf_line(text, m, sc, a)
        if lf: edits.append(lf)
    edits = drop_nested(edits)
    return apply_edits(text, edits), len(edits), []


_CAT = None


def leaf_line(text, m, sc, a):
    """The row's own ctor/dtor as an explicit `// Leaf:` selector (edit), when the row has none. With a virtual
    destructor through the root, the object also carries the class's scalar deleting destructor (`??_G<Cls>`), and
    the verifier's default selector (the display name `Cls`, a substring of every `<Cls>@` symbol) would grade the
    largest of them. The catalog name `Cls::Cls` / `Cls::~Cls` gives `??0Cls@` / `??1Cls@`."""
    global _CAT
    if re.search(r'^// Leaf:', text, re.M):
        return None
    if _CAT is None:
        _CAT = {}
        root = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp')
        for l in open(os.path.join(root, 'rebuild/oracles/auto-re-candidates.tsv'), encoding='utf-8', errors='ignore'):
            q = l.split('\t')
            if len(q) > 1: _CAT[q[0].lower()] = q[1]
    nm = _CAT.get(a, '')
    mm = re.fullmatch(r'(?:\w+::)*(\w+)::(~?)(\w+)', nm)
    if not mm or mm.group(1) != mm.group(3) or not any(k.kind == 'class' and k.name == mm.group(1) for k in sc):
        return None
    sel = '??%s%s@' % ('1' if mm.group(2) else '0', mm.group(1))
    eol = text.find('\n')
    if text.startswith('//') and eol >= 0:
        return (eol + 1, eol + 1, '// Leaf: %s\n' % sel)
    return (0, 0, '// Leaf: %s\n' % sel)


if __name__ == '__main__':
    run(transform, sys.argv[1:])
