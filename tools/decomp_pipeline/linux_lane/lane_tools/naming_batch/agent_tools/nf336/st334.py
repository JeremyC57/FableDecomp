# st334.py: STATUS lines (work/st_script.tsv) for the HELD / PORTC entries of work/class.tsv not already in work/st.tsv:
#   HELD  -> skipped-held: every wrong table line's witness is off limits (the why tags from work/offlimits_why.tsv)
#   PORTC -> skipped-port: every reachable wrong line binds through stand-in scopes found in neither PDB name set (NF299 Q4 census form)
import os, collections
A = os.path.dirname(os.path.dirname(os.path.abspath(__file__))); W = A + '/work'
E = [l.rstrip('\n').split('\t') for l in list(open(A + '/ENTRIES.tsv'))[1:]]
done = set(l.split('\t')[0] for l in open(W + '/st.tsv'))
why = dict(l.rstrip('\n').split('\t', 1) for l in open(W + '/offlimits_why.tsv'))
L = collections.defaultdict(list)
for l in open(W + '/tri_b.tsv'):
    q = l.rstrip('\n').split('\t'); L[int(q[0])].append(q)
out = open(W + '/st_script.tsv', 'w'); n = collections.Counter()
for l in open(W + '/class.tsv'):
    i, t, c = l.split('\t')[:3]
    if i in done or c not in ('HELD', 'PORTC'): continue
    e = E[int(i) - 1]; ls = L[int(i)]
    held = sorted(set('%s(%s)' % (q[2], why.get(q[2], q[3]).replace('\t', ' ')) for q in ls if q[3] != '-'))
    if c == 'HELD':
        st = 'skipped-held'; r = 'every wrong table line\'s witness is off limits: ' + ', '.join(held)
    else:
        sc = sorted(set(s for q in ls if q[3] == '-' for s in q[6].split(',') if s and s != '-'))
        fr = sorted(set(q[2] for q in ls if q[3] == '-'))
        st = 'skipped-port'; r = 'census (NF299 Q4): the reachable binder(s) %s bind through stand-in scope(s) %s found in neither PDB name set: a class port (T3)' % (', '.join(fr), ', '.join(sc))
        if held: r += '; off limits: ' + ', '.join(held)
    wl = ','.join(sorted(set('0x' + q[2].upper() for q in ls)))
    out.write('\t'.join([i, '0x' + t.upper(), e[6], 'T1 other', st, wl, '-', r]) + '\n'); n[st] += 1
print(n)
