# fam_isstop.py <ov> <rows.txt>: 0x008FD010 ?IsStopping@CAIStateGroupBase@@IBE_NXZ (Ego_r 0x00847E60: protected const member).
# Rows whose `bool IsStopping();` / `bool IsStopping() const;` sits in a row-local class CAIStateGroupBase: const + protected.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, acclib, inputs
FIN = '?IsStopping@CAIStateGroupBase@@IBE_NXZ'
BT, BS = inputs.bindings(); B = BT['008fd010']
def tr(t, a):
    rx = r'\bbool\s+IsStopping\s*\(\s*\)\s*(?:const\s*)?;'
    r = acclib.respell_decl(t, rx, 'bool IsStopping() const;', 'protected')
    if r[0] is None: return None, r[1]
    if r[1] != 'CAIStateGroupBase': return None, 'declared in %s' % r[1]
    t2 = r[0]
    t2 = re.sub(r'(bool IsStopping\(\) const;)(\s*)// 0x008[Ff][Dd]010(?! \(Ego_r)', r'\1\2// 0x008FD010 (Ego_r.pdb protected const)', t2, count=1)
    return t2, 'IsStopping const protected'
def ck(S0, S1, a):
    return F.diffcheck(S0, S1, lambda s: s in B or 'IsStopping' in s, lambda s: s == FIN, [FIN])
F.run(sys.argv[1], F.rows_from(sys.argv[2]), tr, ck, 'isstop')
