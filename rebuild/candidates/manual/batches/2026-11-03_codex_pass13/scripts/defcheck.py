#!/usr/bin/env python3
# Default-path check of every distinct changed pass-13 header version: row version vs package base vs our tree.
# Preprocesses with system headers stubbed (like hdrcheck2) and no FABLE_RETAIL_ macro defined.
import os, sys, re, json, csv, hashlib, subprocess, collections, tempfile
C, R = sys.argv[1], sys.argv[2]
stub = 'tools/decomp_pipeline/linux_lane/lane_tools/coordinator/stubinc'
base = json.load(open(f'{R}/hdr/BASE_MANIFEST.json'))
def obj(h): return f'{R}/hdr/objects/{h}.h'
def pp(path, incdir):
    args = ['cpp', '-P', '-nostdinc', '-undef', '-D_MSC_VER=1310', '-D_M_IX86=600', '-D_WIN32', '-I', stub, '-I', incdir, path]
    for _ in range(200):
        r = subprocess.run(args, capture_output=True, text=True)
        m = re.search(r'fatal error: ([^:]+): No such file', r.stderr)
        if not m: break
        p = os.path.join(stub, m.group(1)); os.makedirs(os.path.dirname(p), exist_ok=True); open(p, 'w').close()
    return [re.sub(r'fable_static_assert_\d+', 'SA', l) for l in r.stdout.splitlines() if l.strip()], r.returncode
seen = {}; rows_of = collections.defaultdict(list)
for r in csv.DictReader(open(f'{C}/hdr_pairs.tsv'), delimiter='\t'):
    if r['kind'] != 'changed': continue
    p = f"{C}/hdr/{r['address']}/{r['header']}"; h = hashlib.sha256(open(p, 'rb').read()).hexdigest()
    rows_of[(r['header'], h)].append(r['address']); seen[(r['header'], h)] = r['address']
out = open(f'{C}/defcheck.tsv', 'w'); out.write('header\tversion\trows\trow_vs_base\tbase_vs_ours\trow_vs_ours\tnew_macros\n')
cnt = collections.Counter()
for (rel, h), a in sorted(seen.items()):
    inc = f'{C}/hdr/{a}'  # resolve includes in the row's own context
    rv, e1 = pp(f'{inc}/{rel}', inc)
    ov, e2 = pp(f'rebuild/include/{rel}', 'rebuild/include')
    if rel in base:
        bv, e3 = pp(obj(base[rel]), inc); rb = 'same' if rv == bv else 'DIFF'; bo = 'same' if bv == ov else 'moved'
    else: rb, bo = 'nobase', '-'
    ro = 'same' if rv == ov else 'DIFF'
    mine = set(re.findall(r'FABLE_RETAIL_\w+', open(f'rebuild/include/{rel}', errors='replace').read()))
    theirs = set(re.findall(r'FABLE_RETAIL_\w+', open(f'{inc}/{rel}', errors='replace').read()))
    nm = ' '.join(sorted(m[13:] for m in theirs - mine))
    cnt[(rb, bo)] += 1
    out.write(f"{rel}\t{h[:12]}\t{' '.join(rows_of[(rel, h)])}\t{rb}\t{bo}\t{ro}\t{nm}\n")
for k, v in sorted(cnt.items()): print(k, v)
