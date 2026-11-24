# status.py (NF337): STATUS.tsv for all 48 ENTRIES from batches/*/rows.txt (done) + work/status_extra.tsv (address<TAB>status<TAB>reason).
import glob, os
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF337'
ent = [l.rstrip('\n').split('\t') for l in open(A + '/ENTRIES.tsv')]
done = {}
for f in sorted(glob.glob(A + '/batches/*/rows.txt')):
    b = f.split('/')[-2]
    for a in open(f).read().split(): done[a] = b
ex = {}
if os.path.exists(A + '/work/status_extra.tsv'):
    for l in open(A + '/work/status_extra.tsv'):
        p = l.rstrip('\n').split('\t')
        if len(p) >= 3: ex[p[0]] = p[1:3]
out = []
for e in ent:
    a = e[0]
    if a in done and a not in ex: out.append((a, 'done', '%s: moved onto %s (operator= declared out of line; one `jmp`), moved test; unit with its operator= row' % (done[a], e[2])))
    elif a in ex: out.append((a, ex[a][0], ex[a][1]))
    else: out.append((a, 'pending', 'in progress'))
open(A + '/STATUS.tsv', 'w').write('address\tstatus\treason\n' + ''.join('\t'.join(r) + '\n' for r in out))
import collections; print(collections.Counter(r[1] for r in out))
