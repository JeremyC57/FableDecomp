#!/usr/bin/env python3
"""CBaseIntelligentPointer::~CBaseIntelligentPointer (0x00A01C10) is virtual in retail.

Ego_r.pdb / FableWin.pdb: ??1CBaseIntelligentPointer@@UAE@XZ (public virtual). The class has a vtable
(0x0129C95C, vptr at +0, PData at +4). Landed models that already give the class a vptr through another
`virtual` member but declare the destructor plain (`~CBaseIntelligentPointer();`, mangles QAE) get
`virtual ` prepended to that declaration. The class already has a vptr, so no member moves; the recheck
proves the leaf bytes did not change (a new vtable slot could only matter to a leaf that calls a later slot).
Only a non-namespaced `class|struct CBaseIntelligentPointer` body, a depth-1 `;`-terminated destructor
declaration, and a body that contains another `virtual` member are edited.
Second shape: a body with no `virtual` member whose first data member is an explicit vptr slot
(`void* _vptr;` / `__vftable` / `VTable` / `vptr` / `vtable` / `vftable`): that member line is removed and the
destructor made virtual, so the compiler's vptr takes the same +0 slot and every other member keeps its offset
(a leftover use of the removed member fails to compile, which the recheck reports). Models with no vptr at all
are left alone: making the destructor virtual there would add one. Idempotent.
The second shape changed the bytes of 22 of its 33 rows (dtor helpers) and broke 3 tests, so only the verified
file list fam_cbipdtor.files.txt may be passed: it holds every shape-1 file and the 9 shape-2 files that passed.
usage: fam_cbipdtor.py [--report] <file>...
"""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib

CLS = 'CBaseIntelligentPointer'
VPTR = re.compile(r'\s*(?:public\s*:\s*)?(?P<decl>void\s*\*\s*(?:_vptr|__vftable|VTable|vptr|vtable|vftable|_vtable|m_vtable|vtbl)\s*;)')
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
            if not re.search(r'\bvirtual\b', body):
                v = VPTR.match(m, lb + 1)
                if not v: info.append('NO_VPTR'); continue
                # remove the whole explicit-vptr line (with its comment)
                s0 = text.rfind('\n', 0, v.start('decl')) + 1
                e0 = text.find('\n', v.end('decl')); e0 = len(text) if e0 < 0 else e0 + 1
                if text[s0:v.start('decl')].strip() or m[v.end('decl'):e0].strip():
                    edits[v.start('decl')] = (v.end('decl') - v.start('decl'), '')   # shares its line
                else:
                    edits[s0] = (e0 - s0, '')
                edits[d.start()] = (0, 'virtual ')
                info.append('FIX_VPTR'); continue
            edits[d.start()] = (0, 'virtual ')
            info.append('FIX')
    return lib.apply(text, edits), info

if __name__ == '__main__':
    rep = '--report' in sys.argv
    for f in [a for a in sys.argv[1:] if a != '--report']:
        if rep:
            print(f, fix(open(f, encoding='utf-8', errors='surrogateescape').read())[1]); continue
        ch, info = lib.rw(f, fix)
        if ch: print('edited', f, info.count('FIX') + info.count('FIX_VPTR'))
