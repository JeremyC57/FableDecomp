# st.py <addr> <status> <reason...>: set one STATUS.tsv line (address status pdb_spelling reason); keeps ENTRIES order
import sys
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF325')
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
P = A / 'STATUS.tsv'; cur = {}
if P.exists():
    for l in P.read_text().splitlines()[1:]:
        p = l.split('\t'); cur[p[0]] = p
a, s, r = sys.argv[1].lower(), sys.argv[2], ' '.join(sys.argv[3:])
pdb = {}
for e in E:
    if e[3] or e[0] not in pdb: pdb[e[0]] = e[3] or pdb.get(e[0], '')
cur[a] = [a, s, pdb.get(a, '-'), r]
order = list(dict.fromkeys(e[0] for e in E))   # NF325: an address listed twice in ENTRIES (0x00BBDDB0) gets one line
P.write_text('address\tstatus\tpdb_spelling\treason\n' + ''.join('\t'.join(cur[x]) + '\n' for x in order if x in cur) + ''.join('\t'.join(v) + '\n' for k, v in cur.items() if k not in order))
