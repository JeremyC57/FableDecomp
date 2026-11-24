# assemble334.py: hand-back files from work/:
#   STATUS.tsv = work/st.tsv (hand-made, done / done-partial / skipped / rejected) + work/st_script.tsv (st334.py: HELD / PORTC census)
#                + work/man_reasons.tsv (opened by hand: idx status reason) + work/fam_dtor.txt (decision D1, skipped-held)
#   APPLIED.tsv (done*), RENAMES.tsv (section 2 caller respellings: work/ren2.tsv), TABLE_CHANGES.tsv (work/tc.tsv), REJECTED.tsv
#   (work/rej.tsv), rows.txt (rows/*.cpp minus tests), work/unreached.txt (ENTRIES indices with no STATUS line)
import os
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); W = A + '/work'
rd = lambda p: [l.rstrip('\n') for l in open(p) if l.strip()]
E = [l.rstrip('\n').split('\t') for l in list(open(A + '/ENTRIES.tsv'))[1:]]
st = {}
for l in rd(W + '/st.tsv'): p = l.split('\t'); st[p[0]] = p
for l in rd(W + '/st_script.tsv'):
    p = l.split('\t'); assert p[0] not in st, p[0]; st[p[0]] = p
for l in rd(W + '/man_reasons.tsv'):
    i, s, r = l.split('\t', 2); assert i not in st, i; e = E[int(i) - 1]
    st[i] = [i, '0x' + e[0].upper(), e[6], 'T1 other', s, '-', '-', 'opened by hand: ' + r]
for i in open(W + '/fam_dtor.txt').read().split():
    if i in st: continue
    e = E[int(i) - 1]
    st[i] = [i, '0x' + e[0].upper(), e[6], 'T1 other', 'skipped-held', '-', '-', 'decision D1 (destructor family): the binders call a free / fastcall X_Destruct stand-in (or a ??_G row stub) for the PDB destructor; the respell needs the class model (shared headers are flat, vm=0) and the landed tests stub X_Destruct: one decision for the family, not mass-edited']
rows = sorted(st.values(), key=lambda p: int(p[0]))
open(A + '/STATUS.tsv', 'w').write('idx\taddress\tpdb_spelling\ttier_fix\tstatus\trows_changed\tspellings_before_after\tnote\n' + ''.join('\t'.join(p) + '\n' for p in rows))
open(A + '/APPLIED.tsv', 'w').write('entry\taddress\tstatus\tbinders_respelled\tnote\n' + ''.join('\t'.join([p[0], p[1], p[4], p[5], p[7]]) + '\n' for p in rows if p[4].startswith('done')))
open(A + '/RENAMES.tsv', 'w').write('# Section 1 (self renames): none\n# Section 2\ncaller\told_symbol\tnew_symbol\tcallee\n' + '\n'.join(rd(W + '/ren2.tsv')) + '\n')
open(A + '/TABLE_CHANGES.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n' + '\n'.join(rd(W + '/tc.tsv')) + '\n')
open(A + '/REJECTED.tsv', 'w').write('address\tidx\tchange\treason\n' + '\n'.join(rd(W + '/rej.tsv')) + '\n')
rf = sorted(f[:-4] for f in os.listdir(A + '/rows') if f.endswith('.cpp') and not f.endswith('_test.cpp'))
open(A + '/rows.txt', 'w').write('\n'.join(rf) + '\n'); open(W + '/rows_changed.txt', 'w').write('\n'.join(rf) + '\n')
un = [str(i) for i in range(1, len(E) + 1) if str(i) not in st]
open(W + '/unreached.txt', 'w').write(' '.join(un) + '\n')
import collections; c = collections.Counter(p[4] for p in rows)
print(len(rows), 'status lines', dict(c), len(rf), 'rows; unreached', len(un), un)
