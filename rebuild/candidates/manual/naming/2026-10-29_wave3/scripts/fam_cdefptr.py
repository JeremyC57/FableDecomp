#!/usr/bin/env python3
"""Return CDefinitionManager::GetPDefFromGlobalIndex (0x009AD9E0) as the class template it is.

Retail: ?GetPDefFromGlobalIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@J@Z
  = CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long) const
(Ego_r.pdb + FableWin.pdb; DECISIONS.tsv proven, caller 007c0e80). Rows model the result as a plain
`struct CDefPointer { CDefBase* m_p; ... }` and the getter as non-const, so they emit
?GetPDefFromGlobalIndex@CDefinitionManager@@QAE?AUCDefPointer@@J@Z.
Shape handled (source rows only): exactly one file-scope `struct CDefPointer` (no base, no template, no out-of-line
member) and exactly one class CDefinitionManager whose public `CDefPointer GetPDefFromGlobalIndex(long x)` is the
only declaration of that name. Edit: `struct CDefPointer {` becomes `template <class T> class CDefPointer { public:`
(the body does not use T, so every instance has the struct's layout and inline members); every code token
`CDefPointer` outside that body becomes `CDefPointer<const CDefClassBase>`; the getter gets `const`
(and its out-of-line definition, when the row has one); `class CDefClassBase;` is declared before the template when
the file does not declare it. Blockers: `struct CDefClassBase` (the key would mangle U), `CDefPointer<` or
`CDefPointer::` already present, the getter under protected/private, any other GetPDefFromGlobalIndex declaration.
Idempotent (the struct is a template after one run).
usage: fam_cdefptr.py [--check] <file>...
"""
import os, re, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from famlib import mask, scopes, apply_edits, tokens, run, statements, row_of, access_at, at_global, drop_nested

T = 'CDefPointer<const CDefClassBase>'
GDECL = re.compile(r'^\s*CDefPointer\s+GetPDefFromGlobalIndex\s*\(\s*long\s+\w+\s*\)\s*(?P<c>const\s*)?;\s*$')


def transform(text, path):
    a, is_test = row_of(path)
    if is_test:
        return text, 0, []
    m = mask(text)
    sc = scopes(m)
    dp = [s for s in sc if s.kind == 'class' and s.name == 'CDefPointer']
    if not dp:
        return text, 0, []
    if len(dp) != 1:
        return text, 0, ['MULTI CDefPointer']
    d = dp[0]
    hs = m.rfind(d.key, 0, d.lb)
    head = re.sub(r'\s+', ' ', m[hs:d.lb]).strip()
    if head != 'struct CDefPointer' or not at_global(d):
        return text, 0, ['HEAD %s' % head]
    j = hs - 1
    while j >= 0 and m[j] in ' \t\r\n': j -= 1
    if m[j - 7:j + 1] == 'template' or m[j] == '>':
        return text, 0, ['TEMPLATE']
    if re.search(r'\bCDefPointer\s*(<|::)', m):
        return text, 0, ['CDEFPOINTER_QUALIFIED']
    if re.search(r'\bstruct\s+CDefClassBase\b', m):
        return text, 0, ['STRUCT CDefClassBase']
    dm = [s for s in sc if s.kind == 'class' and s.name == 'CDefinitionManager']
    if len(dm) != 1:
        return text, 0, ['CDM %d' % len(dm)] if dm else []
    c = dm[0]
    decl = [(x, y) for x, y in statements(m, c.lb, c.rb) if re.search(r'\bGetPDefFromGlobalIndex\b', m[x:y])]
    if len(decl) != 1 or not GDECL.match(m[decl[0][0]:decl[0][1]]):
        return text, 0, ['GETTER_DECL']
    x, y = decl[0]
    if access_at(m, c, x) != 'public':
        return text, 0, ['GETTER_ACCESS %s' % access_at(m, c, x)]
    edits = []
    # out-of-line definitions of the getter
    for mm in re.finditer(r'\bCDefinitionManager\s*::\s*GetPDefFromGlobalIndex\s*\(\s*long\s+\w+\s*\)\s*(const\b)?', m):
        if not mm.group(1):
            edits.append((mm.end(), mm.end(), ' const'))
    if not GDECL.match(m[x:y]).group('c'):
        k = m.rfind(';', x, y)
        edits.append((k, k, ' const'))
    # the template
    lbe = d.lb + 1
    edits.append((hs, d.lb, 'template <class T> class CDefPointer '))
    edits.append((lbe, lbe, ' public:'))
    if not re.search(r'\bclass\s+CDefClassBase\b', m[:hs]):
        ls = text.rfind('\n', 0, hs) + 1
        edits.append((ls, ls, 'class CDefClassBase;\n'))
    for p in tokens(m, 'CDefPointer'):
        if d.lb < p < d.rb or hs <= p < d.lb: continue
        edits.append((p, p + len('CDefPointer'), T))
    edits = drop_nested(edits)
    return apply_edits(text, edits), len(edits), []


if __name__ == '__main__':
    run(transform, sys.argv[1:])
