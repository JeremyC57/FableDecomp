#!/usr/bin/env python3
"""Call the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0) through engine/CDataInputStream.h.

Retail: ?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z (private; Ego_r.pdb + FableWin.pdb, DECISIONS.tsv
proven). Needs the regenerated engine/CDataInputStream.h (w3 HEADER_MEMBERS: the private member plus the
FABLE_CDATAINPUTSTREAM_FRIEND hook).

Shape handled: a file that includes engine/CDataInputStream.h directly (no `#define CDataInputStream ...` rename)
and models retail's inline CDataInputStream::Read in ONE file-scope helper class `V : [public] CDataInputStream`
that declares its own `void ReadWithSrcChunkOverflow(unsigned char*, long);` (so the call mangles
`?ReadWithSrcChunkOverflow@V@@...`). The transform
  * deletes that member declaration (the unqualified calls inside V then find the base's private member),
  * puts `#define FABLE_CDATAINPUTSTREAM_FRIEND <key> V` before the header include (V becomes a friend),
  * respells an out-of-line stub definition `void V::ReadWithSrcChunkOverflow(` (behaviour tests that link the
    row object) to `void CDataInputStream::ReadWithSrcChunkOverflow(`.
Any other use of the name outside V (member access through another object, a second helper, a qualified call)
is a blocker: no edit. Idempotent (a second run finds no declaration in V).
usage: fam_rwsco_hdr.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import mask, scopes, apply_edits, tokens, prev_sig, prev_word, run, del_span, at_global, innermost, statements

NAME = 'ReadWithSrcChunkOverflow'
INC = re.compile(r'^[ \t]*#[ \t]*include[ \t]*"engine/CDataInputStream\.h"[^\n]*\n', re.M)
DECL = re.compile(r'^\s*void\s+ReadWithSrcChunkOverflow\s*\(\s*unsigned\s+char\s*\*\s*\w*\s*,\s*long\s*\w*\s*\)\s*;$')
HOOK = 'FABLE_CDATAINPUTSTREAM_FRIEND'


def transform(text, path):
    m = mask(text)
    inc = list(INC.finditer(text))
    if len(inc) != 1 or HOOK in m:
        return text, 0, ([] if not inc or HOOK in m else ['MULTI_INCLUDE'])
    if re.search(r'#\s*define\s+(CDataInputStream|FABLE_ENGINE_CDATAINPUTSTREAM_H)\b', m[:inc[0].start()]):
        return text, 0, ['RENAMED_INCLUDE']
    sc = scopes(m)
    views = []
    for s in sc:
        if s.kind != 'class' or not at_global(s):
            continue
        hs = m.rfind(s.key, 0, s.lb)
        head = m[hs:s.lb]
        if not re.match(r'(class|struct)\s+\w+\s*:\s*(public\s+)?CDataInputStream\s*$', head.strip()):
            continue
        for st, en in statements(m, s.lb, s.rb):
            if DECL.match(m[st:en]):
                views.append((s, st, en))
    if not views:
        return text, 0, []
    if len(views) != 1:
        return text, 0, ['MULTI_VIEW']
    s, dst, den = views[0]
    if s.lb < inc[0].start():
        return text, 0, ['VIEW_BEFORE_INCLUDE']
    edits, notes = [], []
    ool = re.compile(r'\bvoid\s+%s\s*::\s*%s\s*\(' % (re.escape(s.name), NAME))
    oolhits = [(mm.start(), mm.end()) for mm in ool.finditer(m)]
    for p in tokens(m, NAME):
        if dst <= p < den:
            continue
        if any(a <= p < b for a, b in oolhits):
            continue
        if not (s.lb < p < s.rb):
            return text, 0, ['OUTSIDE_USE line %d' % (text.count('\n', 0, p) + 1)]
        j = prev_sig(m, p)
        if m[j] in '.>:':
            return text, 0, ['MEMBER_OR_QUALIFIED_USE line %d' % (text.count('\n', 0, p) + 1)]
    edits.append(del_span(text, m, dst, den))
    for a, b in oolhits:
        q = m.index(s.name, a)
        edits.append((q, q + len(s.name), 'CDataInputStream'))
    edits.append((inc[0].start(), inc[0].start(),
                  '#define %s %s %s   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)\n'
                  % (HOOK, s.key, s.name)))
    return apply_edits(text, edits), len(edits), notes


if __name__ == '__main__':
    run(transform, sys.argv[1:])
