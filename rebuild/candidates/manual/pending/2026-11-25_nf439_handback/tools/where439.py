#!/usr/bin/env python3
"""where439.py: for each wrong spelling line of work/cls.tsv and each witness row, where the wrong callee is declared:
LOCAL (row defines class <scope> and names the method), FREE (row declares a free function of that name), HDR (name only via includes), NOSRC."""
import sys, os, re, csv, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = str(nf.A)
L = [l.rstrip('\n').split('\t') for l in open(W + '/work/cls.tsv')]
out = []; c = collections.Counter()
for t, s, wit, k, P in L:
    m = re.match(r'^\?(\w+)@((?:[\w$?]+@)*?)@', s)
    name = m.group(1) if m else None; scope = (m.group(2).split('@')[0] if m and m.group(2) else '')
    for w in wit.split(','):
        if w == 'self' or not re.match(r'^[0-9a-f]{8}$', w): continue
        f = nf.src_of(w)
        if not f or not f.exists(): where = 'NOSRC'
        else:
            x = f.read_text(encoding='utf-8', errors='replace')
            if name is None: where = 'NONAME'
            elif scope and re.search(r'\b(class|struct)\s+%s\b[^;]*\{' % re.escape(scope), x) and re.search(r'\b%s\b' % re.escape(name), x): where = 'LOCAL'
            elif not scope and re.search(r'\b%s\s*\(' % re.escape(name), x): where = 'FREE'
            elif re.search(r'\b%s\b' % re.escape(name), x): where = 'HDR?'
            else: where = 'ABSENT'
        c[(k, where)] += 1; out.append((t, s, w, k, where, P))
open(W + '/work/where.tsv', 'w').write(''.join('\t'.join(x) + '\n' for x in out))
for kv in sorted(c.items(), key=lambda x: -x[1]): print(kv)
