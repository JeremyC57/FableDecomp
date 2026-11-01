# mkdef22.py <final index jsonl>: DEFERRED.tsv -- the wave 21 DEFERRED_W21_INPUT.tsv rows carried into wave 22 (rows this wave resolved
# dropped: every listed row now names no spelling bound at the target other than the final; partially resolved rows narrowed), the queue's
# deferred / partial targets (runs/deferred_queue.tsv, superseding the carried entry of the same target) and the wave-22 family leftovers
# (runs/deferred22_manual.tsv, superseding the carried entries of their targets).
import os, sys, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); RUNS = os.path.join(W, 'runs')
BT, BS = inputs.bindings()
IX = {}
for l in open(sys.argv[1]):
    r = json.loads(l); IX[r['a']] = set(r.get('u', [])) | set(r.get('d', []))
rd = lambda p: [l.rstrip('\n').split('\t') for l in open(p) if l.strip()] if os.path.exists(p) else []
QD = rd(os.path.join(RUNS, 'deferred_queue.tsv')); MAN = rd(os.path.join(RUNS, 'deferred22_manual.tsv'))
over = set(r[1] for r in QD) | {'006a80a0'}   # the ~CCharString carried entries narrow on their own (the manual entry adds the ccsns leftovers)
out, res = [], []
for r in rd(os.path.join(W, 'DEFERRED_W21_INPUT.tsv'))[1:]:
    if len(r) < 5: continue
    rows_, t, fin = r[0].split(), r[1], r[2]
    if t in over: continue
    fp = fin[:-3] if fin.endswith('...') else None
    wrong = set(s for s in BT.get(t, ()) if s != fin and not (fp and s.startswith(fp)))
    ar = [a for a in rows_ if re.match(r'^[0-9a-f]{8}$', a)]
    left = [a for a in ar if (IX.get(a, set()) & wrong or not wrong)]
    if ar and wrong and not left: res.append((r[0], t)); continue
    if left and len(left) < len(ar): r[0] = ' '.join(left)
    r[4] = re.sub(r'^\[([\w-]+)\]', r'[\1, carried from wave 21]', r[4]) if re.match(r'^\[[\w-]+\]', r[4]) else '[carried from wave 21] ' + r[4]
    out.append(r)
hdr = ['row', 'target', 'final_symbol', 'still_emitted', 'reason']
open(os.path.join(W, 'DEFERRED.tsv'), 'w').write(''.join('\t'.join(x) + '\n' for x in [hdr] + QD + MAN + out))
open(os.path.join(RUNS, 'w21_deferred_resolved.tsv'), 'w').write(''.join('%s\t%s\n' % x for x in res))
print('queue', len(QD), 'manual', len(MAN), 'carried', len(out), 'resolved', len(res), res[:20])
