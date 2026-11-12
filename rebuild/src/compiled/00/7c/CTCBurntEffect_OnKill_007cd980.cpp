// CTCBurntEffect::OnKill() -- the kill path: FinalCleanup
// Leaf: ?OnKill@CTCBurntEffect@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CD980, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 14 of CTCBurntEffect's vtable 0x0126EFBC (the slot holds this body in 1 vtable(s), owner CTCBurntEffect); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCBurntEffect@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCBurntEffect::vfunc_14 (rebuild/src/compiled/00/7c/gapscan_sub007cd980_007cd980.cpp).
// No shared header for CTCBurntEffect: minimal local definition (RTTI base CTCBase), the override and the FinalCleanup it calls.
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
class CTCBurntEffect : public CTCBase {
public:
    virtual void OnKill();
private:
    void FinalCleanup();
};
void CTCBurntEffect::OnKill()
{
    FinalCleanup();
}
