#!/usr/bin/env python3
# Recheck the earlier (killed) agent's scratch: payload sources/tests/leaf/flags vs the handoff row files and SELECTED_GRADE;
# oracle lengths vs STATUS retail_size and catalog; codex STATUS rows. usage: python3 -I check_prior.py <handoff> <codex12>
import sys, json, csv, hashlib
from pathlib import Path
X, C = Path(sys.argv[1]), Path(sys.argv[2])
st = {r['address']: r for r in csv.DictReader(open(X/'STATUS.tsv'), delimiter='\t')}
orc = {r['address']: r for r in csv.DictReader(open(C/'oracle.tsv'), delimiter='\t')}
rows = [r for r in csv.DictReader(open(C/'rows.tsv'), delimiter='\t')]
claimed = sorted(a for a, r in st.items() if r['status'] != 'PARKED')
print('claimed', len(claimed), 'rows.tsv', len(rows), 'same set', sorted(r['address'] for r in rows) == claimed)
for r in rows:
    a = r['address']; d = X/'rows'/a; sel = json.loads((d/'SELECTED_GRADE.json').read_text())
    p = json.loads((C/'payloads'/(a+'.json')).read_text())['result']['authored'][0]
    bad = []
    if p['source_cpp'].encode() != (d/(a+'.cpp')).read_bytes(): bad.append('SRC')
    if hashlib.sha256((d/(a+'.cpp')).read_bytes()).hexdigest() != sel['source_sha256']: bad.append('SRCSHA')
    if p['test_cpp'].encode() != (d/(a+'_test.cpp')).read_bytes(): bad.append('TEST')
    if p['leaf'] != sel['leaf']: bad.append('LEAF')
    if st[a]['assigned_name'] != sel['leaf']: bad.append('STATUSLEAF')
    if list(sel.get('flags') or []) != list(p.get('flags') or []): bad.append('FLAGS')
    if int(orc[a]['length']) != int(st[a]['retail_size']) or int(r['length']) != int(st[a]['retail_size']): bad.append('LEN')
    if sel['status'] != st[a]['status']: bad.append('SELSTATUS')
    print(a, st[a]['status'], r['length'], sel.get('flags'), 'static_assert' in p['test_cpp'], ' '.join(bad) or 'ok')
