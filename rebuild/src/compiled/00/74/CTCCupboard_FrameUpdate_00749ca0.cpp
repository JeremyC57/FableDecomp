// CTCCupboard::FrameUpdate() -- the frame update: UpdateAnimation
// Leaf: ?FrameUpdate@CTCCupboard@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00749CA0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 9 of CTCCupboard's vtable 0x0123AB9C (the slot holds this body in 1 vtable(s), owner CTCCupboard); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts FrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?FrameUpdate@CTCCupboard@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCCupboard::vfunc_9 (rebuild/src/compiled/00/74/gapscan_sub00749ca0_00749ca0.cpp).
// No shared header for CTCCupboard: minimal local definition (RTTI base CTCBase), the override and the UpdateAnimation it calls.
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
class CTCCupboard : public CTCBase {
public:
    virtual void FrameUpdate();
private:
    void UpdateAnimation();
};
void CTCCupboard::FrameUpdate()
{
    UpdateAnimation();
}
