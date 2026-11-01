# mkdef18.py <final index jsonl>: runs/deferred_extra.tsv -- wave 17 DEFERRED_W17_INPUT.tsv rows carried into wave 18, minus the rows this
# wave resolved (every listed row now names no spelling bound at the target other than the final), plus the wave-17 non-queue
# deferrals (runs/deferred18_manual.tsv).
import os, sys, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); RUNS = os.path.join(W, 'runs')
BT, BS = inputs.bindings()
IX = {}
for l in open(sys.argv[1]):
    r = json.loads(l); IX[r['a']] = set(r.get('u', [])) | set(r.get('d', []))
Q = set(inputs.na(l.split('\t')[1]) for l in open(os.path.join(W, 'QUEUE_W18_input.tsv')) if re.match(r'^(0x)?[0-9a-fA-F]{6,8}$', l.split('\t')[1].strip()))
out, res = [], []
for l in list(open(os.path.join(W, "DEFERRED_W17_INPUT.tsv")))[1:]:
    r = l.rstrip('\n').split('\t')
    if len(r) < 5: continue
    rows_, t, fin = r[0].split(), r[1], r[2]
    fp = fin[:-3] if fin.endswith('...') else None
    wrong = set(s for s in BT.get(t, ()) if s != fin and not (fp and s.startswith(fp)))
    left = [a for a in rows_ if re.match(r'^[0-9a-f]{8}$', a) and (IX.get(a, set()) & wrong or not wrong)]
    if rows_ and rows_ != ['-'] and wrong and not left:
        res.append((r[0], t)); continue
    if left and len(left) < len([a for a in rows_ if re.match(r'^[0-9a-f]{8}$', a)]): r[0] = ' '.join(left)
    r[4] = re.sub(r'^\[([\w-]+)\]', r'[\1, carried from wave 17]', r[4]) if re.match(r'^\[[\w-]+\]', r[4]) else '[carried from wave 17] ' + r[4]
    out.append(r)
man = os.path.join(RUNS, 'deferred18_manual.tsv')
if os.path.exists(man): out += [l.rstrip('\n').split('\t') for l in open(man) if l.strip()]
open(os.path.join(RUNS, 'deferred_extra.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
open(os.path.join(RUNS, 'w17_deferred_resolved.tsv'), 'w').write(''.join('%s\t%s\n' % x for x in res))
print('carried', len(out), 'resolved', len(res), res[:20])
