#!/usr/bin/env python3
"""scope438.py <RESIDUE.tsv> <out_prefix>: SCOPE_ONLY generator. A wrong spelling ?m@S@@<tail> whose PDB spelling is ?m@Q@@<tail>
(same member, same tail, S and Q single plain identifiers) where S is a LOCAL class of the witness row: rename S -> Q in that row's
text (whole-word). No shared header is included (the d1 pilot showed that route changes bytes). Every other symbol the rename touches
is decided by scopegate438.py from the objects (each changed relocation must land on a target whose table / PDB name is the new symbol).
Screens: S defined exactly once in the row (`struct S` / `class S` with a body), Q not named anywhere in the row, the row not in an
ACCEPT batch, lock free or NF438, not in T3 col 2/3. One rename per row (S -> Q pairs that disagree for one row are residue).
Writes <out>_PLAN.tsv (address, S, Q, old, new, target), <out>_rows/, <out>_RESIDUE.tsv."""
import sys, os, re, csv, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
R = '/home/user/FableDecomp'; A = str(nf.A); S_ = str(nf.S)
res_in, outp = sys.argv[1:3]
acc = set()
for b in os.listdir(A + '/batches'):
    if not b.endswith('_hold') and os.path.exists(A + '/batches/%s/rows.txt' % b): acc |= set(open(A + '/batches/%s/rows.txt' % b).read().split())
t3 = set()
for l in open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv').read().splitlines()[1:]:
    q = l.split('\t'); t3 |= {x.lower() for x in re.findall(r'0x([0-9a-fA-F]{8})', ' '.join(q[1:3]))}
def lockfree(a):
    p = S_ + '/w22/nflocks/%s/owner' % a
    return (not os.path.exists(p)) or open(p).read().strip() in ('released', 'NF443')
want = collections.defaultdict(list); out_res = []
for t, s, w, why in list(csv.reader(open(res_in), delimiter='\t'))[1:]:
    if not why.startswith('SCOPE_ONLY'): continue
    p = re.search(r'\(PDB (\S+)\)', why).group(1)
    ms = re.match(r'^\?(\w+)@(\w+)@@', s); mq = re.match(r'^\?(\w+)@(\w+)@@', p)
    if not ms or not mq or ms.group(1) != mq.group(1): out_res.append((t, s, w, 'SCOPE_ONLY: scope not a single plain identifier on both sides')); continue
    want[w].append((ms.group(2), mq.group(2), s, p, t))
plan = []
for w, L in sorted(want.items()):
    pairs = {(x[0], x[1]) for x in L}
    def r(why):
        for x in L: out_res.append((x[4], x[2], w, 'SCOPE_ONLY: ' + why))
    if len(pairs) != 1: r('row needs more than one scope rename'); continue
    Sx, Q = pairs.pop(); src = nf.src_of(w)
    if not src: r('no landed row at the witness'); continue
    x = open(src, encoding='utf-8').read()
    if w in acc: r('row is in an NF438 ACCEPT batch'); continue
    if not lockfree(w): r('locked by another agent'); continue
    if w in t3: r('row named in T3_PORT_QUEUE col 2/3'); continue
    if len(re.findall(r'\b(struct|class)\s+%s\b[^;{]*\{' % re.escape(Sx), x)) != 1: r('stand-in %s is not defined exactly once in the row (header model or forward only)' % Sx); continue
    if re.search(r'\b%s\b' % re.escape(Q), x): r('row already names %s' % Q); continue
    new = re.sub(r'\b%s\b' % re.escape(Sx), Q, x)
    note = ('// Naming (NF438 scope gen, 2026-11-25): the local model %s is renamed to its retail class %s, so its member calls bind the PDB\n'
            '//   spellings (%s); every other symbol the rename touches is confirmed against the table by its retail target (scopegate423).\n') % (Sx, Q, ', '.join(sorted({y[3] for y in L}))[:300])
    new = note + new
    os.makedirs(outp + '_rows', exist_ok=True); open(outp + '_rows/%s.cpp' % w, 'w', encoding='utf-8').write(new)
    for y in L: plan.append((w, Sx, Q, y[2], y[3], y[4]))
with open(outp + '_PLAN.tsv', 'w') as f:
    for x in plan: f.write('\t'.join(x) + '\n')
with open(outp + '_RESIDUE.tsv', 'w') as f:
    for x in out_res: f.write('\t'.join(x) + '\n')
print('rows', len({x[0] for x in plan}), 'plan lines', len(plan), 'residue', len(out_res)); print(collections.Counter(x[3].split(':')[1].strip()[:60] for x in out_res).most_common(10))
