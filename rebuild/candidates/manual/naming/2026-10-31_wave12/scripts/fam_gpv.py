# fam_gpv.py <ov> <rows.txt>: 0x007EE540 ?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ (Ego_r + FableWin: const member of
# CTCVillageMember returning CTCVillage*; every caller passes the interface-0x23 component in ecx).
#  F: a free `RET __fastcall GFGetPVillageInterface(X* p);` -> class CTCVillageMember { GetPVillage() const }, calls cast the argument.
#  M: a member `RET GetPVillage();` / `GetPVillage() const` of a row-local class CTCVillageMember returning CVillage* / CTCVillage_Calls*
#     -> const, returning CTCVillage* (the row's stand-in village class renamed CTCVillage when the row has no CTCVillage).
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs, f2m, acclib
FIN = '?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ'
BT, BS = inputs.bindings(); B = BT['007ee540']
CLS = 'class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)'
def tr(t, a):
    c = f2m.code_only(t)
    for fn in ('GFGetPVillageInterface', 'GFGetVillage'):
        if re.search(r'\b__fastcall\s+' + fn + r'\s*\(', c):
            if not re.search(r'\bclass\s+CTCVillage\b|\bstruct\s+CTCVillage\b', c): return None, 'no CTCVillage class in the row'
            r = f2m.free_to_member(t, fn, CLS, 'CTCVillageMember', '((CTCVillageMember*)%s)->GetPVillage()', 'CTCVillage*')
            if r[0] is None: return None, r[1]
            t2 = re.sub(r'\bstruct(\s+CTCVillage\s*);', r'class\1;', r[0])
            return t2, 'free %s -> CTCVillageMember::GetPVillage (%d calls)' % (fn, r[1])
    m = re.search(r'\b(\w+)\s*\*\s*GetPVillage\s*\(\s*\)\s*(const\s*)?;', c)
    if m:
        cs = acclib.class_span(t, m.start())
        if not cs or cs[1] != 'CTCVillageMember': return None, 'GetPVillage in %s' % (cs and cs[1])
        old = m.group(1)
        if old == 'CTCVillage' and m.group(2): return None, 'already final'
        t2 = t[:m.start()] + 'CTCVillage* GetPVillage() const;' + t[m.end():]
        if old != 'CTCVillage':
            if re.search(r'\b(class|struct)\s+CTCVillage\b', c): return None, 'row has CTCVillage and %s' % old
            t2 = re.sub(r'\bstruct(\s+%s\s*)\{' % old, r'class\1{ public:', t2)
            t2 = re.sub(r'\bstruct(\s+%s\s*);' % old, r'class\1;', t2)
            t2 = re.sub(r'\b%s\b' % old, 'CTCVillage', t2)
        t2 = re.sub(r'\bstruct(\s+CTCVillageMember\s*)\{', r'class\1{ public:', t2)
        t2 = re.sub(r'\bstruct(\s+CTCVillageMember\s*);', r'class\1;', t2)
        return t2, 'member GetPVillage -> const CTCVillage* (%s)' % old
    return None, 'no shape'
def ck(S0, S1, a):
    ren = lambda s: s.replace('UCTCVillage@@', 'VCTCVillage@@').replace('UCTCVillageMember@@', 'VCTCVillageMember@@')
    return F.diffcheck(S0, S1, lambda s: s in B or ren(s) in S1 or 'Village' in s, lambda s: s == FIN or ren(s) in set(map(ren, S0)) or 'CTCVillage@@' in s, [FIN])
F.run(sys.argv[1], F.rows_from(sys.argv[2]), tr, ck, 'gpv')
