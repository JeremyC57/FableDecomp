# eqsum.py: evidence/equiv_summary.txt from the equiv runs (a TSV counts only when its .out log has the `rows checked N` line) and the
# mechanical "every includer" checks on equiv_new2.tsv: src set = unguarded set minus the 18 REREAD definers; every src row compiled or a
# hole; every test row rechecked (vland) or a hole test.
import os, re
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); E = A + '/evidence/'; W = A + '/work/'
runs = [('equiv_unguarded_new.tsv', 'equiv.out', 'HEAD tree v first (unguarded) header, all rows'),
        ('equiv_new2_changedset.tsv', 'eqH.out', 'HEAD tree v final CDefinitionManager.h (guarded), the 159 rows the unguarded all-rows run flagged'),
        ('equiv_unguarded_v_guarded.tsv', 'eqC.out', 'unguarded v guarded, the 159-row changed set'),
        ('equiv_plant2_v_new2.tsv', 'eqE.out', 'planted control: final v final + plant, the 159-row changed set'),
        ('equiv_cm_mentioners.tsv', 'eqG.out', 'final tree v + CCombatManager.h opt-in, the 210 rows whose source names CCombatManager'),
        ('equiv_cm_plant_mentioners.tsv', 'eqF.out', 'planted control: + opt-in v + opt-in + plant, the same 210 rows'),
        ]
out = []
def rows(f): return [l.rstrip('\n').split('\t') for l in open(E + f)] if os.path.exists(E + f) else None
for f, log, what in runs:
    lg = open(W + log).read() if os.path.exists(W + log) else ''
    done = re.search(r'rows checked (\d+) -> differing lines in \S*' + re.escape({'equiv_unguarded_new.tsv': 'equiv_new.tsv'}.get(f, f)), lg)   # the unguarded run wrote equiv_new.tsv (renamed after)
    r = rows(f)
    if r is None: out.append('%-32s MISSING  %s' % (f, what)); continue
    src = sorted(set(x[0] for x in r if x[1] == 'src')); tst = sorted(set(x[0] for x in r if x[1] == 'test'))
    out.append('%-32s %s rows_checked=%s src=%d test=%d other=%d  %s' % (f, 'COMPLETE' if done else 'INCOMPLETE', done.group(1) if done else '-',
               len(src), len(tst), len([x for x in r if x[1] not in ('src', 'test')]), what))
holes = set('00584b86 005d9aa0 006628e0 006ac200 007eac10 007fc210 00801920 0082d2d0 0040c790 007bd2d1 00859ef0 00591b7f 00774d20'.split())
comp = set(l.split('\t')[0] for l in open(W + 'nset.tsv'))
rr = set(l.split('\t')[0] for l in open(E + 'equiv_unguarded_v_guarded.tsv'))
ung = set(x[0] for x in rows('equiv_unguarded_new.tsv') if x[1] == 'src')
r = rows('equiv_new2_changedset.tsv'); s2 = set(x[0] for x in r if x[1] == 'src'); t2 = set(x[0] for x in r if x[1] == 'test')
out.append('')
out.append('check 1: equiv_new2_changedset src set == unguarded src set (%d) minus the %d REREAD definers: %s (extra %s, missing %s)' % (
    len(ung), len(rr), s2 == ung - rr, sorted(s2 - (ung - rr)), sorted((ung - rr) - s2)))
out.append('check 2: every equiv_new2_changedset src row compiled (nset) or a hole: %s (outside: %s)' % (s2 <= comp | holes, sorted(s2 - comp - holes)))
rech = set('00627be0 0064e978 006c14d0 00774b40'.split())
out.append('check 3: every equiv_new2_changedset test row rechecked with vland or a hole test: %s (tests %s; outside: %s)' % (
    t2 <= rech | holes, sorted(t2), sorted(t2 - rech - holes)))
cov = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'eqsum_coverage.txt')).read()   # the coverage argument, kept with the tool
open(E + 'equiv_summary.txt', 'w').write('\n'.join(out) + '\n\n' + cov); print('\n'.join(out))
