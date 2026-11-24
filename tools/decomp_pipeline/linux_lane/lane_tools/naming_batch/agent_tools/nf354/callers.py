#!/usr/bin/env python3
"""callers.py <target-hex>: landed rows (build_candidates.ps1 entries) whose retail body (auto-re-candidates bytes) holds an E8/E9 rel32 to
<target>. Prints address, call count, source path. Agent NF354 census helper; reads only."""
import csv, re, struct, sys
R = '/home/user/FableDecomp'
t = int(sys.argv[1], 16)
PS = open(R + '/rebuild/build_candidates.ps1', encoding='utf-8').read()
src = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", PS, re.S):
    a = re.search(r"\bAddress = '([0-9A-Fa-f]{8})'", m.group(1)); s = re.search(r"\bSource = '([^']*)'", m.group(1))
    if a: src.setdefault(a.group(1).lower(), s.group(1) if s else '-')
csv.field_size_limit(10**9)
for r in csv.DictReader(open(R + '/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8-sig'), delimiter='\t'):
    a = r['address'].lower().replace('0x', '')
    if a not in src: continue
    b = bytes.fromhex(r['bytes']); va = int(a, 16); n = 0
    for i in range(len(b) - 4):
        if b[i] in (0xE8, 0xE9) and ((va + i + 5 + struct.unpack_from('<i', b, i + 1)[0]) & 0xffffffff) == t: n += 1
    if n: print('%s\t%d\t%s' % (a, n, src[a]))
