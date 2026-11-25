#!/usr/bin/env python3
"""fin_r2.py: NF427 round 2 finalize. Part 2 (r2/p2a..p2d): CHECKS.tsv gets the round-2 head/new/mutant grades (round-1 grades
asserted equal for head/new; every other column is the rows' and stays), name gate from evidence/name_gate.tsv (r2 run). Task B
(r2/t1..t3): CHECKS.tsv / RESULTS.tsv for the test-only rows. Logs copied into r2/<b>/logs. Prints a summary."""
import os, re, csv, shutil, collections
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); W = A + '/work'
def grades(f):
    g = {}
    for l in open(f, errors='replace'):
        m = re.match(r'^([0-9a-f]{8})\s+(\S+)\s+(PASS|FAIL|LINK_FAIL|TCC_FAIL|NO_TEST)\s', l)
        if m: g[m.group(1)] = '%s %s' % (m.group(2), m.group(3))
    return g
def ng(f):
    out = {}
    for r in csv.reader(open(f), delimiter='\t'):
        if len(r) >= 2: out[r[0]] = (r[1], r[2] if len(r) > 2 else '')
    return out
summ = []
for b in ('p2a', 'p2b', 'p2c', 'p2d'):
    B = A + '/r2/' + b; rows = open(B + '/rows.txt').read().split()
    h, n, m = (grades('%s/vland_r2%s%s.log' % (W, b, k)) for k in 'hnm')
    h1, n1 = grades('%s/vland_%sh.log' % (W, b)), grades('%s/vland_%sn.log' % (W, b))
    ngr = ng(B + '/evidence/name_gate.tsv')
    rd = list(csv.reader(open(B + '/CHECKS.tsv'), delimiter='\t')); hd = rd[0]
    if 'name_gate_r2' not in hd: hd += ['test_r2', 'name_gate_r2']
    bad = []
    for r in rd[1:]:
        a = r[0]
        assert h[a] == h1[a] and n[a] == n1[a], (b, a, h[a], h1[a], n[a], n1[a])
        r[hd.index('head_grade')] = h[a]; r[hd.index('new_grade')] = n[a]; r[hd.index('mutant')] = m.get(a, 'NOT RUN')
        tl = [x for x in ngr[a][1].split('|') if '(test)' in x]
        r[len(hd) - 2:] = ['header form (round 2)', ngr[a][0] + (' [test-side: %s]' % tl if tl else ' [test-side: 0]')]
        if not n[a].endswith('PASS') or not m.get(a, '').endswith('FAIL') or tl: bad.append(a)
    with open(B + '/CHECKS.tsv', 'w') as fo:
        for r in rd: fo.write('\t'.join(r) + '\n')
    os.makedirs(B + '/logs', exist_ok=True)
    for k in 'hnm': shutil.copy('%s/vland_r2%s%s.log' % (W, b, k), '%s/logs/vland_%s.log' % (B, k))
    summ.append('%s rows %d new %s mutant %s name_gate %s bad %s' % (b, len(rows), dict(collections.Counter(n.values())), dict(collections.Counter(m.values())),
                dict(collections.Counter(v[0] for v in ngr.values())), bad))
for t in ('t1', 't2', 't3'):
    B = A + '/r2/' + t
    if not os.path.exists('%s/vland_%sm.log' % (W, t)): summ.append('%s not finished' % t); continue
    rows = open(B + '/rows.txt').read().split()
    h, n, m = (grades('%s/vland_%s%s.log' % (W, t, k)) for k in 'hnm')
    ngr = ng(B + '/evidence/name_gate.tsv') if os.path.exists(B + '/evidence/name_gate.tsv') else {}
    gen = {r['address']: r for r in csv.DictReader(open('%s/tb/%s_gen.tsv' % (W, t)), delimiter='\t')}
    ck = ['address\thead_grade\tnew_grade\tgrade_same\tmutant\tengine_header\tstand_ins\tname_gate_test_side\tname_gate_row_side']
    res = ['address\tverdict\treason']; bad = []
    for a in rows:
        g = ngr.get(a, ('NOT RUN', ''))
        tl = [x for x in g[1].split('|') if '(test)' in x]
        rl = [x.strip() for x in g[1].split('|') if x.strip() and '(test)' not in x]
        same = h.get(a, '?').split()[0] == n.get(a, '?').split()[0]
        ok = same and n.get(a, '').endswith('PASS') and m.get(a, '').endswith('FAIL') and not tl and g[0] != 'NOT RUN'
        if not ok: bad.append(a)
        ck.append('\t'.join([a, h.get(a, '?'), n.get(a, '?'), 'yes' if same else 'NO', m.get(a, 'NOT RUN'), gen[a]['engine_header'], gen[a]['stand_ins'],
                             '0' if not tl else ';'.join(tl), g[0] + (' (row: %s)' % '; '.join(rl)[:300] if rl else '')]))
        res.append('%s\t%s\t%s' % (a, 'ACCEPT' if ok else 'HOLD', 'test-only port onto the shared headers (NF427 round 2): row unchanged (grade %s at HEAD and with the new test), new test %s, mutant %s; no class body in the test; engine/<C>.h %s'
                   % (h.get(a, '?').split()[0], n.get(a, '?').split()[-1], m.get(a, '?'), gen[a]['engine_header'])))
    open(B + '/CHECKS.tsv', 'w').write('\n'.join(ck) + '\n'); open(B + '/RESULTS.tsv', 'w').write('\n'.join(res) + '\n')
    os.makedirs(B + '/logs', exist_ok=True)
    for k in 'hnm': shutil.copy('%s/vland_%s%s.log' % (W, t, k), '%s/logs/vland_%s.log' % (B, k))
    shutil.copy('%s/tb/%s_gen.tsv' % (W, t), B + '/evidence/port.tsv')
    summ.append('%s rows %d head %s new %s mutant %s name_gate %s bad %s' % (t, len(rows), dict(collections.Counter(h.values())), dict(collections.Counter(n.values())),
                dict(collections.Counter(m.values())), dict(collections.Counter(v[0] for v in ngr.values())), bad))
print('\n'.join(summ))
