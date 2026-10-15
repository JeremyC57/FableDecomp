import sys, os, csv, json, re
S = os.environ['S']; A = f'{S}/w22/agentNN'
rows = {r['address'].lower(): r for r in csv.DictReader(open(f'{S}/w22/chunkNN.tsv', encoding='utf-8-sig'), delimiter='\t')}
auth = []; orc = ['address\tname\tlength\tbytes']
for f in sorted(os.listdir(f'{A}/src')):
    a = f[:-4]; src = open(f'{A}/src/{f}').read(); test = open(f'{A}/test/{f}').read()
    q = re.search(r'^// (.+?)\(', src, re.M).group(1).split('::')
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
