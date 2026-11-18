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
#ifdef FABLE_RETAIL_BLACKJACK_DEAL_PASS23
    // Both original PDBs and original0275a3d0 call graph; native007da760/007daef0.
    C3DVector GetPosition(long,long);
    void DealCard();
    // Both PDB constructor/override records; declarations support true class lifetime.
    CTCTavernGameCardBlackjack(CThing&);
    virtual CCharString GetClassTypeName() const;
    virtual CWideString GetBestScore();
    virtual CWideString GetCurrentScore();
    virtual CWideString GetRequiredScore();
#endif
    virtual void NewRound();                                             // 32 0x007DB2E0
#ifdef FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_CARDSMAINGAME
    // Retail opt-in (agentC42, row 0x007DB6A0 CardsMainGame): the slot-56 override and the members it uses (Ego_r.pdb / FableWin.pdb
    // names; addresses = the landed rows / reloc bindings). HasGoneBust (landed 0x007DA8B0) and CardsFaceDown
    // (?CardsFaceDown@CTCTavernGameCardBlackjack@@QAE_NXZ, no retail out-of-line copy) are defined by the row's TU text.
    virtual void CardsMainGame();                                        // 56 0x007DB6A0
    void Split();                                                        // 0x007DB5C0
    void DealCard();                                                     // 0x007DAEF0
    bool CanSplit();                                                     // 0x007DA1A0
    long GetHandValue(std::vector<CCard*>* cards, bool includeFaceDown); // 0x007DA230
    bool HasPlayerWonHand();                                             // 0x007DA900
    bool HasPlayerDrawnHand();                                           // 0x007DA9B0
    bool HasGoneBust();                                                  // 0x007DA8B0
    bool CardsFaceDown();
#endif
#ifdef FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_SCORETEXT
    // e01 opt-in (rows 0x007DA400 / 0x007DA520 / 0x007DA640): the slot-44 / 45 / 47 overrides (Ego_r.pdb / FableWin.pdb
    // ?GetCurrentScore / ?GetRequiredScore / ?GetBet@CTCTavernGameCardBlackjack@@UAE?AVCWideString@@XZ; Ego_d blackjack TU order) and
    // GetHandValue 0x007DA230 (landed) when CARDSMAINGAME does not declare it. Declarations only: no layout change.
#ifndef FABLE_RETAIL_BLACKJACK_DEAL_PASS23
    virtual CWideString GetCurrentScore();                               // 44 0x007DA400
    virtual CWideString GetRequiredScore();                              // 45 0x007DA520
#endif
    virtual CWideString GetBet();                                        // 47 0x007DA640
#ifndef FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_CARDSMAINGAME
    long GetHandValue(std::vector<CCard*>* cards, bool includeFaceDown); // 0x007DA230
#endif
#endif
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
FABLE_STATIC_ASSERT_AT(37, offsetof(CTCTavernGameCardBlackjack, DealersCards) == 0x318);
FABLE_STATIC_ASSERT_AT(38, offsetof(CTCTavernGameCardBlackjack, CurrentPackCardIndex) == 0x35c);
FABLE_STATIC_ASSERT_AT(39, offsetof(CTCTavernGameCardBlackjack, Doubled) == 0x374);
FABLE_STATIC_ASSERT_AT(40, sizeof(CTCTavernGameCardBlackjack) == 0x378);
#endif
