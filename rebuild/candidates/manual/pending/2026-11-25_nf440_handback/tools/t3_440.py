#!/usr/bin/env python3
"""t3_440.py: T3.txt for the hand-back: per family address (NF428 FAMILY_TABLE), NF440's outcome."""
import sys, os, glob, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from lib440 import *
fam = tsv(P('rebuild/candidates/manual/pending/2026-11-25_nf428_handback/FAMILY_TABLE.tsv'))
asg = {f[0]: f for f in tsv(W + '/evidence/assign.tsv')}
inb = {}
for r in glob.glob(W + '/batches/nf440_*/RESULTS.tsv'):
    b = r.split('/')[-2]
    for f in tsv(r): inb[f[0]] = (b, f[1], f[4][:160])
skip = {l.split('\t')[0]: l.split('\t')[1].strip() for l in open(W + '/work/skip_t2.tsv')}
out = []; C = collections.Counter()
for f in fam:
    a = f[0]
    if a in inb: s = '%s %s: %s' % (inb[a][1], inb[a][0], inb[a][2])
    elif a in asg and asg[a][6] and not asg[a][7].startswith('HOLD'):
        x = asg[a]; s = 'CANDIDATE %s %s (%s): %s' % (x[3], x[6], 'not batched: ' + skip.get(a, 'T3 precision 13/24') , x[7][:120])
    elif a in asg: s = 'HOLD ' + asg[a][7][:160]
    else: s = 'NF428 ' + f[3] + ': ' + f[4][:140] + ' (not started by NF440)'
    k = s.split(' ')[0] + (' ' + s.split(' ')[1] if s.startswith(('ACCEPT', 'HOLD nf440', 'CANDIDATE')) else '')
    w = s.split(' '); C[' '.join(w[:2]) if w[0] in ('ACCEPT','CANDIDATE') or w[1].startswith('nf440') else ('HOLD (no T / collision / TU conflict; reason per line)' if w[0]=='HOLD' else 'NF428 ' + w[1].rstrip(':') + ' (not started by NF440)')] += 1; out.append('nf440\t%s\t%s' % (a, s))
hdr = ['# NF440 T3 lines (2026-11-25): one summary line per outcome, then one line per family address.']
hdr += ['nf440\t%d addresses\t%s' % (n, k) for k, n in C.most_common()]
open(W + '/T3.txt', 'w').write('\n'.join(hdr + out) + '\n'); print('\n'.join(hdr))
