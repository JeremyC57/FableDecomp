# mkdec10.py <after.jsonl> : DECISIONS_W10.tsv -- one line per wave-10 target (runs/targets2.tsv): final spelling, PDB lists,
# queue evidence, emitters of each bound wrong spelling before (idx/full.jsonl, the CURRENT repo) and after (ov_final rows
# substituted), and the wave-10 status with its reason (NOTES below; default derived from the counts).
import sys, os, json, collections, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BT, BS = inputs.bindings(); NM = inputs.names()
E0, _ = inputs.index(os.path.join(W, 'idx/full.jsonl')); E1, _ = inputs.index(sys.argv[1])
Q = collections.defaultdict(list)
for l in open(os.path.join(W, 'QUEUE_575_585_mig.tsv')):
    q = l.rstrip('\n').split('\t')
    if len(q) >= 4: Q[inputs.na(q[1])].append((q[0], q[3]))
NOTES = {}
for l in open(os.path.join(W, 'runs/dec_notes.tsv')):
    q = l.rstrip('\n').split('\t')
    if len(q) >= 3 and not l.startswith('#'): NOTES[q[0]] = (q[1] or None, q[2])
out = [('address', 'final_symbol', 'pdb', 'chunks', 'status', 'wrong_emitters_before', 'wrong_emitters_after', 'final_emitters_after', 'reason', 'queue_evidence')]
cnt = collections.Counter()
for r in csv.reader(open(os.path.join(W, 'runs/targets2.tsv')), delimiter='\t'):
    t, fin, ch = r[0], r[1], r[2]
    wrong = sorted(s for s in BT.get(t, ()) if s != fin)
    b = sum(len(E0.get(s, ())) for s in wrong); a = sum(len(E1.get(s, ())) for s in wrong)
    left = ['%s(%d)' % (s[:90], len(E1[s])) for s in wrong if E1.get(s)]
    pdb = ('E' if fin in NM['Ego_r.pdb'] else '-') + ('F' if fin in NM['FableWin.pdb'] else '-')
    st, why = NOTES.get(t, (None, ''))
    if st is None:
        st = 'no change needed' if b == 0 else ('applied' if a == 0 else ('partly applied' if a < b else 'open'))
    if left and st.startswith('applied'): st = 'partly applied'
    if st == 'open' and not why: why = 'not reached in wave 10 (rest of the queue, ~600-row budget); the emitters are listed'
    cnt[st.split(':')[0]] += 1
    ev = ' | '.join('%s: %s' % (c, e[:300]) for c, e in Q.get(t, []))
    out.append((t, fin, pdb, ch, st, str(b), str(a), str(len(E1.get(fin, ()))), why + ((' ; still emitted: ' + ' '.join(left[:6])) if left else ''), ev))
open(os.path.join(W, 'DECISIONS_W10.tsv'), 'w').write(''.join('\t'.join(x) + '\n' for x in out))
print(cnt)
