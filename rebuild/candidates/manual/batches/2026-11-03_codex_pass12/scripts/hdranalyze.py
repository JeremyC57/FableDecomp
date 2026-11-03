#!/usr/bin/env python3
# Per-row header needs in the pass 11 shape plus the owner-rule verdict for every edited existing header.
# usage (repo root): python3 -I hdranalyze.py <codex12> <out_row_needs.tsv> <out_header_detail.tsv>
import sys, csv, re, hashlib, subprocess, collections
from pathlib import Path
C = Path(sys.argv[1]); cur = Path('rebuild/include'); base = C/'basetree/rebuild/include'
rows = [r['address'] for r in csv.DictReader(open(C/'rows.tsv'), delimiter='\t')]
det = collections.defaultdict(dict)
for r in csv.DictReader(open(C/'needs_detail.tsv'), delimiter='\t'): det[r['address']][r['header']] = r['state']
hcl = {}
for a in rows:
    f = C/'hclog'/(a+'.txt')
    if f.exists():
        for l in f.read_text().splitlines():
            p = l.split(' ', 2)
            if p[0] == 'CHANGED': hcl[(a, p[1])] = p[2]
sha = lambda p: hashlib.sha256(p.read_bytes()).hexdigest()
MAC = re.compile(r'FABLE_RETAIL_\w+')
CLS = re.compile(r'^\s*(?:class|struct)\s+(?:__declspec\([^)]*\)\s*)?(\w+)', re.M)
norm = lambda s: re.sub(r'[^A-Z0-9]', '', s.upper())
versions = collections.defaultdict(set); cache = {}
for a in rows:
    for h, s in det[a].items():
        if s not in ('same', 'new'): versions[h].add(sha(C/'hdr'/a/h))
dt = open(sys.argv[3], 'w')
dt.write('address\theader\tstate\tversions_across_rows\tvs_ours_default\tvs_base_default\tnew_macros\tnew_feature_macros\tverdict\n')
out = open(sys.argv[2], 'w')
out.write('address\tkind\tn_modified\tn_new\tmodified\tnew\tshape\tdefault_vs_ours_diff\tfeature_macros_defined\n')
rowmac = {r['address']: r for r in csv.DictReader(open(C/'row_macros.tsv'), delimiter='\t')}
for a in rows:
    mod = sorted(h for h, s in det[a].items() if s not in ('same', 'new'))
    new = sorted(h for h, s in det[a].items() if s == 'new')
    kind = 'C_modifies' if mod else ('B_newonly' if new else 'A_none')
    feats = set(); ddiff = []; shapes = set()
    for h in mod:
        cx = C/'hdr'/a/h; o = cur/h; b = base/h; s = det[a][h]
        vo = hcl.get((a, h), '?').split(' ')[0]
        key = (h, sha(cx))
        if key not in cache:
            vb = '-'
            if b.exists():
                r = subprocess.run(['python3', '-I', str(C/'scripts/hdrpair.py'), str(b), str(cx), str(C/'hdr'/a), str(C/'hctool/stub2')], capture_output=True, text=True)
                vb = (r.stdout.split(' ') or ['?'])[0].strip()
            txt = cx.read_text(errors='replace'); ours = o.read_text(errors='replace')
            nm = sorted(set(MAC.findall(txt)) - set(MAC.findall(ours)))
            classes = {norm(c) for c in CLS.findall(txt)} | {norm(Path(h).stem)}
            fm = [m for m in nm if norm(m[len('FABLE_RETAIL_'):]) not in classes]
            cache[key] = (vb, nm, fm)
        vb, nm, fm = cache[key]
        if vo.startswith('default=same'):
            v = 'branch_only_class' if not fm else 'branch_only_feature_macros'
        elif vb.startswith('default=same'):
            v = 'stale_cut_rebase' + ('' if not fm else '+feature_macros')
        else:
            v = 'default_changed' + ('_both_new' if s == 'both_new' else '') + ('' if not fm else '+feature_macros')
        if not vo.startswith('default=same'): ddiff.append(h)
        shapes.add(v.split('+')[0]); feats |= set(fm)
        dt.write(f'{a}\t{h}\t{s}\t{len(versions[h])}\t{vo}\t{vb}\t{" ".join(nm)}\t{" ".join(fm)}\t{v}\n')
    dm = set(rowmac[a]['defined_macros'].split()) if a in rowmac else set()
    dfeat = sorted(m for m in (set(rowmac[a]['new_feature_macros'].split()) if a in rowmac else set()))
    if kind != 'C_modifies': shape = '-'
    elif not ddiff and not dfeat and shapes <= {'branch_only_class'}: shape = 'C1_class_branches_default_same'
    else: shape = 'C2_needs_consolidation'
    out.write(f'{a}\t{kind}\t{len(mod)}\t{len(new)}\t{" ".join(mod)}\t{" ".join(new)}\t{shape}\t{len(ddiff)}\t{" ".join(dfeat)}\n')
print('ok')
