# auto.py <mode A|B> <idx..>: rows spec lines (work/rows_auto.tsv, appended) for feasible absent entries (work/feas.tsv):
# defines = the smallest landed precedent's #define FABLE_* list (a row defining a member of X under every switch of the class branch,
# else the smallest row defining them; never a *_PASS<nn> precedent) + the class switches if missing + X's _RTTI_DECLS opt-in;
# mode A: the HEAD row's own includes + the precedent's forward `class X;` lines; mode B: also the precedent's #include lines first.
# definition head from work/decls.tsv. Prints idx, precedent.
import re, sys, subprocess
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF319')
R = Path('/home/user/FableDecomp/rebuild/src/compiled')
E = [l.rstrip('\n').split('\t') for l in open(A / 'ENTRIES.tsv', encoding='utf-8')][1:]
F = {int(l.split('\t')[0]): l.rstrip('\n').split('\t') for l in open(A / 'work/feas.tsv')}
FD = {int(l.split('\t')[0]): l.rstrip('\n').split('\t') for l in open(A / 'work/feas_decl.tsv')}
F.update(FD)
D = {int(l.split('\t')[0]): l.rstrip('\n').split('\t') for l in open(A / 'work/decls.tsv')}
PRE = {}
for l in open(A / 'work/prec_override.tsv') if (A / 'work/prec_override.tsv').exists() else []:
    p = l.rstrip('\n').split('\t'); PRE[int(p[0])] = p[1]
XD = {}
for l in open(A / 'work/extra_defs.tsv') if (A / 'work/extra_defs.tsv').exists() else []:
    p = l.rstrip('\n').split('\t'); XD[int(p[0])] = p[1].split(',')
XI = {}
for l in open(A / 'work/extra_inc.tsv') if (A / 'work/extra_inc.tsv').exists() else []:
    p = l.rstrip('\n').split('\t'); XI[int(p[0])] = p[1].split(',')
mode = sys.argv[1]; out = open(A / 'work/rows_auto.tsv', 'a')
for i in [int(x) for x in sys.argv[2:]]:
    _, X, _, sws = F[i]; sws = sws.split(',')
    e = E[i - 1]
    if i in PRE: prec = R / PRE[i]
    else:
        r = subprocess.run(['grep', '-rlE', r'^\s*#\s*define\s+' + sws[0] + r'\b', str(R), '--include=*.cpp'], capture_output=True, text=True).stdout.split()
        c = []
        for f in r:
            if e[7] in f: continue
            t = open(f, encoding='utf-8', errors='replace').read()
            if not all(re.search(r'^\s*#\s*define\s+' + s + r'\b', t, re.M) for s in sws): continue
            if re.search(r'_PASS\d', ''.join(re.findall(r'#\s*define\s+(\w+)', t))): continue
            own = bool(re.search(r'(^|[^\w:])' + X + r'::~?\w+\s*\(', t))
            c.append((0 if own else 1, len(t), f))
        if not c: print(i, 'NO_PRECEDENT'); continue
        c.sort(); prec = Path(c[0][2])
    pt = prec.read_text(encoding='utf-8', errors='replace').split('\n')
    pdef = []
    for l in pt:
        m = re.match(r'\s*#\s*define\s+(FABLE_\w+)\s*($|//)', l)
        if m and m.group(1) not in pdef: pdef.append(m.group(1))
    for s in sws:
        if s not in pdef: pdef.append(s)
    for s in XD.get(i, []):
        if s not in pdef: pdef.insert(0, s)
    if i not in FD: pdef.append('FABLE_RETAIL_%s_RTTI_DECLS' % X.upper())
    fwd = [l.strip() for l in pt if re.match(r'\s*(class|struct)\s+\w+\s*;\s*$', l)]
    inc = [re.match(r'\s*#\s*include\s+"([^"]+)"', l).group(1) for l in pt if re.match(r'\s*#\s*include\s+"', l)] if mode == 'B' else []
    extra = XI.get(i, []) + inc + fwd
    print('%d\t%s\t%s\t%s' % (i, ','.join(pdef), D[i][3], ','.join(extra)), file=out)
    print(i, X, prec.name, len(pdef), 'defines', len(extra), 'extra')
