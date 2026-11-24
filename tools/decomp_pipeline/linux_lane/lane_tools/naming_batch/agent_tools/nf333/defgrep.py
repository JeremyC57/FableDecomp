# defgrep.py: per ENTRIES line, other landed sources with a definition-shaped line of X::M( (not a call), and any // Leaf: naming the
# PDB spelling (another self claiming it). -> work/defgrep.tsv
import re, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF333')
R = Path('/home/user/FableDecomp/rebuild/src/compiled')
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
o = open(A / 'work/defgrep.tsv', 'w')
for i, e in enumerate(E, 1):
    m = re.match(r"\?([^@]+)@([^@]+)@@", e[3]); M, X = m.group(1), m.group(2)
    r = subprocess.run(['grep', '-rnE', r'^[A-Za-z_][^;=]*[^.>:\w]' + X + r'::' + M + r'\s*\(', str(R), '--include=*.cpp'], capture_output=True, text=True).stdout
    for l in r.splitlines():
        f = l.split(':')[0]
        if e[7] in f: continue
        print(i, e[0], 'DEFLINE', f.replace(str(R) + '/', ''), l.split(':', 2)[2].strip()[:120], sep='\t', file=o)
    r = subprocess.run(['grep', '-rlF', '// Leaf: ' + e[3], str(R), '--include=*.cpp'], capture_output=True, text=True).stdout
    for f in r.split():
        if e[7] not in f: print(i, e[0], 'LEAF', f.replace(str(R) + '/', ''), '', sep='\t', file=o)
o.close()
