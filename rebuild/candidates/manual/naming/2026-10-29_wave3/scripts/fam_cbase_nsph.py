#!/usr/bin/env python3
"""Respell the address-named CBase roots whose slot-0 placeholder derived classes redeclare (wave-2A PH_USED).

Wave 3 variant of naming/w2a/fam_cbase_ns.py (same rules, see below), for the roots wave 2A left as PH_USED: a
root WITH a dtor statement whose placeholder `virtual void v00()` is redeclared by derived classes. Here each
derived redeclaration (a depth-1 `[virtual] void v00();` / empty inline body in a class that derives from the root,
directly or through other classes of the file) and each empty out-of-line `void X::v00() { }` of such a class is
deleted: the derived class's destructor (declared or implicit) is virtual through the root and overrides slot 0,
so every later slot keeps its index. Any other use of the placeholder (a call, a qualified use, a non-empty body)
stays a blocker. Files without such a root are left alone (so this composes with fam_cbase_ns in any order).


Retail has two polymorphic roots (Ego_r.pdb + FableWin.pdb, RTTI, wave-1 CONFLICTS.md):
  CBaseClass             ctor 0x0099A2D0 ??0CBaseClass@@QAE@XZ            (public)
                         dtor 0x0099A2E0 ??1CBaseClass@@UAE@XZ            (public virtual)
  CBaseClassNonCopyable  ctor 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ (protected)
                         dtor 0x0099A300 ??1CBaseClassNonCopyable@@MAE@XZ (protected virtual)
Landed rows model them as `namespace N_0099a2d0 { struct CBase {...}; } using N_0099a2d0::CBase;`
(N_0099a300 for the non-copyable root; the class is sometimes CBaseObject / CScriptBase /
CBaseClass). This transform turns that block into a file-scope `class CBaseClass` /
`class CBaseClassNonCopyable` and renames every code use (bare, `N_...::X`, base clauses, init
lists, `X::~X`).

Body rules (depth-1 statements of the root):
  ctor `X(...)`            kept (access: public / protected)
  dtor `[virtual] ~X()`    made virtual (access: public / protected)
  `virtual void v00()` / `v0` / `VSlot0` / `VTableAnchor` (no parameters): the placeholder for
          slot 0, which is the root's virtual destructor in retail. With a dtor statement it is
          dropped (the dtor becomes virtual: same single slot); without one it becomes
          `virtual ~X()` (keeping a test's inline body).
          When derived classes in the file redeclare the placeholder (their slots would shift), a
          ctor-only root keeps it unchanged and a root with a dtor statement is a blocker (PH_USED).
  access labels            dropped (one label is written after the brace; behaviour tests
                           (`*_test.cpp`) keep the non-copyable root public: they delete through it)
  anything else (data members, other virtuals, base clauses) -> blocker, no edit.
A non-virtual dtor with no placeholder virtual (no vptr in the model) is a blocker (NOVPTR):
making it virtual would add a vptr.
Comments: `X::X`, `X::~X`, and `X` right before a root address become the PDB spelling.
Idempotent (a second run finds no N_0099a2d0 / N_0099a300 namespace).
usage: fam_cbase_nsph.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from fam_cbase_ph import leaf_line
from famlib import row_of, skipped, mask, scopes, apply_edits, tokens, prev_sig, comment_spans, run, drop_nested, del_span, innermost, statements

ROOTS = {'N_0099a2d0': ('CBaseClass', 'public'), 'N_0099a300': ('CBaseClassNonCopyable', 'protected')}
PLACEHOLDER = re.compile(r'^\s*virtual\s+void\s+(?:v0+|v00|VSlot0|VTableAnchor)\s*\(\s*(?:void)?\s*\)\s*')
# rows left alone (both files): see DEFERRED.tsv
SKIP = {
    '00404720': 'behaviour test FAILs before and after (pre-existing; opeq EXCLUDED.tsv)',
    '00aa7df0': "own ~CChunkedFile turns virtual (UAE): code identical, but the verifier leaf '~CChunkedFile' then grades the larger ??_G",
}
ADDR = re.compile(r'0x0099A(?:2D0|2E0|2F0|300)\b', re.I)


def transform(text, path):
    if skipped(path, SKIP):
        return text, 0, ['SKIP ' + skipped(path, SKIP)]
    m = mask(text)
    sc = scopes(m)
    notes, edits = [], []
    blocks = [s for s in sc if s.kind == 'ns' and s.name in ROOTS and s.parent is None]
    if not blocks:
        return text, 0, notes
    plan = []   # (ns scope, class scope, old name, new name, access)
    for ns in blocks:
        inner = [s for s in sc if s.parent is ns]
        cls = [s for s in inner if s.kind == 'class']
        rest = m[ns.lb + 1:ns.rb]
        if len(cls) != 1 or len(inner) != 1:
            notes.append('NS_SHAPE %s' % ns.name); return text, 0, notes
        c = cls[0]
        hs = m.rfind(c.key, ns.lb, c.lb)
        outside = m[ns.lb + 1:hs] + m[c.rb + 1:ns.rb]
        if outside.strip(' ;\n\t') or ':' in m[hs:c.lb]:
            notes.append('NS_SHAPE %s' % ns.name); return text, 0, notes
        # behaviour tests delete through the root pointer: they keep it public
        ool = re.search(r'\b%s\s*::\s*~?\s*%s\s*\(' % (c.name, c.name), m)
        acc = 'public' if (path.endswith('_test.cpp') and not ool) else ROOTS[ns.name][1]
        plan.append((ns, c, c.name, ROOTS[ns.name][0], acc))
    names = [p[2] for p in plan]
    for new in set(p[3] for p in plan):
        if tokens(m, new):
            notes.append('EXISTS %s' % new); return text, 0, notes
    usings = {}
    for mm in re.finditer(r'\busing\s+(N_0099a2d0|N_0099a300)\s*::\s*(\w+)\s*;', m):
        usings.setdefault(mm.group(2), []).append(mm)
    for old, us in usings.items():
        if len(set(u.group(1) for u in us)) > 1:
            notes.append('USING_BOTH %s' % old); return text, 0, notes
    for ns, c, old, new, acc in plan:
        stm = statements(m, c.lb, c.rb)
        ctor = dtor = ph = None
        for st, en in stm:
            s = m[st:en]
            if re.match(r'(public|private|protected)\s*:', s):
                edits.append(del_span(text, m, st, en)); continue
            if PLACEHOLDER.match(s):
                if ph is not None: notes.append('TWO_PLACEHOLDERS'); return text, 0, notes
                ph = (st, en); continue
            if re.match(r'^\s*(?:virtual\s+)?~\s*%s\s*\(\s*(?:void)?\s*\)' % old, s):
                dtor = (st, en); continue
            if re.match(r'^\s*(?:inline\s+)?%s\s*\(\s*(?:void)?\s*\)' % old, s):
                ctor = (st, en); continue
            notes.append('BODY %s: %s' % (old, re.sub(r'\s+', ' ', s)[:60])); return text, 0, notes
        keep_ph = False
        if ph is None or dtor is None:
            return text, 0, notes          # wave-2A shapes (fam_cbase_ns)
        phn = re.match(r'\s*virtual\s+void\s+(\w+)', m[ph[0]:ph[1]]).group(1)
        outside = [p for p in tokens(m, phn) if not ph[0] <= p < ph[1]]
        if not outside:
            return text, 0, notes          # fam_cbase_ns handles it
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
        for p in outside:
            k = innermost(sc, p)
            line = text.count('\n', 0, p) + 1
            if k is None or k.kind != 'class':
                vs = m.rfind('void', 0, p)
                od = re.match(r'void\s+(\w+)\s*::\s*%s\s*\(\s*(?:void)?\s*\)\s*\{\s*\}' % phn, m[vs:])
                if od and vs + od.start(1) < p < vs + od.end() and k is None:
                    if derives(od.group(1)) or od.group(1) == old:
                        edits.append(del_span(text, m, vs, vs + od.end())); continue
                    if od.group(1) in bases:
                        continue
                notes.append('PH_USED %s line %d' % (phn, line)); return text, 0, notes
            if k is c or not derives(k.name):
                if k is c:
                    notes.append('PH_USED %s line %d' % (phn, line)); return text, 0, notes
                continue                    # an unrelated class's own placeholder
            hit = None
            for st, en in statements(m, k.lb, k.rb):
                if st <= p < en: hit = (st, en)
            if hit is None or not re.match(r'^\s*(?:virtual\s+)?void\s+%s\s*\(\s*(?:void)?\s*\)\s*(?:;|\{\s*\}\s*;?)\s*$' % phn,
                                             m[hit[0]:hit[1]]):
                notes.append('PH_USED %s line %d' % (phn, line)); return text, 0, notes
            edits.append(del_span(text, m, hit[0], hit[1]))
        if dtor is not None and ph is None and not re.match(r'^\s*virtual\b', m[dtor[0]:dtor[1]]):
            notes.append('NOVPTR %s' % old); return text, 0, notes
        if dtor is None and ph is None:
            notes.append('NOVPTR %s (no virtual)' % old); return text, 0, notes
        # head: `struct|class X` -> `class NEW`, then one access label
        hm = re.search(r'\b(class|struct)\s+%s\s*$' % old, m[:c.lb].rstrip() + ' ')
        hs = m.rfind(c.key, 0, c.lb)
        edits.append((hs, c.lb + 1, 'class %s {\n%s:%s' % (new, acc, '' if m[c.lb + 1] == '\n' else ' ')))
        if ph is not None and not keep_ph:
            st, en = ph
            if dtor is not None:
                # drop the placeholder line (with its trailing comment) when it is alone on its line
                a = text.rfind('\n', 0, st) + 1
                b = text.find('\n', en); b = len(text) if b < 0 else b
                if m[a:st].strip() == '' and m[en:b].strip() == '' and not (a <= c.lb < b):
                    edits.append((a, b + 1 if b < len(text) else b, ''))
                else:
                    edits.append((st, en, ''))
            else:
                pm = PLACEHOLDER.match(m[st:en])
                edits.append((st, st + pm.end(), 'virtual ~%s() ' % new if m[st + pm.end()] == '{' else 'virtual ~%s()' % new))
        if dtor is not None and not re.match(r'^\s*virtual\b', m[dtor[0]:dtor[1]]):
            k = dtor[0]
            while m[k] in ' \t\r\n': k += 1
            edits.append((k, k, 'virtual '))
        # namespace head and closing brace
        nh = m.rfind('namespace', 0, ns.lb)
        edits.append((nh, hs, ''))
        edits.append((c.rb + 1, ns.rb + 1, ';'))
    # usings
    for old, us in usings.items():
        for u in us:
            a = text.rfind('\n', 0, u.start()) + 1
            b = text.find('\n', u.end()); b = len(text) if b < 0 else b
            edits.append(del_span(text, m, u.start(), u.end()))
    # qualified uses N_xxx::X -> NEW
    for mm in re.finditer(r'(?<![\w:])(?:::\s*)?(N_0099a2d0|N_0099a300)\s*::\s*(\w+)\b', m):
        if any(u.start() <= mm.start() < u.end() for us in usings.values() for u in us):
            continue
        new = ROOTS[mm.group(1)][0]
        edits.append((mm.start(), mm.end(), new))
    # bare uses: only names brought in by a using directive
    for ns, c, old, new, acc in plan:
        if old not in usings and not (ns.lb < 0):
            pass
        for p in tokens(m, old):
            j = prev_sig(m, p)
            if j >= 1 and m[j] == ':' and m[j - 1] == ':':
                # X::X / X::~X and N_...::X (handled above)
                k = j - 2
                while k >= 0 and m[k] in ' \t': k -= 1
                e = k + 1
                while k >= 0 and (m[k].isalnum() or m[k] == '_'): k -= 1
                q = m[k + 1:e]
                if q in ROOTS: continue
            inside_ns = ns.lb < p < ns.rb
            if old in usings or inside_ns or (j >= 1 and m[j] == ':' and m[j - 1] == ':'):
                edits.append((p, p + len(old), new))
        # comments
        for a, b in comment_spans(text, m):
            seg = text[a:b]
            for mm in re.finditer(r'\b%s::~%s\b|\b%s::%s\b|\b%s\b(?=[^\n]{0,24}%s)' % (old, old, old, old, old, ADDR.pattern), seg):
                g = mm.group(0)
                rep = g.replace(old, new) if '::' in g else new
                edits.append((a + mm.start(), a + mm.end(), rep))
    if not path.endswith('_test.cpp'):
        lf = leaf_line(text, m, sc, row_of(path)[0])     # the dtor turns virtual: select the row's own symbol
        if lf: edits.append(lf)
    edits = drop_nested(edits)
    return apply_edits(text, edits), len(edits), notes


if __name__ == '__main__':
    run(transform, sys.argv[1:])
