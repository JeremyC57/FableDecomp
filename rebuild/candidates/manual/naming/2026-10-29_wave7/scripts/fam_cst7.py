# wave 7 family cst7: member functions the PDBs declare const that row-local models declare non-const (same bytes, the mangled
# name differs only in the this-qualifier Q A -> Q B). Ego_r.pdb + FableWin.pdb:
#   CBaseIntelligentPointer::GetPItem 0x00A01B50     ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ
#   CDefinitionManager::GetDefGlobalIndexFromName 0x009AD410  ?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z
#   CPlayerManager::GetMainPlayerNumber 0x00449700   ?GetMainPlayerNumber@CPlayerManager@@QBEJXZ
#   CWorldMap::GetRegionNumberMapIsIn 0x004FC190     ?GetRegionNumberMapIsIn@CWorldMap@@QBEJJ@Z
# Transform (idempotent): the one declaration of the method inside the named class (no const twin) gains ` const`, as does an
# out-of-line `Class::Method(...)` definition in the same file; the row is then compiled (lane cl.exe /Od, timeout 120 s) and
# left alone on any error.
import re, sys, os
W = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, W); sys.path.insert(0, os.path.join(W, 'tools'))
from fam_ref6 import enclosing_class
import cc7
SPEC = [('CBaseIntelligentPointer', 'GetPItem', r'CBaseObject[ \t]*\*'),
        ('CDefinitionManager', 'GetDefGlobalIndexFromName', r'long'),
        ('CPlayerManager', 'GetMainPlayerNumber', r'long'),
        ('CWorldMap', 'GetRegionNumberMapIsIn', r'long')]
def fix(t, tag):
    n = 0; why = []
    for cls, meth, ret in SPEC:
        rx = re.compile(r'(?<![\w:])(%s[ \t]+%s[ \t]*\(([^()]*)\))([ \t]*)(const[ \t]*)?;' % (ret, meth))
        ds = [m for m in rx.finditer(t) if enclosing_class(t, m.start()) == cls]
        if not ds: continue
        if len(ds) > 1: why.append('%s: %d decls' % (meth, len(ds))); continue
        d = ds[0]
        if d.group(4): continue
        t = t[:d.end(1)] + ' const' + t[d.end(1):]; n += 1
        t = re.sub(r'(\b%s::%s[ \t]*\([^()]*\))([ \t]*\n?[ \t]*\{)' % (cls, meth), r'\1 const\2', t)
    if not n: return None, 'already' if not why else '; '.join(why)
    e = cc7.errors(t, tag)
    if e: return None, 'compile: line %d %s' % (e[0][0], e[0][1][:100])
    return t, 'ok %d' % n
if __name__ == '__main__':
    for p in sys.argv[1:]:
        t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
        new, why = fix(t, os.path.basename(p)[:-4])
        if new is None: print('skip', p, why); sys.stdout.flush(); continue
        open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(new); print('edited', p, why); sys.stdout.flush()
