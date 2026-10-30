# cluster2.py <Class> <filelist> <out> [n]: like cluster.py but via common.find_block (template prefixes allowed)
import sys, re, hashlib, collections, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); from common import find_block, norm
cls, lst, out = sys.argv[1:4]
groups = collections.defaultdict(list); first = {}
for f in open(lst).read().split():
    t = open(f, encoding='utf-8', errors='replace').read(); b = find_block(t, cls)
    if b is None: k = "NOBLOCK"
    else:
        n = norm(t[b[0]:b[1]]); k = hashlib.md5(n.encode()).hexdigest()[:8]; first.setdefault(k, n)
    groups[k].append(f)
with open(out, 'w') as o:
    for k, v in sorted(groups.items(), key=lambda kv: -len(kv[1])):
        for f in v: o.write(f"{k}\t{f}\n")
for k, v in sorted(groups.items(), key=lambda kv: -len(kv[1]))[:int(sys.argv[4]) if len(sys.argv) > 4 else 15]:
    print(len(v), k, first.get(k, '')[:700])
