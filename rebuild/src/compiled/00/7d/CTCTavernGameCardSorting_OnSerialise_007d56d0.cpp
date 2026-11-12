// CTCTavernGameCardSorting::OnSerialise(CPersistContext&, bool) -- serialises the card-game base state, then TimeLimitNumRoundsToWin (+0x318) and TimeLimitBestNumRounds (+0x31c) through CPersistContext::Transfer<long>, default 0
// Leaf: ?OnSerialise@CTCTavernGameCardSorting@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D56D0, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCTavernGameCardSorting's vtable 0x0126F5B4 (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardSorting); the Xbox 360 Test PDB virtual order (third source) aligned on 40 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCTavernGameCardSorting@@UAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCTavernGameCardSorting::vfunc_1 (rebuild/src/compiled/00/7d/CTCTavernGameCardSorting_vfunc1_007d56d0.cpp).
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
#include "engine/card_sorting_types.h"
#include "engine/CPersistContext.h"
void CTCTavernGameCardSorting::OnSerialise(CPersistContext& context, bool transfer)
{
    CTCTavernGameCardBase::OnSerialise(context, transfer);
    context.Transfer("TimeLimitNumRoundsToWin", TimeLimitNumRoundsToWin, 0L);
    context.Transfer("TimeLimitBestNumRounds", TimeLimitBestNumRounds, 0L);
}
