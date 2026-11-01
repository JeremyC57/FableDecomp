# fam_free.py <family> <ov> <rows.txt>: a free `RET __fastcall NAME(X* p);` stand-in that is really a thiscall PDB member (the
# argument in ecx is `this`): the declaration becomes the PDB class (f2m.free_to_member), each call casts its argument.
#   FAM[f] = (target, final, [stand-in names], class decl, class name, call format, new return type, required-class regex or None)
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs, f2m, und
FAM = {
  'isat': ('00f35b30', '?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ', None,
           'class CEntityScriptBase { public: bool IsActiveThreadTerminating() const; };   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)',
           'CEntityScriptBase', '((const CEntityScriptBase*)%s)->IsActiveThreadTerminating()', 'bool', None),
  'gpv': ('007ee540', '?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ', None,
           'class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)',
           'CTCVillageMember', '((CTCVillageMember*)%s)->GetPVillage()', 'CTCVillage*', r'\b(class|struct)\s+CTCVillage\b'),
}
fam = sys.argv[1]; T, FIN, NAMES, DECL, CLS, CALL, RETN, NEED = FAM[fam]
BT, BS = inputs.bindings(); B = BT[T]
E, IX = None, None
def names_of(a):
    # the row's bound fastcall stand-ins of the target (from the baseline index)
    global E, IX
    if IX is None: E, IX = inputs.index()
    r = IX.get(a, {})
    out = []
    for s in r.get('u', []):
        if s in B and s != FIN:
            d = und.get([s])[s]
            m = re.search(r'__fastcall (\w+)\(', d) or (re.match(r'@(\w+)@\d+$', s) and re.match(r'@(\w+)@\d+$', s))
            if m: out.append(m.group(1))
    return out
def tr(t, a):
    ns = NAMES or names_of(a)
    if not ns: return None, 'no fastcall stand-in'
    if NEED and not re.search(NEED, f2m.code_only(t)): return None, 'required class missing'
    done = []
    for fn in ns:
        r = f2m.free_to_member(t, fn, DECL if not done else '', CLS, CALL, RETN)
        if r[0] is None: return None, '%s: %s' % (fn, r[1])
        t = r[0]; done.append('%s(%d)' % (fn, r[1]))
    return t, 'free %s -> %s' % (' '.join(done), FIN)
def ck(S0, S1, a):
    return F.diffcheck(S0, S1, lambda s: s in B, lambda s: s == FIN, [FIN])
F.run(sys.argv[2], F.rows_from(sys.argv[3]), tr, ck, fam + 'f')
