# p2srcgrep361.py: Part 2 source leg for GetDef<CMaterialDef> 0x0066C340. Every landed row (build_candidates.ps1 Source) whose source text,
# quoted includes of other rebuild/src/compiled files followed (folded TUs), has (a) `GetDef<[const ]CMaterialDef`, or (b) a
# `CDefPointer<const CMaterialDef>` plus a `GetDef(` call (T deduced), or (c) GETDEF_GLOBAL_BODY plus any CMaterialDef token.
# Output p2/srcgrep.tsv: row legs(a,b,c) source.
import sys, re, os
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF361/tools'); import nf
R = '/home/user/FableDecomp'; C = R + '/rebuild/src/compiled'
PA = re.compile(r'GetDef\s*<\s*(?:const\s+)?(?:::)?CMaterialDef\b')
PB1 = re.compile(r'CDefPointer\s*<\s*const\s+(?:::)?CMaterialDef\s*>'); PB2 = re.compile(r'\bGetDef\s*\(')
PC1 = re.compile(r'FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY'); PC2 = re.compile(r'\bCMaterialDef\b')
def text(p, seen):
    if p in seen or not os.path.exists(p): return ''
    seen.add(p); t = open(p, encoding='utf-8', errors='replace').read(); out = [t]
    for m in re.finditer(r'^\s*#\s*include\s+"([^"]+)"', t, re.M):
        q = os.path.normpath(os.path.join(os.path.dirname(p), m.group(1)))
        if not os.path.exists(q): q = os.path.normpath(os.path.join(R + '/rebuild/include', m.group(1)))
        if q.startswith(C) and q.endswith(('.cpp', '.inl', '.h')): out.append(text(q, seen))
    return '\n'.join(out)
rows = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", nf.PS, re.S):
    a = re.search(r"\bAddress = '([0-9A-Fa-f]{8})'", m.group(1)); s = re.search(r"\bSource = '([^']*)'", m.group(1))
    if a and s: rows.setdefault(a.group(1).lower(), s.group(1))
o = open(str(nf.A) + '/p2/srcgrep.tsv', 'w'); n = 0
for a, s in sorted(rows.items()):
    t = text(os.path.join(C, s), set()); t2 = re.sub(r'//[^\n]*', '', t)
    legs = []
    if PA.search(t2): legs.append('a')
    if PB1.search(t2) and PB2.search(t2): legs.append('b')
    if PC1.search(t2) and PC2.search(t2): legs.append('c')
    if legs: n += 1; print('%s\t%s\t%s' % (a, ','.join(legs), s), file=o)
o.close(); print('rows', n)
