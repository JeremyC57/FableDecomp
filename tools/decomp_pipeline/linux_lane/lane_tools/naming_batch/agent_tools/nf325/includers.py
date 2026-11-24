# includers.py (NF325): the landed rows / tests whose include closure can reach a changed header (hdr/engine/*.h): reverse closure over
# every quoted #include in rebuild/include (resolved next to the includer, then from the include root), then every row source / test
# that includes a header of that closure or #includes another compiled row (folded TUs, followed transitively). A row outside this set
# never reads a changed file, so its preprocessed text cannot change; equiv_check over the set is the same proof, faster.
import os, re, glob, collections
R = '/home/user/FableDecomp/rebuild'; I = R + '/include'
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF325'
inc = re.compile(r'^\s*#\s*include\s*"([^"]+)"', re.M)
def resolve(frm, name):
    for base in (os.path.dirname(frm), I):
        p = os.path.normpath(os.path.join(base, name))
        if os.path.exists(p): return p
    return None
rev = collections.defaultdict(set)
for h in glob.glob(I + '/**/*.h', recursive=True):
    for n in inc.findall(open(h, errors='replace').read()):
        p = resolve(h, n)
        if p: rev[p].add(h)
changed = {I + '/engine/' + os.path.basename(f) for f in glob.glob(A + '/hdr/engine/*.h')}
clos = set(changed); todo = list(changed)
while todo:
    x = todo.pop()
    for y in rev.get(x, ()):
        if y not in clos: clos.add(y); todo.append(y)
bc = open(R + '/build_candidates.ps1', encoding='utf-8', errors='replace').read(); rows = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", bc, re.S):
    d = dict(re.findall(r"(\w+) = '([^']*)'", m.group(1)))
    if 'Address' in d and 'Source' in d: rows[d['Address'].lower()] = [R + '/src/compiled/' + d['Source']] + ([R + '/tests/' + d['TestSource']] if d.get('TestSource') else [])
hit = set(); filehit = {}
def touches(f, seen=()):
    if f in filehit: return filehit[f]
    if not os.path.exists(f) or f in seen: return False
    r = False
    for n in inc.findall(open(f, errors='replace').read()):
        p = resolve(f, n)
        if p and (p in clos or (p.endswith('.cpp') and touches(p, seen + (f,)))): r = True; break
    filehit[f] = r; return r
for a, fs in rows.items():
    if any(touches(f) for f in fs): hit.add(a)
open(A + '/work/includers.txt', 'w').write('\n'.join(sorted(hit)) + '\n')
print('closure headers', len(clos), 'rows', len(rows), 'includer rows', len(hit))
