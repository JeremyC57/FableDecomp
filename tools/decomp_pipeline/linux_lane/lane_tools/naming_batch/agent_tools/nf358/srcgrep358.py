# srcgrep358.py: NF358 source leg. Every landed row (build_candidates.ps1 Source) whose source text, with quoted `../`-style includes of
# other rebuild/src/compiled files followed (folded TUs), matches a copy's pattern. Output work/srcgrep.tsv: copy row pattern-hit source.
import sys, re, os
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF358/tools'); import nf
R = '/home/user/FableDecomp'; C = R + '/rebuild/src/compiled'
P = {
 '00429c6f': re.compile(r'CCountedPointer\s*<\s*(?:::)?(?:NUISystem::)?CComponent\b[^>]*>', re.S),
 '0042b312': re.compile(r'GetDef\s*<\s*(?:const\s+)?(?:::)?(?:NUISystem::)?CUIMiscThingsDef\b'),
 '0042b040': re.compile(r'GetDef\s*<\s*(?:const\s+)?(?:::)?CThingObjectDef\b'),
}
RS = re.compile(r'\b[Rr]eset\s*\(')
def text(p, seen):
    if p in seen or not os.path.exists(p): return ''
    seen.add(p); t = open(p, encoding='utf-8', errors='replace').read(); out = [t]
    for m in re.finditer(r'^\s*#\s*include\s+"([^"]+)"', t, re.M):
        q = os.path.normpath(os.path.join(os.path.dirname(p), m.group(1)))
        if not os.path.exists(q): q = os.path.normpath(os.path.join('/home/user/FableDecomp/rebuild/include', m.group(1)))   # `../src/compiled/..` resolves via -I rebuild/include
        if q.startswith(C) and q.endswith(('.cpp', '.inl', '.h')): out.append(text(q, seen))
    return '\n'.join(out)
rows = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", nf.PS, re.S):
    a = re.search(r"\bAddress = '([0-9A-Fa-f]{8})'", m.group(1)); s = re.search(r"\bSource = '([^']*)'", m.group(1))
    if a and s: rows.setdefault(a.group(1).lower(), s.group(1))
o = open(nf.W / 'srcgrep.tsv', 'w')
for a, s in sorted(rows.items()):
    t = text(os.path.join(C, s), set())
    for c, p in P.items():
        if p.search(t):
            extra = ''
            if c == '00429c6f':
                if not RS.search(t): continue
                extra = 'reset_token'
            o.write('%s\t%s\t%s\t%s\n' % (c, a, extra or 'getdef_token', s))
o.close()
