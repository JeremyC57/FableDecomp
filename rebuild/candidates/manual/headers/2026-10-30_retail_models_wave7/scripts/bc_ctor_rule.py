#!/usr/bin/env python3
"""bc_ctor_rule.py <clusters.tsv> <cluster>...: per row, does retail call CBaseClass() 0x0099A2D0 (e8 rel32), store the
CBaseClass vtable 0x01231710 as an immediate, and does the row source construct through `CBaseClass()` / copy?"""
import sys, re, os, struct
R = os.environ["FABLE_ROOT"]
cat = {}
for l in open(R + "/rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q = l.split("\t"); cat[q[0].lower()] = q
want = set(sys.argv[2:])
from collections import Counter
tot = Counter()
for l in open(sys.argv[1]):
    k, f = l.split()
    if k not in want: continue
    a = re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1)
    if a not in cat: continue
    b = bytes.fromhex(cat[a][3]); va = int(a, 16)
    call = any(b[i] == 0xE8 and (va + i + 5 + struct.unpack_from("<i", b, i + 1)[0]) & 0xFFFFFFFF == 0x0099A2D0 for i in range(len(b) - 4))
    store = struct.pack("<I", 0x01231710) in b
    tot[(k, call, store)] += 1
    if len(sys.argv) > 2 and os.environ.get("V"): print(k, a, call, store, f)
for k, v in sorted(tot.items()): print(v, k)
