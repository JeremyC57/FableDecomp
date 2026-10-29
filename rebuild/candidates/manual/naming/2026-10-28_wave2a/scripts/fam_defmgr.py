#!/usr/bin/env python3
"""Respell the game definition manager getter 0x0044C6B0 to its PDB form
`?Get@CGameDefinitionManager@@SIAAV1@XZ` (static CGameDefinitionManager& __fastcall
CGameDefinitionManager::Get(void); Ego_r.pdb + FableWin.pdb name sets). CGameDefinitionManager
derives from CDefinitionManager (whose members, e.g. GetDef, the callers use), single inheritance
at offset 0, so a CGameDefinitionManager& converts to CDefinitionManager& without code.

Shapes (no arguments; pointer and reference results are the same register, so no call bytes move):
  MEMBER  `static [const] <CGameDefinitionManager|CDefinitionManager>[*&] [cc] Get();` in a
          file-scope `class CGameDefinitionManager` -> `static CGameDefinitionManager& __fastcall
          Get();`; when the old result was the base, the class gets `: public CDefinitionManager`
          (only when its head has no base clause and the file defines CDefinitionManager before it);
          `->` after a call becomes `.`, other pointer uses take `&`.
  BASEGET `static CDefinitionManager[*&] [cc] Get();` in a file-scope `class CDefinitionManager`
          (and no CGameDefinitionManager in the file) -> the declaration leaves CDefinitionManager
          and `class CGameDefinitionManager : public CDefinitionManager { public: static
          CGameDefinitionManager& __fastcall Get(); };` follows its body; calls
          `CDefinitionManager::Get()` -> `CGameDefinitionManager::Get()`.
  FREE    file-scope `CDefinitionManager*|CGameDefinitionManager* [cc] GetDefinitionManager();` /
          `GetGameDefinitionManager();` -> the member (a new class derives from the file's
          CDefinitionManager), calls -> `CGameDefinitionManager::Get()` with `->` -> `.`.
Blockers are printed as notes. Idempotent.  usage: fam_defmgr.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import (run, mask, scopes, apply_edits, drop_nested, member_respell, free_to_static,
                    global_classes, tokens, del_span, innermost, decl_like)

CLS = 'CGameDefinitionManager'
MEMBER = 'static CGameDefinitionManager& __fastcall Get();'
CFG = dict(cls=CLS, ns=[], names=['Get'], new='Get', ret='CGameDefinitionManager&', base_ok={'CDefinitionManager'})
FREE = dict(names=['GetDefinitionManager', 'GetGameDefinitionManager'], ret=r'C(?:Game)?DefinitionManager\s*[*&]',
            params=lambda p: p in ('', 'void'), cls=CLS, member=MEMBER, comment='// 0x0044C6B0',
            qual='CGameDefinitionManager::Get', defhead='CGameDefinitionManager& CGameDefinitionManager::Get',
            defparams=lambda p: '' if p in ('', 'void') else None,
            header=r'#\s*include\s*[<"]engine/C(?:Game)?DefinitionManager\.h[>"]', anchor=r'(?i)0x0*44C6B0\b',
            ref=True, base='CDefinitionManager')
BDECL = re.compile(r'\bstatic\s+CDefinitionManager\s*(?P<rk>[*&])\s*(?:(?:__fastcall|__cdecl)\s+)?Get\s*\(\s*(?:void)?\s*\)\s*;')
BCALL = re.compile(r'(?<![\w:])(?:::\s*)?CDefinitionManager\s*::\s*Get\s*\(\s*\)')


def baseget(text):
    m = mask(text)
    if tokens(m, CLS):
        return text, [], []
    sc = scopes(m)
    bs = global_classes(m, sc, 'CDefinitionManager')
    if len(bs) != 1:
        return text, [], []
    b = bs[0]
    ds = [d for d in BDECL.finditer(m, b.lb, b.rb) if innermost(sc, d.start()) is b]
    if len(ds) != 1:
        return text, [], []
    d = ds[0]
    if re.search(r'CDefinitionManager\s*[*&]?\s*(?:__fastcall\s+)?CDefinitionManager\s*::\s*Get\s*\(', m):
        return text, [], ['BASEGET out-of-line definition']
    edits = [del_span(text, m, d.start(), d.end())]
    semi = m.index(';', b.rb)
    edits.append((semi + 1, semi + 1, '\nclass CGameDefinitionManager : public CDefinitionManager { public: '
                  'static CGameDefinitionManager& __fastcall Get(); };   // 0x0044C6B0'))
    for c in BCALL.finditer(m):
        if c.start() < b.rb:
            return text, [], ['BASEGET call inside CDefinitionManager']
        edits.append((c.start(), c.end(), 'CGameDefinitionManager::Get()'))
        if d.group('rk') == '*':
            t = c.end()
            while t < len(m) and m[t] in ' \t\r\n': t += 1
            edits.append((t, t + 2, '.') if m.startswith('->', t) else (c.start(), c.start(), '&'))
    return apply_edits(text, drop_nested(edits)), edits, []


def transform(text, path):
    notes, n = [], 0
    m = mask(text); sc = scopes(m)
    e, nn, done = member_respell(text, m, sc, CFG)
    notes += nn
    if done and e:
        text = apply_edits(text, drop_nested(e)); n += len(e)
    text, e, nn = baseget(text)
    notes += nn; n += len(e)
    m = mask(text); sc = scopes(m)
    e, nn, done = free_to_static(text, m, sc, FREE)
    notes += nn
    if done and e:
        text = apply_edits(text, drop_nested(e)); n += len(e)
    return text, n, notes


if __name__ == '__main__':
    run(transform, sys.argv[1:])
