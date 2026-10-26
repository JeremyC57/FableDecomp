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