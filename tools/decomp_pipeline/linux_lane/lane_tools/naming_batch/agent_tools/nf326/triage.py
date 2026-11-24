# triage.py: per ENTRIES line, how engine/<Class>.h declares the PDB member: the #if stack around each declaration
# (include guard dropped), inline body or declaration only. Output work/triage.tsv
import re, sys
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF326')
R = Path('/home/user/FableDecomp')
GUARD = re.compile(r"#\s*ifndef\s+(FABLE_RETAIL_HEADER_\w+|\w+_H|\w+_HPP|\w+_INCLUDED)\s*$")
def decls(h, meth):
    t = h.read_text(encoding='utf-8', errors='replace'); L = t.split('\n'); stack = []; out = []
    for n, ln in enumerate(L, 1):
        s = ln.strip()
        if re.match(r"#\s*if", s): stack.append(s); continue
        if re.match(r"#\s*(else|elif)", s) and stack: stack[-1] = stack[-1] + ' |ELSE:' + s; continue
        if re.match(r"#\s*endif", s) and stack: stack.pop(); continue
        c = ln.split('//')[0]
        if re.search(r"(^|[^\w:.>])" + re.escape(meth) + r"\s*\(", c) and not re.search(r"(->|\.)\s*" + re.escape(meth) + r"\s*\(", c):
            rest = ' '.join(L[n - 1:n + 3]).split('//')[0]
            k = rest.find(meth); depth = 0; j = rest.find('(', k); body = '?'
            for i in range(j, len(rest)):
                if rest[i] == '(': depth += 1
                elif rest[i] == ')':
                    depth -= 1
                    if depth == 0:
                        tail = rest[i + 1:].lstrip()
                        tail = re.sub(r"^(const\s*)?", "", tail).lstrip()
                        body = 'INLINE' if tail.startswith('{') else ('DECL' if tail.startswith(';') or tail.startswith('=') else '?' + tail[:10])
                        break
            inner = [x for x in stack if not GUARD.match(x)]
            out.append((n, body, ' & '.join(inner) or '-', c.strip()[:110]))
    return out
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
sel = set(int(x) for x in sys.argv[1:]) if len(sys.argv) > 1 else None
o = open(A / 'work/triage.tsv', 'w')
for i, e in enumerate(E, 1):
    if sel and i not in sel: continue
    m = re.match(r"\?([^@]+)@([^@]+)@@", e[3]); meth, X = m.group(1), m.group(2)
    h = R / 'rebuild/include/engine' / (X + '.h')
    if not h.exists(): print(i, e[0], X, meth, 'NOHEADER', sep='\t', file=o); continue
    D = decls(h, meth)
    if not D: print(i, e[0], X, meth, 'ABSENT', sep='\t', file=o)
    for n, b, st, c in D: print(i, e[0], X, meth, 'L%d' % n, b, st, c, sep='\t', file=o)
o.close()
