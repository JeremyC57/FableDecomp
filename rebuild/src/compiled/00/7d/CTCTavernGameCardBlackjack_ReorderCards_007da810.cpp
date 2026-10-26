// CTCTavernGameCardBlackjack::ReorderCards() -- for every card of every player hand play the deal sound and move the card to its laid-out position for that hand and slot
// Leaf: ReorderCards
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA810, length 160 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("TAVERN_GAME_CARD_DEAL" @ 0x0126F568), CTCTavernGame::PlaySound(const
// CCharString&) @ 0x005EA950, ~CCharString @ 0x0099EAE0, CTCTavernGameCardBlackjack::GetPosition(long hand, long card) @ 0x007DA760 (C3DVector
// through the hidden pointer), CCard::Move(const C3DVector&, bool) @ 0x008F0660 (false).
// Layout (retail, as 0x007DA120): PlayersCards (vector<vector<CCard*> >, 12-byte elements) +0x324/+0x328.
// Shape: the hand index is stepped in the outer for header before the hand pointer (`++index, ++hand`, kept in the frame; the other
// order puts the index in ecx); the inner bound
// is `size()` re-read each pass (`sar ; jb`); the sound name is a named CCharString in its own block; STLport operator[] as `*(begin()+n)`
// (begin() read before the Move argument is built).
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};
class CCard { public:
    void Move(const C3DVector& position, bool immediately);   // 0x008F0660
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
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950 (CTCTavernGame)
    C3DVector GetPosition(long hand, long card);        // 0x007DA760
    void ReorderCards();
};

void CTCTavernGameCardBlackjack::ReorderCards()
{
    long handIndex = 0;
    for (CVector<CCard*>* hand = PlayersCards.begin(); hand != PlayersCards.end(); ++handIndex, ++hand) {
        for (unsigned int i = 0; i < hand->size(); ++i) {
            {
                CCharString sound("TAVERN_GAME_CARD_DEAL");
                PlaySound(sound);
            }
            (*hand)[i]->Move(GetPosition(handIndex, i), false);
        }
    }
}