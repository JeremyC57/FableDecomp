# mkq28.py: QUEUE_W29.tsv (every QUEUE_W31_input.tsv row + outcome) from runs/qscan_base.tsv (HEAD index) / runs/qscan_final.tsv (index
# after ov_final), the family rows files and the per-target notes in mkq31_notes.py; the queue's deferred / partial rows go to
# runs/deferred_queue.tsv (merged into DEFERRED.tsv by mkdef29.py).
import os, re, collections
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); RUNS = os.path.join(W, 'runs')
f = lambda p: [l.rstrip('\n').split('\t') for l in open(p)][1:]
A = f(os.path.join(RUNS, 'qscan_base.tsv')); B = f(os.path.join(RUNS, 'qscan_final.tsv'))
Q = [l.rstrip('\n').split('\t') + [''] * 5 for l in open(os.path.join(W, 'QUEUE_W31_input.tsv'))]
if Q and Q[0][0] == 'chunk': Q, A, B = Q[1:], A[1:], B[1:]   # wave 27: the input carries a header row
FR = set(l.strip() for l in open(os.path.join(RUNS, 'final.rows.txt')) if l.strip())
def wrows(s):
    r = set()
    for part in s.split(' | '):
        if ': ' in part: r |= set(x for x in part.split(': ', 1)[1].split() if re.match(r'^[0-9a-f]{8}$', x))
    return r
def _rows(fam):
    p = os.path.join(RUNS, fam + '.rows.txt')
    return [l.strip() for l in open(p) if l.strip()] if os.path.exists(p) else []
exec(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'mkq31_notes.py')).read())
out = [['chunk', 'address', 'correct_symbol', 'evidence', 'wrong_spellings_seen', 'outcome', 'rows_edited', 'rows_left', 'detail']]
dfr = []
cnt = collections.Counter()
for i, (q, a, b) in enumerate(zip(Q, A, B), 1):
    t = a[1]
    w0, w1 = wrows(a[9]), wrows(b[9])
    if i in N_I:
        o, note = N_I[i]
        ed = sorted((set(x for x in (w0 - w1) if x in FR)) | set(EDX_I.get(i, ())))
        left = sorted(LEFT_I[i]) if i in LEFT_I else sorted(set(w1))
    else:   # carried unchanged from the wave 27 queue (not reached this wave)
        o = q[5] or 'deferred'; note = '[carried, not reached in wave 31] ' + (q[8] or 'appended mid-wave; not reached')
        ed = []; left = q[7].split() or sorted(set(w1))
    cnt[o] += 1
    out.append(q[:5] + [o, ' '.join(ed), ' '.join(left), note])
    if o in ('deferred', 'respelled-partial'):
        dfr.append([' '.join(left) or '-', t, a[2], 'row-local spellings (queue wrong_spellings_seen)', '[%s] %s' % (o, note)])
open(os.path.join(W, 'QUEUE_W31.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
# one entry per target
_s = {}
for r in dfr: _s.setdefault(r[1], r)
open(os.path.join(RUNS, 'deferred_queue.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in _s.values()))
print(dict(cnt), 'queue deferrals', len(_s))
