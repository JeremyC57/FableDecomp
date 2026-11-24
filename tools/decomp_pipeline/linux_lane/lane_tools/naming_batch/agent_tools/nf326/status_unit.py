# status_unit.py (NF326): STATUS.tsv = the ten unit members (decision per member, from CENSUS.tsv + evidence/copyproof_vland.txt +
# evidence/selfproof_vland.txt: the objects vland --recheck-landed built) + work/status_other.tsv (the other 53 ENTRIES). One line per ENTRIES address, in ENTRIES order.
import collections
from pathlib import Path
A = Path(__file__).resolve().parent.parent
E = [l.split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8').read().splitlines()[1:]]
C = {l.split('\t')[2]: l.split('\t') for l in open(A / 'CENSUS.tsv').read().splitlines()[1:]}
cp = collections.defaultdict(list)
for l in open(A / 'evidence/copyproof_vland.txt'):
    a, v = l.split()[:2]
    for m in [x.split(' v retail')[0].strip() for x in l.split(';') if ' v retail ' in x]: cp[m].append((a, v))
sp = {l.split()[0]: l.split()[1] for l in open(A / 'evidence/selfproof_vland.txt') if l.strip()}
ROWS = set(open(A / 'rows.txt').read().split())
other = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in open(A / 'work/status_other.tsv')}
out = open(A / 'STATUS.tsv', 'w'); out.write('address\tstatus\tpdb_spelling\treason\n')
for e in E:
    a = e[0]
    if a in C and a not in ('004aa840', '004abe90'):   # the dtor / copy-ctor census lines are context, not unit members
        c = C[a]; m = c[0]; pdb = c[1]
        em = [x for x in c[4].split(',') if x and x != '-']
        eq = [x.split(':')[0] for x in em if x.endswith('BYTES_EQUAL')]; df = [x.split(':')[0] for x in em if x.endswith('BYTES_DIFF')]
        resp = [x for x, v in cp.get(m, []) if v == 'OK']
        left = [x for x in df if x not in resp]
        r = ('(a) drafted and proven, held for the ruling the prefix asks for (NOTES Q1): self rows/%s.cpp defines %s inline on the real '
             'class and emits it pick-any through the nf86 / nf88 / NF92 qualified-call scaffolding (selfproof %s: retail bytes, HEAD body '
             'equal, +1 anonymous-namespace helper); %d landed copies emit it pick-any (%d equal retail at HEAD, %d respelled to the self '
             'body and now equal, copyproof OK%s); retail callers bind the PDB spelling already (%s); link: no duplicate on it, same '
             'unresolved set (evidence/link2_unit_*.txt, evidence/link2_all_*_force.txt)') % (a, pdb, sp.get(a, '?'), len(em), len(eq), len(resp),
             ('; NOT respelled: %s (its own leaf goes DIFFER under every spelling tried, NOTES Q2)' % ','.join(left)) if left else '',
             c[7] if c[7] != '-' else 'none')
        out.write('%s\tskipped-held\t%s\t%s\n' % (a, pdb, r))
    else:
        o = other[a]; r = o[2]
        if a in ROWS: r += ('. Also a unit copy row: this row defines unit members in-TU and is respelled in rows/%s.cpp (one-definition respell, '
                            'pair graded MATCH / RELOCATION_MATCH, copyproof_vland OK); its own name is unchanged') % a
        out.write('%s\t%s\t%s\t%s\n' % (a, o[1], e[3], r))
out.close(); print(sum(1 for _ in open(A / 'STATUS.tsv')) - 1, 'lines')
