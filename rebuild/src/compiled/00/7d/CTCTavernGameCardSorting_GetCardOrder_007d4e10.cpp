// CTCTavernGameCardSorting::GetCardOrder() -- the card order, only in a round-limit game (else 0)
// Leaf: ?GetCardOrder@CTCTavernGameCardSorting@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D4E10, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 57 of CTCTavernGameCardSorting's vtable 0x0126F5B4 (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardSorting); the Xbox 360 Test PDB virtual order (third source) aligned on 40 TLC-named slots of that vtable puts GetCardOrder(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetCardOrder@CTCTavernGameCardSorting@@UAEJXZ; `ret` matches the signature.
// Was: CTCTavernGameCardSorting::vfunc_57 (rebuild/src/compiled/00/7d/CTCTavernGameCardSorting_vfunc57_007d4e10.cpp).
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
#include "engine/card_sorting_types.h"
long CTCTavernGameCardSorting::GetCardOrder()
{
    long order = 0;
    switch (GameType) {
    case ECGT_ROUND_LIMIT: order = CardOrder; break;
    }
    return order;
}
