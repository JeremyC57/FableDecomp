#!/usr/bin/env python3
"""Respell the user-profile manager getter 0x0040D2A0 to its PDB form
`?Get@CUserProfileManager@@SIAAV1@XZ` (static CUserProfileManager& __fastcall
CUserProfileManager::Get(void); Ego_r.pdb + FableWin.pdb name sets; CUserProfileManager has its
own static Get -- the CTBaseSingleton<T>::Get spellings in the PDB are for other T).

Shapes (no arguments, the result is the same register value as a pointer or a reference, so the
call bytes do not change):
  MEMBER    `static CUserProfileManager[*&] [cc] Get();` in a file-scope `class CUserProfileManager`
            -> `static CUserProfileManager& __fastcall Get();`; `CUserProfileManager::Get()->` ->
            `.Get().`, other pointer uses take `&`.
  SINGLETON calls `CTBaseSingleton<CUserProfileManager>::Get()` -> `CUserProfileManager::Get()`
            (`->` -> `.` when the template returned T*), with the member inserted at the end of the
            file's `class CUserProfileManager` body; the template stays for its other T.
  FREE      file-scope `CUserProfileManager[*&] [cc] GetUserProfileManager();` -> the member, calls
            `GetUserProfileManager()` -> `CUserProfileManager::Get()` (`->` -> `.`).
Blockers are printed as notes. Idempotent.  usage: fam_profilemgr.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import (run, mask, scopes, apply_edits, drop_nested, member_respell, free_to_static,
                    global_classes, tokens, deref_site, qual_start, del_span, decl_like)

CLS = 'CUserProfileManager'
MEMBER = 'static CUserProfileManager& __fastcall Get();'
CFG = dict(cls=CLS, ns=[], names=['Get'], new='Get', ret='CUserProfileManager&')
FREE = dict(names=['GetUserProfileManager'], ret=r'CUserProfileManager\s*[*&]', params=lambda p: p in ('', 'void'),
            cls=CLS, member=MEMBER, comment='// 0x0040D2A0', qual='CUserProfileManager::Get',
            defhead='CUserProfileManager& CUserProfileManager::Get', defparams=lambda p: '' if p in ('', 'void') else None,
            header=r'#\s*include\s*[<"]engine/CUserProfileManager\.h[>"]', anchor=None, ref=True)
SPECIAL = re.compile(r'\btemplate\s*<\s*>\s*CUserProfileManager\s*&\s*(?:__fastcall\s+)?CTBaseSingleton\s*<\s*CUserProfileManager\s*>\s*::\s*Get\s*\(\s*\)\s*(?P<end>[;{])')
SINGLETON = re.compile(r'(?<![\w:])(?:::\s*)?CTBaseSingleton\s*<\s*CUserProfileManager\s*>\s*::\s*Get\s*\(\s*\)')


def singleton(text):
    m = mask(text)
    specs = list(SPECIAL.finditer(m))
    calls = [x for x in SINGLETON.finditer(m) if not any(sp.start() <= x.start() < sp.end() for sp in specs)]
    if not calls and not specs:
        return text, [], []
    if (re.search(r'\bCTBaseSingleton\s*<\s*T\s*>\s*::\s*Get\s*\(\s*\)\s*\{', m)
            or re.search(r'template\s*<[^>]*>\s*(?:class|struct)\s+CTBaseSingleton\b[^{;]*\{[^}]*\bGet\s*\(\s*\)\s*\{', m)):
        # a generic template definition (tests) backs the stand-in for every T
        return text, [], ['SINGLETON generic template definition']
    sc = scopes(m)
    cl = global_classes(m, sc, CLS)
    if len(cl) != 1:
        return text, [], ['SINGLETON no %s body' % CLS]
    c = cl[0]
    if any(c.lb < p < c.rb and decl_like(m, p, 'Get') for p in tokens(m, 'Get')):
        return text, [], ['SINGLETON %s already declares Get' % CLS]
    if any(x.start() < c.lb for x in calls) or any(x.start() < c.rb for x in specs):
        return text, [], ['SINGLETON call before the class body']
    tm = re.search(r'template\s*<[^>]*>\s*(?:class|struct)\s+CTBaseSingleton\b[^{;]*\{(.*?)\}', m, re.S)
    ptr = bool(tm and re.search(r'static\s+T\s*\*\s*(?:__fastcall\s+)?Get\s*\(', tm.group(1)))
    acc = '' if (c.key == 'struct' and not re.search(r'\b(private|protected)\s*:', m[c.lb:c.rb])) else 'public:\n'
    edits = [(c.rb, c.rb, ('\n' if m[c.rb - 1] != '\n' else '') + acc + '    ' + MEMBER + '   // 0x0040D2A0\n')]
    for sp in specs:
        # an explicit specialization of the template stand-in: its declaration goes, a test's
        # definition becomes the member's definition
        if sp.group('end') == ';':
            edits.append(del_span(text, m, sp.start(), sp.end()))
        else:
            edits.append((sp.start(), sp.end() - 1, 'CUserProfileManager& CUserProfileManager::Get() '))
    for x in calls:
        edits.append((x.start(), x.end(), 'CUserProfileManager::Get()'))
        if ptr:
            t = x.end()
            while t < len(m) and m[t] in ' \t\r\n': t += 1
            edits.append((t, t + 2, '.') if m.startswith('->', t) else (x.start(), x.start(), '&'))
    return apply_edits(text, drop_nested(edits)), edits, []


def transform(text, path):
    notes, n = [], 0
    m = mask(text); sc = scopes(m)
    e, nn, done = member_respell(text, m, sc, CFG)
    notes += nn
    if done and e:
        text = apply_edits(text, drop_nested(e)); n += len(e)
    text, e, nn = singleton(text)
    notes += nn; n += len(e)
    m = mask(text); sc = scopes(m)
    e, nn, done = free_to_static(text, m, sc, FREE)
    notes += nn
    if done and e:
        text = apply_edits(text, drop_nested(e)); n += len(e)
    return text, n, notes


if __name__ == '__main__':
    run(transform, sys.argv[1:])
