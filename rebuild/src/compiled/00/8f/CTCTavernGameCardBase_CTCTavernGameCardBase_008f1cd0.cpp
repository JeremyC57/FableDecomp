// CTCTavernGameCardBase::CTCTavernGameCardBase(CThing* thing) -- CTCTavernGame(thing), empty card / pack / particle-effect vectors, no selected or highlighted card, no pointer object
// Leaf: ??0CTCTavernGameCardBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F1CD0, length 105 bytes (`ret 4`); stores vtable 0x01288294 (CTCTavernGameCardBase,
// rtti-vtables.tsv).
// Callees: CTCTavernGame::CTCTavernGame(CThing*) @ 0x005EC8A0.
// Layout (Ego_r.pdb CTCTavernGameCardBase 0x338, retail: CTCTavernGame is 0xF0 and every vector 12 bytes): GamePositionRHSet +0xF0,
// GamePositionPos +0x108, GamePositionActionPointIndex +0x114, MinX..MaxY +0x118..+0x124, Cards +0x128, Pack +0x134, Pointer +0x140,
// PointerScale +0x144, ParticleEffects +0x148, Counter +0x154, RoundFinished +0x158, Times[52] +0x15C, BestTimes[52] +0x22C, NumCards +0x2FC,
// GameType +0x300, CardOrder +0x304, SelectedCard +0x308, HighlightedCard +0x30C, CurrentMovingCard +0x310.
// Shape: the three vectors (STLport _Vector_base + _STLP_alloc_proxy model) and the two card pointers in the init list, one zero register;
// PointerScale and Pointer are body assignments (stored last, PointerScale first).
struct CThing;
struct CCard;
struct CThingObject;
template <class T> struct CAllocator {              // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {             // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template<class T> struct CVector : CVectorBase<T> { // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
};
struct CIntelligentPointerThing { void* m_vfptr; CThing* m_pItem; };
struct CTCTavernGame {
    virtual ~CTCTavernGame();
    unsigned char _body04[0xf0 - 0x04];         // CTCBase + CTCTavernGame members (retail 0xF0)
    CTCTavernGame(CThing* thing);               // 0x005EC8A0
};
struct CTCTavernGameCardBase : CTCTavernGame {  // vtable 0x01288294
    unsigned char GamePositionRHSet[0x18];      // +0xf0 CRightHandedSet
    float GamePositionPos[3];                   // +0x108 C3DVector
    long  GamePositionActionPointIndex;         // +0x114
    float MinX, MaxX, MinY, MaxY;               // +0x118
    CVector<CCard*> Cards;                      // +0x128
    CVector<CCard*> Pack;                       // +0x134
    CThingObject* Pointer;                      // +0x140
    float PointerScale;                         // +0x144
    CVector<CIntelligentPointerThing> ParticleEffects;   // +0x148
    float Counter;                              // +0x154
    bool  RoundFinished;                        // +0x158
    float Times[52];                            // +0x15c
    float BestTimes[52];                        // +0x22c
    long  NumCards;                             // +0x2fc
    long  GameType;                             // +0x300 ECardGameType
    long  CardOrder;                            // +0x304
    CCard* SelectedCard;                        // +0x308
    CCard* HighlightedCard;                     // +0x30c
    long  CurrentMovingCard;                    // +0x310
    CTCTavernGameCardBase(CThing* thing);
    virtual ~CTCTavernGameCardBase();
};

CTCTavernGameCardBase::CTCTavernGameCardBase(CThing* thing)
    : CTCTavernGame(thing)
    , SelectedCard(0)
    , HighlightedCard(0)
{
    PointerScale = 0;
    Pointer = 0;
}