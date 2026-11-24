# chunkfix.py <idx..>: chunk-class alias drafts: FABLE_RETAIL_CCHUNKEDFILECHUNK beside the class switch (hoisted above the includes), the local
# CCharString model dropped (fable_string.h comes with CChunkedFileChunk.h), the definition's return type taken from the header declaration.
import re, sys
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF333')
T = {}
for l in open(A / 'work/triage.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) > 7 and p[5] == 'DECL': T.setdefault(int(p[0]), (p[2], p[3], p[7]))
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
for i in [int(x) for x in sys.argv[1:]]:
    a = E[i - 1][0]; X, M, decl = T[i]
    s = (A / 'work/att' / (a + '.cpp')).read_text(encoding='utf-8')
    s = re.sub(r"^class CCharString \{ public:\n(?:[^\n]*\n)*?\};\n", "", s, count=1, flags=re.M)
    ret = re.sub(r'^(public:|private:|protected:)?\s*virtual\s+', '', decl).split(M + '(')[0].strip()
    s = re.sub(r"^(?!//)[^\n;{}]*?\b" + re.escape(X) + "::" + re.escape(M) + r"\s*\(", ret + " " + X + "::" + M + "(", s, count=1, flags=re.M)
    if not re.search(r"^#define FABLE_RETAIL_CCHUNKEDFILECHUNK\b", s, re.M):
        s = re.sub(r"^(#define FABLE_RETAIL_\w+)", r"#define FABLE_RETAIL_CCHUNKEDFILECHUNK\n\1", s, count=1, flags=re.M)
    (A / 'work' / ('h_' + a + '.cpp')).write_text(s, encoding='utf-8')
    import subprocess; subprocess.run(['python3', str(A / 'tools/hoist.py'), str(A / 'work' / ('h_' + a + '.cpp')), str(A / 'rows' / (a + '.cpp'))])
    print(i, a, ret)
