# mkdef24.py <final index jsonl>: DEFERRED.tsv -- the wave 22 DEFERRED_W23_INPUT.tsv rows carried into wave 24 (rows this wave resolved
# dropped: every listed row now names no spelling bound at the target other than the final; partially resolved rows narrowed), the queue's
# deferred / partial targets (runs/deferred_queue.tsv, superseding the carried entry of the same target) and the wave-24 family leftovers
# (runs/deferred24_manual.tsv, superseding the carried entries of their targets).
import os, sys, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); RUNS = os.path.join(W, 'runs')
BT, BS = inputs.bindings()
IX = {}
for l in open(sys.argv[1]):
    r = json.loads(l); IX[r['a']] = set(r.get('u', [])) | set(r.get('d', []))
rd = lambda p: [l.rstrip('\n').split('\t') for l in open(p) if l.strip()] if os.path.exists(p) else []
QD = rd(os.path.join(RUNS, 'deferred_queue.tsv')); MAN = rd(os.path.join(RUNS, 'deferred24_manual.tsv'))
over = set(r[1] for r in QD) | set(r[1] for r in MAN)
# targets the queue settled this wave (respelled / done): their carried entries are resolved
over |= set(q[1].lower().replace('0x', '') for q in rd(os.path.join(W, 'QUEUE_W24.tsv'))[1:] if q[5] in ('respelled', 'done') and ' ' not in q[1])
FR = set(l.strip() for l in open(os.path.join(RUNS, 'final.rows.txt')) if l.strip())
out, res = [], []
for r in rd(os.path.join(W, 'DEFERRED_W23_INPUT.tsv'))[1:]:
    if len(r) < 5: continue
    rows_, t, fin = r[0].split(), r[1], r[2]
    if t in over: continue
    if t == '006a80a0':   # the filt leftovers: the final is a per-filter pattern, not one symbol -- drop the rows this wave respelled (family filt6)
        r[0] = ' '.join(a for a in rows_ if a not in FR)
        r[4] = '[carried from wave 23] ' + r[4] + ' -- wave 24: the six mis-identified rows (0x006156E0 0x006D3FB0 0x00777000 0x007EE660 0x009223A0 0x0093F930) re-identified and respelled (family filt6)'
        out.append(r); continue
    fp = fin[:-3] if fin.endswith('...') else None
    wrong = set(s for s in BT.get(t, ()) if s != fin and not (fp and s.startswith(fp)))
    ar = [a for a in rows_ if re.match(r'^[0-9a-f]{8}$', a)]
    left = [a for a in ar if (IX.get(a, set()) & wrong or not wrong)]
    if ar and wrong and not left: res.append((r[0], t)); continue
    if left and len(left) < len(ar): r[0] = ' '.join(left)
    r[4] = re.sub(r'^\[([\w-]+)\]', r'[\1, carried from wave 23]', r[4]) if re.match(r'^\[[\w-]+\]', r[4]) else '[carried from wave 23] ' + r[4]
    out.append(r)
hdr = ['row', 'target', 'final_symbol', 'still_emitted', 'reason']
open(os.path.join(W, 'DEFERRED.tsv'), 'w').write(''.join('\t'.join(x) + '\n' for x in [hdr] + QD + MAN + out))
open(os.path.join(RUNS, 'w23_deferred_resolved.tsv'), 'w').write(''.join('%s\t%s\n' % x for x in res))
print('queue', len(QD), 'manual', len(MAN), 'carried', len(out), 'resolved', len(res), res[:20])
