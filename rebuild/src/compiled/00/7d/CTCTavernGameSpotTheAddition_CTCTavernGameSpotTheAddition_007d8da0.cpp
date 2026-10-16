// CTCTavernGameSpotTheAddition::CTCTavernGameSpotTheAddition(CThing* owner) -- constructor: tavern-game ctor, own vtable, empty knick-knack vector, target time from the def's last prize score, pointer and its scale cleared
// Retail: Fable.exe @ 0x007D8DA0, 113 bytes (`ret 4`; vtable 0x0126F814 = CTCTavernGameSpotTheAddition per rtti-vtables.tsv).
// Callees: CTCTavernGame::CTCTavernGame(CThing*) 0x005EC8A0; CTCTavernGame::GetTavernGameDef(CDefPointer*) 0x005EA000; the
//          def's virtual slot 1 when its count reaches 0.
// Layout: retail = Ego_r.pdb minus 0x18: Pointer +0x104, PointerScale +0x108, KnickKnacks (12-byte vector) +0x110;
//         CTCTavernGame::TargetTime +0x98 (PDB +0xb4). Def: PrizeScores (8-byte {Score, Mult}) end pointer +0x178.
// Shape: the vector is a member object (_Vector_base + _STLP_alloc_proxy model) zeroed after the vtable store; the two
//        scalars are body assignments after the def read, in retail store order; the release is the CDefPointer dtor's
//        inline ReleaseTemporary() without a null test (the shared edi zero replaces `test`).
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

struct KnickKnack { unsigned char _data[0x10]; };

struct CTCTavernGame {
    CTCTavernGame(CThing* owner);              // 0x005EC8A0
    virtual void v00();
    void GetTavernGameDef(CDefPointer* out);   // 0x005EA000
    unsigned char _pad_0x04[0x94];
    float         TargetTime;                  // +0x98
    unsigned char _pad_0x9c[0x54];
};

struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    long          CurrentRound;                // +0xf0
    float         CurrentTime;                 // +0xf4
    CThing*       Cover;                       // +0xf8
    float         CoverTimer;                  // +0xfc
    bool          UpdatedKnickKnacks;          // +0x100
    CThingObject* Pointer;                     // +0x104
    float         PointerScale;                // +0x108
    float         DistFromObject;              // +0x10c
    CRetailVector<KnickKnack> KnickKnacks;     // +0x110
    CTCTavernGameSpotTheAddition(CThing* owner);
    virtual void v00();
};

CTCTavernGameSpotTheAddition::CTCTavernGameSpotTheAddition(CThing* owner)
    : CTCTavernGame(owner)
{
    CDefPointer def;
    GetTavernGameDef(&def);
    TargetTime = (def->PrizeScoresEnd - 1)->Score;
    PointerScale = 0.0f;
    Pointer = 0;
}