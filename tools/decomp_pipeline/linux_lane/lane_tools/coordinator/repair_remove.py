#!/usr/bin/env python3
"""Remove landed rows so a respelled / rebased source can be re-landed with vland.py --land.

    python3 repair_remove.py <addr> [<addr> ...]      (repo root)

For each address: `git rm` its source and test, drop its block from rebuild/build_candidates.ps1, and drop every
reloc_bindings.tsv row whose CALLER is that address (vland --land writes the new ones). The catalog row stays.
Re-land right after: fixmove.py, keypayload.py, vland.py <payload> <oracle> --land, bindings_multi_check.py (must print 0).
"""
import re, subprocess, sys
ps = 'rebuild/build_candidates.ps1'
t = open(ps, encoding='utf-8', newline='').read()
b = 'rebuild/oracles/reloc_bindings.tsv'
L = open(b).read().split('\n')
for a in [x.lower() for x in sys.argv[1:]]:
    m = re.search(r"\n    \[pscustomobject\]@\{\s*Address = '%s'.*?\n    \}" % a, t, re.S)
    if not m:
        sys.exit('not landed: ' + a)
    blk = m.group(0)
    src = re.search(r"Source = '([^']*)'", blk).group(1)
    tst = re.search(r"TestSource = '([^']*)'", blk).group(1)
    subprocess.run(['git', 'rm', '-q', 'rebuild/src/compiled/' + src, 'rebuild/tests/' + tst], check=True)
    t = t.replace(blk, '', 1)
    n = len(L)
    L = [l for l in L if not (len(l.split('\t')) > 2 and l.split('\t')[2] == a)]
    print('removed', a, src, 'bindings', n - len(L))
open(ps, 'w', encoding='utf-8', newline='').write(t)
open(b, 'w').write('\n'.join(L))
