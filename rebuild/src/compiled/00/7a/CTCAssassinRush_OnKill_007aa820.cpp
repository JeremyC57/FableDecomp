// CTCAssassinRush::OnKill() -- CTCBase virtual (vtable 0x0126CF2C slot 14): tail-calls the private FinalCleanup
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AA820, 5 bytes: `jmp 0x007AA4D0` (+ int3 padding).
// Callees: CTCAssassinRush::FinalCleanup @ 0x007AA4D0 (landed; private, ?FinalCleanup@CTCAssassinRush@@AAEXXZ).
// Naming: Ego_r.pdb defines ?OnKill@CTCAssassinRush@@UAEXXZ (Ego_r 0x007AF2C0; there OnKill 0x007AF2C0 and OnRemove
//   0x007AF2D0 sit 16 bytes apart in that order, as retail 0x007AA820 / 0x007AA830 do). Of CTCAssassinRush's
//   out-of-line virtuals in the PDB (OnKill, OnRemove, OnInitialActivate, FrameUpdate, GetInterfaceType,
//   GetClassTypeName) retail overrides slots 2, 6 (OnInitialActivate), 9 (FrameUpdate), 14, 21 (GetInterfaceType) and
//   22 (GetClassTypeName); slot 14 is OnKill in CTCBase's order (13 OnDie, 15 OnLevelUnload), so slot 2 is OnRemove
//   (CTCBase::OnRemove is the empty 0x006867F0 every non-overriding vtable carries there; CTCAvoidRegionExit, whose
//   PDB defines both, overrides the same two slots). OnRemove (slot 2) is the twin 0x007AA830. Was landed as a free-function forwarder
//   (gapscan "CTCAssassinRush::vfunc_14").
// Shape: `this` is already in ecx, so the call to the non-virtual member is a bare `jmp rel32`.
// Leaf: ?OnKill@CTCAssassinRush@@UAEXXZ
class CTCAssassinRush {
public:
    virtual ~CTCAssassinRush();
    virtual void OnRemove();                // CTCBase slot 2
    virtual void OnKill();                  // CTCBase slot 14
private:
    void FinalCleanup();                    // 0x007AA4D0
};

void CTCAssassinRush::OnKill()
{
    FinalCleanup();
}
