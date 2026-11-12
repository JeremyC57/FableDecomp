// CTCActionUseBed::OnSerialise(CPersistContext&, bool) -- persists UseableByHero (+0x20) and OwnedByHero (+0x21) through CPersistContext::Transfer<bool>, default false
// Leaf: ?OnSerialise@CTCActionUseBed@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EBE50, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCActionUseBed's vtable 0x0123F784 (the slot holds this body in 1 vtable(s), owner CTCActionUseBed); the Xbox 360 Test PDB virtual order (third source) aligned on 15 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCActionUseBed@@UAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCActionUseBed::vfunc_1 (rebuild/src/compiled/00/7e/CTCActionUseBed_vfunc1_007ebe50.cpp).
// No shared header for CTCActionUseBed: minimal local definition on its RTTI base CTCActionUseBase (shared header), Ego_r.pdb
// members at their offsets.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CPersistContext.h"
#include "engine/CTCActionUseBase.h"
class CTCActionUseBed : public CTCActionUseBase {
public:
    virtual void OnSerialise(CPersistContext&, bool);                       // slot 1
private:
    bool WaitingForFadeOut;                                                 // +0x14
    bool WaitingForFadeIn;                                                  // +0x15
    bool InAtomicAction;                                                    // +0x16
    bool WaitingForConfirmation;                                            // +0x17
    long PlayerNumberOfThingUsingBed;                                       // +0x18
    long Cost;                                                              // +0x1c
    bool UseableByHero;                                                     // +0x20
    bool OwnedByHero;                                                       // +0x21
};
void CTCActionUseBed::OnSerialise(CPersistContext& context, bool)
{
    context.Transfer("UseableByHero", UseableByHero, false);
    context.Transfer("OwnedByHero", OwnedByHero, false);
}
