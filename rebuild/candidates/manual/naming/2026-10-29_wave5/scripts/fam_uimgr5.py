#!/usr/bin/env python3
"""NUISystem::CManager::GetInstance (0x0041E5F2) when the file already models NUISystem::CManager (wave 5).

Final symbol (DECISIONS.tsv, proven; Ego_r.pdb + FableWin.pdb): ?GetInstance@CManager@NUISystem@@SIPAV12@XZ.
Wave 4B's fam_uimgr4.py blocked these rows (HAS_CMANAGER): the row has `namespace NUISystem { class CManager {...}; }`
and a separate stand-in `struct CFrontEndManager { static NUISystem::CManager* __fastcall GetInstance(); };` whose
only member is that getter. Edit: the getter moves into the CManager model (`static CManager* __fastcall
GetInstance();`, public), the stand-in definition is deleted, `CFrontEndManager::GetInstance` -> 
`NUISystem::CManager::GetInstance` in code and comments. Blocked: any other member or use of the stand-in, two
CManager bodies. Sources only (the tests model their own fakes). Idempotent. usage: fam_uimgr5.py [--report] <file>...
"""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib

STANDINS = ['CFrontEndManager']


def fix(text):
    m = lib.mask(text)
    cms = [b for b in lib.class_bodies(m, 'CManager') if b[0] == 'class']
    if len(cms) != 1: return text, ['BLOCK CManager bodies=%d' % len(cms)]
    key, lb, rb, hs = cms[0]
    nss = [(n.end() - 1, lib.match(m, n.end() - 1, '{', '}')) for n in re.finditer(r'\bnamespace\s+NUISystem\s*\{', m)]
    if not any(a < hs < b for a, b in nss): return text, ['BLOCK CManager not in NUISystem']
    if re.search(r'\bGetInstance\b', m[lb:rb]): return text, ['BLOCK CManager already has GetInstance']
    for x in STANDINS:
        sb = lib.class_bodies(m, x)
        if not sb: continue
        if len(sb) != 1: return text, ['BLOCK stand-in bodies']
        k2, lb2, rb2, hs2 = sb[0]
        if lib.in_ns(m, hs2): return text, ['BLOCK stand-in namespaced']
        body = re.sub(r'^\s*(?:public\s*:\s*)?', '', m[lb2 + 1:rb2])
        if not re.fullmatch(r'\s*static\s+NUISystem\s*::\s*CManager\s*\*\s*__fastcall\s+GetInstance\s*\(\s*(?:void)?\s*\)\s*;\s*', body):
            return text, ['BLOCK stand-in members']
        end = rb2 + 1 + re.match(r'\s*;', m[rb2 + 1:]).end()
        ls = text.rfind('\n', 0, hs2) + 1
        le = text.find('\n', end); le = len(text) if le < 0 else le + 1
        if m[ls:hs2].strip() or m[end:le].strip(): return text, ['BLOCK stand-in line']
        uses = [u for u in re.finditer(r'\b%s\b' % x, m) if not (hs2 <= u.start() < end)]
        ed = {}
        for u in uses:
            if not re.match(r'%s\s*::\s*GetInstance\b' % x, m[u.start():]): return text, ['BLOCK other use of %s' % x]
            ed[u.start()] = (len(x), 'NUISystem::CManager')
        for u in re.finditer(r'\b%s::GetInstance\b' % x, text):      # comments
            if m[u.start():u.start() + len(x)] != x: ed[u.start()] = (len(x), 'NUISystem::CManager')
        acc = lib.access_at(m, key, lb, rb)
        ins = '%s    static CManager* __fastcall GetInstance();                 // 0x0041E5F2\n' % ('' if acc == 'public' else 'public:\n')
        ls2 = text.rfind('\n', 0, rb) + 1
        if m[ls2:rb].strip(): return text, ['BLOCK CManager tail line']
        ed[ls2] = (0, ins)
        ed[ls] = (le - ls, '')
        return lib.apply(text, ed), ['CALL'] * len(uses) + ['FIX']
    return text, ['BLOCK no stand-in']


if __name__ == '__main__':
    rep = '--report' in sys.argv
    for f in [a for a in sys.argv[1:] if a != '--report']:
        if '_test.cpp' in f: continue
        if rep:
            print(f, fix(open(f, encoding='utf-8', errors='surrogateescape').read())[1]); continue
        ch, info = lib.rw(f, fix)
        if ch: print('edited', f, info.count('CALL'))
