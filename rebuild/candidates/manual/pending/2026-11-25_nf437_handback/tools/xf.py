#!/usr/bin/env python3
# xf.py: NF437 LowerBound-split transform helpers (row-local map stand-in -> shared CVectorMap typedef).
import re
TD = 'CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >'
def match_brace(s, i):
    d = 0
    while i < len(s):
        if s[i] == '{': d += 1
        elif s[i] == '}':
            d -= 1
            if d == 0: return i
        i += 1
    raise ValueError('unbalanced')
def remove_struct(s, name):
    m = re.search(r'^[ \t]*(struct|class)\s+%s\s*(:[^{;]*)?\{' % re.escape(name), s, re.M)
    if not m: return s, None
    e = match_brace(s, m.end() - 1)
    e2 = s.index(';', e) + 1
    if s[e2:e2+1] == '\n': pass
    # keep a trailing comment on the closing line
    lineend = s.find('\n', e2); lineend = len(s) if lineend < 0 else lineend + 1
    return s[:m.start()] + '@@TD_%s@@\n' % name + s[lineend:], m.start()
def remove_defs(s, scope, meth='LowerBound'):
    n = 0
    while True:
        m = re.search(r'^[^\n;{}]*\b%s::%s\s*\([^)]*\)\s*(const)?\s*\{' % (re.escape(scope), meth), s, re.M)
        if not m: return s, n
        e = match_brace(s, m.end() - 1)
        le = s.find('\n', e); le = len(s) if le < 0 else le + 1
        s = s[:m.start()] + s[le:]; n += 1
def drop_empty_pragmas(s):
    return re.sub(r'#pragma auto_inline\(off\)\n((?:[ \t]*//[^\n]*\n|[ \t]*\n)*)#pragma auto_inline\(on\)\n', r'\1', s)
NOTE = ('// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local map stand-in %s (%s) replaced by the shared\n'
        '//   %s typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): CThing::Components at +0x44 (0x10 bytes),\n'
        '//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@%s spelling at %s.\n')
def typedef_block(name, old, const_tgt, inc_enum=True):
    t = ('QBE' if const_tgt else 'QAE'), ('0x004365B0' if const_tgt else '0x0040F020')
    return (NOTE % (name, old, 'CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> >', t[0], t[1]) +
            ('#include "engine/ETCInterfaceType.h"\n' if inc_enum else '') + '#include "engine/CVectorMap.h"\n' + 'typedef %s %s;\n' % (TD, name))
