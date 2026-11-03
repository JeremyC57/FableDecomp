#!/usr/bin/env python3
# Rebuild one verified row's header overlay (headers that differ from rebuild/include at regrade time, 2026-11-03).
# usage: python3 restore_overlay.py <addr> <empty outdir>; grade with FABLE_HDR_OVERLAY=<outdir> over the current tree.
import sys, os, json, hashlib
here = os.path.dirname(os.path.abspath(__file__)); a, out = sys.argv[1], sys.argv[2]
for rel, s in json.load(open(f'{here}/{a}/hdr_manifest.json')).items():
    assert not os.path.isabs(rel) and '..' not in rel.split('/')
    data = open(f'{here}/objects/{s}.h', 'rb').read(); assert hashlib.sha256(data).hexdigest() == s
    p = os.path.join(out, rel); os.makedirs(os.path.dirname(p), exist_ok=True); open(p, 'wb').write(data)
