# triage338.py (NF338): TRIAGE.tsv = per ENTRIES target: T3 tag(s) + where (col2 / unit list / prose), the recorded reason (full text),
# the per-binder state on today's tree (stale2.tsv: STANDIN_LEFT = stand-in identifier still in the binder's code; SIG_ONLY = signature-only
# difference; NONE_LEFT = stand-in gone from the binder's source), the binder flags (OFF = reserved elsewhere), work/verdict.tsv overrides
# (compiled witness objects), and a verdict: holds / holds(binder reserved) / stale-table / reread-needed.
import re, os, collections
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); R = '/home/user/FableDecomp'
E = [l.rstrip('\n').split('\t') for l in open(A + '/ENTRIES.tsv', encoding='utf-8')][1:]
RR = {l.split('\t')[0]: l.rstrip('\n').split('\t') for l in list(open(A + '/work/reread.tsv'))[1:]}
Q = [l.rstrip('\n').split('\t') for l in open(R + '/rebuild/candidates/manual/naming/T3_PORT_QUEUE.tsv', encoding='utf-8')][1:]
why = {l.split('\t')[0]: l.split('\t')[1].strip() for l in open(A + '/work/offlimits_why.tsv')}
V = {}
if os.path.exists(A + '/work/verdict.tsv'):
    for l in open(A + '/work/verdict.tsv'):
        p = l.rstrip('\n').split('\t')
        if len(p) >= 3: V[p[0]] = (p[1], p[2])
B = collections.defaultdict(list)
for l in open(A + '/work/stale2.tsv'):
    i, t, w, st, x, p = l.rstrip('\n').split('\t'); B[i].append((w, st, x))
o = open(A + '/TRIAGE.tsv', 'w')
o.write('idx\ttarget\tt3_tags\tt3_where\tunit_kind\tbinders(witness:state[:flags])\tverdict\tbasis\trecorded_reason\n')
for i, e in enumerate(E, 1):
    t = e[0]; r = RR[str(i)]
    full = ' || '.join('%s: %s :: %s' % (q[0], q[2], q[3] if len(q) > 3 else '') for q in Q if len(q) > 1 and q[1].lower() == '0x' + t)
    if not full: full = r[8]
    bs = []; states = []
    for w, st, x in B.get(str(i), []):
        f = ('OFF' if w in why else '')
        bs.append('%s:%s%s' % (w, st, ':' + f if f else '')); states.append((st, f, w))
    if str(i) in V: verdict, basis = V[str(i)]
    elif r[4] == 'col3prose': verdict, basis = 'prose-only', 'named only in another line\'s prose (coordinator: list in NOTES, do not take)'
    elif not states: verdict, basis = 'holds', 'no wrong table line left besides the self / PDB line; the T3 item is a self port'
    elif all(f for st, f, w in states): verdict, basis = 'holds(binder reserved)', 'every wrong-spelling witness is itself reserved (T3 / other agent)'
    elif any(st == 'STANDIN_LEFT' and not f for st, f, w in states) and not any(st in ('SIG_ONLY', 'NONE_LEFT') and not f for st, f, w in states):
        verdict, basis = 'holds', 'stand-in identifier still in every free binder\'s code'
    else: verdict, basis = 'reread-needed', 'a free binder has only a signature difference or its stand-in is gone: compile it'
    o.write('\t'.join([str(i), t, r[5], r[4], r[6], ' '.join(bs) or '-', verdict, basis, full.replace('\t', ' ')]) + '\n')
o.close()
c = collections.Counter(l.split('\t')[6] for l in list(open(A + '/TRIAGE.tsv'))[1:]); print(dict(c))
