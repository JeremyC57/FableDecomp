"""inv.py <tag>: implied retail CREATION order of our slots: retail final order (fmap offsets) pulled back through the
quicksort permutation of our own presort key sequence."""
import json, re, sys
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agent886/t8/fr')
import qs, rord
D = rord.D; tag = sys.argv[1]
ev = json.load(open(D + tag + '.json'))['events']
ps = [e for e in ev if e[0] == 'presort'][0]
pe = [e for e in ev if e[0] == 'packend'][0]
B = ps[2]; edi = ps[3]
def mk(s, pos): return {'pos': pos, 'mem': tuple(s[1] or []), 'size': s[2], 'w': s[3], 'flag': s[6]}
pre = [mk(s, k) for k, s in enumerate(ps[4])]
v = list(pre); qs.qsort(v, B, len(v) - 1, edi)
final_pos = {x['mem']: k for k, x in enumerate(v)}     # membership -> final index
# retail final order: rord rows (post-sort slots, by retail offset)
rows = {tuple(s[1] or []): r for s, r in zip(pe[4], rord.rows)}
items = [(rows[x['mem']]['ret'], x) for x in v[B:] if rows[x['mem']]['ret'] is not None]
items.sort(key=lambda t: -t[0])
# sigma: initial pos -> final pos ; retail final position k (among B..) -> implied initial = sigma^-1
sig_inv = {final_pos[x['mem']]: x['pos'] for x in pre}
out = []
for k, (ret, x) in enumerate(items):
    fpos = B + k
    implied = sig_inv.get(fpos)
    out.append((implied, x['pos'], ret, x, rows[x['mem']]))
out.sort(key=lambda t: t[0] if t[0] is not None else 999)
for implied, ours, ret, x, r in out:
    print(f"implied {implied!s:>4} ours {ours:4d} {'  ' if implied == ours else '<>'} sz {x['size']:3d} w {x['w']:4d} d {r['d']:6d} ret {ret:6d} " + ','.join(r['names'])[:70])
