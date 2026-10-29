#!/usr/bin/env python3
"""ReadWithSrcChunkOverflow 0x00993CA0 is a PRIVATE CDataInputStream member in retail
(Ego_r.pdb + FableWin.pdb `?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z`; wave-1
DECISIONS.tsv). Shape handled here: a file-scope local `class CDataInputStream` model (no
engine/CDataInputStream.h include) that declares it under `public:` and calls it only from its own
inline members (the inline Read slow path). The declaration moves to the end of the class body
under `private:` (access and declaration order of a non-virtual member function change no layout
and no code; only QAE -> AAE in the mangled name). Calls from outside the class body (they would
need a friend) are a blocker. Idempotent.  usage: fam_rwsco_private.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import run, mask, scopes, apply_edits, drop_nested, global_classes, tokens, innermost, access_at, del_span

NAME = 'ReadWithSrcChunkOverflow'
DECL = re.compile(r'\bvoid\s+ReadWithSrcChunkOverflow\s*\(\s*unsigned\s+char\s*\*\s*\w*\s*,\s*long\s*\w*\s*\)\s*;')


def transform(text, path):
    m = mask(text)
    if re.search(r'#\s*include\s*[<"]engine/CDataInputStream\.h[>"]', text):
        return text, 0, []
    sc = scopes(m)
    cl = global_classes(m, sc, 'CDataInputStream')
    if len(cl) != 1:
        return text, 0, []
    c = cl[0]
    ds = [d for d in DECL.finditer(m, c.lb, c.rb) if innermost(sc, d.start()) is c]
    if len(ds) != 1:
        return text, 0, []
    d = ds[0]
    if access_at(m, c, d.start()) == 'private':
        return text, 0, []
    outside = [p for p in tokens(m, NAME) if not (c.lb < p < c.rb)]
    if outside:
        return text, 0, ['OUTSIDE_CALL line %d' % (text.count('\n', 0, outside[0]) + 1)]
    a = text.rfind('\n', 0, d.start()) + 1
    b = text.find('\n', d.end()); b = len(text) if b < 0 else b
    line_alone = m[a:d.start()].strip() == '' and m[d.end():b].strip() == ''
    decl_text = text[d.start():b].rstrip() if line_alone else text[d.start():d.end()]
    edits = [del_span(text, m, d.start(), d.end()) if line_alone else (d.start(), d.end(), '')]
    ins = ('\n' if m[c.rb - 1] != '\n' else '') + 'private:\n    ' + decl_text + ('' if '0x00993CA0' in decl_text.upper().replace('0X', '0x') else '   // 0x00993CA0 (private)') + '\n'
    edits.append((c.rb, c.rb, ins))
    return apply_edits(text, drop_nested(edits)), len(edits), []


if __name__ == '__main__':
    run(transform, sys.argv[1:])
