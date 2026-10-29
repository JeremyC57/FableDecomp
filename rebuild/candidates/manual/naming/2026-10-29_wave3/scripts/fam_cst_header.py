#!/usr/bin/env python3
"""Model CScriptThing with the shared header once it carries the retail virtual destructor.

Retail: ~CScriptThing 0x004AA840 ??1CScriptThing@@UAE@XZ (public virtual; Ego_r.pdb + FableWin.pdb, DECISIONS.tsv
proven). Needs the regenerated engine/CScriptThing.h (w3 HEADER_MEMBERS virtual_dtor: `virtual ~CScriptThing();`
in place of the `void* __vftable` word, same +0 vptr, sizeof 0xC).

Shape handled (wave-2B fam_autotiny rows): a file-scope local stand-in
    class CScriptThing { public: virtual ~CScriptThing(); };   // 0x004AA840 ...
becomes `#include "engine/CScriptThing.h"` (the row only calls the destructor through
`reinterpret_cast<CScriptThing*>(p)->CScriptThing::~CScriptThing()`). Blockers: a second CScriptThing body, any
other CScriptThing member use, or the header already included. Idempotent.
usage: fam_cst_header.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import mask, run, tokens

LOCAL = re.compile(r'^[ \t]*class[ \t]+CScriptThing[ \t]*\{[ \t]*public:[ \t]*virtual[ \t]+~CScriptThing\(\)[ \t]*;[ \t]*\};[^\n]*\n', re.M)


def transform(text, path):
    m = mask(text)
    if 'engine/CScriptThing.h' in text:
        return text, 0, []
    hits = list(LOCAL.finditer(text))
    if not hits:
        return text, 0, []
    if len(hits) != 1 or len(re.findall(r'\b(class|struct)\s+CScriptThing\s*[{:]', m)) != 1:
        return text, 0, ['MULTI CScriptThing']
    h = hits[0]
    for p in tokens(m, 'CScriptThing'):
        if h.start() <= p < h.end():
            continue
        ctx = m[max(0, p - 40):p + 40]
        if not re.search(r'reinterpret_cast\s*<\s*CScriptThing\s*\*\s*>|CScriptThing\s*::\s*~\s*CScriptThing', ctx):
            return text, 0, ['OTHER_USE line %d' % (text.count('\n', 0, p) + 1)]
    new = '#include "engine/CScriptThing.h"   // ~CScriptThing 0x004AA840 (Ego_r.pdb ??1CScriptThing@@UAE@XZ)\n'
    return text[:h.start()] + new + text[h.end():], 1, []


if __name__ == '__main__':
    run(transform, sys.argv[1:])
