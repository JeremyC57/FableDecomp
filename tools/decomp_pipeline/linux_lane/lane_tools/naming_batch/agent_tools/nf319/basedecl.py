# basedecl.py <idx..>: for an ENTRIES line, walk the base chain of engine/<X>.h (retail class branch bases, recursively through
# engine/<Base>.h) and print every line declaring virtual M( in each base header, with its #if stack. Override evidence.
import re, sys
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319')
INC = Path('/home/user/FableDecomp/rebuild/include/engine')
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
def bases(X):
    h = INC / (X + '.h')
    if not h.exists(): return []
    t = h.read_text(encoding='utf-8', errors='replace'); out = []
    for m in re.finditer(r"(?:class|struct)\s+" + re.escape(X) + r"\s*:\s*([^{;]*)\{", t):
        for b in m.group(1).split(','):
            b = re.sub(r'\b(public|protected|private|virtual)\b', '', b).strip()
            if b and b not in out: out.append(b)
    return out
def decls(X, M):
    h = INC / (X + '.h')
    if not h.exists(): return ['NOHEADER']
    L = h.read_text(encoding='utf-8', errors='replace').split('\n'); st = []; out = []
    for n, ln in enumerate(L, 1):
        s = ln.strip()
        if re.match(r"#\s*if", s): st.append(s); continue
        if re.match(r"#\s*(else|elif)", s) and st: st[-1] += ' |ELSE'; continue
        if re.match(r"#\s*endif", s) and st: st.pop(); continue
        c = ln.split('//')[0]
        if re.search(r"\b" + re.escape(M) + r"\s*\(", c) and 'virtual' in c:
            out.append('L%d [%s] %s' % (n, ' & '.join(x for x in st[1:])[:120], c.strip()[:120]))
    return out
for i in [int(x) for x in sys.argv[1:]]:
    e = E[i - 1]; m = re.match(r"\?([^@]+)@([^@]+)@@", e[3]); M, X = m.group(1), m.group(2)
    print('#%d %s %s::%s  %s' % (i, e[0], X, M, e[3]))
    seen = set(); todo = bases(X)
    while todo:
        B = todo.pop(0)
        if B in seen: continue
        seen.add(B)
        for d in decls(B, M): print('   %s: %s' % (B, d))
        todo += bases(B)
    print('   chain:', ' '.join(seen))
