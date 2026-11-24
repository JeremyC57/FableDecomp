# retfix.py <idx..>: rows/<a>.cpp: the X::M definition takes the header's declared return type; for an enum return (E...), a bare
# integer literal `return N;` in the body becomes `return (E)N;` (NF307 D3 literal-as-PDB-enum form). Prints old -> new.
import re, sys
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF338')
T = {}
for l in open(A / 'work/triage.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) > 7 and p[5] == 'DECL': T.setdefault(int(p[0]), (p[2], p[3], p[7]))
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
for i in [int(x) for x in sys.argv[1:]]:
    a = E[i - 1][0]; X, M, decl = T[i]; p = A / 'rows' / (a + '.cpp'); s = p.read_text(encoding='utf-8')
    ret = re.sub(r'^(public:|private:|protected:)?\s*virtual\s+', '', decl).split(M + '(')[0].strip()
    m = re.search(r"^([^\n;{}#/]*?)\b" + re.escape(X) + "::" + re.escape(M) + r"\s*\(", s, re.M)
    if not m: print(i, a, 'NODEF'); continue
    old = m.group(1).strip()
    s = s[:m.start()] + ret + " " + s[m.start() + len(m.group(1)):]
    if re.match(r'E[A-Z]\w+$', ret):
        k = s.find('{', m.start()); d = 1; j = k + 1
        while d: d += {'{': 1, '}': -1}.get(s[j], 0); j += 1
        body = re.sub(r'\breturn\s+(-?(?:0x[0-9a-fA-F]+|\d+))\s*;', r'return (%s)\1;' % ret, s[k:j])
        s = s[:k] + body + s[j:]
    p.write_text(s, encoding='utf-8'); print(i, a, repr(old), '->', ret)
