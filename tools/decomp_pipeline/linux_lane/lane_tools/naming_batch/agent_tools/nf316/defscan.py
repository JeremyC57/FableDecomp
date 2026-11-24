# defscan.py <objdir> <idx...>: every census-row object's DEF symbols naming the entry's method (same-TU strong copies -> T3 check)
import sys, re
A = '/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad/w22/agentNF316'
sys.path.insert(0, A + '/tools'); import nf, os
E = nf.entries(); C = {}
for l in open(A + '/work/census.tsv'):
    p = l.rstrip('\n').split('\t'); C[int(p[0])] = [x for x in p[2].split(',') if x]
for i in map(int, sys.argv[2:]):
    e = E[i - 1]; pdb = e[6]; m = re.match(r'\?\?[0-9A-Z_](\w+)@', pdb) if pdb.startswith('??') else re.match(r'\?(\w+)@', pdb)
    key = ('??' + pdb[2] + m.group(1) + '@') if pdb.startswith('??') else ('?' + m.group(1) + '@' + pdb.split('@')[1] + '@')
    for r in C.get(i, []):
        ob = os.path.join(sys.argv[1], r + '.obj')
        if not os.path.exists(ob): continue
        d = [x[4:] for x in nf.ext_syms(ob) if x.startswith('DEF ') and key in x]
        if d: print('#%d %s binder %s DEF %s' % (i, e[0], r, ' '.join(d)))
