# port.py <idx> [precedent-src]: alias-route port draft. Starts from the script attempt work/att/<a>.cpp (or work/h_<a>.cpp when present),
# replaces its preamble with the smallest landed precedent's: every #define of the precedent first (then the attempt's own), then the
# precedent's #include lines, then the attempt's other #include lines; every other attempt line (local declarations, body) keeps its order.
# Precedent: a landed row defining a member of X under the switch, else the smallest landed row defining the switch. Writes rows/<a>.cpp.
import re, sys, subprocess, os
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336')
R = Path('/home/user/FableDecomp/rebuild/src/compiled')
T = {}
for l in open(A / 'work/triage.tsv'):
    p = l.rstrip('\n').split('\t')
    if len(p) > 6 and p[5] == 'DECL': T.setdefault(int(p[0]), (p[2], p[3], re.findall(r'#ifdef (\w+)', p[6])))
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
i = int(sys.argv[1]); e = E[i - 1]; a = e[0]; X, M, sw = T[i]
def defines_all(t): return all(re.search(r'^\s*#\s*define\s+' + s + r'\b', t, re.M) for s in sw)
if len(sys.argv) > 2: prec = Path(sys.argv[2])
else:
    cands = []
    r = subprocess.run(['grep', '-rlE', r'^\s*#\s*define\s+' + sw[0] + r'\b', str(R), '--include=*.cpp'], capture_output=True, text=True).stdout.split()
    for f in r:
        if e[7] in f: continue
        t = open(f, encoding='utf-8', errors='replace').read()
        if not defines_all(t) or '_PASS' in ''.join(re.findall(r'#\s*define\s+(\w+)', t)): continue
        own = bool(re.search(r'(^|[^\w])' + X + r'::~?\w+\s*\(', t))
        cands.append((0 if own else 1, len(t), f))
    if not cands: print(i, a, 'NO_PRECEDENT'); sys.exit(1)
    cands.sort(); prec = Path(cands[0][2])
pt = prec.read_text(encoding='utf-8', errors='replace').split('\n')
pdef = [l.split('//')[0].rstrip() for l in pt if re.match(r'\s*#\s*define\s+FABLE_\w+\s*($|//)', l)]
pinc = [l.split('//')[0].rstrip() for l in pt if re.match(r'\s*#\s*include\s', l)]
src = A / 'work' / ('h_%s.cpp' % a); src = src if src.exists() else A / 'work/att' / (a + '.cpp')
L = src.read_text(encoding='utf-8').split('\n')
head = []
k = 0
while k < len(L) and (L[k].startswith('//') or not L[k].strip()): head.append(L[k]); k += 1
rest = L[k:]
adef = [l for l in rest if re.match(r'\s*#\s*define\s+FABLE_\w+', l)]
ainc = [l for l in rest if re.match(r'\s*#\s*include\s', l)]
key = lambda l: re.sub(r'\s+', ' ', l.split('//')[0].strip())
defs = pdef + [l for l in adef if key(l) not in {key(x) for x in pdef}]
incs = pinc + [l for l in ainc if key(l) not in {key(x) for x in pinc}]
body = [l for l in rest if l not in adef and l not in ainc]
pre_note = '// Preamble: the define / include block of the landed precedent %s.' % prec.name
out = head + [pre_note] + defs + incs + body
(A / 'rows' / (a + '.cpp')).write_text('\n'.join(out), encoding='utf-8')
print(i, a, 'OK', prec.name)
