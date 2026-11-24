# bind.py <objdir> <idx...>: per ENTRIES line, what each census row's object binds at the entry address (xr: relocations resolved
# against the retail bytes) and, for the self row, the DEF symbols naming the method; flags each spelling OK / WRONG v pdb_spelling
import sys, re, io, contextlib, os
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF307'
sys.path.insert(0, A + '/tools'); import nf
od = sys.argv[1]; E = nf.entries(); C = {}
for l in open(A + '/work/census.tsv'):
    p = l.rstrip('\n').split('\t'); C[int(p[0])] = (p[1], [x for x in p[2].split(',') if x])
for i in map(int, sys.argv[2:]):
    e = E[i - 1]; pdb = e[6]; addr = e[0]; meth = re.match(r'\?+(\w+)@', pdb).group(1)
    print('#%d %s %s' % (i, addr, pdb))
    for r in [addr] + C[i][1]:
        ob = os.path.join(od, r + '.obj')
        if not os.path.exists(ob): print('   %s%s NOOBJ' % (r, '(self)' if r == addr else '')); continue
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            try: nf.cmd_xr(r, ob)
            except Exception as ex: print('XR_ERR', ex)
        b = [l for l in out.getvalue().splitlines() if l.startswith(addr)]
        s = ''
        if r == addr:
            d = [x[4:] for x in nf.ext_syms(ob) if x.startswith('DEF ') and ('?' + meth + '@') in x]
            s = ' DEF=' + ','.join(('OK:' if x == pdb else 'WRONG:') + x for x in d)
        print('   %s%s %s%s' % (r, '(self)' if r == addr else '', ' '.join(('OK ' if l.split('\t')[1] == pdb else 'WRONG ') + l.split('\t')[1] + ' ' + l.split('\t')[2] for l in b) or '-', s))
