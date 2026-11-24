#!/usr/bin/env python3
"""pdbfields.py (NF337) <types.txt> <out> <class>...: stream an existing `llvm-pdbutil dump -types` text (the PDB TPI stream) once; keep every LF_FIELDLIST's LF_MEMBER lines in memory
only by type index, then print the members of each named non-forward-ref LF_CLASS / LF_STRUCTURE (type records = the PDB's TPI)."""
import subprocess, sys, re
pdb, out, want = sys.argv[1], sys.argv[2], set(sys.argv[3:])
p = open(pdb, errors="replace")
fl = {}; cls = []; cur = None; kind = None; curname = None
for l in p:
    m = re.match(r'\s*(0x[0-9A-F]+) \| (LF_\w+) .*?(?:`(.*)`)?\s*$', l)
    if m:
        cur, kind, curname = m.group(1), m.group(2), m.group(3)
        if kind == 'LF_FIELDLIST': fl[cur] = []
        continue
    if kind == 'LF_FIELDLIST' and 'LF_MEMBER' in l: fl[cur].append(l.strip())
    if kind in ('LF_CLASS', 'LF_STRUCTURE') and curname in want:
        f = re.search(r'field list: (0x[0-9A-F]+)', l)
        if f and 'forward ref' not in l: cls.append((curname, cur, f.group(1)))
        if 'options:' in l and 'forward ref' in l and cls and cls[-1][1] == cur: cls.pop()
p.close()
with open(out, 'w') as o:
    for n, ti, f in cls:
        o.write(f'## {n} {ti} field list {f}\n')
        for x in fl.get(f, []): o.write('  ' + x + '\n')
print('classes', len(cls))
