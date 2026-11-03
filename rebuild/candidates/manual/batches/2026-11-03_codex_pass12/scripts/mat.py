#!/usr/bin/env python3
# Rebuild a pass-12 row's header context (base + selected overlay) with every object hash-checked.
# usage: python3 -I mat.py <handoff_root> <addr> <outdir>
import sys, json, hashlib, os
from pathlib import Path, PurePosixPath
root, addr, out = Path(sys.argv[1]), sys.argv[2], Path(sys.argv[3])
def safe(n):
    p = PurePosixPath(n)
    if p.is_absolute() or '..' in p.parts or '\\' in n or ':' in n: raise SystemExit('unsafe path '+n)
    return Path(*p.parts)
def sha(b): return hashlib.sha256(b).hexdigest()
base = json.loads((root/'hdr/BASE_MANIFEST.json').read_text())
row = json.loads((root/'rows'/addr/'HEADER_MANIFEST.json').read_text())
sel = json.loads((root/'rows'/addr/'SELECTED_GRADE.json').read_text())
# the selected grade's manifest must carry the same header map as the row copy
hist = json.loads((root/safe(sel['header_manifest'])).read_text())
notes = []
if hist['headers'] != row['headers']: notes.append('ROW_MANIFEST_NE_SELECTED')
canon = sha(json.dumps(row['headers'], sort_keys=True, separators=(',', ':')).encode())
if canon != sel.get('header_map_sha256'): notes.append('MAPSHA?')
src = (root/'rows'/addr/(addr+'.cpp')).read_bytes()
if sha(src) != sel['source_sha256']: notes.append('SOURCE_SHA_MISMATCH')
if row.get('source_sha256') and row['source_sha256'] != sel['source_sha256']: notes.append('ROWMAN_SOURCE_SHA_NE')
files = dict(base['headers']); files.update(row['headers'])
for n, h in files.items():
    safe(n)
    assert len(h) == 64 and all(c in '0123456789abcdef' for c in h), n
    if sha((root/'hdr/objects'/(h+'.h')).read_bytes()) != h: raise SystemExit('corrupt object '+h)
if out.exists() and any(out.iterdir()): raise SystemExit('output not empty')
out.mkdir(parents=True, exist_ok=True)
for n, h in sorted(files.items()):
    t = out/safe(n); t.parent.mkdir(parents=True, exist_ok=True)
    t.write_bytes((root/'hdr/objects'/(h+'.h')).read_bytes())
print(addr, len(files), len(row['headers']), ' '.join(notes) or 'ok')
