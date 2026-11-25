import sys, glob, os, re, collections
E = sys.argv[1]; out = []; tot = collections.Counter()
def rd(p):
    d = {}
    for l in open(p):
        if l.startswith('#') or not l.strip(): continue
        q = l.rstrip('\n').split('\t'); d[re.sub(r'\?A0x[0-9a-f]{8}', '?A0x', q[-1])] = (q[0], q[2], q[7], q[8])
    return d
for h in sorted(glob.glob(E + '/H_*.pkall.tsv')):
    a = os.path.basename(h)[2:10]; H = rd(h); N = rd(E + '/N_%s.pkall.tsv' % a)
    flips = []; added = []; dropped = []
    for k in set(H) | set(N):
        if k in H and k in N:
            if H[k] != N[k]: flips.append('%s %s->%s' % (k[:60], H[k], N[k]))
        elif k in N: added.append('%s:%s' % (N[k][0], k[:50])); tot['added ' + N[k][0] + ' ' + ('LB' if 'LowerBound' in k else 'Find' if '?Find@' in k else 'begin/end' if re.match(r'\?(begin|end)@', k) else k[:40])] += 1
        else: dropped.append('%s:%s' % (H[k][0], k[:50])); tot['dropped ' + H[k][0] + ' ' + k[:50]] += 1
    out.append('%s\tflips=%d\t%s\tadded=%s\tdropped=%s' % (a, len(flips), ' | '.join(flips), ' '.join(sorted(added)), ' '.join(sorted(dropped))))
print('\n'.join(x[:400] for x in out)); print(); [print(v, k) for k, v in tot.most_common()]
