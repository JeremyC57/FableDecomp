// CTCTavernGameCardBlackjack::vfunc_53() -- after the deal: unless the player has split, play the card-flip sound, turn the dealer's second card and the player's first two cards and go to state 0x16; otherwise go to state 0x17
// Leaf: vfunc_53
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA120, length 120 bytes (padded row 128); CTCTavernGameCardBlackjack vtable 0x0126F8F4 slot 53.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("TAVERN_GAME_CARD_FLIP" @ 0x0126F6C4), CTCTavernGame::PlaySound(const
// CCharString&) @ 0x005EA950 (manifest "CTCBetting::PlaySoundW" is the shared body), ~CCharString @ 0x0099EAE0, CCard::Turn @ 0x008F0620 (x3),
// CTCTavernGame::SetState(ETavernGameStates) @ 0x005E9B20.
// Layout (Ego_r.pdb CTCTavernGameCardBlackjack, retail with 12-byte vectors and the 0x318-byte retail CTCTavernGameCardBase): DealersCards
// +0x318, PlayersCards (vector<vector<CCard*> >) +0x324, Bets +0x330, CurrentPlayerCards +0x33C, ... HasPlayerSplit +0x368.
// Shape: the sound name is a named CCharString in its own block; STLport operator[] as `*(begin() + n)` (each access re-reads the vector).
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CCard {
    void Turn();                                // 0x008F0620
};
template<class T> struct CVector {              // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
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
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950 (CTCTavernGame)
    void SetState(long state);                  // 0x005E9B20 (CTCTavernGame)
    void vfunc_53();
};

void CTCTavernGameCardBlackjack::vfunc_53()
{
    if (!HasPlayerSplit) {
        {
            CCharString sound("TAVERN_GAME_CARD_FLIP");
            PlaySound(sound);
        }
        DealersCards[1]->Turn();
        PlayersCards[0][0]->Turn();
        PlayersCards[0][1]->Turn();
        SetState(0x16);
    } else {
        SetState(0x17);
    }
}