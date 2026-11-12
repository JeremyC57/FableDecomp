// CTCTavernGameCardBlackjack::CardsFinishedMoving() -- unless the player has split, flips the dealer's second card and the player's first two cards (TAVERN_GAME_CARD_FLIP) and lets them turn up; else straight to the main game
// Leaf: ?CardsFinishedMoving@CTCTavernGameCardBlackjack@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DA120, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 53 of CTCTavernGameCardBlackjack's vtable 0x0126F8F4 (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardBlackjack); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts CardsFinishedMoving(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?CardsFinishedMoving@CTCTavernGameCardBlackjack@@UAEXXZ; `ret` matches the signature.
// Was: CTCTavernGameCardBlackjack::vfunc_53 (rebuild/src/compiled/00/7d/CTCTavernGameCardBlackjack_vfunc53_007da120.cpp).
// No shared header for CTCTavernGameCardBlackjack: minimal local definition on its RTTI base CTCTavernGameCardBase, Ego_r.pdb
// members up to HasPlayerSplit (retail offsets: the three STLport vectors are 12 bytes each, so HasPlayerSplit is +0x368).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_game.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
class CTCTavernGameCardBlackjack : public CTCTavernGameCardBase {
public:
    enum EBlackjackWinStates { };                           // W4EBlackjackWinStates (scope and enumerators not needed here)
    virtual void CardsFinishedMoving();                     // slot 53
private:
    std::vector<CCard*> DealersCards;                       // +0x318
    std::vector<std::vector<CCard*> > PlayersCards;         // +0x324
    std::vector<long> Bets;                                 // +0x330
    std::vector<CCard*>* CurrentPlayerCards;                // +0x33c
    std::vector<CCard*>* CurrentDealingCards;               // +0x340
    CCard* NewlyDealtCard;                                  // +0x344
    float MostMoneyWon;                                     // +0x348
    float MoneyWonThisSitting;                              // +0x34c
    long Answer;                                            // +0x350
    long CurrentPlayerCardsIndex;                           // +0x354
    long CurrentDealingCardsIndex;                          // +0x358
    long CurrentPackCardIndex;                              // +0x35c
    float CurrentBet;                                       // +0x360
    EBlackjackWinStates WinState;                           // +0x364
    bool HasPlayerSplit;                                    // +0x368
};
void CTCTavernGameCardBlackjack::CardsFinishedMoving()
{
    if (!HasPlayerSplit) {
        PlaySoundW("TAVERN_GAME_CARD_FLIP");
        DealersCards[1]->Turn();
        PlayersCards[0][0]->Turn();
        PlayersCards[0][1]->Turn();
        SetState(ETGS_CARDS_TURNING_UP);
    } else {
        SetState(ETGS_CARDS_MAIN_GAME);
    }
}
