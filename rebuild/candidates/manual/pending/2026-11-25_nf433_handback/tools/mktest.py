#!/usr/bin/env python3
"""mktest.py <W> <dir> <tspec>: tests/<a>_test.cpp from blocks 'addr|what the test checks' + C++ text (blocks split by '@@').
The prelude is the row's own #define / #include lines, verbatim (so the classes come from the same shared headers); `@PASS@` becomes
the pass line printing `PASS_<a>` and the HEAD pass_pattern (passpat.tsv); `@FAIL@` the fail line."""
import sys, os, re
W, d, spec = sys.argv[1:4]
pp = {l.split('\t')[0]: l.split('\t')[1] for l in open(W + '/passpat.tsv').read().splitlines()}
os.makedirs(d + '/tests', exist_ok=True)
for blk in open(spec).read().split('\n@@\n'):
    L = blk.strip('\n').split('\n')
    if not L or not L[0].strip(): continue
    a, what = L[0].split('|', 1)
    row = open(f'{d}/rows/{a}.cpp').read()
    pre = [l for l in row.splitlines() if l.startswith('#define ') or l.startswith('#include ')]
    m = re.search(r'^// (\S+?)\(', row, re.M)
    head = [f'// Behaviour test for 0x{a.upper()} {m.group(1) if m else ""} (NF433): links the row object (the row is called by its own',
            '// symbol: a qualified call, not through the vftable). No class body is defined here: every class comes from the row\'s shared',
            '// headers (its prelude below, verbatim); callees are defined here as out-of-line members that record their calls.',
            '// ' + what]
    body = '\n'.join(L[1:]).replace('@PASS@', f'printf("PASS_{a} {pp[a]}\\n"); return 0;').replace('@FAIL@', f'{{ printf("FAIL_{a}\\n"); return 1; }}')
    open(f'{d}/tests/{a}_test.cpp', 'w').write('\n'.join(head + pre + ['#include <stdio.h>', '#include <string.h>', '']) + body.strip('\n') + '\n')
    print(a, 'test ok')
