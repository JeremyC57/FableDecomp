#!/usr/bin/env python3
"""CScriptThing::~CScriptThing (0x004AA840) is virtual in retail.

Ego_r.pdb / FableWin.pdb: ??1CScriptThing@@UAE@XZ (public virtual; DECISIONS.tsv proven). Landed models that already
give CScriptThing a vptr through another `virtual` member but declare the destructor plain (`~CScriptThing();`,
mangles QAE) get `virtual ` prepended to that declaration. The class already has a vptr, so no member moves; the
recheck proves the leaf bytes did not change. Only a non-namespaced `class|struct CScriptThing` body, a depth-1
`;`-terminated destructor declaration, and a body that contains another `virtual` member are edited (fam_cbipdtor.py
shape 1; its explicit-vptr shape is not used here, it changed bytes for CBaseIntelligentPointer). Idempotent.
usage: fam_cstdtor.py [--report] <file>...
"""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib

CLS = 'CScriptThing'
DTOR = re.compile(r'(?<![\w:])~\s*%s\s*\(\s*(?:void\s*)?\)\s*;' % CLS)

def fix(text):
    m = lib.mask(text); nsr = lib.ns_ranges(m); edits = {}; info = []
    for key, lb, rb, hs in lib.class_bodies(m, CLS):
        if lib.in_ns(m, hs, nsr): info.append('NAMESPACED'); continue
        body = m[lb + 1:rb]
        for d in DTOR.finditer(m, lb, rb):
            if not lib.depth1(m, lb, d.start()): continue
            pre = m[max(lb + 1, d.start() - 30):d.start()]
            if re.search(r'\bvirtual\s*$', pre): info.append('OK'); continue
            if not re.search(r'\bvirtual\b', body): info.append('NO_VPTR'); continue
            edits[d.start()] = (0, 'virtual ')
            info.append('FIX')
    return lib.apply(text, edits), info

if __name__ == '__main__':
    rep = '--report' in sys.argv
    for f in [a for a in sys.argv[1:] if a != '--report']:
        if rep:
            print(f, fix(open(f, encoding='utf-8', errors='surrogateescape').read())[1]); continue
        ch, info = lib.rw(f, fix)
        if ch: print('edited', f, info.count('FIX'))
