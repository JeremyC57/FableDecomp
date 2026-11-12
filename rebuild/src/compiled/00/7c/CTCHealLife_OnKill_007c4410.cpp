// CTCHealLife::OnKill() -- the kill path: FinalCleanup
// Leaf: ?OnKill@CTCHealLife@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C4410, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 14 of CTCHealLife's vtable 0x0126E36C (the slot holds this body in 1 vtable(s), owner CTCHealLife); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCHealLife@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCHealLife::vfunc_14 (rebuild/src/compiled/00/7c/gapscan_sub007c4410_007c4410.cpp).
// No shared header for CTCHealLife: minimal local definition (RTTI base CTCBase), the override and the FinalCleanup it calls.
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
class CTCHealLife : public CTCBase {
public:
    virtual void OnKill();
private:
    void FinalCleanup();
};
void CTCHealLife::OnKill()
{
    FinalCleanup();
}
