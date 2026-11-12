#!/usr/bin/env python3
"""addmatch.py <chunk-dir> <addr> '<module>' '<name: METHOD ONLY>' <src.cpp> <test.cpp> [--flags /d2inlS39 ...]
Add (or replace) a matched row in <chunk-dir>/payload.json and <chunk-dir>/oracle.tsv in the landing formats: address = 8 lowercase
hex, pass_pattern = <addr>_TEST PASS, oracle with its header row and trailing 0xCC/0x90 padding stripped (bytes from
$S/batch/*_oracle.tsv). Prints what it wrote. Always exits 0."""
import csv, glob, json, os, sys
try:
    a = sys.argv[1:]; flags = []
    if '--flags' in a:
        i = a.index('--flags'); flags = a[i + 1:]; a = a[:i]
    W, addr, mod, name, src, test = a[0], a[1].lower().replace('0x', '').zfill(8), a[2], a[3], a[4], a[5]
    if name.startswith(mod + '::'): name = name[len(mod) + 2:]
    csv.field_size_limit(10**8); retail = None
    for f in sorted(glob.glob(os.environ['S'] + '/batch/*_oracle.tsv'), key=os.path.getmtime, reverse=True):
        for r in csv.DictReader(open(f, encoding='utf-8-sig'), delimiter='\t'):
            if r['address'].lower().replace('0x', '') == addr: retail = bytes.fromhex(r['bytes']); break
        if retail: break
    if retail is None: print('no oracle bytes for', addr); sys.exit(0)
    while retail and retail[-1] in (0xCC, 0x90): retail = retail[:-1]
    pp = os.path.join(W, 'payload.json'); d = json.load(open(pp)) if os.path.exists(pp) else {'result': {'authored': []}}
    row = {'address': addr, 'module': mod, 'name': name, 'source_cpp': open(src, encoding='utf-8').read(), 'test_cpp': open(test, encoding='utf-8').read(), 'pass_pattern': addr + '_TEST PASS'}
    if flags: row['flags'] = flags
    d['result']['authored'] = [c for c in d['result']['authored'] if c['address'].lower().replace('0x', '') != addr] + [row]
    json.dump(d, open(pp, 'w'), indent=1)
    op = os.path.join(W, 'oracle.tsv'); rows = {}
    if os.path.exists(op):
        for l in open(op).read().splitlines()[1:]:
            c = l.split('\t')
            if len(c) >= 4: rows[c[0]] = l
    rows[addr] = '\t'.join([addr, f'{mod}::{name}', str(len(retail)), retail.hex()])
    open(op, 'w').write('address\tname\tlength\tbytes\n' + '\n'.join(rows[k] for k in sorted(rows)) + '\n')
    print('payload rows:', [c['address'] for c in d['result']['authored']], '| oracle', addr, len(retail), 'bytes')
except Exception as ex:
    print('addmatch.py could not write:', type(ex).__name__, str(ex)[:200])
sys.exit(0)
