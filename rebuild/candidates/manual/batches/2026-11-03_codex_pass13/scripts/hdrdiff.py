#!/usr/bin/env python3
# For each restored pass-13 row context: which headers are new, identical to ours, or changed; which macros each source defines.
import os, sys, re, hashlib, collections
C, REPO = sys.argv[1], 'rebuild/include'
out = open(f'{C}/hdr_pairs.tsv', 'w'); out.write('address\theader\tkind\n')
summ = collections.Counter(); per_hdr = collections.defaultdict(set); macros = collections.Counter(); rowmac = {}
for a in sorted(os.listdir(f'{C}/hdr')):
    root = f'{C}/hdr/{a}'
    for dp, _, fs in os.walk(root):
        for f in fs:
            p = os.path.join(dp, f); rel = os.path.relpath(p, root); ours = os.path.join(REPO, rel)
            if not os.path.exists(ours): k = 'new'
            elif open(p, 'rb').read() == open(ours, 'rb').read(): k = 'same'
            else: k = 'changed'
            summ[k] += 1
            if k != 'same':
                out.write(f'{a}\t{rel}\t{k}\n'); per_hdr[(rel, k)].add(hashlib.sha256(open(p, 'rb').read()).hexdigest())
    src = open(f'{C}/payloads/{a}.json').read()
    ms = set(re.findall(r'#define (FABLE_RETAIL_\w+)', src.replace('\\n', '\n')))
    rowmac[a] = ms; macros.update(ms)
print('pairs', dict(summ))
ch = sorted(((r, k, len(v)) for (r, k), v in per_hdr.items()), key=lambda x: -x[2])
print('distinct changed/new headers', len(ch), ' with >1 version:', sum(1 for x in ch if x[2] > 1))
for r, k, n in ch[:25]: print(f'  {k:8} {n} versions  {r}')
print('source macros:', len(macros)); print('  ', ' '.join(f'{m[13:]}:{n}' for m, n in macros.most_common(60)))
