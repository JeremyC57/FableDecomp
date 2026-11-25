#!/usr/bin/env python3
"""mkpay.py <W> <tag> <rowsdir> <addr...>: from the HEAD payload work/p_<tag>h (recheck.py) write work/p_<tag>n (source_cpp = rowsdir/rows/<a>.cpp,
test_cpp = rowsdir/tests/<a>_test.cpp, leaf = the `// Leaf:` line of the new row) and work/p_<tag>m (source_cpp = rowsdir/mut/<a>.cpp).
The HEAD pass_pattern must be present in the new test (kept unchanged)."""
import json, sys, os, re, shutil
W, tag, rd = sys.argv[1:4]
h = json.load(open(f'{W}/work/p_{tag}h/payload.json'))
for kind in 'nm':
    n = json.loads(json.dumps(h))
    for x in n['result']['authored']:
        a = x['address']
        src = open(f'{rd}/rows/{a}.cpp', encoding='utf-8').read()
        tst = open(f'{rd}/tests/{a}_test.cpp', encoding='utf-8').read()
        assert x['pass_pattern'] in tst, (a, x['pass_pattern'])
        leaf = re.search(r'^//\s*Leaf:\s*(\S+)', src, re.M).group(1)
        x['leaf'] = leaf; x['test_cpp'] = tst
        x['source_cpp'] = src if kind == 'n' else open(f'{rd}/mut/{a}.cpp', encoding='utf-8').read()
    os.makedirs(f'{W}/work/p_{tag}{kind}', exist_ok=True)
    json.dump(n, open(f'{W}/work/p_{tag}{kind}/payload.json', 'w'))
    shutil.copy(f'{W}/work/p_{tag}h/oracle.tsv', f'{W}/work/p_{tag}{kind}/oracle.tsv')
print('payloads', len(h['result']['authored']))
