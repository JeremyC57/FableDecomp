// CTCRockTrollShield::FrameUpdate() -- counts the shield's recharge down; when it runs out the shield health is restored to 5
// Leaf: ?FrameUpdate@CTCRockTrollShield@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AED80, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 9 of CTCRockTrollShield's vtable 0x0126D41C (the slot holds this body in 1 vtable(s), owner CTCRockTrollShield); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts FrameUpdate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?FrameUpdate@CTCRockTrollShield@@UAEXXZ; `ret` matches the signature.
// Was: CTCRockTrollShield::vfunc_9 (rebuild/src/compiled/00/7a/CTCRockTrollShield_vfunc9_007aed80.cpp).
// No shared header for CTCRockTrollShield: minimal local definition on its RTTI base CTCBase (Ego_r.pdb members health +0x0C,
// rechargeCounter +0x10; no containers, so the retail offsets are the PDB's).
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
class CTCRockTrollShield : public CTCBase {
public:
    virtual void FrameUpdate();                              // slot 9
    long health;                                             // +0x0C
    long rechargeCounter;                                    // +0x10
};
void CTCRockTrollShield::FrameUpdate()
{
    if (rechargeCounter != 0 && --rechargeCounter == 0) {
        health = 5;
        rechargeCounter = 0;
    }
}
