#!/usr/bin/env python3
"""Call NUISystem::CManager::GetInstance (0x0041E5F2) as the CManager static it is.

Retail: ?GetInstance@CManager@NUISystem@@SIPAV12@XZ (Ego_r.pdb + FableWin.pdb; DECISIONS.tsv proven, CONFLICTS.md:
the ctor it calls stores the CManager vtable; NUISystem::CFrontEndManager::GetInstance is the twin at 0x00595582).
Shape handled: a row that models both classes in namespace NUISystem,
    class CManager { ... };                                   (data members and CManager methods)
    class CFrontEndManager : public CManager { public: static CFrontEndManager* __fastcall GetInstance(); };
and calls `NUISystem::CFrontEndManager::GetInstance()` (0x0041E5F2) to reach CManager members. The stand-in
derived class adds nothing but the getter, so it goes: GetInstance is declared in CManager
(`static CManager* __fastcall GetInstance();`) and every code token `CFrontEndManager` becomes `CManager` (the object
is the CManager, so no layout or call changes). Blockers: CFrontEndManager with any other member or another base,
no CManager body before it in the same namespace, CManager already declaring GetInstance, more than one body.
Second shape (lone model): `namespace NUISystem { class CFrontEndManager { ... static CFrontEndManager* __fastcall
GetInstance(); ... }; }` with no CManager body in the file, whose other members are data members, numbered virtual
slot placeholders (`virtual ... v012()` / `VSlot3` / `Slot06`) or methods that only NUISystem::CManager has in the
PDB name sets (CManager-only names; CreateComponent only as (long) or (const CCharString&, bool)): the class IS the
CManager, so every code token `CFrontEndManager` becomes `CManager`.
Comments `CFrontEndManager::GetInstance` next to 0x0041E5F2 follow. Idempotent.
usage: fam_uimgr.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import mask, scopes, apply_edits, tokens, run, del_span, statements, row_of, comment_spans, drop_nested, ns_path, prev_sig

DECL = re.compile(r'^\s*static\s+(?:NUISystem\s*::\s*)?CFrontEndManager\s*\*\s*__fastcall\s+GetInstance\s*\(\s*(?:void)?\s*\)\s*;\s*$')


def transform(text, path):
    m = mask(text)
    sc = scopes(m)
    fe = [s for s in sc if s.kind == 'class' and s.name == 'CFrontEndManager']
    if not fe:
        return text, 0, []
    if len(fe) != 1:
        return text, 0, ['MULTI CFrontEndManager']
    f = fe[0]
    hs = m.rfind(f.key, 0, f.lb)
    head = re.sub(r'\s+', ' ', m[hs:f.lb]).strip()
    if head == 'class CFrontEndManager':
        return lone(text, m, sc, f, hs)
    if head == 'struct CFrontEndManager':
        return filescope(text, m, sc, f, hs, path)
    if not re.fullmatch(r'class CFrontEndManager : public (?:NUISystem::)?CManager', head):
        return text, 0, ['HEAD %s' % head]
    st = [x for x in statements(m, f.lb, f.rb) if not re.match(r'\s*public\s*:\s*$', m[x[0]:x[1]])]
    if len(st) != 1 or not DECL.match(m[st[0][0]:st[0][1]]):
        return text, 0, ['BODY']
    nsp = ns_path(f)
    cm = [s for s in sc if s.kind == 'class' and s.name == 'CManager' and ns_path(s) == nsp and s.rb < hs]
    if len(cm) != 1 or nsp != ['NUISystem']:
        return text, 0, ['NO_CMANAGER']
    c = cm[0]
    if re.search(r'\bGetInstance\b', m[c.lb:c.rb]):
        return text, 0, ['CMANAGER_HAS_GETINSTANCE']
    edits = []
    # the stand-in class goes (with its trailing comment and line)
    en = f.rb + 1
    k = en
    while k < len(m) and m[k] in ' \t': k += 1
    if k < len(m) and m[k] == ';': en = k + 1
    edits.append(del_span(text, m, hs, en))
    body = m[c.lb:c.rb]
    acc = '' if (re.search(r'\bpublic\s*:', body) and not re.search(r'\b(private|protected)\s*:', body)) else 'public:\n'
    ins = ('' if m[c.rb - 1] == '\n' else '\n') + acc + \
        '    static CManager* __fastcall GetInstance();                      // 0x0041E5F2 ?GetInstance@CManager@NUISystem@@SIPAV12@XZ\n'
    edits.append((c.rb, c.rb, ins))
    for p in tokens(m, 'CFrontEndManager'):
        if hs <= p < en: continue
        edits.append((p, p + len('CFrontEndManager'), 'CManager'))
    for a, b in comment_spans(text, m):
        seg = text[a:b]
        for mm in re.finditer(r'\bCFrontEndManager::GetInstance\b(?=[^\n]{0,24}0x0041E5F2)', seg, re.I):
            edits.append((a + mm.start(), a + mm.end(), 'CManager::GetInstance'))
    edits = drop_nested(edits)
    return apply_edits(text, edits), len(edits), []


_CM = None
_CMALL = None


def cmanager_only():
    global _CM, _CMALL
    if _CM is None:
        root = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp')
        mem = {'CManager': set(), 'CFrontEndManager': set()}
        for fn in ('egor_pdb_names.tsv', 'fablewin_pdb_names.tsv'):
            for l in open(os.path.join(root, 'ghidra_out', fn), encoding='utf-8', errors='ignore'):
                q = l.rstrip('\n').split('\t')
                if len(q) < 2: continue
                mm = re.match(r'\?(\w+)@(CManager|CFrontEndManager)@NUISystem@@', q[1])
                if mm: mem[mm.group(2)].add(mm.group(1))
        _CM = mem['CManager'] - mem['CFrontEndManager']
        _CMALL = mem['CManager']
    return _CM


def lone(text, m, sc, f, hs):
    if ns_path(f) != ['NUISystem']:
        return text, 0, ['LONE_NS %s' % ns_path(f)]
    if [s for s in sc if s.kind == 'class' and s.name == 'CManager']:
        return text, 0, ['LONE_HAS_CMANAGER']
    cmo = cmanager_only()
    got = False
    for a, b in statements(m, f.lb, f.rb):
        x = re.sub(r'\s+', ' ', m[a:b]).strip()
        if re.match(r'^(public|private|protected)\s*:$', x): continue
        if DECL.match(m[a:b]): got = True; continue
        fm = re.search(r'(~?\w+)\s*\(', x)
        if not fm:
            continue                                   # a data member
        n = fm.group(1)
        if re.fullmatch(r'(?:v|VSlot|Slot|f|m)\d+', n) and x.startswith('virtual'):
            continue
        if n == 'CreateComponent':
            if re.search(r'CreateComponent\s*\(\s*long\s*\w*\s*\)', x) or \
               re.search(r'CreateComponent\s*\(\s*const\s+CCharString\s*&\s*\w*\s*,\s*bool\s*\w*\s*\)', x):
                continue
            return text, 0, ['LONE_MEMBER %s' % x[:60]]
        if n in cmo:
            continue
        return text, 0, ['LONE_MEMBER %s' % x[:60]]
    if not got:
        return text, 0, ['LONE_NO_GETINSTANCE']
    edits = [(p, p + len('CFrontEndManager'), 'CManager') for p in tokens(m, 'CFrontEndManager')]
    for a, b in comment_spans(text, m):
        seg = text[a:b]
        for mm in re.finditer(r'\bCFrontEndManager::GetInstance\b(?=[^\n]{0,24}0x0041E5F2)', seg, re.I):
            edits.append((a + mm.start(), a + mm.end(), 'CManager::GetInstance'))
    return apply_edits(text, drop_nested(edits)), len(edits), []


def filescope(text, m, sc, f, hs, path):
    """Third shape: a file-scope `struct CFrontEndManager { ... };` (no base) that stands for NUISystem::CManager
    (it declares the 0x0041E5F2 getter, so it emits ?GetInstance@CFrontEndManager@@SIPAU1@XZ). Members allowed:
    data members, numbered virtual slot placeholders, virtuals whose name NUISystem::CManager has in the PDB name
    sets, non-virtual methods only CManager has (CreateComponent as (long) or (const CCharString&, bool)).
    Edit: `struct CFrontEndManager {` -> `namespace NUISystem { class CManager { public:`, `};` -> `}; }`, the body's
    own tokens -> CManager, every other code token -> NUISystem::CManager. A test is edited only when it defines a
    member out of line (`CFrontEndManager::`), i.e. links the row's object."""
    a, is_test = row_of(path)
    if f.parent is not None:
        return text, 0, ['FS_NESTED']
    if is_test and not re.search(r'\bCFrontEndManager\s*::', m):
        return text, 0, []
    if [s for s in sc if s.kind == 'class' and s.name == 'CManager'] or re.search(r'\bCManager\b', m):
        return text, 0, ['FS_HAS_CMANAGER']
    cmo = cmanager_only()
    got = False
    for x, y in statements(m, f.lb, f.rb):
        st = re.sub(r'\s+', ' ', m[x:y]).strip()
        if re.match(r'^(public|private|protected)\s*:$', st): continue
        if re.match(r'^static\s+(?:NUISystem\s*::\s*)?(?:CFrontEndManager|CManager)\s*\*\s*(?:__fastcall\s+)?GetInstance\s*\(\s*(?:void)?\s*\)', st):
            got = True; continue
        fm = re.search(r'(~?\w+)\s*\(', st)
        if not fm:
            if re.search(r'\b(SLOTS\d*|\w+\s*\()', st):
                return text, 0, ['FS_MEMBER %s' % st[:60]]
            continue                                   # a data member
        n = fm.group(1)
        virt = st.startswith('virtual')
        if virt and re.fullmatch(r'(?:v|VSlot|Slot|f|m)\d+', n):
            continue
        if n == 'CreateComponent':
            if re.search(r'CreateComponent\s*\(\s*long\s*\w*\s*\)', st) or \
               re.search(r'CreateComponent\s*\(\s*const\s+CCharString\s*&\s*\w*\s*,\s*bool\s*\w*\s*\)', st):
                continue
            return text, 0, ['FS_MEMBER %s' % st[:60]]
        if n in cmo or (virt and n in _CMALL):
            continue
        return text, 0, ['FS_MEMBER %s' % st[:60]]
    if not got:
        return text, 0, ['FS_NO_GETINSTANCE']
    en = f.rb + 1
    k = en
    while k < len(m) and m[k] in ' \t': k += 1
    if k >= len(m) or m[k] != ';':
        return text, 0, ['FS_DECLARATOR']
    edits = [(hs, f.lb, 'namespace NUISystem { class CManager '), (f.lb + 1, f.lb + 1, ' public:'),
             (k + 1, k + 1, ' }')]
    for p in tokens(m, 'CFrontEndManager'):
        if hs <= p < f.lb: continue
        if f.lb < p < f.rb:
            edits.append((p, p + len('CFrontEndManager'), 'CManager'))
        else:
            j = prev_sig(m, p)
            if m[j] == ':' and m[j - 1] == ':':
                return text, 0, ['FS_QUALIFIED line %d' % (text.count('\n', 0, p) + 1)]
            edits.append((p, p + len('CFrontEndManager'), 'NUISystem::CManager'))
    for a2, b2 in comment_spans(text, m):
        seg = text[a2:b2]
        for mm in re.finditer(r'\bCFrontEndManager::GetInstance\b(?=[^\n]{0,24}0x0041E5F2)', seg, re.I):
            edits.append((a2 + mm.start(), a2 + mm.end(), 'NUISystem::CManager::GetInstance'))
    return apply_edits(text, drop_nested(edits)), len(edits), []


if __name__ == '__main__':
    run(transform, sys.argv[1:])
