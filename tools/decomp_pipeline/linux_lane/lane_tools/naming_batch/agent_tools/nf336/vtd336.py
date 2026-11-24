#!/usr/bin/env python3
"""vtd336.py <vtable va> <nslots>: retail vtable slot targets with the catalog name (auto-re-candidates.tsv) of each target; screen only"""
import sys, struct, os, csv
csv.field_size_limit(10**9)
d = open(os.environ['FABLE_EXE'], 'rb').read()
pe = struct.unpack_from('<I', d, 0x3C)[0]; ns = struct.unpack_from('<H', d, pe + 6)[0]; osz = struct.unpack_from('<H', d, pe + 20)[0]
base = struct.unpack_from('<I', d, pe + 52)[0]; so = pe + 24 + osz; secs = []
for i in range(ns):
    va, vs, raw = struct.unpack_from('<I', d, so + i * 40 + 12)[0], struct.unpack_from('<I', d, so + i * 40 + 8)[0], struct.unpack_from('<I', d, so + i * 40 + 20)[0]
    secs.append((base + va, vs, raw))
def rd(va):
    for s, n, r in secs:
        if s <= va < s + n: return struct.unpack_from('<I', d, r + va - s)[0]
names = {}
for r in csv.DictReader(open('/home/user/FableDecomp/rebuild/oracles/auto-re-candidates.tsv', encoding='utf-8-sig'), delimiter='\t'):
    names[r['address'].lower().replace('0x','')] = r.get('name','')
vt = int(sys.argv[1], 16)
for s in range(int(sys.argv[2])):
    t = rd(vt + 4 * s); print('slot', s, '%08X' % t, names.get('%08x' % t, '-'))
