#!/usr/bin/env python3
# Annotate header_detail.tsv: additive-only edits vs our header, and vacuous default checks (#error-guarded retail headers).
# usage (repo root): python3 -I hdrpost.py <codex12> <in_detail> <out_detail> <in_needs> <out_needs>
import sys, csv, difflib
from pathlib import Path
C = Path(sys.argv[1])
rows = list(csv.DictReader(open(sys.argv[2]), delimiter='\t'))
add = {}
for r in rows:
    ours = Path('rebuild/include')/r['header']; cx = C/'hdr'/r['address']/r['header']
    d = list(difflib.unified_diff(ours.read_text(errors='replace').splitlines(), cx.read_text(errors='replace').splitlines(), n=0))
    minus = sum(1 for l in d if l.startswith('-') and not l.startswith('---'))
    plus = sum(1 for l in d if l.startswith('+') and not l.startswith('+++'))
    r['lines_removed_vs_ours'] = str(minus); r['lines_added_vs_ours'] = str(plus)
    v = r['verdict']
    if 'same[0](cpp-err)' in r['vs_ours_default']: v += ' (default is an #error guard: compared under the class macro, see hclog)'
    if minus == 0 and not r['new_feature_macros'] and v.startswith('default_changed'): v = 'additive_decls_on_ours'
    r['verdict'] = v; add[(r['address'], r['header'])] = v
cols = list(rows[0].keys())
w = csv.DictWriter(open(sys.argv[3], 'w'), cols, delimiter='\t', lineterminator='\n'); w.writeheader(); w.writerows(rows)
needs = list(csv.DictReader(open(sys.argv[4]), delimiter='\t'))
for n in needs:
    if n['kind'] != 'C_modifies': continue
    vs = [add[(n['address'], h)] for h in n['modified'].split()]
    if all(v == 'additive_decls_on_ours' for v in vs): n['shape'] = 'C0_additive_only'
w = csv.DictWriter(open(sys.argv[5], 'w'), list(needs[0].keys()), delimiter='\t', lineterminator='\n'); w.writeheader(); w.writerows(needs)
