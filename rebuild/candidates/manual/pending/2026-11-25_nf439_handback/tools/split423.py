#!/usr/bin/env python3
"""split423.py <batch>: after finalize423, move the HOLD rows of <batch> into <batch>_hold (rows/, orig/, RESULTS/CHECKS/UNITS/renames,
rows.txt; no TABLE_CHANGES) and leave only ACCEPT rows in <batch>. Re-run finalize423 on <batch> afterwards (its table census must
not count HOLD rows as moved)."""
import sys, os, csv, shutil
W = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF439'
b = sys.argv[1]; B = W + '/batches/' + b; H = B + '_hold'
res = list(csv.reader(open(B + '/RESULTS.tsv'), delimiter='\t')); hold = {r[0] for r in res[1:] if r[1] != 'ACCEPT'}
if not hold: print(b, 'no HOLD'); sys.exit()
for d in ('rows', 'tests', 'orig/rows', 'orig/tests', 'evidence'): os.makedirs(H + '/' + d, exist_ok=True)
for a in hold:
    for x in ('rows/%s.cpp', 'tests/%s_test.cpp', 'orig/rows/%s.cpp', 'orig/tests/%s_test.cpp'):
        if os.path.exists(B + '/' + x % a): shutil.move(B + '/' + x % a, H + '/' + x % a)
def filt(name, keep_hold):
    if not os.path.exists(B + '/' + name): return
    L = [l for l in open(B + '/' + name)]
    hdr = L[:1] if name.endswith('.tsv') and not name.startswith('renames') else []
    body = L[len(hdr):]
    open(H + '/' + name, 'w').write(''.join(hdr + [l for l in body if l.split('\t')[0].strip() in hold]))
    open(B + '/' + name, 'w').write(''.join(hdr + [l for l in body if l.split('\t')[0].strip() not in hold]))
for n in ('RESULTS.tsv', 'CHECKS.tsv', 'UNITS.tsv', 'renames.tsv', 'rows.txt'): filt(n, True)
print(b, 'moved', len(hold), 'HOLD rows to', b + '_hold')
