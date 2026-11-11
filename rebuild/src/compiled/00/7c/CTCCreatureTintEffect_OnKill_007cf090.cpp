// CTCCreatureTintEffect::OnKill() -- the kill path: FinalCleanup
// Leaf: ?OnKill@CTCCreatureTintEffect@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CF090, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 14 of CTCCreatureTintEffect's vtable 0x0126F13C (the slot holds this body in 1 vtable(s), owner CTCCreatureTintEffect); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCCreatureTintEffect@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCCreatureTintEffect::vfunc_14 (rebuild/src/compiled/00/7c/gapscan_sub007cf090_007cf090.cpp).
// No shared header for CTCCreatureTintEffect: minimal local definition (RTTI base CTCBase), the override and the FinalCleanup it calls.
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
class CTCCreatureTintEffect : public CTCBase {
public:
    virtual void OnKill();
private:
    void FinalCleanup();
};
void CTCCreatureTintEffect::OnKill()
{
    FinalCleanup();
}
