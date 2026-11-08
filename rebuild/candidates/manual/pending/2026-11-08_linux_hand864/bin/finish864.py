import sys, os, csv, json, re
S = os.environ['S']; A = f'{S}/w22/agent864'
rows = {r['address'].lower(): r for r in csv.DictReader(open(f'{S}/batch/chunk864_oracle.tsv', encoding='utf-8-sig'), delimiter='\t')}
auth = []; orc = ['address\tname\tlength\tbytes']
# retail_asm-grade sources (owner rule 2026-10-24) live in retail_asm/<addr>.cpp with their test in test/<addr>.cpp (chunk 690)
srcs = [(d, f) for d in ('src', 'retail_asm') if os.path.isdir(f'{A}/{d}') for f in sorted(os.listdir(f'{A}/{d}')) if f.endswith('.cpp') and not f.endswith('.test.cpp')]
for d, f in sorted(srcs, key=lambda t: t[1]):
    a = f[:-4]; src = open(f'{A}/{d}/{f}').read(); test = open(f'{A}/test/{f}').read()
    q = re.search(r'^// ([^(\s]+?operator\(\)|[^(]+?)\(', src, re.M).group(1).split('::')   # `operator()(...)` keeps its name
    mod, name = '::'.join(q[:-1]) or '_global', q[-1]
    c = {'address': a, 'module': mod, 'name': name, 'source_cpp': src, 'test_cpp': test, 'pass_pattern': f'{a}_TEST PASS'}
    if name.startswith('~'): c['leaf'] = '??1' + name[1:]
    m = re.search(r'^// Flags: (.+)$', src, re.M)
    ml = re.search(r'^// Leaf: (\S+)$', src, re.M)
    if ml: c['leaf'] = ml.group(1)
    if m: c['flags'] = m.group(1).split()
    auth.append(c)
    r = rows[a]; b = bytes.fromhex(r['bytes'])
    while b and b[-1] in (0xcc, 0x90): b = b[:-1]
    orc.append(f"{a}\t{mod}::{name}\t{len(b)}\t{b.hex()}")
json.dump({'result': {'authored': auth}}, open(f'{A}/payload.json', 'w'), indent=1)
open(f'{A}/oracle.tsv', 'w').write('\n'.join(orc) + '\n')
print(len(auth), 'authored')
