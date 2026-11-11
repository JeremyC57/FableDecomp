// CTCTavernGameCardBlackjack::CardsMoving() -- blackjack moves its cards like any card game
// Leaf: ?CardsMoving@CTCTavernGameCardBlackjack@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DA070, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 52 of CTCTavernGameCardBlackjack's vtable 0x0126F8F4 (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardBlackjack); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts CardsMoving(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?CardsMoving@CTCTavernGameCardBlackjack@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCTavernGameCardBlackjack::vfunc_52 (rebuild/src/compiled/00/7d/gapscan_sub007da070_007da070.cpp).
// No shared header for CTCTavernGameCardBlackjack: minimal local definition on its RTTI base CTCTavernGameCardBase.
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
#include "engine/card_sorting_types.h"
class CTCTavernGameCardBlackjack : public CTCTavernGameCardBase {
public:
    virtual void CardsMoving();                              // slot 52
};
void CTCTavernGameCardBlackjack::CardsMoving()
{
    CTCTavernGameCardBase::CardsMoving();
}
