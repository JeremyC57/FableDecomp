# show.py <idx...>: per ENTRIES line, the PDB spelling (demangled) and, for the self and every census row that mentions the method /
# ctor outside comments, each such line (+ continuation lines while parentheses are open), plus the row's lock / off-limits tags
import sys, re, os
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336'
sys.path.insert(0, A + '/tools'); sys.path.insert(0, '/home/user/FableDecomp/tools/decomp_pipeline/naming'); import nf
import msvc_demangle as M
def dm(s):
    try: return M.pretty(s) if hasattr(M, 'pretty') else M.demangle(s)
    except Exception: return s
E = nf.entries(); C = {}
for l in open(A + '/work/census.tsv'):
    p = l.rstrip('\n').split('\t'); C[int(p[0])] = [x for x in p[2].split(',') if x]
OFF = set(open(A + '/work/offlimits.txt').read().split())
for i in map(int, sys.argv[1:]):
    e = E[i - 1]; pdb = e[6]
    if pdb.startswith('??0') or pdb.startswith('??1'):
        cls = re.match(r'\?\?[01](\w+)@', pdb).group(1); meth = cls
        pat = re.compile((r'~\s*' if pdb[2] == '1' else r'(?<![~\w])') + re.escape(cls) + r'\s*\(')
    elif pdb.startswith('??') and not pdb.startswith('??$'):
        OPS = {'2': 'new', '3': 'delete', 'R': '()', '4': '=', '8': '==', '9': '!=', 'A': '[]', 'M': '<', 'O': '>', 'H': '+', 'G': '-', 'D': '*', 'B': '', 'C': '->', 'Y': '+=', 'Z': '-=', '_4': '[]'}
        op = OPS.get(pdb[2], '?'); meth = 'operator' + op
        pat = re.compile(r'operator\s*' + re.escape(op) + (r'\s*\(' if op != '()' else r'\s*\(') if op else r'operator\s+\w')
    else:
        mm = re.match(r'\?\?\$(\w+)@', pdb) or re.match(r'\?(\w+)@', pdb) or re.match(r'[_@]?(\w+?)@\d+$', pdb) or re.match(r'_?(\w+)$', pdb)   # NF334: extern "C" names
        meth = mm.group(1); pat = re.compile(r'\b' + re.escape(meth) + r'\s*[<(]')
    print('#%d %s %s\n   PDB %s' % (i, e[0], pdb, dm(pdb)))
    for part in e[11].split(' || '):
        m = re.match(r'(\S+) \(([^)]*)\)', part)
        if m and m.group(1) != pdb: print('   WRONG [%s] %s' % (m.group(2), dm(m.group(1))))
    wit = set(re.findall(r'\(([0-9a-f]{8})\)', e[11]))
    rows = [e[0]] + C.get(i, [])
    if os.environ.get('SHOW_WIT'):
        print('   census=%d (only self + wrong-spelling witnesses shown)' % len(C.get(i, [])))
        wrong = set(m.group(2) for m in re.finditer(r'(\S+) \(([^)]*)\)', e[11]) if m.group(1) != pdb)
        rows = [r for r in rows if r == e[0] or r in wrong]
    for r in rows:
        p = nf.src_of(r)
        tag = ('(self)' if r == e[0] else '') + (' OFF' if r in OFF else '') + ('' if nf.lock_owner(r) in ('released', 'NF334') else ' LOCK=' + nf.lock_owner(r))
        if not p or not p.exists(): print('  %s%s NOSRC' % (r, tag)); continue
        L = p.read_text(encoding='utf-8', errors='replace').split('\n'); out = []
        n = 0
        while n < len(L):
            c = L[n].split('//')[0]
            if c.strip() and not c.lstrip().startswith('*') and pat.search(c):
                s = c.strip(); k = n
                while s.count('(') > s.count(')') and k + 1 < len(L) and k < n + 3: k += 1; s += ' ' + L[k].split('//')[0].strip()
                out.append('L%d: %s' % (n + 1, s[:260])); n = k
            n += 1
        if out or r == e[0]:
            print('  %s%s' % (r, tag))
            for o in out[:8]: print('      ' + o)
