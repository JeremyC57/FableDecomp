# prec.py <idx..>: per ENTRIES line, landed precedent rows that define a member of the class under the same class-model switch
# (smallest first), with their #define / #include preamble. -> stdout
import re, sys, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF327')
R = Path('/home/user/FableDecomp')
T = {}
for l in open(A / 'work/triage.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) > 6 and p[5] == 'DECL': T.setdefault(int(p[0]), (p[2], p[3], p[6]))
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
nmax = int(__import__('os').environ.get('NPREC', '3'))
for i in [int(x) for x in sys.argv[1:]]:
    if i not in T: print('#%d no DECL triage' % i); continue
    X, M, st = T[i]; sw = re.findall(r'#ifdef (\w+)', st)
    r = subprocess.run(['grep', '-rlE', r'(^|[^\w])' + X + r'::~?\w+\s*\(', str(R / 'rebuild/src/compiled'), '--include=*.cpp'], capture_output=True, text=True).stdout.split()
    c = []
    for f in r:
        t = open(f, encoding='utf-8', errors='replace').read()
        if all(re.search(r'^\s*#\s*define\s+' + s + r'\b', t, re.M) for s in sw) and E[i - 1][7] not in f:
            c.append((len(t), f))
    c.sort()
    print('#%d %s %s::%s switch=%s precedents=%d' % (i, E[i - 1][0], X, M, ','.join(sw), len(c)))
    for n, f in c[:nmax]:
        t = open(f, encoding='utf-8', errors='replace').read().split('\n')
        pre = [l for l in t if re.match(r'\s*#\s*(define|include|pragma|undef)', l)]
        print('  %s (%d bytes)' % (f.replace(str(R) + '/rebuild/src/compiled/', ''), n))
        for l in pre[:40]: print('     ' + l[:150])
