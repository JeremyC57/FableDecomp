#!/usr/bin/env python3
"""CCharString::CCharString(const char*, long) (0x0099EBF0): the length parameter is `long` in retail.

Ego_r.pdb / FableWin.pdb / SETTLED_NAMES: ??0CCharString@@QAE@PBDJ@Z. Landed models declare `int` (PBDH).
int and long are both 32-bit on x86 VC7.1, so the change only moves the mangled name H -> J.
Edits, for the non-namespaced CCharString class only:
  * a depth-1 constructor declaration/definition `CCharString(const char* [a], int [b] [= d])` inside a
    `class|struct CCharString` body;
  * an out-of-line definition `CCharString::CCharString(const char* [a], int [b])`;
the `int` becomes `long`. Left alone: namespaced models (they mangle CCharString@<ns>), `#define CCharString X`
aliases, bodies that already declare a (const char*, long) constructor next to the int one.
In an edited file, a comment line that mentions CCharString or 0x99EBF0 and spells the parameter list
`(const char*, int` is respelled `long` too (comments only; code outside the constructor is never touched). Idempotent.
usage: fam_ccsctor.py [--report] <file>...
"""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib

CMT = re.compile(r'\(\s*const\s+char\s*\*\s*\w*\s*,\s*(?P<t>int)\b')
PAR = re.compile(r'^(\s*const\s+char\s*\*\s*\w*\s*,\s*)(int)(\s+\w+)?(\s*=\s*-?\w+)?\s*$')

def fix(text):
    m = lib.mask(text); nsr = lib.ns_ranges(m); edits = {}; info = []
    if re.search(r'#\s*define\s+CCharString\b', m): return text, ['MACRO']
    sites = []
    for key, lb, rb, hs in lib.class_bodies(m, 'CCharString'):
        if lib.in_ns(m, hs, nsr): info.append('NAMESPACED'); continue
        for c in re.finditer(r'(?<![~\w:])CCharString\s*\(', m[:rb]):
            if c.start() < lb or not lib.depth1(m, lb, c.start()): continue
            sites.append(c)
    for c in re.finditer(r'\bCCharString\s*::\s*CCharString\s*\(', m):
        if lib.in_ns(m, c.start(), nsr): info.append('NAMESPACED_DEF'); continue
        sites.append(c)
    longs = 0; fixes = []
    for c in sites:
        lp = c.end() - 1; rp = lib.match(m, lp, '(', ')')
        ptxt = m[lp + 1:rp]
        if re.fullmatch(r'\s*const\s+char\s*\*\s*\w*\s*,\s*long(\s+\w+)?(\s*=\s*-?\w+)?\s*', ptxt): longs += 1; continue
        pm = PAR.match(ptxt)
        if pm: fixes.append(lp + 1 + pm.start(2))
    if fixes and longs: return text, info + ['HAS_LONG']
    for off in fixes: edits[off] = (3, 'long'); info.append('FIX')
    if fixes:
        # keep comments truthful: a comment line naming this constructor with `(const char*, int ...)` says long
        for c in CMT.finditer(text):
            if m[c.start()] != ' ': continue          # code, not a comment (comments are blanked in the mask)
            line = text[text.rfind('\n', 0, c.start()) + 1:text.find('\n', c.start()) if text.find('\n', c.start()) >= 0 else len(text)]
            if re.search(r'CCharString|99ebf0', line, re.I):
                edits[c.start('t')] = (3, 'long')
    return lib.apply(text, edits), info

if __name__ == '__main__':
    rep = '--report' in sys.argv
    for f in [a for a in sys.argv[1:] if a != '--report']:
        if rep:
            print(f, fix(open(f, encoding='utf-8', errors='surrogateescape').read())[1]); continue
        ch, info = lib.rw(f, fix)
        if ch: print('edited', f, info.count('FIX'))
