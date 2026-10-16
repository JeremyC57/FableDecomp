// CTCTavernGameCoinGolf::CTCTavernGameCoinGolf(CThing* owner) -- constructor: tavern-game ctor, the inlined coin-base members (coin vector, held coin, arrow) zeroed, own vtable, target round = the def's last prize score
// Leaf: ??0CTCTavernGameCoinGolf
// Retail: Fable.exe @ 0x007D49E0, 122 bytes (`ret 4`). Vtable store 0x0126F49C = CTCTavernGameCoinGolf (rtti-vtables.tsv);
//         byte-identical to the CTCTavernGameShoveHaPenny ctor 0x007D09C0 apart from the vtable.
// Callees: CTCTavernGame::CTCTavernGame(CThing*) 0x005EC8A0; CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000;
//          _ftol 0x00BFEA70; the def's virtual slot 1 when its count reaches 0.
// Layout: retail CTCTavernGameCoinBase = Ego_r.pdb minus 0x18: Arrow +0xf0, Coins (12-byte vector) +0xfc, Held +0x108;
//         CTCTavernGame::TargetRound +0x9c (PDB +0xb8). Def: PrizeScores (8-byte {Score, Mult}) end pointer +0x178.
// Shape: the coin-base constructor is inline and stores no vtable of its own (`__declspec(novtable)` would be the
//        source-level reason; VC7.1 keeps a base vtable store otherwise) -- its zero words precede the derived vtable
//        store, Held in the init list and Arrow as a body assignment (retail stores Arrow after Held); the def pointer is released by the CDefPointer dtor's inline ReleaseTemporary() without a null test.
class CThing;
struct CThingObject;

struct CPrizeScoreDef { float Score; long Mult; };

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CTavernGameDef : CDefVirtual, CRefCounted {
    unsigned char _pad_0x08[0x16c];
    CPrizeScoreDef* PrizeScoresBegin;           // +0x174
    CPrizeScoreDef* PrizeScoresEnd;             // +0x178
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { Object->ReleaseTemporary(); }
    CTavernGameDef* operator->() const { return Object; }
};

template <class T> struct SRetailAllocProxy {   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    T* _M_data;
    SRetailAllocProxy(T* data) : _M_data(data) {}
};
template <class T> struct CRetailVector {       // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    SRetailAllocProxy<T> _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};

struct Coin { unsigned char _data[0x3c]; };

struct CTCTavernGame {
    CTCTavernGame(CThing* owner);              // 0x005EC8A0
    virtual void v00();
    void GetTavernGameDef(CDefPointer* out);   // 0x005EA000
    unsigned char _pad_0x04[0x98];
    long          TargetRound;                 // +0x9c
    unsigned char _pad_0xa0[0x50];
};

struct __declspec(novtable) CTCTavernGameCoinBase : CTCTavernGame {
    CThingObject*       Arrow;                 // +0xf0
    CThingObject*       Arrow1;                // +0xf4
    CThingObject*       Arrow2;                // +0xf8
    CRetailVector<Coin> Coins;                 // +0xfc
    Coin*               Held;                  // +0x108
    CTCTavernGameCoinBase(CThing* owner) : CTCTavernGame(owner), Held(0) { Arrow = 0; }
};

struct CTCTavernGameCoinGolf : CTCTavernGameCoinBase {
    CTCTavernGameCoinGolf(CThing* owner);
    virtual void v00();
};

CTCTavernGameCoinGolf::CTCTavernGameCoinGolf(CThing* owner)
    : CTCTavernGameCoinBase(owner)
{
    CDefPointer def;
    GetTavernGameDef(&def);
    TargetRound = (long)(def->PrizeScoresEnd - 1)->Score;
}
