#!/usr/bin/env python3
"""census.py <dir> <addr...>: other landed definers of each row's new self symbol: self_symbols.tsv lines at other addresses, `// Leaf:` lines in
other landed rows, and out-of-line `CGameScriptInterface::<M>(` definitions (not in a class body) in other landed sources. Tests are excluded."""
import sys, re, os, subprocess
R = '/home/user/FableDecomp'; d = sys.argv[1]
ss = [l.split('\t') for l in open(R + '/rebuild/oracles/self_symbols.tsv').read().splitlines()[1:]]
for a in sys.argv[2:]:
    leaf = re.search(r'^// Leaf: (\S+)', open(f'{d}/rows/{a}.cpp').read(), re.M).group(1)
    m = re.match(r'\?(\w+)@', leaf).group(1)
    hits = [f'self_symbols {x[0]}' for x in ss if len(x) > 1 and x[1] == leaf and x[0] != a]
    out = subprocess.run(['grep', '-rlF', leaf, R + '/rebuild/src/compiled'], capture_output=True, text=True).stdout.split()
    hits += ['leaf ' + os.path.basename(o) for o in out if not o.endswith(f'_{a}.cpp')]
    out = subprocess.run(['grep', '-rlE', r'CGameScriptInterface::%s\s*\(' % m, R + '/rebuild/src/compiled'], capture_output=True, text=True).stdout.split()
    hits += ['body ' + os.path.basename(o) for o in out if not o.endswith(f'_{a}.cpp')]
    print(a, leaf, 'NONE' if not hits else ' | '.join(hits))
