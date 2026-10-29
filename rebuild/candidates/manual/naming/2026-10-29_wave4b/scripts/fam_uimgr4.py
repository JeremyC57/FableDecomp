#!/usr/bin/env python3
"""Call NUISystem::CManager::GetInstance (0x0041E5F2) through a model that IS NUISystem::CManager (wave-4 shapes).

Retail: ?GetInstance@CManager@NUISystem@@SIPAV12@XZ (Ego_r.pdb + FableWin.pdb; DECISIONS.tsv proven; CONFLICTS.md:
the ctor it calls stores the CManager vtable). Wave 3's fam_uimgr.py left the stand-in models whose members were not
CManager-only PDB names. This script renames a stand-in class to NUISystem::CManager when every member is
  * a data member, a virtual function (any name: a vtable slot, no symbol is emitted), a constructor/destructor, or
  * a non-virtual member with an inline body (a test fake or an inlined accessor, no external symbol), or
  * a non-virtual declaration whose name NUISystem::CManager has in the PDB name sets (Ego_r + FableWin), or
  * the static GetInstance itself (`static X* [cc] GetInstance();`, any calling convention: no arguments, so
    __cdecl and __fastcall calls are the same bytes).
Stand-in classes: STANDINS below, at file scope or directly in namespace NUISystem, no base clause, one body.
Edit: the head becomes `class CManager { public:` (class key: the PDB mangles V), wrapped in `namespace NUISystem {
... }` when the stand-in sat at file scope; GetInstance becomes `static CManager* __fastcall GetInstance();`; every
other code token X becomes `CManager` inside namespace NUISystem and `NUISystem::CManager` outside it (qualified
`NUISystem::X` -> `NUISystem::CManager`). Comments `X::GetInstance` next to 0x0041E5F2 follow.
Blocked: an existing CManager in the file, a base clause, a non-virtual member declared without a body that
CManager does not have (it would emit a new wrong spelling), two stand-ins.
Tests are edited only when they define `X::...` out of line (they then link the row's object). Idempotent.
usage: fam_uimgr4.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import mask, scopes, apply_edits, tokens, run, statements, row_of, comment_spans, drop_nested, ns_path, prev_sig

STANDINS = ['CFrontEndManager', 'CFrontEndManager_Calls', 'CFrontEndManagerGraphicsView', 'CManagerGraphicsView',
            'CManagerCalls', 'CFrontEndManager_Data', 'CFrontEndManager_00404c00', 'Manager', 'CUIManager']
_CM = None


def cm_names():
    global _CM
    if _CM is None:
        root = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp')
        _CM = set()
        for fn in ('egor_pdb_names.tsv', 'fablewin_pdb_names.tsv'):
            for l in open(os.path.join(root, 'ghidra_out', fn), encoding='utf-8', errors='ignore'):
                q = l.rstrip('\n').split('\t')
                if len(q) < 2: continue
                mm = re.match(r'\?(\w+)@CManager@NUISystem@@', q[1])
                if mm: _CM.add(mm.group(1))
    return _CM


GI = re.compile(r'^static\s+(?:(?:class|struct)\s+)?(?:NUISystem\s*::\s*)?(?P<r>\w+)\s*\*\s*(?:(?:__fastcall|__cdecl|__stdcall)\s+)?GetInstance\s*\(\s*(?:void)?\s*\)\s*(?P<e>;|\{)')


def transform(text, path):
    a, is_test = row_of(path)
    m = mask(text)
    sc = scopes(m)
    cand = []
    for s in sc:
        if s.kind != 'class' or s.name not in STANDINS: continue
        np = ns_path(s)
        if np not in ([], ['NUISystem']): continue
        for x, y in statements(m, s.lb, s.rb):
            st = re.sub(r'\s+', ' ', m[x:y]).strip()
            st = re.sub(r'^(?:(?:public|private|protected)\s*:\s*)+', '', st)
            g = GI.match(st)
            if g and g.group('r') == s.name: cand.append(s); break
    if not cand:
        return text, 0, []
    if len(cand) != 1:
        return text, 0, ['MULTI stand-ins']
    f = cand[0]
    X = f.name
    if len([s for s in sc if s.kind == 'class' and s.name == X]) != 1:
        return text, 0, ['MULTI %s bodies' % X]
    if is_test and not re.search(r'\b%s\s*::\s*\w+\s*\(' % X, m[:f.lb] + ' ' * (f.rb - f.lb) + m[f.rb:]):
        return text, 0, []
    if re.search(r'\bCManager\b', m):
        return text, 0, ['HAS_CMANAGER']
    hs = m.rfind(f.key, 0, f.lb)
    head = re.sub(r'\s+', ' ', m[hs:f.lb]).strip()
    if head != '%s %s' % (f.key, X):
        return text, 0, ['HEAD %s' % head]
    cmn = cm_names()
    gis = []
    for x, y in statements(m, f.lb, f.rb):
        raw = m[x:y]
        st = re.sub(r'\s+', ' ', raw).strip()
        st2 = re.sub(r'^(?:(?:public|private|protected)\s*:\s*)+', '', st)
        if not st2: continue
        if st2.startswith('#'): return text, 0, ['PREPROC']
        g = GI.match(st2)
        if g:
            gis.append((x + raw.index('static'), g)); continue
        if st2.startswith(('typedef', 'enum', 'friend', 'using')): continue
        if re.match(r'^(?:struct|class|union)\b', st2):
            if '(' in st2.split('{', 1)[0]: return text, 0, ['MEMBER %s' % st2[:50]]
            continue
        fm = re.search(r'(~?\w+)\s*\(', st2.split('{', 1)[0].split('=', 1)[0])
        if not fm:
            if re.search(r'\b[A-Z_]{3,}\d*\s*\(', st2) or re.search(r'\w\s*\(', st2):
                return text, 0, ['MEMBER %s' % st2[:60]]
            continue                                   # data member
        n = fm.group(1)
        if st2.startswith('virtual') or n in (X, '~' + X) or n.startswith('operator'):
            continue
        body = '{' in st2 and st2.index('{') > st2.index('(')
        if st2.startswith('static') and not body:
            return text, 0, ['STATIC %s' % st2[:60]]
        if body or n in cmn:
            continue
        return text, 0, ['NONCM_MEMBER %s' % st2[:60]]
    if len(gis) != 1:
        return text, 0, ['GI count %d' % len(gis)]
    np = ns_path(f)
    edits = []
    # head
    if np == []:
        edits.append((hs, f.lb + 1, 'namespace NUISystem { class CManager { public:'))
        en = f.rb + 1
        k = en
        while k < len(m) and m[k] in ' \t': k += 1
        if k >= len(m) or m[k] != ';':
            return text, 0, ['DECLARATOR']
        edits.append((k + 1, k + 1, ' }'))
    else:
        edits.append((hs, f.lb + 1, 'class CManager {' + (' public:' if f.key == 'struct' else '')))
    gp, g = gis[0]
    ge = gp + len(re.match(r'static\s+(?:(?:class|struct)\s+)?(?:NUISystem\s*::\s*)?\w+\s*\*\s*(?:(?:__fastcall|__cdecl|__stdcall)\s+)?GetInstance\s*\(\s*(?:void)?\s*\)', m[gp:]).group(0))
    edits.append((gp, ge, 'static CManager* __fastcall GetInstance()'))
    nsr = [s for s in sc if s.kind == 'ns' and s.name == 'NUISystem']
    for p in tokens(m, X):
        if hs <= p < f.lb + 1 or gp <= p < ge: continue
        inside = f.lb < p < f.rb
        j = prev_sig(m, p)
        qual = j >= 1 and m[j] == ':' and m[j - 1] == ':'
        if qual:
            q0 = j - 2
            while q0 >= 0 and m[q0] in ' \t': q0 -= 1
            w = re.search(r'(\w+)$', m[:q0 + 1])
            if w and w.group(1) == 'NUISystem':
                edits.append((p, p + len(X), 'CManager')); continue
            if w is None:
                edits.append((p, p + len(X), 'NUISystem::CManager')); continue   # ::X
            return text, 0, ['QUALIFIED %s::%s' % (w.group(1) if w else '', X)]
        in_ns = inside or any(s.lb < p < s.rb for s in nsr)
        edits.append((p, p + len(X), 'CManager' if in_ns else 'NUISystem::CManager'))
    # cast/ctor uses like `X::GetInstance` inside the namespace keep the short name; comments
    for a2, b2 in comment_spans(text, m):
        seg = text[a2:b2]
        for mm in re.finditer(r'\b%s::GetInstance\b(?=[^\n]{0,24}0x0041E5F2)' % re.escape(X), seg, re.I):
            edits.append((a2 + mm.start(), a2 + mm.end(), 'CManager::GetInstance'))
    return apply_edits(text, drop_nested(edits)), len(edits), []


if __name__ == '__main__':
    run(transform, sys.argv[1:])
