#ifndef FABLE_ENGINE_CTCTAVERNGAMECARDBLACKJACK_H
#define FABLE_ENGINE_CTCTAVERNGAMECARDBLACKJACK_H
// CTCTavernGameCardBlackjack (agentC40, row 0x007DB2E0 NewRound): the blackjack card game (vtable 0x0126F8F4; slot 32 NewRound
// 0x007DB2E0 at 0x0126F974). Members: Ego_r.pdb names and order (936 bytes there) at retail = PDB - 0x20 (the card base is 0x318 in
// retail), as the landed destructor 0x007DCA90 models them: DealersCards +0x318, PlayersCards +0x324 (0x007DB304), Bets +0x330,
// CurrentPackCardIndex +0x35C (0x007DB361), HasPlayerSplit +0x368, Doubled +0x374 (0x007DB59F). Overrides: Ego_r.pdb names.
// Declarations only.
#include <stddef.h>
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CTCTavernGameCardBase.h"
enum EBlackjackWinStates;                                                // MS forward enum (Ego_r.pdb member type; values not needed here)
class CTCTavernGameCardBlackjack : public CTCTavernGameCardBase {
public:
    virtual void NewRound();                                             // 32 0x007DB2E0
    std::vector<CCard*>               DealersCards;                      // +0x318
    std::vector<std::vector<CCard*> > PlayersCards;                      // +0x324
    std::vector<long>                 Bets;                              // +0x330
    std::vector<CCard*>*              CurrentPlayerCards;                // +0x33c
    std::vector<CCard*>*              CurrentDealingCards;               // +0x340
    CCard*                            NewlyDealtCard;                    // +0x344
    float                             MostMoneyWon;                      // +0x348
    float                             MoneyWonThisSitting;               // +0x34c
    long                              Answer;                            // +0x350
    long                              CurrentPlayerCardsIndex;           // +0x354
    long                              CurrentDealingCardsIndex;          // +0x358
    long                              CurrentPackCardIndex;              // +0x35c
    float                             CurrentBet;                        // +0x360
    EBlackjackWinStates               WinState;                          // +0x364
    bool                              HasPlayerSplit;                    // +0x368
    bool                              HasPlayerFinishedRound;            // +0x369
    bool                              HasDealerStartedTakingCards;       // +0x36a
    long                              AmountWonThisRound;                // +0x36c
    long                              NumHandsWonThisRound;              // +0x370
    bool                              Doubled;                           // +0x374
};
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBlackjack, DealersCards) == 0x318);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBlackjack, CurrentPackCardIndex) == 0x35c);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBlackjack, Doubled) == 0x374);
FABLE_STATIC_ASSERT(sizeof(CTCTavernGameCardBlackjack) == 0x378);
#endif
