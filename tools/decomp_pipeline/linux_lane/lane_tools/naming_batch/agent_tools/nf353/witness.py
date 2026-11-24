# witness.py <addr...>: drop test part 1 (NF340 scope line): reloc_bindings.tsv lines whose witness is the row and whose symbol the new
# object no longer names at all (neither DEF nor UND) -> would need a rewitness. Prints per row the count and the lines. Agent NF353.
import sys, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); R = str(nf.R)
wit = {}
for l in open(R + '/rebuild/oracles/reloc_bindings.tsv', encoding='utf-8').read().splitlines()[1:]:
    f = l.split('\t')
    if len(f) >= 3: wit.setdefault(f[2].lower(), []).append((f[0], f[1]))
tot = 0
for a in sys.argv[1:]:
    hn = {x[4:] for x in nf.ext_syms(A + '/work/obj/H_%s.obj' % a)}; nn = {x[4:] for x in nf.ext_syms(A + '/work/obj/N_%s.obj' % a)}
    lost = [(s, t) for s, t in wit.get(a, []) if s in hn and s not in nn]
    tot += len(lost)
    print('%s\twitness_lines=%d\tlost=%d\t%s' % (a, len(wit.get(a, [])), len(lost), ';'.join('%s->%s' % (s[:80], t) for s, t in lost)))
print('TOTAL_LOST', tot)
