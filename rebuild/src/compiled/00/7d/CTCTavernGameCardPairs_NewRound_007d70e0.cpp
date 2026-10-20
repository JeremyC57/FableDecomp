// CTCTavernGameCardPairs::NewRound() -- card-base NewRound, then deals the round (as many pack cards as the card-order layout has positions onto the table list), sets the timed game type, the round count from the def's layouts, and clears the counter and the alternate selected card
// Leaf: ?NewRound@CTCTavernGameCardPairs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D70E0, 244 bytes (plain `ret`); CTCTavernGameCardPairs vtable 0x0126F6FC
// slot 32 (Ego_r.pdb `?NewRound@CTCTavernGameCardPairs@@UAEXXZ`). The CardSorting override 0x007D5A00 (landed) is the
// same deal without the trailing stores and with a ShuffleCards call.
// Callees: CTCTavernGameCardBase::NewRound() 0x008F0960 (non-virtual base call); CTCTavernGameCardBase::GetCardDef(
//          CDefPointer<const CTavernGameCardBaseDef>*) 0x008F0B20; GetCardOrder (slot 57, `call [edx+0E4h]`); the table
//          list's vector<CCard*>::_M_insert_overflow(CCard**, CCard* const&, const __true_type&, size_t, bool) 0x007D5D60
//          (this TU's instantiation); the def's virtual slot 1 when its count reaches zero.
// Layout (retail, as the landed CardSorting NewRound): Cards +0x128, Pack +0x134, NumCards +0x2fc; CTCTavernGame::NumRounds
//          +0x24 (PDB +0x2c); CTCTavernGameCardBase::Counter +0x154 (PDB +0x178), GameType +0x300 (PDB +0x324);
//          CTCTavernGameCardPairs::AlternateSelectedCard +0x318 (PDB +0x338); CTavernGameCardBaseDef CardOrders +0x6c (0x48-byte
//          CCardPositionsDef records, `imul 38E38E39h ; sar 4`).
// Shape: as the CardSorting body; the trailing stores follow the deal in source order (GameType first, the round count
//        from CardOrders.size(), then the two zero stores from the re-zeroed loop register); the def is released through
//        the def's inline `DecRef(); if (GetRefCount() == 0) Destroy();` with its null test.
struct CCard;
struct C2DVector { float X, Y; };

struct __true_type { __true_type() { } };
inline void* operator new(unsigned int, void* where) { return where; }
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned int size() const { return end() - begin(); }
    T& operator[](unsigned int n) const { return *(begin() + n); }
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned long fill_len, bool atend);   // 0x007D5D60
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
};

struct CCardPositionsDef {
    unsigned char _defbase[0x3c];               // CDefClassBase (retail 0x3c)
    CRetailVector<C2DVector> Offset;            // +0x3c
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
struct CTavernGameCardBaseDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0x64];
    CRetailVector<CCardPositionsDef> CardOrders;   // +0x6c
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

struct CTCTavernGameCardBase {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void NewRound();                    // slot 32
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual long GetCardOrder();                // slot 57
    virtual void v58();
    virtual void v59();
    virtual void ShuffleCards();                // slot 60
    unsigned char _pad_0x04[0x20];
    long          NumRounds;                    // +0x24 (CTCTavernGame)
    unsigned char _pad_0x28[0x100];
    CRetailVector<CCard*> Cards;                // +0x128
    CRetailVector<CCard*> Pack;                 // +0x134
    unsigned char _pad_0x140[0x14];
    float         Counter;                      // +0x154
    unsigned char _pad_0x158[0x1a4];
    long          NumCards;                     // +0x2fc
    long          GameType;                     // +0x300
    void GetCardDef(CDefPointer* out);          // 0x008F0B20
};
struct CTCTavernGameCardPairs : CTCTavernGameCardBase {
    unsigned char _pad_0x304[0x14];
    void*         AlternateSelectedCard;        // +0x318 (CCard*)
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