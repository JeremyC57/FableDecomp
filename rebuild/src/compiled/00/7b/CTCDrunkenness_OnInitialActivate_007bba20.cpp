// CTCDrunkenness::OnInitialActivate() -- starts sober
// Leaf: ?OnInitialActivate@CTCDrunkenness@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BBA20, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 6 of CTCDrunkenness's vtable 0x0126DEB4 (the slot holds this body in 1 vtable(s), owner CTCDrunkenness); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnInitialActivate@CTCDrunkenness@@UAEXXZ; `ret` matches the signature.
// Was: CTCDrunkenness::vfunc_6 (rebuild/src/compiled/00/7b/CTCDrunkenness_vfunc6_007bba20.cpp).
// No shared header for CTCDrunkenness: minimal local definition on its RTTI base CTCBase (Ego_r.pdb members Drunk +0x0C,
// Drunkenness +0x10, FrameLastUpdatedSobering +0x14, SecondsPerFrame +0x18, ThisDrunkennessThreshold +0x1C).
#define FABLE_RETAIL_CTCBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
class CTCDrunkenness : public CTCBase {
public:
    virtual void OnInitialActivate();                        // slot 6
    bool Drunk;                                              // +0x0C
    float Drunkenness;                                       // +0x10
    long FrameLastUpdatedSobering;                           // +0x14
    float SecondsPerFrame;                                   // +0x18
    float ThisDrunkennessThreshold;                          // +0x1C
};
void CTCDrunkenness::OnInitialActivate()
{
    Drunkenness = 0.0f;
    Drunk = false;
}
