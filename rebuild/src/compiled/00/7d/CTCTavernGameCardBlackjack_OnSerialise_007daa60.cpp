// CTCTavernGameCardBlackjack::OnSerialise(CPersistContext&, bool) -- serialises the card-game base state, then MostMoneyWon (+0x348) through CPersistContext::Transfer<float>, default 0.0f
// Leaf: ?OnSerialise@CTCTavernGameCardBlackjack@@UAEXAAVCPersistContext@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DAA60, thiscall, `ret 8`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 1 of CTCTavernGameCardBlackjack's vtable 0x0126F8F4 (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardBlackjack); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts OnSerialise(CPersistContext &, bool) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnSerialise@CTCTavernGameCardBlackjack@@UAEXAAVCPersistContext@@_N@Z; `ret` matches the signature.
// Was: CTCTavernGameCardBlackjack::vfunc_1 (rebuild/src/compiled/00/7d/CTCTavernGameCardBlackjack_vfunc1_007daa60.cpp).
// No shared header for CTCTavernGameCardBlackjack: minimal local definition on its RTTI base CTCTavernGameCardBase, Ego_r.pdb
// members up to MostMoneyWon (retail offsets: the three STLport vectors are 12 bytes each, so MostMoneyWon is +0x348).
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
#include "engine/card_sorting_types.h"
#include "engine/CPersistContext.h"
class CTCTavernGameCardBlackjack : public CTCTavernGameCardBase {
public:
    virtual void OnSerialise(CPersistContext&, bool);       // slot 1
private:
    std::vector<CCard*> DealersCards;                       // +0x318
    std::vector<std::vector<CCard*> > PlayersCards;         // +0x324
    std::vector<long> Bets;                                 // +0x330
    std::vector<CCard*>* CurrentPlayerCards;                // +0x33c
    std::vector<CCard*>* CurrentDealingCards;               // +0x340
    CCard* NewlyDealtCard;                                  // +0x344
    float MostMoneyWon;                                     // +0x348
};
void CTCTavernGameCardBlackjack::OnSerialise(CPersistContext& context, bool transfer)
{
    CTCTavernGameCardBase::OnSerialise(context, transfer);
    context.Transfer("MostMoneyWon", MostMoneyWon, 0.0f);
}
