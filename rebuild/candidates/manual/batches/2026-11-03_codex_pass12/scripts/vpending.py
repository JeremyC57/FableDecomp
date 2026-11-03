#!/usr/bin/env python3
# Copy each claimed row's source/test/naming evidence and its proven header overlay (the row's include closure as
# graded, every file hash-checked against the materialized context) into verified_pending/<addr>/.
# usage (repo root): python3 -I vpending.py <handoff> <codex12> <dest>
import sys, csv, json, shutil, hashlib
from pathlib import Path
X, C, D = Path(sys.argv[1]), Path(sys.argv[2]), Path(sys.argv[3])
EXTRA = ['NAMING_EVIDENCE.md', 'NAME_FIXES.tsv', 'RELOCATIONS.json', 'SELECTED_GRADE.json', 'PACKING_EVIDENCE.md',
         'HEADER_LAYOUT_EVIDENCE.md', 'PDB_SYMBOL_EVIDENCE.json', 'VENDOR_HANDOFF_NOTE.md', 'CHAR_ACCESSOR_IDENTITY.json',
         'VIRTUAL_IDENTITIES.json', 'CALL_SEQUENCE_AUDIT.json', 'LEAF_SYMBOL.txt']
MAINREV = {'00402510', '0051fd80', '00520d00', '00b2d9f0'}
det = {}
for r in csv.DictReader(open(C/'needs_detail.tsv'), delimiter='\t'): det.setdefault(r['address'], []).append((r['header'], r['state']))
sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()
allp = []; n = 0
for a in [l.strip() for l in open(C/'_r.txt') if l.strip()]:
    s, d = X/'rows'/a, D/a
    if d.exists(): shutil.rmtree(d)
    d.mkdir(parents=True)
    for f in [a+'.cpp', a+'_test.cpp'] + EXTRA + (['MAIN_REVIEW.json'] if a in MAINREV else []):
        if (s/f).exists(): shutil.copyfile(s/f, d/f)
    hs = open(d/'HEADER_STATES.tsv', 'w'); hs.write('header\tstate_vs_our_tree\tsha256\n')
    for h, st in sorted(det.get(a, [])):
        src = C/'hdr'/a/h; dst = d/'hdr'/h; dst.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(src, dst); assert sha(dst) == sha(C/'slim'/a/h); n += 1
        hs.write(f'{h}\t{st}\t{sha(dst)}\n')
    allp.append(json.loads((C/'payloads'/(a+'.json')).read_text())['result']['authored'][0])
(D/'payload.json').write_text(json.dumps({'result': {'authored': allp}}, indent=1))
print(len(allp), 'rows', n, 'header files')
