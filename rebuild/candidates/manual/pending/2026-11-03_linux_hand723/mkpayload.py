# mkpayload.py <agentdir> <oracle.tsv> : build payload.json + oracle.tsv from matched src/ and retail_asm/ sources
import json, os, re, sys, glob
d, orc = sys.argv[1], sys.argv[2]
rows = {l.split('\t')[0]: l for l in open(orc).read().splitlines()[1:]}
hdr = open(orc).read().splitlines()[0]
out, keep = [], []
for f in sorted(glob.glob(d + '/src/*.cpp') + glob.glob(d + '/retail_asm/*.cpp')):
    a = os.path.basename(f)[:8]
    src = open(f).read()
    if not re.search(r'^// Grade: (RELOCATION_)?MATCH', src, re.M): continue
    leaf = re.search(r'^// Leaf: (\S+)', src, re.M).group(1)
    fl = re.search(r'^// Flags: (.*)$', src, re.M).group(1).split()
    name = rows[a].split('\t')[1]
    m = re.match(r"^// (\w+)::(~?\w+)", src)
    e = {'address': a, 'module': m.group(1), 'name': m.group(2), 'source_cpp': src,
         'test_cpp': open(f'{d}/test/{a}_test.cpp').read(), 'pass_pattern': f'{a}_TEST PASS', 'leaf': leaf}
    if fl and fl != ['none']: e['flags'] = fl
    out.append(e); keep.append(rows[a])
json.dump({'result': {'authored': out}}, open(d + '/payload.json', 'w'), indent=1)
open(d + '/oracle.tsv', 'w').write('\n'.join([hdr] + keep) + '\n')
print(len(out), [e['address'] for e in out])
