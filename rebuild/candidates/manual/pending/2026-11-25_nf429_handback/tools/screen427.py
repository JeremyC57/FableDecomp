#!/usr/bin/env python3
"""screen427.py <addr file>: stay-off screen per address: lock owner, newest COORDINATION_CLAIMS line naming the address or its class
(account B -> skip), T3_PORT_QUEUE lines naming it (col 2 address / col 3 unit). Prints address, lock, claim, t3 (or '-')."""
import sys, os, re, csv
R = '/home/user/FableDecomp'; S = os.environ['S']
rows = open(sys.argv[1]).read().split()
cl = {}
for r in csv.DictReader(open(os.path.dirname(os.path.abspath(__file__)) + '/../evidence/classify.tsv'), delimiter='\t'): cl[r['addr']] = r['class']
claims = [l.rstrip('\n').split('\t') for l in open(R + '/rebuild/candidates/manual/COORDINATION_CLAIMS.tsv')][1:]
t3 = [l.rstrip('\n').split('\t') for l in open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv')][1:]
for a in rows:
    p = f'{S}/w22/nflocks/{a}/owner'; lk = open(p).read().strip() if os.path.exists(p) else 'none'
    A8 = a.upper(); pat = re.compile(r'(0x)?%s\b' % A8, re.I)
    cs = [c for c in claims if len(c) > 2 and (pat.search(c[2]) or (cl.get(a) and re.search(r'\b%s\b' % re.escape(cl[a]), c[2])))]
    newest = cs[-1] if cs else None
    cstr = '%s/%s/%s' % (newest[1], newest[4], newest[0]) if newest else '-'
    ts = [q for q in t3 if len(q) > 2 and (pat.search(q[1]) or pat.search(q[2]))]
    print('%s\t%s\t%s\t%s' % (a, lk, cstr, ';'.join('%s:%s' % (q[0], q[2][:30]) for q in ts) or '-'))
