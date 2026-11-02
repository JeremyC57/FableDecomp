# fixren25.py: post-fixes RENAMES.tsv from mkren25.py where the leaf heuristic cannot tell:
#  - the pixfmt caller rows (0x009BEBB0 0x009F35D0 0x009F37E0 0x009F4190: no Leaf line, the heuristic picked the inline
#    ??0CPixelFormat; 0x009F2F60 / 0x009F9D80: their Leaf names the row's CSurface_Methods / CTexture_Methods view) keep
#    their file and module -- only the own row 0x00A681F0 is renamed.
#  - 0x0122AB10 / AB30 / AB50 (family smp, Leaf _$E2): the atexit destructors of MatrixBufferPool / VectorBufferPool /
#    GroupVectorBufferPool (Ego_r.pdb and FableWin.pdb both list `dynamic atexit destructor for '<name>''; the initialisers
#    0x01224450 / 0x01224470 / 0x01224490 register them); file rule as 0x01229A10 (global_dynamicatexitdestructorfor<name>).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from rows import rows
R = rows()
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
p = os.path.join(W, 'RENAMES.tsv'); T = [l.rstrip('\n').split('\t') for l in open(p)]
DROP = {'009bebb0', '009f2f60', '009f35d0', '009f37e0', '009f4190', '009f9d80'}
out = [T[0]] + [r for r in T[1:] if r[0] not in DROP]
for a, g in (('0122ab10', 'MatrixBufferPool'), ('0122ab30', 'VectorBufferPool'), ('0122ab50', 'GroupVectorBufferPool')):
    d = R[a]; src, tst = d['Source'], d.get('TestSource', '')
    base = 'global_dynamicatexitdestructorfor%s_%s' % (g, a)
    out.append([a, os.path.basename(src)[:-4], "`dynamic atexit destructor for '%s''" % g, d['Module'], '_global',
                'rebuild/src/compiled/' + src, 'rebuild/src/compiled/' + os.path.dirname(src) + '/' + base + '.cpp',
                ('rebuild/tests/' + tst) if tst else '', ('rebuild/tests/' + os.path.dirname(tst) + '/' + base + '_test.cpp') if tst else '',
                "naming wave 25 (smp): the row is the _$E2 atexit destructor of %s (Ego_r.pdb / FableWin.pdb `dynamic atexit destructor for '%s''; "
                "registered by its initialiser row); was a free ClearCachedDefs on an AutoTinyFieldMethodsTarget stand-in" % (g, g)])
open(p, 'w').write(''.join('\t'.join(r) + '\n' for r in out)); print(len(out) - 1, 'renames after fix')
