# tidy.py <addr...>: alias-route rows: drop an emptied alias class used nowhere else, reword the script's "alias route" #define comment,
# drop the stale "Stand-in (owner rule 2026-11-05)" line, and add one NF323 provenance line after the Leaf (old -> new spelling).
import re, sys
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF334')
E = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')}
for a in sys.argv[1:]:
    p = A / 'rows' / (a + '.cpp'); s = p.read_text(encoding='utf-8'); e = E[a]
    m = re.match(r"\?([^@]+)@([^@]+)@@", e[3]); X = m.group(2)
    for mm in list(re.finditer(r"\n?^[ \t]*(?:struct|class)\s+(\w+)\s*:\s*(?:public\s+)?" + re.escape(X) + r"\b[^{;]*\{\s*\};[ \t]*\n?", s, re.M)):
        al = mm.group(1)
        if len(re.findall(r"\b" + al + r"\b", s)) == 1: s = s.replace(mm.group(0), '\n', 1)
    s = re.sub(r"[ \t]+// alias route: engine/\S+ declares \w+ under this class-model switch", "", s)
    s = re.sub(r"^// Stand-in \(owner rule 2026-11-05\): the real method on a stand-in class named after the real class[^\n]*\n?", "", s, flags=re.M)
    T = {}
    for l in open(A / 'work/triage.tsv'):
        q = l.rstrip('\n').split('\t')
        if len(q) > 6 and q[5] == 'DECL': T.setdefault(q[1], re.findall(r'#ifdef (\w+)', q[6]))
    sw = T.get(a, [])
    note = ("// Switch route (NF323, 2026-11-23): defined on the real class of engine/%s.h (class-model switch %s; was %s);\n"
            "//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.\n") % (X, ' + '.join(sw), e[4])
    s = re.sub(r"^// Switch route \(NF323[^\n]*\n//   the retail RTTI vftable placement[^\n]*\n", "", s, flags=re.M)
    L = s.split('\n'); i = next((k + 1 for k, l in enumerate(L) if l.startswith('// Leaf:')), 0)
    L.insert(i, note.rstrip('\n')); s = '\n'.join(L)
    s = re.sub(r"\n{3,}", "\n\n", s)
    p.write_text(s, encoding='utf-8'); print(a, 'tidied')
