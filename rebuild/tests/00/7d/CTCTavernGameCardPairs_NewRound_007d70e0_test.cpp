// behaviour test for 007d70e0 CTCTavernGameCardPairs::NewRound: base NewRound, deal NumCards (= the layout's position
// count) pack cards onto the table list (in-place and through the overflow insert), then GameType 1, NumRounds = the
// number of layouts, Counter 0, AlternateSelectedCard 0; the def is released
#include <cstdio>
#include <new>
struct CCard { int id; };
struct C2DVector { float X, Y; };
struct __true_type { __true_type() { } };
static int g_overflow = 0, g_baseNewRound = 0, g_destroyed = 0;
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned int size() const { return end() - begin(); }
    T& operator[](unsigned int n) const { return *(begin() + n); }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long, bool)
    {   // test stub: the storage is pre-sized, so just extend the capacity by one and place the value
        ++g_overflow; *position = x; _M_finish = position + 1; _M_end_of_storage = _M_finish;
    }
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};
struct CCardPositionsDef { CRetailVector<C2DVector> Offset; };
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
};
struct CTavernGameCardBaseDef : CDefVirtual, CRefCounted {
    CRetailVector<CCardPositionsDef> CardOrders;
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameCardBaseDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->ReleaseTemporary(); }
    CTavernGameCardBaseDef* operator->() const { return Object; }
};
static CTavernGameCardBaseDef g_def;
struct CTCTavernGameCardBase {
    virtual void NewRound() { ++g_baseNewRound; }
    virtual long GetCardOrder() { return 1; }
    long          NumRounds;
    CRetailVector<CCard*> Cards;
    CRetailVector<CCard*> Pack;
    float         Counter;
    long          NumCards;
    long          GameType;
    void GetCardDef(CDefPointer* out) { ++g_def.m_count; out->Object = &g_def; }
};
struct CTCTavernGameCardPairs : CTCTavernGameCardBase {
    void* AlternateSelectedCard;
    virtual void NewRound();
};
void CTCTavernGameCardPairs::NewRound()
{
    CTCTavernGameCardBase::NewRound();
    CDefPointer def;
    GetCardDef(&def);
    long order = GetCardOrder();
    NumCards = def->CardOrders[order].Offset.size();
    for (long i = 0; i < NumCards; ++i) {
        CCard* card = Pack[i];
        Cards.push_back(card);
    }
    GameType = 1;
    NumRounds = def->CardOrders.size();
    Counter = 0;
    AlternateSelectedCard = 0;
}
int main()
{
    static C2DVector offsets[4];
    static CCardPositionsDef layouts[3];
    for (int i = 0; i < 3; ++i) { layouts[i].Offset._M_start = offsets; layouts[i].Offset._M_finish = offsets + 1 + i; }
    g_def.CardOrders._M_start = layouts; g_def.CardOrders._M_finish = layouts + 3; g_def.m_count = 0;
    static CCard cards[4] = { {10}, {11}, {12}, {13} };
    static CCard* pack[4] = { &cards[0], &cards[1], &cards[2], &cards[3] };
    static CCard* table[8];
    CTCTavernGameCardPairs game;
    game.Pack._M_start = pack; game.Pack._M_finish = pack + 4;
    game.Cards._M_start = table; game.Cards._M_finish = table; game.Cards._M_end_of_storage = table + 1;
    game.GameType = 0; game.NumRounds = 0; game.Counter = 5.0f; game.AlternateSelectedCard = &game;
    game.NewRound();
    bool ok = g_baseNewRound == 1 && game.NumCards == 2 && game.Cards.size() == 2 && table[0] == &cards[0] &&
              table[1] == &cards[1] && g_overflow == 1 && game.GameType == 1 && game.NumRounds == 3 &&
              game.Counter == 0.0f && game.AlternateSelectedCard == 0 && g_destroyed == 1 && g_def.m_count == 0;
    if (!ok) { std::printf("007d70e0_TEST FAIL\n"); return 1; }
    std::printf("007d70e0_TEST PASS\n");
    return 0;
}