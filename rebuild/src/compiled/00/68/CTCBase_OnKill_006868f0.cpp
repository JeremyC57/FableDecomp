// CTCBase::OnKill() -- the empty base implementation of CTCBase's virtual slot 14
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006868F0, 1 byte: `ret` (+ int3 padding).
// Naming: FableWin.pdb and Ego_r.pdb define ?OnKill@CTCBase@@UAEXXZ. 287 RTTI vtables carry this address, all at slot 14
//   (CTCInGameMenu, CTCRotate, CTCUndeadSoul, ...). Slot 14 is OnKill in CTCBase's declaration order (13 OnDie
//   0x006868E0, 15 OnLevelUnload 0x00686900 are labelled); the classes whose PDB defines both OnKill and OnRemove
//   (CTCAssassinRush, CTCForcePushed, CTCAvoidRegionExit) override exactly slots 2 and 14, so slot 2 is OnRemove;
//   5 qualified calls (e.g. the landed CTCAvoidRegionExit::OnKill 0x007E9410). Was landed as a generic `$E2`
//   stub ("CTCBase::vfunc_14"). No identical-code folding in retail, so this empty body is a distinct symbol.
// Leaf: ?OnKill@CTCBase@@UAEXXZ
class CTCBase {
public:
    virtual ~CTCBase();
    virtual void OnRemove();                // slot 2
    virtual void OnKill();                  // slot 14
};

void CTCBase::OnKill()
{
}
