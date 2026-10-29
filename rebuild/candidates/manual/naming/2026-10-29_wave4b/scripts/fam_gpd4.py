#!/usr/bin/env python3
"""CDefinitionManager def getters at 0x009AD9E0 / 0x009ADE10 as the PDB spells them (wave-4 shapes).

PDB (Ego_r.pdb b05940.. and FableWin.pdb 03061ee0..; same order in both, retail order 0x009AD9E0 / 0x009ADA10 /
0x009ADA40 / 0x009ADA90 / ... / 0x009ADE10 / 0x009ADE50 / 0x009ADE90 ParseStringForSymbols):
  0x009AD9E0 ?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z      (const)
  0x009ADA10 ?GetPDefFromGlobalIndex@CDefinitionManager@@IAE?AV?$CDefPointer@VCDefClassBase@@@@J@Z         (protected)
  0x009ADE10 ?GetPDefFromClassIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@J@Z
0x009ADE10 takes (class name, index) -- `ret 0Ch` (result + 2 arguments), the name goes to the class-info lookup
0x009AD2E0, the index selects a global index from that class's list -- and is the const member: the PDB pair is
const then protected non-const, like 0x009AD9E0 / 0x009ADA10 (44 bytes each) and 0x009ADA40 / 0x009ADA90.
The catalog / manifest label "GetPDefFromGlobalIndex" of 0x009ADE10 is propagated.
Shapes (source rows):
  CONST  `CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long[ n]);` bound to 0x009AD9E0 -> the const member
         returning `CDefPointer<const CDefClassBase>`; locals `CDefPointer<CDefClassBase> v = GetPDefFromGlobalIndex(`
         take the same type (same layout: the template body does not use T).
  CLASSIDX the generated GetDef<T> batch: `struct CDefPointer { ... };` (no T), `CDefPointer GetPDefFromGlobalIndex(long
         kind, long index);   // 0x009ADE10` and `bool GetDef_<addr>(long kind, long index, CDefPointer& out)`:
         CDefPointer becomes `template <class T> class CDefPointer { public:` (body unchanged), every other code token
         `CDefPointer<const CDefClassBase>`; the getter `GetPDefFromClassIndex(const CCharString& name, long index)
         const`; the row's own `long kind` (it is pushed unchanged: `push [kind]`) `const CCharString& kind`.
Idempotent.  usage: fam_gpd4.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import mask, scopes, apply_edits, tokens, run, row_of, drop_nested, comment_spans

T = 'CDefPointer<const CDefClassBase>'
CL = re.compile(r'CDefPointer\s+GetPDefFromGlobalIndex\s*\(\s*long\s+kind\s*,\s*long\s+index\s*\)\s*;(?P<c>[ \t]*//[^\n]*0x009ADE10[^\n]*)')
CONSTD = re.compile(r'CDefPointer\s*<\s*CDefClassBase\s*>\s*GetPDefFromGlobalIndex\s*\(\s*long(?:\s+\w+)?\s*\)\s*;')


def transform(text, path):
    a, is_test = row_of(path)
    m = mask(text)
    edits = []
    d = CONSTD.search(m)
    if d and len(CONSTD.findall(m)) == 1 and '009ad9e0' in text[d.end():text.find('\n', d.end())].lower():
        edits.append((d.start(), d.end(), '%s GetPDefFromGlobalIndex(long) const;' % T))
        for v in re.finditer(r'CDefPointer\s*<\s*CDefClassBase\s*>(?=\s*\w+\s*=\s*GetPDefFromGlobalIndex\s*\()', m):
            edits.append((v.start(), v.end(), T))
        DEF = re.compile(r'CDefPointer\s*<\s*CDefClassBase\s*>(?P<q>\s*CDefinitionManager\s*::\s*GetPDefFromGlobalIndex\s*\(\s*long(?:\s+\w+)?\s*\))\s*\{')
        defs = list(DEF.finditer(m))
        if len(defs) > 1: return text, 0, ['MULTI DEF']
        for df in defs:                                 # a test's out-of-line fake: same const signature
            edits.append((df.start(), df.start('q'), T))
            edits.append((df.end('q'), df.end('q'), ' const'))
            lb = df.end() - 1
            rb = [s for s in scopes(m) if s.lb == lb]
            if len(rb) != 1: return text, 0, ['DEF BODY']
            for v in re.finditer(r'CDefPointer\s*<\s*CDefClassBase\s*>', m[lb:rb[0].rb]):
                edits.append((lb + v.start(), lb + v.end(), T))
        return apply_edits(text, drop_nested(edits)), len(edits), []
    c = CL.search(text)
    if not c or is_test:
        return text, 0, []
    if CL.search(m[:c.start()] + ' ' * (c.end() - c.start()) + m[c.end():]):
        return text, 0, ['MULTI']
    sc = scopes(m)
    dp = [s for s in sc if s.kind == 'class' and s.name == 'CDefPointer']
    if len(dp) != 1: return text, 0, ['CDP bodies']
    b = dp[0]
    hs = m.rfind('struct', 0, b.lb)
    if re.sub(r'\s+', ' ', m[hs:b.lb]).strip() != 'struct CDefPointer': return text, 0, ['CDP head']
    if re.search(r'\bCDefPointer\s*<|\bCDefClassBase\b|\bCCharString\b', m): return text, 0, ['TOKENS']
    own = re.compile(r'\bGetDef_%s\s*\(\s*long\s+kind\s*,\s*long\s+index\s*,\s*CDefPointer\s*&\s*out\s*\)' % a)
    owns = list(own.finditer(m))
    if len(owns) != 2: return text, 0, ['OWN %d' % len(owns)]
    calls = list(re.finditer(r'\bGetPDefFromGlobalIndex\s*\(\s*kind\s*,\s*index\s*\)', m))
    if len(calls) != 1: return text, 0, ['CALLS']
    ls = text.rfind('\n', 0, hs) + 1
    edits.append((ls, ls, 'class CCharString;\nclass CDefClassBase;\n'))
    edits.append((hs, b.lb + 1, 'template <class T> class CDefPointer { public:'))
    edits.append((c.start(), c.end(), '%s GetPDefFromClassIndex(const CCharString& name, long index) const;   '
                  '// 0x009ADE10 ?GetPDefFromClassIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@J@Z (hidden result pointer)' % T))
    for o in owns:
        k = m.index('long', o.start())
        edits.append((k, k + 4, 'const CCharString&'))
    for x in calls:
        edits.append((x.start(), x.start() + len('GetPDefFromGlobalIndex'), 'GetPDefFromClassIndex'))
    for p in tokens(m, 'CDefPointer'):
        if hs <= p < b.rb or c.start() <= p < c.end(): continue
        edits.append((p, p + len('CDefPointer'), T))
    for a2, b2 in comment_spans(text, m):
        for mm in re.finditer(r'(?<=GetDef<T>\()long, long, CDefPointer&(?=\))', text[a2:b2]):
            edits.append((a2 + mm.start(), a2 + mm.end(), 'const CCharString&, long, CDefPointer&'))
        for mm in re.finditer(r'\bGetPDefFromGlobalIndex(?= @ 0x009ADE10)', text[a2:b2]):
            edits.append((a2 + mm.start(), a2 + mm.end(), 'GetPDefFromClassIndex'))
    return apply_edits(text, drop_nested(edits)), len(edits), []


if __name__ == '__main__':
    run(transform, sys.argv[1:])
