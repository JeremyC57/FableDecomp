#!/usr/bin/env python3
"""srcof.py <addr>...: print address, Source, TestSource, CompilerFlags from the catalog block of each address (bounded to its own block)."""
import re, sys
txt = open('/home/user/FableDecomp/rebuild/build_candidates.ps1', encoding='utf-8', errors='replace').read()
blocks = re.split(r'\[pscustomobject\]@\{', txt)
idx = {}
for b in blocks:
    m = re.search(r"Address\s*=\s*'([0-9a-fA-Fx]+)'", b)
    if m: idx.setdefault(m.group(1).lower().replace('0x',''), []).append(b)
for a in sys.argv[1:]:
    a = a.lower().replace('0x','')
    for b in idx.get(a, []):
        g = lambda k: (re.search(k + r"\s*=\s*'([^']*)'", b) or [None, '-'])[1]
        print('\t'.join([a, g('Source'), g('TestSource'), g('CompilerFlags'), g('Name'), g('Module')]))
    if a not in idx: print(a, 'NOT_IN_CATALOG')
