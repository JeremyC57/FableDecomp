#!/usr/bin/env python3
# Rebuild one pass-13 row's exact header context from the archive's hash store (data only; every object hash-checked).
# usage: python3 -I restore.py <handoff_root> <addr> <outdir>
import sys, json, hashlib
from pathlib import Path, PurePosixPath
root, addr, out = Path(sys.argv[1]), sys.argv[2], Path(sys.argv[3])
def safe(n):
    p = PurePosixPath(n)
    if p.is_absolute() or '..' in p.parts or '\\' in n or ':' in n: raise SystemExit('unsafe path ' + n)
    return Path(*p.parts)
def load(rel): return json.loads((root / safe(rel)).read_text())
m = load(f'rows/{addr}/HEADER_MANIFEST.json')
paths = dict(load(m['base_manifest']))
ov = load(m['overlay_manifest'])
if ov.get('schema') == 2:
    ref = dict(load(ov['reference_manifest']))
    for k in ov['removed']: ref.pop(k, None)
    ref.update(ov['overrides']); ov = ref
paths.update(ov)
if out.exists() and any(out.iterdir()): raise SystemExit('output not empty')
for n, h in sorted(paths.items()):
    assert len(h) == 64 and all(c in '0123456789abcdef' for c in h), n
    data = (root / 'hdr/objects' / (h + '.h')).read_bytes()
    if hashlib.sha256(data).hexdigest() != h: raise SystemExit('corrupt object ' + h)
    t = out / safe(n); t.parent.mkdir(parents=True, exist_ok=True); t.write_bytes(data)
row = load(f'rows/{addr}/ROW.json')
src = (root / 'rows' / addr / (addr + '.cpp')).read_bytes()
note = 'ok' if hashlib.sha256(src).hexdigest() == row['source_sha256'] else 'SOURCE_SHA_MISMATCH'
print(addr, len(paths), note)
