# cluster.py <Class> <filelist> <out>: hash each file's local class block (comments/whitespace stripped)
import sys, re, hashlib, collections
sys.path.insert(0, __file__.rsplit('/',1)[0]); from extract_class import block
cls, lst, out = sys.argv[1:4]
groups = collections.defaultdict(list); first = {}
for f in open(lst).read().split():
    b = block(open(f, encoding='utf-8', errors='replace').read(), cls)
    if b is None: k = "NOBLOCK"
    else:
        n = re.sub(r'//[^\n]*', '', b); n = re.sub(r'\s+', ' ', n).strip()
        k = hashlib.md5(n.encode()).hexdigest()[:8]; first.setdefault(k, n)
    groups[k].append(f)
with open(out, 'w') as o:
    for k, v in sorted(groups.items(), key=lambda kv: -len(kv[1])):
        for f in v: o.write(f"{k}\t{f}\n")
for k, v in sorted(groups.items(), key=lambda kv: -len(kv[1]))[:int(sys.argv[4]) if len(sys.argv) > 4 else 15]:
    print(len(v), k, first.get(k, '')[:400])
