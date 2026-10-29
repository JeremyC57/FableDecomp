# fam_gws9.py <ov> <rows.txt>: CWorld::GetWorldSeed() const (Ego_r.pdb + FableWin.pdb ?GetWorldSeed@CWorld@@QBEPAKXZ,
# 0x0049D810; W8 proven: body returns [world+8] + 0x1613C = CMainGameComponent::WorldSeed). Rows declared
# `unsigned long* CWorld::GetRandomSeed()` (?GetRandomSeed@CWorld@@QAEPAKXZ / QBEPAKXZ, bound to 0x0049D810).
# Transform: the identifier GetRandomSeed becomes GetWorldSeed in the row, the declaration gets `const`.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib9, acclib9, und
F = '?GetWorldSeed@CWorld@@QBEPAKXZ'
U = und.get([F])
def tf(t, a):
    if 'GetWorldSeed' in t: return None, 'already has GetWorldSeed'
    t = re.sub(r'\bGetRandomSeed\b', 'GetWorldSeed', t)
    t2, why = acclib9.respell(t, U[F][:-6], U[F], 'CWorld', 'GetWorldSeed')
    return (t2, 'ok') if t2 else (None, why)
def ck(S0, S1, a):
    if any('GetRandomSeed@CWorld@@' in s for s in S1): return 'old still emitted'
    if F not in S1: return 'final not emitted'
    return None
famlib9.run(sys.argv[1], [l.strip() for l in open(sys.argv[2]) if l.strip()], tf, ck, 'gws9')
