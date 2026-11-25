import re, sys, collections, os
R = '/home/user/FableDecomp'; S = os.environ['S']
rows = [l.rstrip('\n').split('\t') for l in open(S + '/nf425_test_debt.txt') if not l.startswith('#')]
sig = collections.defaultdict(list)
for kind, a, tp, loc in rows:
    t = open(R + '/' + tp).read()
    names = set(re.findall(r'\b(C[A-Z]\w+)\b', t))
    n = t
    for nm in sorted(names, key=len, reverse=True): n = re.sub(r'\b%s\b' % nm, 'K', n)
    n = re.sub(r'0x[0-9A-Fa-f]+|\b\d+\b', 'N', n); n = re.sub(r'[0-9a-f]{8}', 'A', n)
    n = '\n'.join(l for l in n.splitlines() if not l.startswith('//') and 'alternatename' not in l)
    n = re.sub(r'\n    (bool|long|float|unsigned char|char|short|unsigned long|void\*|int|unsigned int|unsigned short) [^\n]*;', '\n    M;', n)
    n = re.sub(r'(\n    M;)+', '\n    M;', n)
    sig[n].append((a, kind, tp.split('/')[-1]))
out = sorted(sig.items(), key=lambda x: -len(x[1]))
print(len(out), 'shapes')
for i, (k, v) in enumerate(out):
    open(S + '/w22/agentNF427/work/tb/shape%02d.txt' % i, 'w').write(k + '\n\n' + '\n'.join('\t'.join(x) for x in v) + '\n')
    print(i, len(v), v[0][2])
