# bindchk.py <idx..>: for each ENTRIES line, which compiled objects (work/obj new + H_ HEAD, work/hobj controls) define / reference
# the PDB spelling or any other spelling sharing its `?Name@Scope@@` prefix (old access / const forms)
import sys, re, glob, os
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336/tools'); import nf
E = nf.entries(); objs = sorted(glob.glob(str(nf.W / 'obj' / '*.obj')) + glob.glob(str(nf.W / 'hobj' / '*.obj')))
cache = {}
def syms(o):
    if o not in cache: cache[o] = nf.ext_syms(o)
    return cache[o]
for i in map(int, sys.argv[1:]):
    pdb = E[i - 1][6]; m = re.match(r'(\?[^@]+@(?:[^@]+@)*@)', pdb); pre = m.group(1)
    print('#%d %s %s' % (i, E[i - 1][0], pdb))
    for o in objs:
        hits = sorted(x for x in syms(o) if x[4:].startswith(pre))
        if hits:
            tag = os.path.basename(os.path.dirname(o)) + '/' + os.path.basename(o)
            print('   %-22s %s' % (tag, ' '.join(('=' if x[4:] == pdb else '!') + x for x in hits)))
