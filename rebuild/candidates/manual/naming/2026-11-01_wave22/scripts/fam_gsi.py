# fam_gsi.py <ov> <rows.txt>: wave-10 decision for 0x0143E8F8 (no PDB data symbol): CGameScriptInterface* g_pGameScriptInterface
# (?g_pGameScriptInterface@@3PAVCGameScriptInterface@@A, bound from 00cbe716). Every landed use dispatches through
# CGameScriptInterface vtable slots (SetTimer slot 89 = 0x006E76E0, GetWorldFrame slot 653 = 0x0088E070, ...), so the
# timer-manager / thing-manager / GameCalls stand-in types are views of that object: the stand-in type becomes the class
# CGameScriptInterface (when the row names no other CGameScriptInterface) and the variable takes the decided name.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs
FIN = '?g_pGameScriptInterface@@3PAVCGameScriptInterface@@A'
BT, BS = inputs.bindings(); OLDG = BT['0143e8f8'] - {FIN}
DECL = re.compile(r'extern\s+(?:struct\s+|class\s+)?(\w+)\s*\*\s*(\w+)\s*;([^\n]*0x0143E8F8[^\n]*)', re.I)
ST = {}
def code(t): return re.sub(r'//[^\n]*', '', t)
def tr(t, a):
    ds = DECL.findall(t)
    if len(ds) != 1: return None, 'extern decls %r' % (ds,)
    T, g, cm = ds[0]
    if T == 'CGameScriptInterface' and g == 'g_pGameScriptInterface': return None, 'already'
    c = code(t)
    if g != 'g_pGameScriptInterface' and re.search(r'\bg_pGameScriptInterface\b', c): return None, 'both names'
    old = {g, T}
    if T != 'CGameScriptInterface':
        if re.search(r'\bCGameScriptInterface\b', c):
            # the row already has the class (the shared header): the variable takes its type; the view type stays unused
            t = DECL.sub(lambda m: m.group(0).replace(m.group(1), 'CGameScriptInterface', 1), t, count=1)
            old.discard(T)
        else:
            t = re.sub(r'\b' + T + r'\b', 'CGameScriptInterface', t)
            t = re.sub(r'\bstruct(\s+CGameScriptInterface\s*)\{', r'class\1{ public:', t)
            t = re.sub(r'\bstruct(\s+CGameScriptInterface\s*)([:])', r'class\1\2', t)
            t = re.sub(r'\bstruct(\s+CGameScriptInterface\s*);', r'class\1;', t)
    t = re.sub(r'\b' + g + r'\b', 'g_pGameScriptInterface', t)
    ST[a] = (T, g)
    return t, 'extern %s* %s -> CGameScriptInterface* g_pGameScriptInterface' % (T, g)
def ck(S0, S1, a):
    T, g = ST[a]
    ren = lambda s: re.sub(r'\b' + T + r'\b', 'CGameScriptInterface', s).replace('U' + 'CGameScriptInterface@@', 'VCGameScriptInterface@@').replace(g, 'g_pGameScriptInterface')
    R0 = set(ren(s) for s in S0)
    return F.diffcheck(S0, S1, lambda s: s in OLDG or T in s or g in s, lambda s: s == FIN or s in R0, [FIN] if (S0 & OLDG) else None)
F.run(sys.argv[1], F.rows_from(sys.argv[2]), tr, ck, 'gsi')
