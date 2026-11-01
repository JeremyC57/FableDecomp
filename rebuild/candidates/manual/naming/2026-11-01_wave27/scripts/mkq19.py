# mkq18.py: QUEUE_W19.tsv (every QUEUE_W19_input.tsv row + outcome) and DEFERRED.tsv, from runs/qscan0.tsv (baseline index) /
# runs/qscan1.tsv (index after ov_final), runs/final.rows.txt and the per-target notes below.
# outcome: respelled (edited rows, no row left naming a wrong bound spelling, own row defines the final) / respelled-partial
# (edited, some rows left) / deferred (nothing edited, rows left) / already-correct (nothing to respell) / hold.
import os, re, collections
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); RUNS = os.path.join(W, 'runs')
f = lambda p: [l.rstrip('\n').split('\t') for l in open(p)][1:]
A = f(os.path.join(RUNS, 'qscan_base.tsv')); B = f(os.path.join(RUNS, 'qscan_final.tsv'))
Q = [l.rstrip('\n').split('\t') + [''] * 5 for l in open(os.path.join(W, 'QUEUE_W19_input.tsv'))]
FR = set(l.strip() for l in open(os.path.join(RUNS, 'final.rows.txt')) if l.strip())
FAM = {}
for fam in ['rfail', 'stools', 'c684', 'gif', 'c685', 'c680', 'xfer', 'cmp']:
    p = os.path.join(RUNS, fam + '.rows.txt')
    if os.path.exists(p):
        for l in open(p):
            if l.strip(): FAM.setdefault(l.strip(), []).append(fam)
def wrows(s):
    r = set()
    for part in s.split(' | '):
        if ': ' in part: r |= set(x for x in part.split(': ', 1)[1].split() if re.match(r'^[0-9a-f]{8}$', x))
    return r
exec(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'mkq19_notes.py')).read())
out = [['chunk', 'address', 'correct_symbol', 'evidence', 'wrong_spellings_seen', 'outcome', 'rows_edited', 'rows_left', 'detail']]
dfr = [['row', 'target', 'final_symbol', 'still_emitted', 'reason']]
cnt = collections.Counter()
for q, a, b in zip(Q, A, B):
    t = a[1]; own = a[5]
    w0, w1 = wrows(a[9]), wrows(b[9])
    n0 = set(a[7].split()) - {'...'}; n1 = set(b[7].split()) - {'...'}
    ed = sorted(set(x for x in ((w0 - w1) | (n1 - n0) | ({own} if own and b[6] == 'Y' and a[6] != 'Y' else set())) if x and x in FR) | set(EDX.get(t, ())))
    left = set(w1)
    if own and b[6] != 'Y' and not b[6].startswith('err'): left.add(own)
    if t in CLEAR_LEFT: left = set()   # the notes explain: own row correct under the compiler's / a display-string / a bare-name final
    if t in ('0121a460', '0121a4f0', '0121a580', '0121a7ca', '01228ed8', '00c057d0'): left.discard(own)
    o = ('respelled' if not left else 'respelled-partial') if ed else ('already-correct' if not left else 'deferred')
    note = ''
    if t == '00440730': o, note = 'hold', 'owner HOLD (brief)'
    if t in N:
        oo, note = N[t]
        if oo: o = oo
    if not note:
        if o == 'respelled': note = 'respelled and rechecked: ' + ', '.join('%s (%s)' % (x, '+'.join(FAM.get(x, ['?']))) for x in ed[:12])
        elif o == 'already-correct': note = 'no landed row names a wrong spelling of the target' + ('' if own else ' (own row not landed)')
    if o in ('deferred', 'respelled-partial') and not (t in N and N[t][1]): note = (note + '; ' if note else '') + skipsum(t)
    cnt[o] += 1
    out.append(q[:5] + [o, ' '.join(ed), ' '.join(sorted(left)), note])
    if o in ('deferred', 'respelled-partial'):
        dfr.append([' '.join(sorted(left)) or '-', t, a[2], 'row-local spellings (queue wrong_spellings_seen)', '[%s] %s' % (o, note)])
open(os.path.join(W, 'QUEUE_W19.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
EXTRA = os.path.join(RUNS, 'deferred_extra.tsv')
QT = set(r[1] for r in dfr[1:]) | set(a[1] for a in A if a[1] in N or True)
if os.path.exists(EXTRA): dfr += [r for r in (l.rstrip('\n').split('\t') for l in open(EXTRA) if l.strip()) if r[1] not in QT or r[4].startswith('[recheck-fail]')]
# dedupe (row, target): a wave-19 entry supersedes the carried wave-18 one
_seen = {}
for r in dfr[1:]:
    k = (r[0], r[1])
    if k not in _seen or ('carried' in _seen[k][4] and 'carried' not in r[4]): _seen[k] = r
dfr = [dfr[0]] + [r for r in dfr[1:] if _seen[(r[0], r[1])] is r]
open(os.path.join(W, 'DEFERRED.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in dfr))
print(dict(cnt), 'deferred rows', len(dfr) - 1)
