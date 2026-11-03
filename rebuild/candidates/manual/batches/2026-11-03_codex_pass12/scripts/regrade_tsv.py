#!/usr/bin/env python3
# regrade_grades.tsv from the second read-only run (logs2). usage: python3 -I regrade_tsv.py <handoff> <codex12> <out.tsv>
import sys, csv, re
from pathlib import Path
X, C, out = Path(sys.argv[1]), Path(sys.argv[2]), sys.argv[3]
st = {r['address']: r for r in csv.DictReader(open(X/'STATUS.tsv'), delimiter='\t')}
o = open(out, 'w'); o.write('address\tname\tcodex_status\tour_grade\ttest\tnotes\n')
n = {}
for a in sorted(st):
    if st[a]['status'] == 'PARKED': continue
    log = (C/'logs2'/(a+'.log')).read_text()
    m = re.search(r'^%s +([A-Z_]+) +(PASS|FAIL|-)' % a, log, re.M)
    g, t = (m.group(1), m.group(2)) if m else ('NO_GRADE', '-')
    w = {}
    for k in re.findall(r'WARN (RELOC_[A-Z]+):', log): w[k] = w.get(k, 0)+1
    notes = []
    if w: notes.append('vland warnings ' + ', '.join(f'{k} {v}' for k, v in sorted(w.items())))
    for l in re.findall(r'WARN RELOC_CONFLICT: (.*)', log): notes.append('CONFLICT ' + l.strip())
    fl = st[a]['flags'].strip()
    if fl: notes.append('flags ' + fl)
    notes.append('own header context $S/codex12/hdr/' + a)
    o.write(f"{a}\t{st[a]['assigned_name']}\t{st[a]['status']}\t{g}\t{t}\t{'; '.join(notes)}\n")
    n[g+'/'+t] = n.get(g+'/'+t, 0)+1
print(n)
