# decl.py <idx...>: per ENTRIES line, every census row (and the self) with the non-comment source lines that mention the method
# (ctor: the class name followed by '('), excluding call-looking lines through -> / . ; flags rows that only include a shared header
# declaring it (HDR) and rows off-limits / locked by others. A fast pre-compile triage of which binders carry a local model.
import sys, re, os
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF334'
sys.path.insert(0, A + '/tools'); import nf
E = nf.entries(); C = {}
for l in open(A + '/work/census.tsv'):
    p = l.rstrip('\n').split('\t'); C[int(p[0])] = [x for x in p[2].split(',') if x]
OFF = set(open(A + '/work/offlimits.txt').read().split())
maxl = int(os.environ.get('DECL_MAX', '6'))
for i in map(int, sys.argv[1:]):
    e = E[i - 1]; pdb = e[6]
    if pdb.startswith('??0') or pdb.startswith('??1'):
        cls = re.match(r'\?\?[01](\w+)@', pdb).group(1); meth = ('~' if pdb[2] == '1' else '') + cls; pat = re.compile((r'~\s*' if pdb[2] == '1' else r'(?<![~\w])') + re.escape(cls) + r'\s*\(')
    else:
        meth = re.match(r'\?(\w+)@', pdb).group(1); pat = re.compile(r'\b' + re.escape(meth) + r'\s*\(')
    print('#%d %s %s [%s]' % (i, e[0], meth, pdb)); nohit = []
    for r in [e[0]] + C.get(i, []):
        p = nf.src_of(r)
        tag = ('(self)' if r == e[0] else '') + (' OFF' if r in OFF else '') + ('' if nf.lock_owner(r) in ('released', 'NF334') else ' LOCK=' + nf.lock_owner(r))
        if not p or not p.exists(): print('  %s%s NOSRC' % (r, tag)); continue
        L = p.read_text(encoding='utf-8', errors='replace').split('\n'); hits = []
        for n, l in enumerate(L, 1):
            c = l.split('//')[0]
            if not c.strip() or c.lstrip().startswith('*'): continue
            if pat.search(c) and not re.search(r'(->|\.)\s*' + re.escape(meth.lstrip('~')) + r'\s*\(', c): hits.append('L%d: %s' % (n, c.strip()[:150]))
        inc = [m.group(1) for m in re.finditer(r'#include\s+"([^"]+)"', '\n'.join(L))]
        lf = [l[9:].strip()[:90] for l in L if l.startswith('// Leaf:')]
        if os.environ.get('DECL_HITS_ONLY') and not hits and r != e[0]: nohit.append(r); continue
        print('  %s%s leaf=%s inc=%d' % (r, tag, lf[0] if lf else '-', len(inc)))
        for h in hits[:maxl]: print('      ' + h)
        if len(hits) > maxl: print('      ... %d more' % (len(hits) - maxl))
    if nohit: print('  no-local-decl rows: %d (%s)' % (len(nohit), ','.join(nohit[:8]) + ('...' if len(nohit) > 8 else '')))
