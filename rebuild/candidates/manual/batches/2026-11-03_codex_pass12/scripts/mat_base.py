#!/usr/bin/env python3
# Materialize the packaged base tree alone (hash-checked). usage: python3 -I mat_base.py <handoff> <outdir>
import sys, json, hashlib
from pathlib import Path, PurePosixPath
X, out = Path(sys.argv[1]), Path(sys.argv[2])
b = json.loads((X/'hdr/BASE_MANIFEST.json').read_text())['headers']
for n, h in sorted(b.items()):
    p = PurePosixPath(n); assert not p.is_absolute() and '..' not in p.parts and '\\' not in n and ':' not in n
    d = (X/'hdr/objects'/(h+'.h')).read_bytes(); assert hashlib.sha256(d).hexdigest() == h
    t = out/Path(*p.parts); t.parent.mkdir(parents=True, exist_ok=True); t.write_bytes(d)
print(len(b))
