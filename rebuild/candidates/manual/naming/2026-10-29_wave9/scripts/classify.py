# classify every (row, bound wrong spelling -> final) of analysis.json by the demangled difference
import sys, os, json, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import und
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
A = json.load(open(os.path.join(W, 'analysis.json')))
syms = set()
for a, o in A.items(): syms.add(o['final']); syms |= set(o['wrong'])
U = und.get(sorted(syms))
ACC = re.compile(r'^(public|protected|private): ')
def parts(d):
    m = ACC.match(d); acc = m.group(1) if m else ''
    s = d[m.end():] if m else d
    cst = s.endswith(') const')
    if cst: s = s[:-6]
    return acc, cst, s
def cls(w, f):
    dw, df = U[w], U[f]
    if dw == w or df == f: return 'undemangled'
    aw, cw, sw = parts(dw); af, cf, sf = parts(df)
    k = []
    if aw != af: k.append('access')
    if cw != cf: k.append('const')
    if sw == sf: return '+'.join(k) or 'same'
    # class key only
    if re.sub(r'\b(class|struct) ', '', sw) == re.sub(r'\b(class|struct) ', '', sf): return '+'.join(k + ['key'])
    if sw.replace('virtual ', '') == sf.replace('virtual ', ''): return '+'.join(k + ['virtual'])
    return 'other'
out = []
C = collections.Counter(); CR = collections.Counter()
for a, o in sorted(A.items()):
    for w, rs in o['wrong'].items():
        c = cls(w, o['final']); C[(o['src'].split(':')[0], c)] += 1; CR[(o['src'].split(':')[0], c)] += len(rs)
        out.append((a, o['src'], c, len(rs), w, o['final'], ','.join(rs)))
open(os.path.join(W, 'classes.tsv'), 'w').write(''.join('\t'.join(map(str, r)) + '\n' for r in out))
for k in sorted(C): print(k, C[k], 'rows', CR[k])
