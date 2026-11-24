#!/usr/bin/env python3
"""select_rows.py <outdir> : the audit row sets (agent NF340).
d2inl.txt  every catalog entry whose CompilerFlags contain /d2inl
sample.txt seed-340 shuffle of the other landed catalog entries (sorted by address first), first 300 that are compilable
           (not toolchain_library, genuine per crawl/purity, present in auto-re-candidates.tsv); skips logged in sample_skips.tsv.
Run from any cwd with python3 -I -B; reads the repo read-only."""
import os, re, sys, random
ROOT = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp')
sys.path.insert(0, os.path.join(ROOT, 'tools', 'decomp_pipeline')); sys.dont_write_bytecode = True
from crawl import purity
out = sys.argv[1]
ps = open(os.path.join(ROOT, 'rebuild/build_candidates.ps1'), encoding='utf-8-sig').read()
ent = {}
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if 'Address' in kv: ent[kv['Address'].lower()] = kv
cat = set(l.split('\t', 1)[0].lower() for l in open(os.path.join(ROOT, 'rebuild/oracles/auto-re-candidates.tsv')).read().splitlines()[1:])
d2 = sorted(a for a, kv in ent.items() if '/d2inl' in kv.get('CompilerFlags', ''))
rest = sorted(a for a in ent if a not in set(d2))
random.Random(340).shuffle(rest)
samp, skips = [], []
for a in rest:
    if len(samp) >= 300: break
    kv = ent[a]
    if a not in cat: skips.append((a, 'not in auto-re-candidates')); continue
    try: src = open(os.path.join(ROOT, 'rebuild/src/compiled', kv['Source']), encoding='utf-8').read()
    except Exception as ex: skips.append((a, f'source unreadable {type(ex).__name__}')); continue
    k = purity.classify(src)
    if k == 'toolchain_library': skips.append((a, 'toolchain_library')); continue
    if not purity.is_genuine_kind(k): skips.append((a, f'not genuine ({k})')); continue
    samp.append(a)
open(f'{out}/d2inl.txt', 'w').write('\n'.join(d2) + '\n')
open(f'{out}/sample.txt', 'w').write('\n'.join(samp) + '\n')
open(f'{out}/sample_skips.tsv', 'w').write('address\treason\n' + ''.join(f'{a}\t{r}\n' for a, r in skips))
print(len(ent), 'catalog entries;', len(d2), '/d2inl;', len(rest), 'population for the sample;', len(samp), 'sampled;', len(skips), 'skipped')
