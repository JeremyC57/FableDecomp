// CTCTavernGameCardSorting::NewGame() -- base NewGame, one card in play, the round count = the def's card-order count, counter reset
// Leaf: ?NewGame@CTCTavernGameCardSorting
// Retail: Fable.exe @ 0x007D4ED0, 108 bytes; CTCTavernGameCardSorting vtable 0x0126F5B4 slot 31 (NewGame; the catalog's
//         "vfunc_31").
// Callees: CTCTavernGameCardBase::NewGame() 0x008F0930 (non-virtual base call); CTCTavernGameCardBase::GetCardDef(CDefPointer*)
//          0x008F0B20; the def's virtual slot 1 when its count reaches 0.
// Layout (retail): CTCTavernGame::NumRounds +0x24 (PDB +0x2c); CTCTavernGameCardBase::Counter +0x154, NumCards +0x300;
//         CTavernGameCardBaseDef::CardOrders (vector<CCardPositionsDef>, 72-byte entries, retail 12-byte vector) +0x6c.
// Shape: `size()` divides the byte span by 72 (`imul 0x38E38E39 ; sar 4`); null-tested CDefPointer dtor with the def's inline
//        ReleaseTemporary().
class CCardPositionsDef { public: unsigned char _data[0x48]; };

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
};
class CTavernGameCardBaseDef : public CDefVirtual, public CRefCounted { public:
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
    void NewGameBase();                         // 0x008F0930 (CTCTavernGameCardBase::NewGame)
    void GetCardDef(CDefPointer* out);          // 0x008F0B20
    unsigned char _pad_0x04[0x20];
    long          NumRounds;                    // +0x24
    unsigned char _pad_0x28[0x12c];
    float         Counter;                      // +0x154
    unsigned char _pad_0x158[0x1a8];
    long          NumCards;                     // +0x300
};

struct CTCTavernGameCardSorting : CTCTavernGameCardBase {
    void NewGame();
};

void CTCTavernGameCardSorting::NewGame()
{
    NewGameBase();
    CDefPointer def;
    GetCardDef(&def);
    NumCards = 1;
    NumRounds = def->CardOrders.size();
    Counter = 0.0f;
}