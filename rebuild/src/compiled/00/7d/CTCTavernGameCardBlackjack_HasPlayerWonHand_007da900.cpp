// CTCTavernGameCardBlackjack::HasPlayerWonHand() -- the current hand beats the dealer: dealer bust, higher total, or an unsplit two-card 21 against a dealer 21 of more cards
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA900, length 168 bytes (plain `ret`). Ego_r.pdb
// `?HasPlayerWonHand@CTCTavernGameCardBlackjack@@QAE_NXZ`.
// Callees: CTCTavernGameCardBlackjack::GetHandValue(vector<CCard*>*, bool) @ 0x007DA230 (second call only).
// Layout (retail; local model -- no engine header for this class, and the PDB's 16-byte STLport vectors are 12 bytes in
// retail, so the members sit 0x20 below Ego_r.pdb): DealersCards +0x318, CurrentPlayerCards +0x33c, HasPlayerSplit +0x368.
// CCard::Value +0x08 (an ace is 11), Turning +0x24, FaceDown +0x25.
// Shape: GetHandValue is an in-class inline member. VC7.1 inlines the FIRST call (the dealer's hand; `includeFaceDown`
// = true folds the visibility test away, and the `dealer > 21` test merges into the bust loop's exit) and emits the second
// call out of line. The vector size() tests compile to `(last - first) & ~3 == 8`.
struct CCard {
    void*         Thing;                     // +0x00 CThingObject*
    long          Name;                      // +0x04
    long          Value;                     // +0x08
    unsigned char _pad_0x0c[0x18];
    bool          Turning;                   // +0x24
    bool          FaceDown;                  // +0x25
};

template<class T> struct CRetailVector {     // retail STLport vector: start, finish, end of storage (12 bytes)
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};

struct CTCTavernGameCardBlackjack {
    unsigned char               _pad_0x000[0x318];
    CRetailVector<CCard*>       DealersCards;          // +0x318
    unsigned char               PlayersCards[0xc];     // +0x324 vector<vector<CCard*> >
    unsigned char               Bets[0xc];             // +0x330 vector<long>
    CRetailVector<CCard*>*      CurrentPlayerCards;    // +0x33c
    CRetailVector<CCard*>*      CurrentDealingCards;   // +0x340
    CCard*                      NewlyDealtCard;        // +0x344
    float                       MostMoneyWon;          // +0x348
    float                       MoneyWonThisSitting;   // +0x34c
    long                        Answer;                // +0x350
    long                        CurrentPlayerCardsIndex;   // +0x354
    long                        CurrentDealingCardsIndex;  // +0x358
    long                        CurrentPackCardIndex;      // +0x35c
    float                       CurrentBet;            // +0x360
    long                        WinState;              // +0x364 EBlackjackWinStates
    bool                        HasPlayerSplit;        // +0x368

    long GetHandValue(CRetailVector<CCard*>* cards, bool includeFaceDown)   // 0x007DA230 out of line
    {
        long total = 0;
        long aces = 0;
        for (CCard** it = cards->_M_start; it != cards->_M_finish; ++it) {
            if (includeFaceDown || (*it)->Turning || !(*it)->FaceDown) {
                long value = (*it)->Value;
                if (value == 11)
                    ++aces;
                total += value;
            }
        }
        while (total > 21 && aces > 0) {
            total -= 10;
            --aces;
        }
        return total;
    }
    bool HasPlayerWonHand();
};

bool CTCTavernGameCardBlackjack::HasPlayerWonHand()
{
    long dealer = GetHandValue(&DealersCards, true);
    if (dealer > 21)
        return true;
    long player = GetHandValue(CurrentPlayerCards, true);
    if (dealer < player)
        return true;
    if (!HasPlayerSplit && dealer == 21 && player == dealer && DealersCards.size() != 2 && CurrentPlayerCards->size() == 2)
        return true;
    return false;
}