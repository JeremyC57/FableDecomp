# fam_decl.py <family> <ov> <rows.txt>: single-declaration respellings of one member declaration inside a row-local class
# whose name is the PDB class (const / access / parameter or return spelling). FAM[family] = (target, final, decl_rx, new_decl,
# access, classes): decl_rx must match the one declaration; the enclosing class must be in `classes`.
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs, acclib
FAM = {
  'gst': ('004c7e50', '?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ', r'\bCScriptThing\s+GetScriptThing\s*\(\s*\)\s*;',
          'CScriptThing GetScriptThing() const;', None, ('CThing', 'CThing_Retail')),
  'gdi': ('009ad410', '?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z',
          r'\blong\s+GetDefGlobalIndexFromName\s*\(\s*const\s+CCharString\s*&\s*(\w*)\s*\)\s*;', None, None, ('CDefinitionManager', 'CDefinitionManager_Retail')),
  'isat': ('00f35b30', '?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ', r'\bbool\s+IsActiveThreadTerminating\s*\(\s*\)\s*;',
          'bool IsActiveThreadTerminating() const;', None, ('CEntityScriptBase',)),
}
RENAME = {'CDefinitionManager_Retail': 'CDefinitionManager', 'CThing_Retail': 'CThing', 'CGameDefinitionManager_Retail': 'CDefinitionManager'}
fam = sys.argv[1]; T, FIN, RX, NEW, ACC, CLS = FAM[fam]
BT, BS = inputs.bindings(); B = BT[T]
def tr(t, a):
    ms = list(re.finditer(RX, t))
    if len(ms) != 1: return None, '%d declarations' % len(ms)
    new = NEW or re.sub(r'\)\s*;$', ') const;', ms[0].group(0))
    r = acclib.respell_decl(t, RX, new, ACC)
    if r[0] is None: return None, r[1]
    if r[1] not in CLS: return None, 'declared in %s' % r[1]
    t2 = r[0]
    if r[1] in RENAME:
        nw = RENAME[r[1]]
        if re.search(r'\b(class|struct)\s+%s\b' % nw, t2): return None, '%s and %s both here' % (r[1], nw)
        t2 = re.sub(r'\bstruct(\s+%s\s*)\{' % r[1], r'class\1{ public:', t2)
        t2 = re.sub(r'\bstruct(\s+%s\s*);' % r[1], r'class\1;', t2)
        t2 = re.sub(r'\b%s\b' % r[1], nw, t2)
    return t2, '%s decl respelled in %s' % (fam, r[1])
def ren(s):
    for o, n in RENAME.items(): s = s.replace('U%s@@' % o, 'V%s@@' % n).replace('V%s@@' % o, 'V%s@@' % n).replace('@%s@@' % o, '@%s@@' % n)
    return s
def ck(S0, S1, a):
    if not (S0 & B): return 'names no bound spelling of %s before' % T
    R0 = set(map(ren, S0))
    return F.diffcheck(S0, S1, lambda s: s in B or ren(s) in S1, lambda s: s == FIN or s in R0, [FIN])
F.run(sys.argv[2], F.rows_from(sys.argv[3]), tr, ck, fam)
