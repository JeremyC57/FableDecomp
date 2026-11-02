# mkdef32.py (from mkdef31.py) <final index jsonl>: DEFERRED.tsv -- the wave 27 DEFERRED_W28_INPUT.tsv rows carried into wave 28 (rows this wave resolved
# dropped: every listed row now names no spelling bound at the target other than the final; partially resolved rows narrowed), the queue's
# deferred / partial targets (runs/deferred_queue.tsv, superseding the carried entry of the same target) and the wave-28 family leftovers
# (runs/deferred29_manual.tsv, superseding the carried entries of their targets).
import os, sys, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); RUNS = os.path.join(W, 'runs')
BT, BS = inputs.bindings()
IX = {}
for l in open(sys.argv[1]):
    r = json.loads(l); IX[r['a']] = set(r.get('u', [])) | set(r.get('d', []))
rd = lambda p: [l.rstrip('\n').split('\t') for l in open(p) if l.strip()] if os.path.exists(p) else []
QD = rd(os.path.join(RUNS, 'deferred_queue.tsv')); MAN = rd(os.path.join(RUNS, 'deferred32_manual.tsv'))
over = set(r[1] for r in QD) | set(r[1] for r in MAN)
# targets the queue settled this wave (respelled / done): their carried entries are resolved
over |= set(q[1].lower().replace('0x', '') for q in rd(os.path.join(W, 'QUEUE_W32.tsv'))[1:] if q[5] in ('respelled', 'done') and ' ' not in q[1])
# the wave 26 findings (DECISIONS_W26 D5) and decision rows this wave respelled on their own rows (no binder to test against)
over |= set()
QM = set(r[1] for r in QD) | set(r[1] for r in MAN)
FR = set(l.strip() for l in open(os.path.join(RUNS, 'final.rows.txt')) if l.strip())
def carry(s):
    # one carried tag: [carried from wave 24] -> [carried from waves 24-25], [carried from waves 23-24] -> [carried from waves 23-25],
    # [<outcome>, ...] -> [<outcome>, ..., carried from wave 25]
    m = re.match(r'^\[carried from waves? (\d+)(?:-(\d+))?\]', s)
    if m: return '[carried from waves %s-31]' % m.group(1) + s[m.end():]
    m = re.match(r'^\[([^]]*)\]', s)
    if m: return '[%s, carried from wave 31]' % m.group(1) + s[m.end():]
    return '[carried from wave 31] ' + s
out, res = [], []
for r in rd(os.path.join(W, 'DEFERRED_W31_INPUT.tsv'))[1:]:
    if len(r) < 5: continue
    rows_, t, fin = r[0].split(), r[1], r[2]
    if r[4].startswith('[hold'): r[4] = carry(r[4]); out.append(r); continue   # waves 29-30: holds are never resolved by the index test
    if t in over:
        if t not in QM: res.append((r[0], t))
        continue
    if t == '006a80a0':   # the filt leftovers: the final is a per-filter pattern, not one symbol (no wave 25 edit)
        r[4] = carry(r[4]); out.append(r); continue
    if r[0].strip() == t:   # wave 32: an own-row signature finding (the row's own definition is not a binding): carried, never index-resolved
        r[4] = carry(r[4]); out.append(r); continue
    fp = fin[:-3] if fin.endswith('...') else None
    wrong = set(s for s in BT.get(t, ()) if s != fin and not (fp and s.startswith(fp)))
    ar = [a for a in rows_ if re.match(r'^[0-9a-f]{8}$', a)]
    left = [a for a in ar if (IX.get(a, set()) & wrong or not wrong)]
    if ar and wrong and not left: res.append((r[0], t)); continue
    if left and len(left) < len(ar): r[0] = ' '.join(left)
    r[4] = carry(r[4])
    out.append(r)
hdr = ['row', 'target', 'final_symbol', 'still_emitted', 'reason']
open(os.path.join(W, 'DEFERRED.tsv'), 'w').write(''.join('\t'.join(x) + '\n' for x in [hdr] + QD + MAN + out))
open(os.path.join(RUNS, 'w31_deferred_resolved.tsv'), 'w').write(''.join('%s\t%s\n' % x for x in res))
print('queue', len(QD), 'manual', len(MAN), 'carried', len(out), 'resolved', len(res), res[:20])
