# fam_base.py <family> <ov> <rows.txt>: stand-in members that are a PDB base accessor (basemove.move), per family config:
#   FAM[f] = (target, final, old_rx (group name), new_name, base, base_decl)
import sys, re, os
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import famlib as F, inputs, basemove
FAM = {
  'isat': ('00f35b30', '?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ',
           r'\b(?:bool|unsigned\s+char)\s+(?P<name>IsDone|IsFinished|IsInterrupted|IsTerminating|IsActiveThreadTerminating)\s*\(\s*\)\s*(?:const\s*)?;',
           'IsActiveThreadTerminating', 'CEntityScriptBase',
           'bool IsActiveThreadTerminating() const;   // 0x00F35B30 (Ego_r.pdb CEntityScriptBase, const)'),
  'isstop': ('008fd010', '?IsStopping@CAIStateGroupBase@@IBE_NXZ', r'\bbool\s+(?P<name>IsStopping)\s*\(\s*\)\s*(?:const\s*)?;',
           'IsStopping', 'CAIStateGroupBase', 'bool IsStopping() const;   // 0x008FD010 (Ego_r.pdb protected const)', 'protected'),
  'gdi2': ('009ad410', '?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z',
           r'\blong\s+(?P<name>GetDefGlobalIndexFromName)\s*\(\s*const\s+CCharString\s*&\s*\w*\s*\)\s*(?:const\s*)?;',
           'GetDefGlobalIndexFromName', 'CDefinitionManager', 'long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410 (Ego_r.pdb CDefinitionManager, const)'),
  'sbisat': ('00cb7940', '?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ',
           r'\b(?:bool|unsigned\s+char)\s+(?P<name>IsActiveThreadTerminating)\s*\(\s*\)\s*;',
           'IsActiveThreadTerminating', 'CScriptBase', 'bool IsActiveThreadTerminating();   // 0x00CB7940 (Ego_r.pdb CScriptBase)'),
  'gpv2': ('007ee540', '?GetPVillage@CTCVillageMember@@QBEPAVCTCVillage@@XZ',
           r'\bCTCVillage\s*\*\s*(?P<name>GetVillage|GetPVillage)\s*\(\s*\)\s*(?:const\s*)?;',
           'GetPVillage', 'CTCVillageMember', 'CTCVillage* GetPVillage() const;   // 0x007EE540 (Ego_r.pdb CTCVillageMember, const)'),
}
fam = sys.argv[1]; T, FIN, RX, NEW, BASE, DECL = FAM[fam][:6]; BACC = FAM[fam][6] if len(FAM[fam]) > 6 else 'public'
BT, BS = inputs.bindings(); B = BT[T]
def tr(t, a):
    r = basemove.move(t, RX, NEW, BASE, DECL, base_acc=BACC, force_cast=os.environ.get('FORCE_CAST') == '1')
    return r
def ck(S0, S1, a):
    if not (S0 & B): return 'names no bound spelling of %s before' % T
    return F.diffcheck(S0, S1, lambda s: s in B, lambda s: s == FIN, [FIN])
F.run(sys.argv[2], F.rows_from(sys.argv[3]), tr, ck, fam)
