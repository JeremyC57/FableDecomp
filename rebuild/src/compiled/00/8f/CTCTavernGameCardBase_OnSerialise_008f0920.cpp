// CTCTavernGameCardBase::OnSerialise(CPersistContext&, bool) -- card games serialise as tavern games
// Leaf: ?OnSerialise@CTCTavernGameCardBase@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F0920, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCTavernGameCardBase's vtable 0x01288294 (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardBase); the Xbox 360 Test PDB virtual order (third source) aligned on 36 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCTavernGameCardBase@@UAEXAAVCPersistContext@@_N@Z; no `ret` to check (NORET).
// Was: CTCTavernGameCardBase::vfunc_1 (rebuild/src/compiled/00/8f/CQREscortTraderManagerScript_OnPersist_008f0920.cpp).
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
#include "engine/card_sorting_types.h"
void CTCTavernGameCardBase::OnSerialise(CPersistContext& context, bool saving)
{
    CTCTavernGame::OnSerialise(context, saving);
}
