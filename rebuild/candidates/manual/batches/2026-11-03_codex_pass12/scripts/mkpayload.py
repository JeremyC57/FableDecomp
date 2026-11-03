#!/usr/bin/env python3
# usage: python3 -I mkpayload.py <handoff_root> <rows.tsv> <outdir>
import sys, json, csv
from pathlib import Path
root, rows, out = Path(sys.argv[1]), sys.argv[2], Path(sys.argv[3]); out.mkdir(exist_ok=True)
allp = []
for r in csv.DictReader(open(rows), delimiter='\t'):
    a = r['address']; d = root/'rows'/a
    sel = json.loads((d/'SELECTED_GRADE.json').read_text())
    c = {"address": a, "module": "_global", "name": a,
         "source_cpp": (d/(a+'.cpp')).read_text(encoding='utf-8'),
         "test_cpp": (d/(a+'_test.cpp')).read_text(encoding='utf-8'),
         "pass_pattern": a+"_TEST PASS", "leaf": sel['leaf']}
    if sel.get('flags'): c['flags'] = list(sel['flags'])
    allp.append(c)
    (out/(a+'.json')).write_text(json.dumps({"result": {"authored": [c]}}, indent=1))
(out/'ALL.json').write_text(json.dumps({"result": {"authored": allp}}, indent=1))
print(len(allp))
