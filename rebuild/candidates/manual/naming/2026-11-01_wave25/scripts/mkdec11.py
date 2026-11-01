# mkdec11.py : DECISIONS_W11.tsv -- one line per queue row (QUEUE_586_593_mig6_8_asmre.tsv): final spelling, PDB lists, emitters of
# each wrong spelling (bound to the target, or quoted in the queue's wrong-spellings column) before (idx/full.jsonl = the CURRENT
# repo) and after (ov_final rows substituted, idx/final.jsonl), and the wave-11 status with its reason (runs/dec_notes11.tsv
# overrides: address<TAB>status<TAB>reason[<TAB>final]; default derived from the counts).
import sys, os, json, collections, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import inputs
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BT, BS = inputs.bindings(); NM = inputs.names()
E0 = collections.defaultdict(set); E1 = collections.defaultdict(set); ROWS1 = {}
for l in open(os.path.join(W, 'idx/final.jsonl')):
    r = json.loads(l); ROWS1[r['a']] = r
for l in open(os.path.join(W, 'idx/full.jsonl')):
    r = json.loads(l)
    for s in r.get('u', []) + r.get('d', []): E0[s].add(r['a'])
    r1 = ROWS1.get(r['a'], r)
    for s in r1.get('u', []) + r1.get('d', []): E1[s].add(r['a'])
NOTES = {}
p = os.path.join(W, 'runs/dec_notes11.tsv')
if os.path.exists(p):
    for l in open(p):
        q = l.rstrip('\n').split('\t')
        if len(q) >= 3 and not l.startswith('#'): NOTES[q[0].lower()] = (q[1] or None, q[2], q[3] if len(q) > 3 else None)
out = [('chunk', 'address', 'final_symbol', 'pdb', 'status', 'wrong_emitters_before', 'wrong_emitters_after', 'final_emitters_after', 'reason', 'queue_evidence')]
cnt = collections.Counter()
for l in open(os.path.join(W, 'QUEUE_586_593_mig6_8_asmre.tsv')):
    q = l.rstrip('\n').split('\t')
    if len(q) < 3: continue
    ch, addr, fin = q[0], q[1], q[2].split(' ')[0]
    ev = q[3] if len(q) > 3 else ''; ws = q[4] if len(q) > 4 else ''
    key = addr.lower().replace('0x', '')
    t = inputs.na(addr) if re.match(r'^(0x)?[0-9a-fA-F]{8}$', addr) else None
    st, why, fo = NOTES.get(key, NOTES.get(t, (None, '', None))) if (key in NOTES or t in NOTES) else (None, '', None)
    if fo: fin = fo
    wrong = set()
    if t: wrong |= set(s for s in BT.get(t, ()) if s != fin)
    wrong |= set(s for s in re.findall(r'[?@][^\s;,()]+@@?[^\s;,()]*', ws) if s != fin and (s in E0 or s in BS))
    b = sum(len(E0.get(s, ())) for s in wrong); a = sum(len(E1.get(s, ())) for s in wrong)
    left = ['%s(%s)' % (s[:90], ','.join(sorted(E1[s])[:3])) for s in sorted(wrong) if E1.get(s)]
    pdb = ('E' if fin in NM['Ego_r.pdb'] else '-') + ('F' if fin in NM['FableWin.pdb'] else '-')
    if st is None:
        st = 'no change needed' if b == 0 else ('applied' if a == 0 else ('partly applied' if a < b else 'open'))
        if st == 'open' and not why: why = 'not reached in wave 11 (budget); the emitters are listed'
    if left and st.startswith('applied'): st = 'partly applied'
    cnt[st.split(':')[0]] += 1
    out.append((ch, addr, fin, pdb, st, str(b), str(a), str(len(E1.get(fin, ()))), why + ((' ; still emitted: ' + ' '.join(left[:6])) if left else ''), ev[:400]))
open(os.path.join(W, 'DECISIONS_W11.tsv'), 'w').write(''.join('\t'.join(x) + '\n' for x in out))
print(cnt)
