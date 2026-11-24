# flat.py <idx..>: flatten-switch entries: HEAD free `R __fastcall X_M(P* self)` -> `R' X::M() [const]` on engine/X.h under its class-model
# switch, KEEPING every other line of the HEAD source (local declarations the body needs). R' = the header's declared return type.
# `(void)self;` is dropped; any other use of the parameter keeps a local `<P>* self = this;` (const per the member). Writes rows/<addr>.cpp.
import re, sys
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF327')
T = {}
for l in open(A / 'work/triage.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) > 7 and p[5] == 'DECL': T.setdefault(int(p[0]), (p[2], p[3], p[6], p[7]))
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
for i in [int(x) for x in sys.argv[1:]]:
    e = E[i - 1]; a = e[0]; X, M, st, decl = T[i]; sw = re.findall(r'#ifdef (\w+)', st)
    src = (A / 'orig' / (a + '.cpp')).read_text(encoding='utf-8')
    const = bool(re.match(r'\?[^@]+@[^@]+@@[A-Z]B', e[3]))
    ret = re.sub(r'^(public:|private:|protected:)?\s*virtual\s+', '', decl).split(M + '(')[0].strip()
    m = re.search(r'^([^\n;{}]*?)\b__fastcall\s+' + re.escape(X) + '_' + re.escape(M) + r'\s*\(([^)]*)\)\s*\{', src, re.M)
    if not m: print(i, a, 'NO_DEF'); continue
    prm = m.group(2).strip(); pn = re.search(r'(\w+)\s*$', prm).group(1) if prm and prm != 'void' else None
    k = m.end(); d = 1
    while d: d += {'{': 1, '}': -1}.get(src[k], 0); k += 1
    body = src[m.end():k - 1]
    if pn:
        body = re.sub(r'\n?[ \t]*\(void\)\s*' + pn + r'\s*;[ \t]*', '', body)
        if re.search(r'\b' + pn + r'\b', body):
            body = '\n    %s%s* %s = this;' % ('const ' if const else '', X, pn) + body
    new = '%s %s::%s()%s\n{%s}' % (ret, X, M, ' const' if const else '', body)
    out = src[:m.start()] + new + src[k:]
    # switch + header
    hdr = '#include "engine/%s.h"' % X
    lines = out.split('\n')
    if not any(re.match(r'\s*#\s*include\s+"engine/%s\.h"' % X, l) for l in lines):
        last = max([n for n, l in enumerate(lines) if re.match(r'\s*#\s*include', l)] or [-1])
        if last < 0:
            last = max([n for n, l in enumerate(lines[:]) if l.startswith('//') and n == 0 or (n > 0 and lines[n].startswith('//') and all(x.startswith('//') for x in lines[:n + 1]))] or [-1])
        lines.insert(last + 1, hdr + '   // switch route: the real class model')
    first = 0
    while first < len(lines) and (lines[first].startswith('//') or not lines[first].strip()): first += 1
    for s in reversed(sw):
        if not any(re.match(r'\s*#\s*define\s+' + s + r'\b', l) for l in lines): lines.insert(first, '#define ' + s)
    lines.insert(0, '// Leaf: ' + e[3]) if not lines[0].startswith('// Leaf:') else lines.__setitem__(0, '// Leaf: ' + e[3])
    (A / 'rows' / (a + '.cpp')).write_text('\n'.join(lines), encoding='utf-8')
    print(i, a, 'OK', 'pn=%s' % pn, 'ret=%s' % ret)
