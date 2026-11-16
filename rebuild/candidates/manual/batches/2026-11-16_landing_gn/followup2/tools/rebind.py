# rebind.py <installed-addrs.txt> <resigned.txt> <wrong_worklist.tsv> [--write]: update reloc_bindings.tsv for re-authored rows.
import os, sys, subprocess, re, collections
S = os.environ['S']
inst = open(sys.argv[1]).read().split(); resigned = set(open(sys.argv[2]).read().split()) | {'00686800'}
P = 'rebuild/oracles/reloc_bindings.tsv'
L = open(P, encoding='utf-8').read().split('\n')
now = {}                                   # row -> {target: symbol}
for a in inst:
    f = subprocess.run(['ls'] + [x for x in __import__('glob').glob('rebuild/src/compiled/*/*/*_%s.cpp' % a)], capture_output=True, text=True).stdout.strip()
    src = open(f, encoding='utf-8').read(); m = re.search(r'^// Leaf:\s*(\S+)', src, re.M)
    out = subprocess.run(['python3', S + '/fu3/xref.py', a, '%s/wine/q/%s/%s.obj' % (S, a, a), m.group(1) if m else ''], capture_output=True, text=True).stdout
    d = {}
    for l in out.splitlines()[1:]:
        r = l.split('\t')
        if len(r) == 4 and r[2] in resigned: d[r[2]] = r[3]
    now[a] = d
changed = 0; present = set()
for i, l in enumerate(L):
    r = l.split('\t')
    if len(r) != 3 or r[0] == 'symbol': continue
    if r[2] in now and r[1] in now[r[2]] and now[r[2]][r[1]] != r[0]:
        L[i] = '\t'.join([now[r[2]][r[1]], r[1], r[2]]); changed += 1
for l in L:
    r = l.split('\t')
    if len(r) == 3: present.add(r[0])
add = []
for a in sorted(now):
    for t, s in sorted(now[a].items()):
        if s not in present: add.append('\t'.join([s, t, a])); present.add(s)
# wrong symbols still emitted by rows not re-authored
for l in open(sys.argv[3]):
    r = l.rstrip('\n').split('\t')
    if r[0] == 'target' or r[1] in now: continue
    if r[3] not in present: add.append('\t'.join([r[3], r[0], r[1]])); present.add(r[3])
print('rows', len(now), 'lines respelled', changed, 'lines added', len(add))
for x in add[:12]: print('  +', x[:150])
if '--write' in sys.argv:
    while L and L[-1] == '': L.pop()
    open(P, 'w', encoding='utf-8').write('\n'.join(L + add) + '\n')
