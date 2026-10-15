// CTCTavernGameCardBlackjack::EnterGame() -- base EnterGame, then clear this sitting's winnings, the hand/pack indices and the current hand
// Leaf: ?EnterGame@CTCTavernGameCardBlackjack
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA020, length 36 bytes. CTCTavernGameCardBlackjack vtable slot 29;
// Ego_r.pdb `?EnterGame@CTCTavernGameCardBlackjack@@UAEXXZ`.
// Callee: CTCTavernGameCardBase::EnterGame() @ 0x008F1E30 (called non-virtually).
// Layout (retail offsets: Ego_r.pdb CTCTavernGameCardBlackjack minus 0x20 -- the card base ends at 0x318 in retail and the
// three blackjack vectors are 12 bytes): CurrentPlayerCards +0x33c, MoneyWonThisSitting +0x34c (float),
// CurrentPlayerCardsIndex +0x354, CurrentPackCardIndex +0x35c. CanSplit 0x007DA1A0 confirms CurrentPlayerCards +0x33c and
// HasPlayerSplit +0x368. No engine header exists for the class; local model.
// Shape: the stores follow the source order from one eax zero (the float included).
struct CCard;

template<class T> struct CRetailVector {     // retail std::vector: first, last, end
    T* first; T* last; T* end_of_storage;
};

struct CTCTavernGameCardBase {
    virtual void v00();
    unsigned char _pad_0x04[0x314];
    virtual void EnterGame();                // 0x008F1E30
};

struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase {
    unsigned char           _pad_0x318[0x24];        // DealersCards, PlayersCards, Bets
    CRetailVector<CCard*>*  CurrentPlayerCards;      // +0x33c
    CRetailVector<CCard*>*  CurrentDealingCards;     // +0x340
    CCard*                  NewlyDealtCard;          // +0x344
    float                   MostMoneyWon;            // +0x348
    float                   MoneyWonThisSitting;     // +0x34c
    long                    Answer;                  // +0x350
    long                    CurrentPlayerCardsIndex; // +0x354
    long                    CurrentDealingCardsIndex;// +0x358
    long                    CurrentPackCardIndex;    // +0x35c
    virtual void EnterGame();
};

void CTCTavernGameCardBlackjack::EnterGame()
{
    CTCTavernGameCardBase::EnterGame();
    MoneyWonThisSitting = 0.0f;
    CurrentPlayerCardsIndex = 0;
    CurrentPackCardIndex = 0;
    CurrentPlayerCards = 0;
}
