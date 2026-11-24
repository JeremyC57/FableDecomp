# abscensus.py: NF299 Q4-style census of the ENTRIES whose engine/<X>.h does not declare the PDB member (ABSENT) or does not exist
# (NOHEADER): per entry, whether the header has a polymorphic class model of X under a FABLE_RETAIL_ switch (`class X : public B`
# inside an #if branch), the switch's landed definers, X's base, and whether a base header declares `virtual ... M(` (an override
# of an existing slot, the only add-only declaration a header could take without a new vtable slot). -> work/abscensus.tsv
import re, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF323')
R = Path('/home/user/FableDecomp/rebuild')
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
T = {}
for l in open(A / 'work/triage.tsv'):
    p = l.rstrip('\n').split('\t')
    if p[4] in ('ABSENT', 'NOHEADER'): T[int(p[0])] = p[4]
def definers(sw):
    r = subprocess.run(['grep', '-rlE', r'^\s*#\s*define\s+' + sw + r'\b', str(R / 'src/compiled'), '--include=*.cpp'], capture_output=True, text=True).stdout.split()
    return len(r)
o = open(A / 'work/abscensus.tsv', 'w')
print('idx\taddr\tclass\tmember\tkind\tmodel\tswitches(definers)\tbase\tbase_declares_M', file=o)
for i, k in sorted(T.items()):
    e = E[i - 1]; m = re.match(r"\?([^@]+)@([^@]+)@@", e[3]); M, X = m.group(1), m.group(2)
    h = R / 'include/engine' / (X + '.h')
    if not h.exists(): print(i, e[0], X, M, k, 'NOHEADER', '-', '-', '-', sep='\t', file=o); continue
    L = h.read_text(encoding='utf-8', errors='replace').split('\n'); stack = []; models = []
    for ln in L:
        s = ln.strip()
        if re.match(r'#\s*if', s): stack.append(s); continue
        if re.match(r'#\s*(else|elif)', s) and stack: stack[-1] += ' |' + s; continue
        if re.match(r'#\s*endif', s) and stack: stack.pop(); continue
        mm = re.match(r'(class|struct)\s+' + re.escape(X) + r'\s*:\s*(?:public\s+)?([\w:<>, ]+?)\s*\{', s)
        if mm: models.append((' & '.join(x for x in stack if not re.match(r'#\s*ifndef\s+\w+_H\b', x)), mm.group(2)))
    if not models:
        gen = 'GENERATED' in ''.join(L[:5])
        print(i, e[0], X, M, k, 'FLAT' + ('(generated)' if gen else ''), '-', '-', '-', sep='\t', file=o); continue
    for st, base in models:
        sws = sorted(set(re.findall(r'FABLE_RETAIL_\w+', st)))
        b = base.split(',')[0].strip(); bh = R / 'include/engine' / (b + '.h'); bd = '-'
        if bh.exists():
            bd = 'YES' if re.search(r'virtual[^;(]*\b' + re.escape(M) + r'\s*\(', bh.read_text(encoding='utf-8', errors='replace')) else 'no'
        print(i, e[0], X, M, k, 'MODEL', ','.join('%s(%d)' % (s, definers(s)) for s in sws) or '(none)', base, bd, sep='\t', file=o)
o.close()
