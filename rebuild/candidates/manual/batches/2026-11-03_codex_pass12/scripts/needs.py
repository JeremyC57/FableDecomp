#!/usr/bin/env python3
# Header needs per row: transitive project-include closure of the row's source+test inside its own materialized
# context, each header compared three ways (row context / packaged base / our current rebuild/include).
# usage (repo root): python3 -I needs.py <handoff_root> <codex12_dir> <rows.tsv>  -> writes needs_detail.tsv, needs.json
import sys, re, json, csv, hashlib
from pathlib import Path
root, C, rows = Path(sys.argv[1]), Path(sys.argv[2]), sys.argv[3]
base = json.loads((root/'hdr/BASE_MANIFEST.json').read_text())['headers']
cur = Path('rebuild/include')
INC = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.M)
def sha(p): return hashlib.sha256(p.read_bytes()).hexdigest()
out = {}
det = open(C/'needs_detail.tsv', 'w'); det.write('address\theader\tstate\n')
for r in csv.DictReader(open(rows), delimiter='\t'):
    a = r['address']; H = C/'hdr'/a
    seen = {}; todo = []
    for f in (root/'rows'/a/(a+'.cpp'), root/'rows'/a/(a+'_test.cpp')):
        todo.append((f.read_text(errors='replace'), None))
    while todo:
        text, d = todo.pop()
        for inc in INC.findall(text):
            cands = ([d/inc] if d else []) + [H/inc]
            for p in cands:
                if p.is_file():
                    rel = str(p.resolve().relative_to(H.resolve()))
                    if rel not in seen:
                        seen[rel] = 1; todo.append((p.read_text(errors='replace'), p.parent))
                    break
    st = {}
    for rel in sorted(seen):
        o = sha(H/rel); c = cur/rel
        if not c.exists(): s = 'new'
        elif sha(c) == o: s = 'same'
        elif base.get(rel) == o: s = 'stale'          # row used the packaged version; our tree moved since
        elif rel not in base: s = 'both_new'          # file absent from the package base; we and Codex each created a version
        elif sha(c) == base[rel]: s = 'edit'          # Codex edited; our tree unchanged since packaging
        else: s = 'edit_conflict'                     # Codex edited and our tree moved too
        st[rel] = s; det.write(f'{a}\t{rel}\t{s}\n')
    out[a] = st
json.dump(out, open(C/'needs.json', 'w'), indent=1)
for a, st in out.items():
    c = {}
    for s in st.values(): c[s] = c.get(s, 0)+1
    print(a, len(st), c)
