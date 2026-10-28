#!/usr/bin/env python3
"""CPersistContext::CheckCRC (0x00404500) is protected in retail.

Ego_r.pdb / FableWin.pdb: ?CheckCRC@CPersistContext@@IAEXPBD@Z (protected, non-virtual, thiscall, const char*).
Landed models declare it public (QAE). Edit: inside a non-namespaced `class|struct CPersistContext { ... }` body,
a public, depth-1, `;`-terminated declaration `void CheckCRC(const char* [name]);` becomes
`protected: void CheckCRC(...); <access>:` where <access> is the label that was in effect (so every other member keeps
its access). Access is not part of the code bytes and a member function's access does not change POD-ness or layout.
Inline-defined CheckCRC bodies, other parameter lists and namespaced models are left alone. Idempotent.
usage: fam_checkcrc.py [--report] <file>...
"""
import sys, re, os, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib

DECL = re.compile(r'\bvoid\s+CheckCRC\s*\(')

def fix(text):
    m = lib.mask(text); nsr = lib.ns_ranges(m); edits = {}; info = []
    for key, lb, rb, hs in lib.class_bodies(m, 'CPersistContext'):
        if lib.in_ns(m, hs, nsr): info.append('NAMESPACED'); continue
        for d in DECL.finditer(m, lb, rb):
            if not lib.depth1(m, lb, d.start()): continue
            lp = d.end() - 1; rp = lib.match(m, lp, '(', ')')
            p = re.sub(r'\s+', ' ', m[lp + 1:rp]).strip()
            if not re.fullmatch(r'const char\s*\*\s*\w*', p): info.append('PARAM ' + p); continue
            k = rp + 1
            while m[k] in ' \t': k += 1
            if m[k] != ';': info.append('NOT_DECL'); continue
            pre = m[max(lb + 1, d.start() - 40):d.start()]
            if re.search(r'\b(virtual|static|inline)\s*$', pre): info.append('QUAL'); continue
            acc = lib.access_at(m, key, lb, d.start())
            if acc != 'public': info.append('ALREADY_' + acc); continue
            edits[d.start()] = (0, 'protected: ')
            edits[k + 1] = (0, ' public:')
            info.append('FIX')
    return lib.apply(text, edits), info

if __name__ == '__main__':
    rep = '--report' in sys.argv
    for f in [a for a in sys.argv[1:] if a != '--report']:
        if rep:
            print(f, fix(open(f, encoding='utf-8', errors='surrogateescape').read())[1]); continue
        ch, info = lib.rw(f, fix)
        if ch: print('edited', f, len([i for i in info if i == 'FIX']))
