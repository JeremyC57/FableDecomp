# fam_gmin.py <ov> <rows.txt>: 0x006E7510 GetCurrentWorldFrameForMessages -> protected CGameScriptInterface::GetMinWorldFrameForMessages
# (and the public GetMaxWorldFrameForMessages declaration -> protected, 0x006E7530 IBE)
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, acc
MIN = '?GetMinWorldFrameForMessages@CGameScriptInterface@@IBEJXZ'
MAX = '?GetMaxWorldFrameForMessages@CGameScriptInterface@@IBEJXZ'
DECL = re.compile(r'^\s*long\s+(GetMinWorldFrameForMessages|GetMaxWorldFrameForMessages)\s*\(\s*\)\s*const\s*;')
def tr(t, a):
    if not re.search(r'\bGetCurrentWorldFrameForMessages\b', t): return None, 'no GetCurrentWorldFrameForMessages'
    t = re.sub(r'\bGetCurrentWorldFrameForMessages\b', 'GetMinWorldFrameForMessages', t)
    t = t.replace('// 0x006E7510 (labelled GetMaxWorldFrameForMessages)', '// 0x006E7510')
    t = t.replace('// 0x006E7510 (descriptive spelling, as bound)', '// 0x006E7510')
    t2 = acc.put(t, lambda l: bool(DECL.match(l)), 'protected')
    if t2 is None: return None, 'no enclosing class for the declaration'
    return t2, 'GetCurrent->GetMin, protected'
def ck(S0, S1, a):
    bad = lambda s: 'GetCurrentWorldFrameForMessages' in s or s == '?GetMaxWorldFrameForMessages@CGameScriptInterface@@QBEJXZ'
    had = any('GetCurrentWorldFrameForMessages' in s for s in S0)
    return F.diffcheck(S0, S1, bad, lambda s: s in (MIN, MAX), [MIN] if had else None)
F.run(sys.argv[1], F.rows_from(sys.argv[2]), tr, ck, 'gmin')
