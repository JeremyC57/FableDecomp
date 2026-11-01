# mklabels16.py: LABELS.tsv -- for every queue target this wave respelled (fully or partly) and the decision / wave-14 targets,
# the qualified name of the settled spelling, when no ghidra_out/labels_*.tsv row already carries that exact name.
import os, sys, re, glob, collections
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.insert(0, os.path.join(W, 'scripts'))
import und
L = collections.defaultdict(set)
for p in glob.glob('/home/user/FableDecomp/ghidra_out/labels_*.tsv'):
    for l in open(p, errors='replace'):
        q = l.rstrip('\n').split('\t')
        if len(q) >= 2: L[q[0].lower().replace('0x', '').zfill(8)].add(q[1])
Q = [l.rstrip('\n').split('\t') for l in open(os.path.join(W, 'QUEUE_W17.tsv'))][1:]
out = [('address', 'name', 'evidence')]
seen = set()
def qn(sym):
    d = und.get([sym])[sym]
    m = re.search(r"([\w:<>, ]+)::`scalar deleting dtor'", d)
    if m: return m.group(1).split(' ')[-1] + "::`scalar deleting destructor'"
    return und.qname(d) if d != sym else None
for q in Q:
    ch, a, fin, ev, ws, o = q[:6]
    if o not in ('respelled', 'respelled-partial'): continue
    t = a.lower().replace('0x', '').zfill(8)
    if not fin.startswith('?'): continue
    if '<' in fin:  # anonymous-namespace data symbol with the TU hash left open: the name without the hash
        m = re.match(r'\?(\w+)@\?A0x', fin); name = "`anonymous namespace'::" + m.group(1) if m else None
    else: name = qn(fin)
    if not name or (t, name) in seen: continue
    seen.add((t, name))
    if name in L.get(t, ()): continue
    out.append((t, name, 'naming wave 16 (%s %s, %s): %s; callers / own row respelled and rechecked (MATCH / RELOCATION_MATCH + PASS)' % (ch, o, 'queue', fin)))
EXTRA = [
 ('004cd9b0', 'CConversationAttitude::GetAnimName', 'naming wave 16 (c674 data): the catalog label GetAnimGroupName is wrong -- 0x004CD9B0 indexes ConversationAttitudeToAnimName (0x013B8A38) and its twin 0x004CD9C0 indexes ConversationAttitudeToAnimGroupName (0x013B8A44); re-authored as Ego_r ?GetAnimName@CConversationAttitude@@SIABVCCharString@@W4EConversationAttitude@@@Z, RELOCATION_MATCH + PASS'),
]
for t, n, e in EXTRA:
    if n not in L.get(t, ()) and (t, n) not in seen: out.append((t, n, e))
open(os.path.join(W, 'LABELS.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print(len(out) - 1, 'labels')
