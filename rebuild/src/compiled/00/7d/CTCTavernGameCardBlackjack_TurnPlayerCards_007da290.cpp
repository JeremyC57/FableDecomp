// CTCTavernGameCardBlackjack::TurnPlayerCards() -- flips the face-up cards of the current hand face down (with the flip sound), then moves on to the next player hand, or ends the player's round after the last one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA290, length 207 bytes (plain `ret`). Ego_r.pdb
// `?TurnPlayerCards@CTCTavernGameCardBlackjack@@QAEXXZ`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("TAVERN_GAME_CARD_FLIP" @ 0x0126F6C4),
// CTCTavernGame::PlaySound(const CCharString&) @ 0x005EA950 (manifest "CTCBetting::PlaySoundW" is the shared body),
// ~CCharString @ 0x0099EAE0, CCard::Turn @ 0x008F0620, CTCTavernGame::SetState(ETavernGameStates) @ 0x005E9B20 (0x16).
// Layout (retail, as the landed 0x007DA120 / 0x007DA810 model: 12-byte STLport vectors and the 0x318-byte retail
// CTCTavernGameCardBase, so the members sit 0x20..0x2c below Ego_r.pdb): PlayersCards +0x324 (12-byte elements, hence
// the divide by 12), CurrentPlayerCards +0x33c, CurrentPlayerCardsIndex +0x354, HasPlayerFinishedRound +0x369.
// CCard::FaceDown +0x25.
// Shape: the hand walk re-reads `CurrentPlayerCards->end()` every pass; the sound name is a named CCharString in its own
// block (the `push ecx` frame word); the index is a `long` against the unsigned size() (`jb`); STLport operator[] as
// `*(begin() + n)`. The single SetState call after the if/else is tail-duplicated into both arms by VC7.1.
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CCard { public:
    void*         Thing;                     // +0x00
    long          Name;                      // +0x04
    long          Value;                     // +0x08
    unsigned char _pad_0x0c[0x18];
    bool          Turning;                   // +0x24
    bool          FaceDown;                  // +0x25
    void Turn();                             // 0x008F0620
};
template<class T> struct CVector {              // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned int size() const { return end() - begin(); }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
struct CTCTavernGameCardBlackjack {
    void* __vfptr;                              // +0x00 (CTCBase)
    unsigned char _base04[0x318 - 0x04];        // CTCTavernGameCardBase (retail 0x318)
    CVector<CCard*> DealersCards;               // +0x318
    CVector< CVector<CCard*> > PlayersCards;    // +0x324
    CVector<long> Bets;                         // +0x330
    CVector<CCard*>* CurrentPlayerCards;        // +0x33c
    CVector<CCard*>* CurrentDealingCards;       // +0x340
    CCard* NewlyDealtCard;                      // +0x344
    float MostMoneyWon;                         // +0x348
    float MoneyWonThisSitting;                  // +0x34c
    long  Answer;                               // +0x350
    long  CurrentPlayerCardsIndex;              // +0x354
    long  CurrentDealingCardsIndex;             // +0x358
    long  CurrentPackCardIndex;                 // +0x35c
    float CurrentBet;                           // +0x360
    long  WinState;                             // +0x364
    bool  HasPlayerSplit;                       // +0x368
    bool  HasPlayerFinishedRound;               // +0x369
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950 (CTCTavernGame)
    void SetState(long state);                  // 0x005E9B20 (CTCTavernGame)
    void TurnPlayerCards();
};

void CTCTavernGameCardBlackjack::TurnPlayerCards()
{
    for (CCard** it = CurrentPlayerCards->begin(); it != CurrentPlayerCards->end(); ++it) {
        if (!(*it)->FaceDown) {
            {
                CCharString sound("TAVERN_GAME_CARD_FLIP");
                PlaySound(sound);
            }
            (*it)->Turn();
        }
    }
    ++CurrentPlayerCardsIndex;
    if (CurrentPlayerCardsIndex >= PlayersCards.size()) {
        CurrentPlayerCards = 0;
        HasPlayerFinishedRound = true;
        CurrentPlayerCardsIndex = 0;
    } else {
        CurrentPlayerCards = &PlayersCards[CurrentPlayerCardsIndex];
    }
    SetState(0x16);
}