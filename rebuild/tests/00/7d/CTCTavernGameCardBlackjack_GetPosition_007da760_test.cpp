// Behaviour test for 007da760: src/007da760.cpp copied verbatim; a card def with two layouts (per-card offsets in
// CardOrders[0], per-hand offsets in CardOrders[hands]) so the scaled sum, the table height and the def release are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCTavernGameCardBlackjack::GetPosition(long hand, long card) -- table position of a card slot: the layout's card offset plus the hand offset of the layout picked by the number of hands, scaled by the card size, at the table height
// Retry 10b (B-12 sweep): the offset sum is bound to a const REFERENCE (`const C2DVector& offset = hand + card;`, the operator+ temporary): retail loads the card offset X right after addressing it, before the hand offset; a value local loads it at the fadd.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA760, length 164 bytes (`ret 0Ch`: hidden C3DVector result + 2).
// Ego_r.pdb `?GetPosition@CTCTavernGameCardBlackjack@@QAE?AVC3DVector@@JJ@Z`.
// Callees: CTCTavernGameCardBase::GetCardDef(CDefPointer<const CTavernGameCardBaseDef>*) @ 0x008F0B20; the def's vtable
// slot 1 on the last release.
// Layout: CTavernGameCardBaseDef (PDB offsets hold) CardWidth +0x2c, CardHeight +0x30, TableSeparation +0x60, CardOrders
// (vector<CCardPositionsDef>) +0x6c; CCardPositionsDef is 0x48 bytes in retail (0x3c-byte def base, 12-byte vector) with
// Offset (vector<C2DVector>) at +0x3c. Blackjack PlayersCards +0x324 (12-byte elements, hence the divide by 12).
// Shape: CardOrders[0] holds the per-card offsets, CardOrders[PlayersCards.size()] the per-hand offsets; their sum goes
// through the inline C2DVector operator+ into an 8-byte frame temporary, written hand + card (the argument, the card
// offset, is addressed and loaded first); the C3DVector constructor's arguments evaluate
// right to left (TableSeparation first); the CDefPointer destructor's null test is dropped (the def was dereferenced) and
// the release is the def's inline `DecRef(); if (GetRefCount() == 0) Destroy();`.
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
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
class CCardPositionsDef { public:
    unsigned char      _defbase[0x3c];          // CDefClassBase (retail 0x3c)
    CVector<C2DVector> Offset;                  // +0x3c
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
class CTavernGameCardBaseDef : public CDefVirtual, public CRefCounted { public:
    unsigned char _pad_0x08[0x24];
    float CardWidth;                            // +0x2c
    float CardHeight;                           // +0x30
    unsigned char _pad_0x34[0x2c];
    float TableSeparation;                      // +0x60
    C2DVector PackOffset;                       // +0x64
    CVector<CCardPositionsDef> CardOrders;      // +0x6c
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
class CCard;
struct CTCTavernGameCardBlackjack {
    void* __vfptr;                              // +0x00 (CTCBase)
    unsigned char _base04[0x318 - 0x04];        // CTCTavernGameCardBase (retail 0x318)
    CVector<CCard*> DealersCards;               // +0x318
    CVector< CVector<CCard*> > PlayersCards;    // +0x324
    void GetCardDef(CDefPointer* out);          // 0x008F0B20 (CTCTavernGameCardBase)
    C3DVector GetPosition(long hand, long card);
};

C3DVector CTCTavernGameCardBlackjack::GetPosition(long hand, long card)
{
    CDefPointer def;
    GetCardDef(&def);
    const C2DVector& offset = def->CardOrders[PlayersCards.size()].Offset[hand] + def->CardOrders[0].Offset[card];
    return C3DVector(def->CardWidth * offset.X, def->CardHeight * offset.Y, def->TableSeparation);
}
// ---- end source ----
static int g_destroys = 0;
void CDefVirtual::v00() {}
void CDefVirtual::Destroy() { ++g_destroys; }
static unsigned char g_defRaw[sizeof(CTavernGameCardBaseDef)];   // raw: PackOffset has no default ctor; the count never reaches 0
static CTavernGameCardBaseDef& g_def = *(CTavernGameCardBaseDef*)g_defRaw;
void CTCTavernGameCardBlackjack::GetCardDef(CDefPointer* out) { ++g_def.m_count; out->Object = &g_def; }
int main()
{
    static C2DVector cardOffsets[3] = { C2DVector(1, 2), C2DVector(3, 4), C2DVector(5, 6) };
    static C2DVector handOffsets[2] = { C2DVector(10, 20), C2DVector(30, 40) };
    static CCardPositionsDef orders[2];
    memset(orders, 0, sizeof(orders));
    orders[0].Offset._M_start = cardOffsets; orders[0].Offset._M_finish = cardOffsets + 3;
    orders[1].Offset._M_start = handOffsets; orders[1].Offset._M_finish = handOffsets + 2;
    g_def.m_count = 1; g_def.CardWidth = 2.0f; g_def.CardHeight = 0.5f; g_def.TableSeparation = 7.0f;
    g_def.CardOrders._M_start = orders; g_def.CardOrders._M_finish = orders + 2;
    static unsigned char raw[sizeof(CTCTavernGameCardBlackjack)];
    CTCTavernGameCardBlackjack* bj = (CTCTavernGameCardBlackjack*)raw;
    static CVector<CCard*> hands[1];
    bj->PlayersCards._M_start = hands; bj->PlayersCards._M_finish = hands + 1;   // one hand -> CardOrders[1]
    C3DVector p = bj->GetPosition(1, 2);                 // hand (30,40) + card (5,6) = (35,46)
    CHECK(p.X == 70.0f && p.Y == 23.0f && p.Z == 7.0f);
    CHECK(g_def.m_count == 1 && g_destroys == 0);
    p = bj->GetPosition(0, 0);                           // (10,20) + (1,2) = (11,22)
    CHECK(p.X == 22.0f && p.Y == 11.0f && p.Z == 7.0f);
    if (fails == 0) printf("007da760_TEST PASS\n"); else printf("007da760_TEST FAIL\n");
    return fails;
}