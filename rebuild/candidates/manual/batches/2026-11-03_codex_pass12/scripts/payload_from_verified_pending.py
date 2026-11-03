#!/usr/bin/env python3
# Build a vland payload for pass-12 verified rows from verified_pending/<addr>/ (no zip needed).
# usage (repo root): python3 -I payload_from_verified_pending.py <verified_pending> <out.json> <addr> [<addr> ...]
# Grade read-only: FABLE_HDR_OVERLAY=<verified_pending>/<addr>/hdr LANDVERIFY_WORK=<dir> python3 -u $T/vland.py <out.json> <oracle>
# (oracle from $T/chunk_pool/oracle_from_exe.py, scratch only). Grade one row per call: each row has its own overlay.
import sys, json
from pathlib import Path
V, out, addrs = Path(sys.argv[1]), sys.argv[2], sys.argv[3:]
rows = []
for a in addrs:
    d = V/a; sel = json.loads((d/'SELECTED_GRADE.json').read_text())
    c = {'address': a, 'module': '_global', 'name': a, 'source_cpp': (d/(a+'.cpp')).read_text(encoding='utf-8'),
         'test_cpp': (d/(a+'_test.cpp')).read_text(encoding='utf-8'), 'pass_pattern': a+'_TEST PASS', 'leaf': sel['leaf']}
    if sel.get('flags'): c['flags'] = list(sel['flags'])
    rows.append(c)
Path(out).write_text(json.dumps({'result': {'authored': rows}}, indent=1)); print(len(rows))
