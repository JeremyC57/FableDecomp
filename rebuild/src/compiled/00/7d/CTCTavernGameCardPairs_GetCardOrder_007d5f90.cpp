// CTCTavernGameCardPairs::GetCardOrder() -- the card order, only in a round-limit game (else 0)
// Leaf: ?GetCardOrder@CTCTavernGameCardPairs@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D5F90, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 57 of CTCTavernGameCardPairs's vtable 0x0126F6FC (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardPairs); the Xbox 360 Test PDB virtual order (third source) aligned on 40 TLC-named slots of that vtable puts GetCardOrder(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetCardOrder@CTCTavernGameCardPairs@@UAEJXZ; `ret` matches the signature.
// Was: CTCTavernGameCardPairs::vfunc_57 (rebuild/src/compiled/00/7d/CTCTavernGameCardPairs_vfunc57_007d5f90.cpp).
// No shared header for CTCTavernGameCardPairs: minimal local definition on its RTTI base CTCTavernGameCardBase.
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
#include "engine/card_sorting_types.h"
class CTCTavernGameCardPairs : public CTCTavernGameCardBase {
public:
    virtual long GetCardOrder();                             // slot 57
};
long CTCTavernGameCardPairs::GetCardOrder()
{
    long order = 0;
    switch (GameType) {
    case ECGT_ROUND_LIMIT: order = CardOrder; break;
    }
    return order;
}
