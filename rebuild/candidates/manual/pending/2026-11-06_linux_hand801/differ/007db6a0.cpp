// CTCTavernGameCardBlackjack::CardsMainGame() -- the blackjack state machine: when the round is finished, step to the next live hand and settle it against the dealer (win / draw / lose, money, sounds, the result text); otherwise run the play state (bust, deal, split / double offers, the dealer's turn)
// Grade: DIFFER(2544v2556) -- case 0x1c's tail: ours cross-jumps its `mov ecx,esi ; call SetState` into the 0x17-empty block's identical tail (0x007DBB23), retail keeps it and jumps to the later ebx=0 epilogue copy 0x007DC03E (after 0x20/0x21's DealCard); plus two register picks in the 0x1d money check (ecx/edx v eax/ecx); 63 lines, 10 attempts
// Next idea: a 0x1c spelling whose return lands on the 0x20/0x21 epilogue copy (retail merges it into the later copy): 0x1c / 0x20 / 0x21 sharing a scope or a common exit
// Leaf: ?CardsMainGame@CTCTavernGameCardBlackjack@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DB6A0, 2556 bytes, plain `ret`; switch table 0x007DC060 (states 0x17..0x25).
// Identity: Ego_r.pdb / FableWin.pdb ?CardsMainGame@CTCTavernGameCardBlackjack@@UAEXXZ (public virtual).
// Callees: CTCTavernGame::GetDef 0x005EA000 (out of line), PlaySoundW 0x005EA950, SetState 0x005E9B20, SetStateAfterTime
//   0x005E9B70, GetText 0x005E9E70, SetStateAfterOK 0x005EB080, SetStateAfterYesNo 0x005EAF30; CCard::Turn 0x008F0620;
//   CTCTavernGameCardBlackjack HasPlayerWonHand 0x007DA900, HasPlayerDrawnHand 0x007DA9B0, HasGoneBust 0x007DA8B0 (inlined
//   twice, called twice), GetHandValue 0x007DA230, DealCard 0x007DAEF0, Split 0x007DB5C0, CanSplit 0x007DA1A0;
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0. CardsFaceDown (Ego_r.pdb
//   ?CardsFaceDown@CTCTavernGameCardBlackjack@@QAE_NXZ) is inline.
// Layout (retail, STLport 12-byte vectors): CTCTavernGame (engine/CTCTavernGame.h retail branch) GameWon +0x21, BaseBet +0x34,
//   TotalWinnings +0x3C, CurrentMoney +0x40, Draw +0xA1, TavernGameState +0xC0; CTCTavernGameCardBase (the landed dtor
//   0x007D5E80 model) Cards +0x128, RoundFinished +0x158, CurrentMovingCard +0x310; CTCTavernGameCardBlackjack (the landed
//   sibling rows) DealersCards +0x318 .. Doubled +0x374; CTavernGameDef text ids (engine/CTavernGameDef.h).
// Retail-asm-helper: GFFloatToLongFloor (0x007DB91A..0x007DB94B, the engine's inline-asm floor of Bets[0] * 1.5f).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#include "engine/CTCTavernGame.h"
#include "engine/CTavernGameDef.h"
#include "engine/CRightHandedSet.h"
#include "engine/C3DVector.h"
#include "engine/GFFloatToLongFloor.h"
#include "stlport/fable_containers.h"

class CThingObject;
enum ECardGameType { };

class CCard {
public:
    void Turn();                                         // 0x008F0620 ?Turn@CCard@@QAEXXZ
    void*         Thing;                                 // +0x00 CThingObject*
    long          Name;                                  // +0x04
    long          Value;                                 // +0x08 (an ace is 11)
    unsigned char _pad_0x0c[0x18];
    bool          Turning;                               // +0x24
    bool          FaceDown;                              // +0x25
};

class CTCTavernGameCardBase : public CTCTavernGame {
public:
    CRightHandedSet                              GamePositionRHSet;          // +0xf0
    C3DVector                                    GamePositionPos;            // +0x108
    long                                         GamePositionActionPointIndex; // +0x114
    float                                        MinX;                       // +0x118
    float                                        MaxX;                       // +0x11c
    float                                        MinY;                       // +0x120
    float                                        MaxY;                       // +0x124
    std::vector<CCard*>                          Cards;                      // +0x128
    std::vector<CCard*>                          Pack;                       // +0x134
    CThingObject*                                Pointer;                    // +0x140
    float                                        PointerScale;               // +0x144
    std::vector<CIntelligentPointer<CThing> >    ParticleEffects;            // +0x148
    float                                        Counter;                    // +0x154
    bool                                         RoundFinished;              // +0x158
    float                                        Times[52];                  // +0x15c
    float                                        BestTimes[52];              // +0x22c
    long                                         NumCards;                   // +0x2fc
    ECardGameType                                GameType;                   // +0x300
    long                                         CardOrder;                  // +0x304
    CCard*                                       SelectedCard;               // +0x308
    CCard*                                       HighlightedCard;            // +0x30c
    long                                         CurrentMovingCard;          // +0x310
    unsigned char                                _pad_0x314[0x4];            // +0x314
};
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, Cards) == 0x128);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, RoundFinished) == 0x158);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBase, CurrentMovingCard) == 0x310);

class CTCTavernGameCardBlackjack : public CTCTavernGameCardBase {
public:
    std::vector<CCard*>                  DealersCards;               // +0x318
    std::vector<std::vector<CCard*> >    PlayersCards;               // +0x324
    std::vector<long>                    Bets;                       // +0x330
    std::vector<CCard*>*                 CurrentPlayerCards;         // +0x33c
    std::vector<CCard*>*                 CurrentDealingCards;        // +0x340
    CCard*                               NewlyDealtCard;             // +0x344
    float                                MostMoneyWon;               // +0x348
    float                                MoneyWonThisSitting;        // +0x34c
    long                                 Answer;                     // +0x350
    long                                 CurrentPlayerCardsIndex;    // +0x354
    long                                 CurrentDealingCardsIndex;   // +0x358
    long                                 CurrentPackCardIndex;       // +0x35c
    float                                CurrentBet;                 // +0x360
    long                                 WinState;                   // +0x364 EBlackjackWinStates
    bool                                 HasPlayerSplit;             // +0x368
    bool                                 HasPlayerFinishedRound;     // +0x369
    bool                                 HasDealerStartedTakingCards; // +0x36a
    long                                 AmountWonThisRound;         // +0x36c
    long                                 NumHandsWonThisRound;       // +0x370
    bool                                 Doubled;                    // +0x374

    virtual void CardsMainGame();
    void Split();                                                    // 0x007DB5C0 (non-virtual: not in the vtable 0x0126F8F4)
    void DealCard();                                                 // 0x007DAEF0
    bool CanSplit();                                                 // 0x007DA1A0
    long GetHandValue(std::vector<CCard*>* cards, bool includeFaceDown);   // 0x007DA230
    bool HasPlayerWonHand();                                         // 0x007DA900
    bool HasPlayerDrawnHand();                                       // 0x007DA9B0
    bool HasGoneBust()                                               // 0x007DA8B0 (landed body)
    {
        std::vector<CCard*>* hand = CurrentPlayerCards;
        long total = 0;
        long aces = 0;
        for (CCard** it = hand->begin(); it != hand->end(); ++it) {
            long value = (*it)->Value;
            if (value == 11)
                ++aces;
            total += value;
        }
        while (total > 21 && aces > 0) {
            total -= 10;
            --aces;
        }
        return total > 21;
    }
    bool CardsFaceDown()
    {
        for (CCard** it = CurrentPlayerCards->begin(); it != CurrentPlayerCards->end(); ++it)
            if (!(*it)->FaceDown)
                return false;
        return true;
    }
};
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBlackjack, DealersCards) == 0x318);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGameCardBlackjack, Doubled) == 0x374);

void CTCTavernGameCardBlackjack::CardsMainGame()
{
    CDefPointer<const CTavernGameDef> def;
    GetDef(&def);
    if (PlayersCards.size() == 0) {
        RoundFinished = true;
        CurrentPlayerCardsIndex = 0;
    }
    if (RoundFinished) {
        do {
            if ((unsigned long)CurrentPlayerCardsIndex >= PlayersCards.size()) {
                for (CCard** it = Cards.begin(); it != Cards.end(); ++it) {
                    if (!(*it)->FaceDown) {
                        PlaySoundW("TAVERN_GAME_CARD_FLIP");
                        (*it)->Turn();
                    }
                }
                CurrentPlayerCards = 0;
                SetState((ETavernGameStates)0x19);
                GameWon = NumHandsWonThisRound >= 0;
                Draw = NumHandsWonThisRound == 0;
                return;
            }
            CurrentPlayerCards = &PlayersCards[CurrentPlayerCardsIndex];
            if (HasGoneBust() || CardsFaceDown())
                ++CurrentPlayerCardsIndex;
        } while (HasGoneBust() || CardsFaceDown());
        if (HasPlayerWonHand()) {
            long winnings;
            std::vector<CCard*>* hand;
            if (!HasPlayerSplit && GetHandValue(hand = CurrentPlayerCards, true) == 21 && hand->size() == 2)
                winnings = GFFloatToLongFloor(Bets[0] * 1.5f);
            else
                winnings = Bets[0];
            CurrentMoney += Bets[0] + winnings;
            TotalWinnings += winnings;
            WinState = 2;
            AmountWonThisRound += winnings;
            ++NumHandsWonThisRound;
            PlaySoundW("TAVERN_GAME_WIN_GAME");
        } else if (HasPlayerDrawnHand()) {
            CurrentMoney += Bets[0];
            WinState = 4;
            PlaySoundW("TAVERN_GAME_WIN_GAME");
        } else {
            TotalWinnings -= Bets[0];
            WinState = 3;
            AmountWonThisRound -= Bets[0];
            --NumHandsWonThisRound;
            PlaySoundW("TAVERN_GAME_LOSE_GAME");
        }
        unsigned long phrase;
        if (WinState == 2)
            phrase = def->WinPhrase;
        else if (WinState == 4)
            phrase = def->DrawPhrase;
        else
            phrase = def->LosePhrase;
        SetStateAfterOK(GetText(phrase), GetText(def->Continue), (ETavernGameStates)0x26);
        return;
    }
    if (HasPlayerFinishedRound && TavernGameState != 0x25 && TavernGameState != 0x23 && TavernGameState != 0x24)
        SetState((ETavernGameStates)0x23);
    switch (TavernGameState) {
    case 0x25:
        DealCard();
        break;
    case 0x17: {
        WinState = 0;
        if (PlayersCards.size() == 0) {
            for (CCard** it = Cards.begin(); it != Cards.end(); ++it) {
                if (!(*it)->FaceDown) {
                    PlaySoundW("TAVERN_GAME_CARD_FLIP");
                    (*it)->Turn();
                }
            }
            SetState((ETavernGameStates)0x19);
            return;
        }
        long index = CurrentPlayerCardsIndex;
        std::vector<CCard*>* hand = &PlayersCards[index];
        CurrentPlayerCards = hand;
        if (HasGoneBust()) {
            WinState = 1;
            TotalWinnings -= Bets[index];
            AmountWonThisRound -= Bets[index];
            --NumHandsWonThisRound;
            PlaySoundW("TAVERN_GAME_LOSE_GAME");
            SetStateAfterOK(GetText(def->BlackjackBusted), GetText(def->Continue), (ETavernGameStates)0x26);
            return;
        }
        if (hand->size() < 2) {
            CurrentDealingCards = hand;
            CurrentDealingCardsIndex = index;
            CurrentMovingCard = -1;
            DealCard();
            return;
        }
        if (CanSplit()) {
            SetStateAfterYesNo(GetText(def->BlackjackSplitGUI), GetText(def->Yes), (ETavernGameStates)0x1c, GetText(def->No),
                               (ETavernGameStates)0x1d);
            return;
        }
        if (GetHandValue(hand, true) != 21 && !Doubled) {
            if (CurrentMoney >= BaseBet && CurrentDealingCards->size() == 2)
                SetState((ETavernGameStates)0x1d);
            else
                SetState((ETavernGameStates)0x1e);
            return;
        }
        Doubled = false;
        CurrentPlayerCardsIndex = ++index;
        if (index == PlayersCards.size()) {
            HasPlayerFinishedRound = true;
            CurrentPlayerCardsIndex = 0;
            SetStateAfterOK(GetText(def->ClickToContinue), GetText(def->Continue), (ETavernGameStates)0x23);
        }
        return;
    }
    case 0x1c:
        Split();
        SetState((ETavernGameStates)0x14);
        return;
    case 0x1e:
        SetStateAfterYesNo(GetText(def->BlackjackHitGUI), GetText(def->Yes), (ETavernGameStates)0x21, GetText(def->No),
                           (ETavernGameStates)0x1f);
        return;
    case 0x1d:
        SetStateAfterYesNo(GetText(def->BlackjackDoubleGUI), GetText(def->Yes), (ETavernGameStates)0x20, GetText(def->No),
                           (ETavernGameStates)0x1e);
        return;
    case 0x1f:
        SetStateAfterYesNo(GetText(def->BlackjackStandGUI), GetText(def->Yes), (ETavernGameStates)0x22, GetText(def->No),
                           (ETavernGameStates)0x17);
        return;
    case 0x20:
    case 0x21:
        if (TavernGameState == 0x20) {
            Bets[CurrentPlayerCardsIndex] += BaseBet;
            CurrentMoney -= BaseBet;
            Doubled = true;
        }
        CurrentDealingCards = CurrentPlayerCards;
        CurrentDealingCardsIndex = CurrentPlayerCardsIndex;
        CurrentMovingCard = -1;
        DealCard();
        return;
    case 0x22:
        if (++CurrentPlayerCardsIndex == PlayersCards.size()) {
            HasPlayerFinishedRound = true;
            CurrentPlayerCardsIndex = 0;
            SetState((ETavernGameStates)0x23);
        } else {
            SetState((ETavernGameStates)0x17);
        }
        return;
    case 0x23:
        if (DealersCards[0]->FaceDown) {
            HasDealerStartedTakingCards = true;
            PlaySoundW("TAVERN_GAME_CARD_FLIP");
            DealersCards[0]->Turn();
            SetStateAfterTime((ETavernGameStates)0x16, 0.5f);
            return;
        }
        if (GetHandValue(&DealersCards, true) < 17) {
            for (CurrentPlayerCards = PlayersCards.begin(); CurrentPlayerCards != PlayersCards.end(); ++CurrentPlayerCards) {
                if (!HasGoneBust()) {
                    std::vector<CCard*>* hand = CurrentPlayerCards;
                    if (GetHandValue(hand, true) != 21 || hand->size() != 2) {
                        SetStateAfterOK(GetText(def->BlackjackDealerTakesCard), GetText(def->Continue), (ETavernGameStates)0x24);
                        return;
                    }
                }
            }
        }
        CurrentPlayerCards = PlayersCards.begin();
        RoundFinished = true;
        CurrentPlayerCardsIndex = 0;
        SetState((ETavernGameStates)0x17);
        return;
    case 0x24:
        CurrentDealingCards = &DealersCards;
        CurrentDealingCardsIndex = PlayersCards.size();
        CurrentMovingCard = -1;
        DealCard();
        return;
    }
}
