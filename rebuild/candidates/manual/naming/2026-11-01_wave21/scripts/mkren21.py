# mkren18.py <final index jsonl>: RENAMES.tsv -- rows whose file name / Module carries the old (wrong) function or class name after
# wave 21: every final row whose object now defines a Leaf symbol that demangles to another qualified name than the catalog's.
# File-name rule (build_candidates.ps1): <Module stripped to [A-Za-z0-9], first 80 chars>_<function stripped>_<addr>.cpp (test: _test).
# Paths are repo-rooted; new_name is the catalog's bare spelling (mklabels19.bare).
import os, sys, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows
import und
R = rows(); W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.argv, _a = [sys.argv[0]], sys.argv[1:]
exec(open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'mklabels21.py')).read().split('for q in Q:')[0])   # bare(), qn()
A = set(l.strip() for l in open(os.path.join(W, 'runs/final.rows.txt')) if l.strip())
FAM = {}
for p in os.listdir(os.path.join(W, 'runs')):
    if p.endswith('.rows.txt') and not re.match(r'^(t\d+|g\d+|q_|in_|base_|final|delta|reidx)', p):
        for l in open(os.path.join(W, 'runs', p)):
            if l.strip(): FAM.setdefault(l.strip(), []).append(p[:-9])
IX = {}
for l in open(_a[0]):
    r = json.loads(l); IX[r['a']] = r.get('d', [])
OLDIX = {}
for l in open(os.path.join(W, 'idx/full.jsonl')):
    r = json.loads(l); OLDIX[r['a']] = r.get('d', [])
strip = lambda s: re.sub(r'[^A-Za-z0-9]', '', s)
def fname(mod, fn, a):
    m = 'global' if mod == '_global' else ('staticinit' if mod == '_static_init' else strip(mod)[:80])
    return '%s_%s_%s' % (m, strip(fn), a)
HELPER = ('??_C@', '__real@', '?Get@CMainGameComponent@@SIPAV1@XZ', '?Get@?$CTBaseSingletonNoDefaultAlloc@', '??D?$CCountedPointer@', '?GetPCurrentThingManager@CThing@@SI', '?GetConstantFPS@CMainGameComponent@@SI', '??$__destroy', '??$_Destroy', '?deallocate@', '??1?$_STLP_alloc_proxy', '??1?$_Vector_base', '??_7', '?Kill_', '?Use_', '?A0x')
# only the own-row families change a row's defined leaf; the caller respells keep it (an anonymous-namespace hash is TU-text derived: ignored)
A = set(a for a in A if set(FAM.get(a, [])) & {'csound', 'voice', 'dbg', 'alloc', 'misc', 'gdst', 'setcopy', 'c689'})
def leaf_of(path):
    try: m = re.search(r'(?m)^// Leaf: (\S+)', open(path, encoding='utf-8', errors='replace').read())
    except IOError: return None
    return m.group(1) if m else None
def leaf_pair(a):
    d0, d1 = set(OLDIX.get(a, [])), set(IX.get(a, []))
    rel = 'rebuild/src/compiled/' + R[a]['Source']
    l1 = leaf_of(os.path.join(W, 'ov_final', rel)) or leaf_of(os.path.join('/home/user/FableDecomp', rel)); l0 = leaf_of(os.path.join('/home/user/FableDecomp', rel))
    c1 = [s for s in d1 if l1 and s.startswith(l1)]; c0 = [s for s in d0 if l0 and s.startswith(l0)]
    if len(c1) == 1:
        o = [s for s in d0 - d1 if not s.startswith(HELPER) and '?A0x' not in s]
        return c1[0], (c0[0] if len(c0) == 1 else (o[0] if len(o) == 1 else None))
    n1 = [s for s in d1 - d0 if not s.startswith(HELPER) and '?A0x' not in s]; n0 = [s for s in d0 - d1 if not s.startswith(HELPER) and '?A0x' not in s]
    lf = R[a].get('Leaf', '') or ''
    if len(n1) != 1:
        n1 = [s for s in d1 - d0 if s.startswith(lf)] if lf else n1
    if len(n0) != 1:
        n0 = [s for s in d0 - d1 if s.startswith(lf)] if lf else n0
    return (n1[0] if len(n1) == 1 else None), (n0[0] if len(n0) == 1 else None)
out = [('address', 'old_name', 'new_name', 'old_module', 'new_module', 'old_source', 'new_source', 'old_test', 'new_test', 'evidence')]
for a in sorted(A):
    if a not in IX: continue
    s1, s0 = leaf_pair(a)
    if not s1 or s1 == s0 or s1 in OLDIX.get(a, []): continue
    name = qn(s1)
    if not name: continue
    name = bare(name)
    # module = the qualified scope, token = the function (dtor '~X' -> X; ??_G -> scalardeletingdestructor)
    if name.endswith("::`scalar deleting destructor'"): mod, tok = name[:-len("::`scalar deleting destructor'")], 'scalardeletingdestructor'
    else:
        d = 0; cut = -1
        for i, ch in enumerate(name):
            if ch == '<': d += 1
            elif ch == '>': d -= 1
            elif ch == ':' and d == 0 and name[i:i + 2] == '::': cut = i
        mod, tok = (name[:cut], name[cut + 2:]) if cut > 0 else ('_global', name)
        tok = tok.lstrip('~')
    src, tst = R[a]['Source'], R[a].get('TestSource', '')
    ns = os.path.dirname(src) + '/' + fname(mod, tok, a) + '.cpp'
    nt = (os.path.dirname(tst) + '/' + fname(mod, tok, a) + '_test.cpp') if tst else ''
    if ns == src and mod == R[a]['Module']: continue
    fams = '+'.join(FAM.get(a, ['?']))
    out.append((a, os.path.basename(src)[:-4], name, R[a]['Module'], mod, 'rebuild/src/compiled/' + src, 'rebuild/src/compiled/' + ns,
                ('rebuild/tests/' + tst) if tst else '', ('rebuild/tests/' + nt) if tst else '',
                'naming wave 21 (%s): the row now defines %s (was %s)' % (fams, s1, s0 or '-')))
open(os.path.join(W, 'RENAMES.tsv'), 'w').write(''.join('\t'.join(r) + '\n' for r in out))
print(len(out) - 1, 'renames')
for r in out[1:]: print(r[0], r[1], '->', os.path.basename(r[6]))
