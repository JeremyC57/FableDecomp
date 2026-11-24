# closure.py <header rel path, e.g. engine/CDefinitionManager.h> <out addr file>: the landed rows whose source or test reaches the header
# through quoted / angle includes, resolved against rebuild/include, the including file's own directory, and (MSVC includer lookup) every
# directory of the include stack is approximated by resolving each include name against rebuild/include and the includer's dir. Folded
# `../src/compiled/...` TUs are followed. Prints counts; writes one bare lowercase address per row.
import sys, os, re
R = '/home/user/FableDecomp'; INC = R + '/rebuild/include'
target = os.path.normpath(os.path.join(INC, sys.argv[1]))
IRE = re.compile(r'^\s*#\s*include\s*[<"]([^">]+)[">]', re.M)
cache = {}
def incs(p):
    if p in cache: return cache[p]
    try: t = open(p, encoding='utf-8', errors='replace').read()
    except Exception: cache[p] = []; return []
    out = []
    for n in IRE.findall(t):
        for base in (os.path.dirname(p), INC):
            q = os.path.normpath(os.path.join(base, n))
            if os.path.isfile(q): out.append(q); break
    cache[p] = out; return out
# reverse BFS (cycle-safe): every file under rebuild/include, plus each row source / test, keyed by its resolved includes
import glob
files = set(os.path.normpath(x) for x in glob.glob(INC + '/**/*', recursive=True) if os.path.isfile(x))
bc = open(R + '/rebuild/build_candidates.ps1', encoding='utf-8', errors='replace').read()
rows = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", bc, re.S):
    d = dict(re.findall(r"(\w+) = '([^']*)'", m.group(1)))
    if 'Address' in d and 'Source' in d: rows.setdefault(d['Address'].lower(), (R + '/rebuild/src/compiled/' + d['Source'], R + '/rebuild/tests/' + d['TestSource'] if d.get('TestSource') else None))
for s, tt in rows.values():
    files.add(os.path.normpath(s))
    if tt: files.add(os.path.normpath(tt))
rev = {}
for f in files:
    for q in incs(f): rev.setdefault(q, set()).add(f)
seen = {target}; todo = [target]
while todo:
    x = todo.pop()
    for y in rev.get(x, ()):
        if y not in seen: seen.add(y); todo.append(y)
hit = [a for a, (s, tt) in sorted(rows.items()) if os.path.normpath(s) in seen or (tt and os.path.normpath(tt) in seen)]
open(sys.argv[2], 'w').write(''.join(a + '\n' for a in hit))
print('rows', len(rows), 'closure', len(hit))
