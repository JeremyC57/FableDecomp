#!/usr/bin/env python3
# usage: python3 -I mkpayload.py <handoff_root> <outdir>   (VERIFIED_MATCH rows only)
import sys, json, csv
from pathlib import Path
root, out = Path(sys.argv[1]), Path(sys.argv[2]); (out / 'payloads').mkdir(parents=True, exist_ok=True)
rows = [r for r in csv.DictReader(open(root / 'STATUS.tsv'), delimiter='\t') if r['status'] == 'VERIFIED_MATCH']
with open(out / 'rows.tsv', 'w') as f:
    f.write('address\tmodule\tname\tlength\n')
    for r in rows:
        a = r['address']; d = root / 'rows' / a; j = json.loads((d / 'ROW.json').read_text())
        c = {"address": a, "module": "_global", "name": a,
             "source_cpp": (d / (a + '.cpp')).read_text(encoding='utf-8'),
             "test_cpp": (d / (a + '_test.cpp')).read_text(encoding='utf-8'),
             "pass_pattern": a + "_TEST PASS", "leaf": j['leaf']}
        if j.get('flags'): c['flags'] = list(j['flags'])
        (out / 'payloads' / (a + '.json')).write_text(json.dumps({"result": {"authored": [c]}}, indent=1))
        f.write(f"{a}\t_global\t{a}\t{j['graded_retail_size']}\n")
print(len(rows))
