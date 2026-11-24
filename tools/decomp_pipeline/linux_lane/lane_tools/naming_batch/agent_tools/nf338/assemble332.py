# assemble332.py: hand-back files from work/: STATUS.tsv + APPLIED.tsv (work/st.tsv, reached entries only), RENAMES.tsv (ren1 + ren2),
# TABLE_CHANGES.tsv (tc.tsv as written), REJECTED.tsv (header), rows.txt (rows/*.cpp minus tests; checked against work/rows_changed.txt)
import os
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF338'; W = A + '/work'
rd = lambda p: [l.rstrip('\n') for l in open(p) if l.strip()]
st = sorted((l.split('\t') for l in rd(W + '/st.tsv')), key=lambda p: int(p[0]))
open(A + '/STATUS.tsv', 'w').write('idx\taddress\tpdb_spelling\ttier_fix\tstatus\trows_changed\tspellings_before_after\tnote\n' +
                                  ''.join('\t'.join(p) + '\n' for p in st))
open(A + '/APPLIED.tsv', 'w').write('entry\taddress\tstatus\tbinders_respelled\tnote\n' +
                                   ''.join('\t'.join([p[0], p[1], p[4], p[5], p[7]]) + '\n' for p in st if p[4].startswith('done')))
open(A + '/RENAMES.tsv', 'w').write('address\tmodule\tname\tmangled_leaf\told_catalog_name\told_leaf\tevidence\n' + '\n'.join(rd(W + '/ren1.tsv')) +
                                   '\n# Section 2\ncaller\told_symbol\tnew_symbol\tcallee\n' + '\n'.join(rd(W + '/ren2.tsv')) + '\n')
open(A + '/TABLE_CHANGES.tsv', 'w').write('action\tsymbol\ttarget\twitness\tnote\n' + '\n'.join(rd(W + '/tc.tsv')) + '\n')
open(A + '/REJECTED.tsv', 'w').write('idx\taddress\treason\n')
rows = sorted(f[:-4] for f in os.listdir(A + '/rows') if f.endswith('.cpp') and not f.endswith('_test.cpp'))
ch = sorted(set(rd(W + '/rows_changed.txt')))
open(A + '/rows.txt', 'w').write('\n'.join(rows) + '\n')
print(len(st), 'status lines', len(rows), 'rows', 'rows==rows_changed' if rows == ch else 'MISMATCH %s %s' % (set(rows) ^ set(ch), ''))
