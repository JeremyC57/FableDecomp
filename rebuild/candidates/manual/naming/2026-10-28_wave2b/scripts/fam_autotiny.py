#!/usr/bin/env python3
"""The AutoTiny member-destructor helper bound to 0x004AA840 is CScriptThing::~CScriptThing.

Ego_r.pdb / FableWin.pdb name 0x004AA840 ??1CScriptThing@@UAE@XZ (public virtual destructor; DECISIONS.tsv proven).
Auto-generated scalar/vector deleting-destructor rows declare it as a free fastcall helper
`extern void __fastcall AutoTiny{First,}MemberDestructor(void* member);` (or the `_004aa840`-suffixed second-member
spelling) and call it on `static_cast<unsigned char*>(self) + N`. A fastcall (void*) call and a qualified thiscall
destructor call are the same bytes (`lea ecx,[self+N] ; call 0x004AA840`), so the edit is:
  * the first such extern becomes `class CScriptThing { public: virtual ~CScriptThing(); };` (later ones are dropped);
  * each call `AutoTinyXxx(expr);` becomes `reinterpret_cast<CScriptThing*>(expr)->CScriptThing::~CScriptThing();`
    (qualified, so the virtual destructor is called directly, as retail does for a member subobject).
The class is a local model on purpose: engine/CScriptThing.h models the vptr as a `void* __vftable` member and has no
destructor, so it cannot emit this call (HEADERS.md). Files that already mention CScriptThing are left alone.
Only rows under rebuild/src/compiled are meant to be passed; the tests keep their recording stubs. Idempotent.
usage: fam_autotiny.py [--report] <file>...
"""
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib

NAMES = r'(?:AutoTinyFirstMemberDestructor|AutoTinyMemberDestructor|AutoTinySecondMemberDestructor_004aa840)'
EXT = re.compile(r'extern\s+void\s+__fastcall\s+' + NAMES + r'\s*\(\s*void\s*\*\s*\w*\s*\)\s*;[ \t]*(?://[^\n]*)?')
CALL = re.compile(r'(?<![\w:])' + NAMES + r'\s*\(')
MODEL = 'class CScriptThing { public: virtual ~CScriptThing(); };   // 0x004AA840 (Ego_r.pdb ??1CScriptThing@@UAE@XZ)'

def fix(text):
    m = lib.mask(text); info = []
    if re.search(r'\bCScriptThing\b', m): return text, ['HAS_CSCRIPTTHING']
    exts = list(EXT.finditer(m))
    if not exts: return text, ['NO_EXTERN']
    edits = {}
    for i, e in enumerate(exts):
        if i == 0: edits[e.start()] = (e.end() - e.start(), MODEL)
        else:
            # drop the whole line
            s = text.rfind('\n', 0, e.start()) + 1; t = text.find('\n', e.end()); t = len(text) if t < 0 else t + 1
            edits[s] = (t - s, '')
    ext_spans = [(e.start(), e.end()) for e in exts]
    for c in CALL.finditer(m):
        if any(a <= c.start() < b for a, b in ext_spans): continue
        lp = c.end() - 1; rp = lib.match(m, lp, '(', ')')
        k = rp + 1
        while m[k] in ' \t': k += 1
        if m[k] != ';': return text, ['ODD_CALL']
        arg = text[lp + 1:rp].strip()
        edits[c.start()] = (rp + 1 - c.start(), 'reinterpret_cast<CScriptThing*>(%s)->CScriptThing::~CScriptThing()' % arg)
        info.append('FIX')
    return lib.apply(text, edits), info

if __name__ == '__main__':
    rep = '--report' in sys.argv
    for f in [a for a in sys.argv[1:] if a != '--report']:
        if rep:
            print(f, fix(open(f, encoding='utf-8', errors='surrogateescape').read())[1]); continue
        ch, info = lib.rw(f, fix)
        if ch: print('edited', f, info.count('FIX'))
