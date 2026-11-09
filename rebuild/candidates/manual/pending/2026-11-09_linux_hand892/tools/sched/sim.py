"""sim.py: re-implementation of VC7.1 c2.dll's list scheduler (0x1078734C family) on a DAG dumped by t13.py.
height (0x107b550a): h(end)=0, h(n) = 1 + max(h(s) + lat(n->s)); prio = ((h + outdeg) << 10) + (fl&4 ? 1<<15 : 0) + (store? 1)
ready list sorted by prio desc, ties idx asc (0x107b5309); per cycle up to 2 picks, one per unit (0x107b4f78), eligible iff
early <= cycle; after each pick the scan restarts at the head; succ early = max(early, cycle + lat)."""
import json, sys, copy
def load(path, pat=None):
    t = json.load(open(path))
    dags = [e[1] for e in t['events'] if e[0] == 'dag']
    return dags
def build(dag):
    N = {}
    for n in dag:
        N[n['idx']] = dict(idx=n['idx'], op=n['op'], fl=n['fl'], unit=n['unit'], succ=[(s[0], s[1]) for s in n['succ']])
    return N
def prio_of(N):
    order = sorted(N)
    h = {}
    for i in reversed(order):
        s = N[i]['succ']
        h[i] = 0 if not s and N[i]['op'] is None and i == order[-1] else (1 + max([h[t] + l for t, l in s], default=-1))
    P = {}
    for i in order:
        n = N[i]
        p = (h[i] + len(n['succ'])) << 10
        if n['fl'] & 4: p += 1 << 15
        if (n['fl'] & 8) and n['op'] in ('fstp', 'fst', 'fistp'): p += 1
        P[i] = p
    return P, h
def schedule(N, width=2, verbose=False):
    P, h = prio_of(N)
    order = sorted(N)
    preds = {i: 0 for i in order}
    for i in order:
        for t, l in N[i]['succ']: preds[t] += 1
    early = {i: 0 for i in order}
    first, last = order[0], order[-1]
    ready = []
    def ins(i):
        ready.append(i); ready.sort(key=lambda k: (-P[k], k))
    def release(i, cyc):
        for t, l in N[i]['succ']:
            early[t] = max(early[t], cyc + l); preds[t] -= 1
            if preds[t] == 0 and t != last: ins(t)
    release(first, 0)
    out = []; cyc = 0
    while ready and cyc < 1000:
        picked = []
        while len(picked) < width:
            got = None
            for i in ready:
                if early[i] > cyc: continue
                if any(N[j]['unit'] == N[i]['unit'] for j in picked): continue
                got = i; break
            if got is None: break
            ready.remove(got); picked.append(got); out.append((cyc, len(picked) - 1, got)); release(got, cyc)
        if verbose and picked: print('c%d' % cyc, ' | '.join('%s#%d' % (N[i]['op'], i) for i in picked))
        cyc += 1
    return out, P, h
if __name__ == '__main__':
    dags = load(sys.argv[1])
    for d in dags:
        N = build(d)
        out, P, h = schedule(N, verbose=True)
        print(' '.join('%s#%d' % (N[i]['op'], i) for c, s, i in out))
        print({i: hex(P[i]) for i in sorted(P)})
