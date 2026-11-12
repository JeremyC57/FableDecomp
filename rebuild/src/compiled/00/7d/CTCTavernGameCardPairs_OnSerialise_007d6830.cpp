// CTCTavernGameCardPairs::OnSerialise(CPersistContext&, bool) -- serialises the card-game base state, then the tavern game's TargetTime (+0x98) through CPersistContext::Transfer<float>, default 0.0f
// Leaf: ?OnSerialise@CTCTavernGameCardPairs@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D6830, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCTavernGameCardPairs's vtable 0x0126F6FC (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardPairs); the Xbox 360 Test PDB virtual order (third source) aligned on 40 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCTavernGameCardPairs@@UAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCTavernGameCardPairs::vfunc_1 (rebuild/src/compiled/00/7d/CTCTavernGameCardPairs_vfunc1_007d6830.cpp).
// No shared header for CTCTavernGameCardPairs: minimal local definition on its RTTI base CTCTavernGameCardBase.
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
#include "engine/card_sorting_types.h"
#include "engine/CPersistContext.h"
class CTCTavernGameCardPairs : public CTCTavernGameCardBase {
public:
    virtual void OnSerialise(CPersistContext&, bool);       // slot 1
};
void CTCTavernGameCardPairs::OnSerialise(CPersistContext& context, bool transfer)
{
    CTCTavernGameCardBase::OnSerialise(context, transfer);
    context.Transfer("TargetTime", TargetTime, 0.0f);
}
