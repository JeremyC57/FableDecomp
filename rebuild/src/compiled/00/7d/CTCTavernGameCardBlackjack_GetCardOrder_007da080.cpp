// CTCTavernGameCardBlackjack::GetCardOrder() -- the card order (blackjack ignores the game type)
// Leaf: ?GetCardOrder@CTCTavernGameCardBlackjack@@UAEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DA080, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 57 of CTCTavernGameCardBlackjack's vtable 0x0126F8F4 (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardBlackjack); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts GetCardOrder(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetCardOrder@CTCTavernGameCardBlackjack@@UAEJXZ; `ret` matches the signature.
// Was: CTCTavernGameCardBlackjack::vfunc_57 (rebuild/src/compiled/00/7d/gapscan_sub007da080_007da080.cpp).
// No shared header for CTCTavernGameCardBlackjack: minimal local definition on its RTTI base CTCTavernGameCardBase.
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
#include "engine/card_sorting_types.h"
class CTCTavernGameCardBlackjack : public CTCTavernGameCardBase {
public:
    virtual long GetCardOrder();                             // slot 57
};
long CTCTavernGameCardBlackjack::GetCardOrder()
{
    return CardOrder;
}
