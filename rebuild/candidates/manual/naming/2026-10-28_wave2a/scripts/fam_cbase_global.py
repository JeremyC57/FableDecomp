#!/usr/bin/env python3
"""Respell a file-scope `CBase` root model to its PDB class, per row.

Retail roots (Ego_r.pdb + FableWin.pdb, RTTI; wave-1 CONFLICTS.md):
  A = CBaseClass             ctor 0x0099A2D0 ??0CBaseClass@@QAE@XZ, dtor 0x0099A2E0 ??1CBaseClass@@UAE@XZ (public)
  N = CBaseClassNonCopyable  ctor 0x0099A2F0 ??0CBaseClassNonCopyable@@IAE@XZ, dtor 0x0099A300
                             ??1CBaseClassNonCopyable@@MAE@XZ (protected; behaviour tests keep it public)
Landed rows spell both roots `struct CBase` at file scope (`??0CBase@@QAE@XZ`, `??1CBase@@UAE@XZ`),
so the text does not say which root a row models: ROLE below is the retail call target of each
row's CBase ctor/dtor (wave-1 pair logs, runs/base1a+base1b: the address its relocation hits).

Body rules (depth-1 statements of the root):
  ctor `CBase()` kept; `virtual ~CBase()` kept; access labels dropped (one label after the brace);
  a placeholder `virtual void v00()` (v0/VSlot0) in a ctor-only root is kept as is (slot 0) --
          switched off in this delivery (PLACEHOLDER_ROOTS = False: blocker, deferred);
  anything else, a non-virtual dtor, or a placeholder next to a dtor -> blocker, no edit.
Every code use of the bare name `CBase` (base clauses, init lists, `CBase::~CBase`) is renamed;
comments `CBase::CBase`, `CBase::~CBase` and `CBase` right before a root address follow.
Idempotent (a second run finds no `CBase`).  usage: fam_cbase_global.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import (skipped, mask, scopes, apply_edits, tokens, prev_sig, prev_word, comment_spans, run,
                    drop_nested, del_span, global_classes, row_of, innermost)
from fam_cbase_ns import statements, PLACEHOLDER, ADDR

A, N = 'CBaseClass', 'CBaseClassNonCopyable'
ROLE = {
    '004138d0': A, '00415c10': N, '00415c30': N, '0049166e': N, '004979a0': A, '00499880': A,
    '0049bb60': N, '0049bb90': N, '004ab170': A, '004bb600': A, '004bc4f0': A, '004bd160': A,
    '004c3250': A, '0051d8e0': N, '005dc3c0': A, '005dc450': A, '005dc4d0': A, '005dc550': A,
    '005dc650': A, '005dc730': A, '005dc7b0': A, '005dc8b0': A, '005dc9b0': A, '005dca60': A,
    '0069ee90': N, '006b0460': A, '006b2a70': N, '006b7380': N, '006b7760': N, '006bbba0': A,
    '006e0480': A, '006e7150': N, '006f9f90': N, '00709b10': A, '00711fa0': N, '00712df0': N,
    '00713180': N, '00713200': N, '00716790': N, '007185d0': N, '00769d00': A, '0076aa30': A,
    '0076b5d0': A, '007ae3d0': A, '007e71d0': N, '00816150': A, '0082a460': N, '008a1e50': A,
    '008a1f30': A, '008a2020': A, '008a2100': A, '008a21e0': A, '008a22d0': A, '008a2690': A,
    '008a2870': A, '008a2950': A, '008a2b30': A, '008a2c10': A, '008a2cf0': A, '008a2de0': A,
    '008a2ec0': A, '008a2fa0': A, '008a3090': A, '008a3170': A, '008a3250': A, '008a3340': A,
    '008a3420': A, '008a3500': A, '008a35f0': A, '008a36d0': A, '008a37b0': A, '008a38a0': A,
    '008a3980': A, '008a3b60': A, '008a3c40': A, '008a3d20': A, '008a3e10': A, '008a3ef0': A,
    '008a40d0': A, '008a4480': A, '008a4650': A, '008a4a00': A, '008a4be0': A, '008a4cc0': A,
    '008a4e90': A, '008a4f70': A, '008a5050': A, '008a5140': A, '008a5220': A, '008a5400': A,
    '008a54e0': A, '008a56c0': A, '008a57b0': A, '008a5980': A, '008a5a70': A, '008a5c40': A,
    '008a5d30': A, '008a5f00': A, '008a5ff0': A, '008a61c0': A, '008a62b0': A, '008a6480': A,
    '008a6b10': A, '008a6ce0': A, '008a6dd0': A, '008a6fa0': A, '008a7090': A, '008a7260': A,
    '008a7350': A, '008a7520': A, '008a7610': A, '009034f0': N, '00903590': N, '00903610': N,
    '00903660': N, '009036a0': N, '00903760': A, '00903ac0': N, '00904430': N, '00905130': N,
    '00905180': N, '009051f0': N, '00905290': N, '0098dfd0': N, '00993af0': N, '00993b50': N,
    '00993e00': N, '00994400': N, '009944a0': N, '00998250': A, '00998a30': A, '0099ad80': N,
    '0099adc0': N, '0099ae00': N, '009ae150': A, '009af1e0': A, '009bba40': A, '009bdbe0': N,
    '009d1090': N, '009d1160': N, '009d5260': N, '009d58d0': N, '009e2830': N, '009e4a20': A,
    '009e5200': N, '009eaff0': A, '009eeb00': A, '009f8690': N, '009fbd00': A, '009fccf0': A,
    '009fcd10': A, '009fcd40': A, '00a02d20': N, '00a0a110': N, '00a0d4a0': N, '00a38500': N,
    '00a39b40': N, '00a39b70': N, '00a39c80': N, '00a39d80': N, '00a445d0': N, '00a44d80': N,
    '00a44e80': N, '00a454f0': N, '00a455a0': N, '00a5b590': N, '00a5b710': N, '00a5b760': N,
    '00a5ec40': N, '00a5f110': N, '00a62fc0': N, '00a63440': N, '00a67ff0': A, '00a68130': A,
    '00a69550': A, '00a76580': N, '00a7a570': N, '00a7a760': N, '00a8df70': N, '00a8e160': N,
    '00a8eda0': N, '00a98100': N, '00aa41c0': N, '00aa7590': N, '00aa76d0': N, '00aa7850': N,
    '00aa7db0': N, '00aa83b0': N, '00aab850': N, '00aab8b0': N, '00aacea0': A, '00ac3e00': N,
    '00aced20': N, '00af2330': N, '00af2620': N, '00af7a90': N, '00b29720': N, '00b2e6c0': A,
    '00b31d80': N, '00b36ce0': N, '00b3adc0': N, '00b492c0': N, '00b49320': N, '00b49950': N,
    '00b52250': N, '00b67040': N, '00b7a880': N, '00b7add0': N, '00b80660': N, '00b80900': N,
    '00b81bc0': N, '00b870a0': A, '00b8b5e0': N, '00b8d780': N, '00b93eb0': N, '00b94a40': N,
    '00b98a50': N, '00b98bc0': N, '00b9cb10': N, '00babb50': N, '00bacdb0': N, '00bb1db0': N,
    '00bb63a0': N, '00bb6ab0': N, '00bb6e00': N, '00bba340': N, '00bba480': N, '00bbbf70': N,
    '00bbc130': N, '00bc4d90': N, '00bc7a50': N, '00bce740': N, '00bda3d0': N, '00be0cd0': N,
    '00be1ba0': N, '00be32a0': N, '00bf3380': N, '00bfc2e0': N, '00bfc370': N, '00cd4500': N,
    '00cd4620': N, '00cd4660': N, '00cd46a0': N, '00cd46e0': N, '00cdb3c0': A, '00cdb9e0': N,
    '00cffb60': A, '00d1dff0': A, '00d1eae0': A, '00d1f610': A, '00d37530': A, '00d81c50': A,
    '00d83fa0': A, '00da7d30': A, '00daa260': A, '00daa700': A, '00dc8030': A, '00dcc140': A,
    '00dcf920': A, '00dde940': A, '00df2380': A, '00e0d6a0': A, '00e241b0': A, '00e33c90': A,
    '00e33e00': A, '00e4a740': A, '00e503d0': A, '00e63730': A, '00e6f040': A, '00e7e640': A,
    '00e7f7f0': A, '00e93080': A, '00e9d800': A, '00ea8130': A, '00ec0ba0': A, '00ed41d0': A,
    '00ed45a0': A, '00ee36a0': A, '00ef5cb0': A, '00ef7ab0': A, '00efeb60': A, '00f37520': N,
}
SKIP = {}
# behaviour tests that define the root's members themselves and also delete through the root pointer
TEST_PUBLIC = {'00bf3380'}
# ctor-only roots with a slot-0 placeholder (`virtual void v00(); CBase();`, 97 NonCopyable rows) are
# the next delivery; this one handles roots that declare the virtual destructor.
PLACEHOLDER_ROOTS = False


def transform(text, path):
    if skipped(path, SKIP):
        return text, 0, ['SKIP ' + skipped(path, SKIP)]
    a, is_test = row_of(path)
    if a not in ROLE:
        return text, 0, []
    new = ROLE[a]
    m = mask(text)
    # a behaviour test keeps a non-copyable root public (it deletes through it) unless it defines
    # the root's members out of line: it then links against the row's object, whose root is protected
    ool = re.search(r'\bCBase\s*::\s*~?\s*CBase\s*\(', m)
    acc = 'public' if (new == A or (is_test and (not ool or a in TEST_PUBLIC))) else 'protected'
    sc = scopes(m)
    cl = global_classes(m, sc, 'CBase')
    if not cl:
        return text, 0, []
    if len(cl) != 1:
        return text, 0, ['MULTI CBase']
    c = cl[0]
    if tokens(m, new):
        return text, 0, ['EXISTS %s' % new]
    hs = m.rfind(c.key, 0, c.lb)
    if ':' in m[hs:c.lb]:
        return text, 0, ['BASES']
    notes, edits = [], []
    ctor = dtor = ph = None
    for st, en in statements(m, c.lb, c.rb):
        s = m[st:en]
        if re.match(r'(public|private|protected)\s*:', s):
            edits.append(del_span(text, m, st, en)); continue
        if PLACEHOLDER.match(s):
            if ph is not None: return text, 0, ['TWO_PLACEHOLDERS']
            ph = (st, en); continue
        if re.match(r'^\s*(?:virtual\s+)?~\s*CBase\s*\(\s*(?:void)?\s*\)', s):
            dtor = (st, en); continue
        if re.match(r'^\s*(?:inline\s+)?CBase\s*\(\s*(?:void)?\s*\)', s):
            ctor = (st, en); continue
        return text, 0, ['BODY %s' % re.sub(r'\s+', ' ', s)[:60]]
    if dtor is not None and not re.match(r'^\s*virtual\b', m[dtor[0]:dtor[1]]):
        return text, 0, ['NOVIRT ~CBase']
    if dtor is not None and ph is not None:
        return text, 0, ['PLACEHOLDER_AND_DTOR']
    if dtor is None and ph is None:
        return text, 0, ['NOVPTR']
    if ph is not None and not PLACEHOLDER_ROOTS:
        return text, 0, ['PLACEHOLDER_ROOT (deferred: ctor-only root with a slot-0 placeholder)']
    edits.append((hs, c.lb + 1, 'class %s {\n%s:%s' % (new, acc, '' if m[c.lb + 1] == '\n' else ' ')))
    for p in tokens(m, 'CBase'):
        j = prev_sig(m, p)
        if j >= 1 and m[j] == ':' and m[j - 1] == ':':
            q = prev_word(m, j - 1)
            if q and q != 'CBase':
                return text, 0, ['QUALIFIED %s::CBase' % q]
        edits.append((p, p + len('CBase'), new))
    for ca, cb in comment_spans(text, m):
        seg = text[ca:cb]
        for mm in re.finditer(r'\bCBase::~CBase\b|\bCBase::CBase\b|\bCBase\b(?=[^\n]{0,24}%s)' % ADDR.pattern, seg):
            g = mm.group(0)
            edits.append((ca + mm.start(), ca + mm.end(), g.replace('CBase', new) if '::' in g else new))
    edits = drop_nested(edits)
    return apply_edits(text, edits), len(edits), notes


if __name__ == '__main__':
    run(transform, sys.argv[1:])
