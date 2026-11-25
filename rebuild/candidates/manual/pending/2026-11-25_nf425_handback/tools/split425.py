#!/usr/bin/env python3
"""split425.py <batch> <hold batch> <reason file: address<TAB>reason>: move the listed rows of <batch> into <hold batch> (rows, tests,
orig copies, selfmap / expect lines, evidence summaries, head payload entries); the hold folder keeps hold_reasons.tsv."""
import sys, os, shutil, json, csv
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
b, h, rf = sys.argv[1:4]; B = A + '/batches/' + b; H = A + '/batches/' + h
why = {l.split('\t')[0]: l.rstrip('\n').split('\t')[1] for l in open(rf) if l.strip()}
rows = open(B + '/rows.txt').read().split(); mv = [a for a in rows if a in why]; keep = [a for a in rows if a not in why]
if not mv: print(b, 'nothing to move'); sys.exit()
for d in ('rows', 'tests', 'orig/rows', 'orig/tests', 'evidence', 'head'): os.makedirs(H + '/' + d, exist_ok=True)
for a in mv:
    for s, d in (('rows/%s.cpp', 'rows/%s.cpp'), ('tests/%s_test.cpp', 'tests/%s_test.cpp'), ('orig/rows/%s.cpp', 'orig/rows/%s.cpp'), ('orig/tests/%s_test.cpp', 'orig/tests/%s_test.cpp')):
        if os.path.exists(B + '/' + s % a): shutil.move(B + '/' + s % a, H + '/' + d % a)
def split_tsv(name):
    if not os.path.exists(B + '/' + name): return
    L_ = open(B + '/' + name).read().splitlines(); hdr = [l for l in L_ if l.startswith('address\t')]
    k = [l for l in L_ if l and l.split('\t')[0] not in mv]; m = [l for l in L_ if l and l.split('\t')[0] in mv]
    open(B + '/' + name, 'w').write('\n'.join(k) + '\n')
    p = H + '/' + name; old = open(p).read().splitlines() if os.path.exists(p) else []
    if hdr and not old: old = hdr
    open(p, 'w').write('\n'.join(old + [x for x in m if x not in hdr]) + '\n')
for n in ('selfmap.tsv', 'expect.tsv', 'evidence/objgate_summary.tsv', 'evidence/pk_summary.txt', 'evidence/census.tsv'): split_tsv(n)
if os.path.exists(B + '/head/payload.json'):
    p = json.load(open(B + '/head/payload.json')); q = json.load(open(H + '/head/payload.json')) if os.path.exists(H + '/head/payload.json') else {'result': {'authored': []}}
    q['result']['authored'] += [x for x in p['result']['authored'] if x['address'] in mv]
    p['result']['authored'] = [x for x in p['result']['authored'] if x['address'] not in mv]
    json.dump(p, open(B + '/head/payload.json', 'w')); json.dump(q, open(H + '/head/payload.json', 'w'))
open(B + '/rows.txt', 'w').write('\n'.join(keep) + '\n')
hr = open(H + '/rows.txt').read().split() if os.path.exists(H + '/rows.txt') else []
open(H + '/rows.txt', 'w').write('\n'.join(hr + mv) + '\n')
with open(H + '/hold_reasons.tsv', 'a') as f:
    for a in mv: f.write('%s\t%s\t%s\n' % (a, b, why[a]))
print(b, 'kept', len(keep), 'moved', len(mv), '->', h)
