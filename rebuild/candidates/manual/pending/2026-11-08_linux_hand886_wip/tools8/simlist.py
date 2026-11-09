"""simlist.py <tag>: replay the 0x10788642 insert/weight calls and compare with c2's list at packer entry."""
import json, sys
D = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886/t8/fr/'
def simulate(wev, size_of):
    lst = []          # list of nids
    wt = {}
    for nid, inc in wev:
        sz = size_of[nid]
        if nid in wt:
            wt[nid] += inc
            p = lst.index(nid)
            q = p - 1
            while q >= 0 and size_of[lst[q]] == sz and wt[lst[q]] < wt[nid]:
                q -= 1
            # place after q
            lst.pop(p); lst.insert(q + 1, nid)
        else:
            wt[nid] = inc
            # after the last node with size <= sz
            q = len(lst) - 1
            while q >= 0 and size_of[lst[q]] > sz: q -= 1
            lst.insert(q + 1, nid)
    return lst, wt
if __name__ == '__main__':
    tag = sys.argv[1]
    ev = json.load(open(D + tag + '.json'))['events']
    pk = [e for e in ev if e[0] == 'pack'][0]
    real = [x[0] for x in pk[3]]
    size_of = {x[0]: x[5] for x in pk[3]}
    wev = []
    on = False
    for e in ev:
        if e[0] == 'pipe': on = 'Action@CManager' in (e[1] or '')
        if e[0] == 'w' and on: wev.append((e[1], e[2])); size_of.setdefault(e[1], e[5])
    lst, wt = simulate(wev, size_of)
    print('real', len(real), 'sim', len(lst), 'equal', lst == real)
    for i, (a, b) in enumerate(zip(real, lst)):
        if a != b: print('first diff at', i, a, b); break
