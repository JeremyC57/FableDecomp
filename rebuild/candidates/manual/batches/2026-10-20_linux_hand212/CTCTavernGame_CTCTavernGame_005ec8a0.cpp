// CTCTavernGame::CTCTavernGame(CThing* thing) -- the tavern-game component: CTCBase(thing), empty UI-component and primitive-handle vectors, null fonts and game master, cleared target and bet timers, five-slot bet / money / winnings handle vectors
// Leaf: ??0CTCTavernGame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EC8A0, 217 bytes (`ret 4`); stores vtable 0x012560BC (CTCTavernGame,
// rtti-vtables.tsv). Ego_r.pdb `??0CTCTavernGame@@QAE@AAVCThing@@@Z` takes a CThing&; the landed derived constructors
// (CTCTavernGameCardBase 0x008F1CD0 and the coin/blackjack ones) bind this address as `CTCTavernGame(CThing*)`, which
// emits the same code, so that spelling is kept for one symbol per address.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800 (Ego_r.pdb spelling; 92 landed callers write CThing*); CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10 (then the
//          CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0); vector<CEnginePrimitiveHandle>::Resize(long)
//          0x00443F90 (x3, five handles each). Data: the 0.0 double pool constant 0x0122ED70.
// Layout (retail; engine/CTCTavernGame.h carries Ego_r.pdb offsets -- every STLport vector is 12 bytes here and the
//         double realigns -- so a local model): UIComponents +0x0c, LastRenderTime +0x18, Handles +0x44, BoxHandles
//         +0x50, BetHandles +0x5c, MoneyHandles +0x68, WinningsHandles +0x74, PScoreFont/PTargetFont/PStatsFont
//         +0x80/+0x88/+0x90, TargetTime +0x98, TargetRound +0x9c, PGameMaster +0xd0, FrameBetWasLastChanged +0xdc,
//         FrameBetChangeStarted +0xe0, HasPrizeBeenGiven +0xe9; the class is 0xf0 bytes (8-aligned by the double).
// Shape: the vectors (STLport _Vector_base + _STLP_alloc_proxy model), the double, the counted pointers and the
//        intelligent pointer are the initialiser list (one shared zero register; the 0.0 load hoisted above the vtable
//        store); the five scalar clears and the three Resize calls are the constructor body, in source order (the
//        vector addresses of the first two Resize calls are formed early in edi/ebp).
struct CThing;
struct CThingAICreature;
struct CFontBank;
struct CCPPointerInfo;
struct CEnginePrimitiveHandle;
namespace NUISystem { struct CComponent; }

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
template <class T> struct CVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
};
struct CHandleVector : CVector<CEnginePrimitiveHandle> {   // vector<CEnginePrimitiveHandle>
    void Resize(long count);                    // 0x00443F90
};

template <class T> struct CCountedPointer {         // Data + Info (8 bytes)
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};

struct CBaseObject;
struct CBaseIntelligentPointer {                    // 8 bytes: vptr + item
    virtual void v00();
    CBaseObject* PItem;
    CBaseIntelligentPointer();                      // 0x00A01B10
};
struct CIntelligentPointerAICreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingAICreature>, vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointerAICreature() { }
};

struct CTCBase {
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    bool          ToBeRemoved;                      // +0x08
    bool          IsInGlobalUpdateTCs;              // +0x09
    unsigned char _pad0a[2];
    CTCBase(CThing& thing);                         // 0x00686800 (Ego_r.pdb takes the reference)
};

typedef void (__cdecl* TTavernGameFunc)();

struct CTCTavernGame : CTCBase {                    // vtable 0x012560BC
    CVector<NUISystem::CComponent*> UIComponents;   // +0x0c
    double        LastRenderTime;                   // +0x18
    bool          IsBeingPlayed;                    // +0x20
    bool          GameWon;                          // +0x21
    long          NumRounds;                        // +0x24
    bool          GameFinished;                     // +0x28
    bool          GameInSession;                    // +0x29
    float         Score;                            // +0x2c
    float         Target;                           // +0x30
    long          BaseBet;                          // +0x34
    long          LastBet;                          // +0x38
    long          TotalWinnings;                    // +0x3c
    long          CurrentMoney;                     // +0x40
    CHandleVector Handles;                          // +0x44
    CHandleVector BoxHandles;                       // +0x50
    CHandleVector BetHandles;                       // +0x5c
    CHandleVector MoneyHandles;                     // +0x68
    CHandleVector WinningsHandles;                  // +0x74
    CCountedPointer<CFontBank> PScoreFont;          // +0x80
    CCountedPointer<CFontBank> PTargetFont;         // +0x88
    CCountedPointer<CFontBank> PStatsFont;          // +0x90
    float         TargetTime;                       // +0x98
    long          TargetRound;                      // +0x9c
    bool          NewBestScore;                     // +0xa0
    bool          Draw;                             // +0xa1
    bool          GameStarted;                      // +0xa2
    long          NumConsecutiveRoundsWon;          // +0xa4
    long          YesState;                         // +0xa8 ETavernGameStates
    long          NoState;                          // +0xac
    long          AltState;                         // +0xb0
    TTavernGameFunc YesFunc;                        // +0xb4
    TTavernGameFunc NoFunc;                         // +0xb8
    TTavernGameFunc AltFunc;                        // +0xbc
    long          TavernGameState;                  // +0xc0
    long          PreviousTavernGameState;          // +0xc4
    long          NextTavernGameState;              // +0xc8
    float         StateTimer;                       // +0xcc
    CIntelligentPointerAICreature PGameMaster;      // +0xd0
    bool          GamePlayed;                       // +0xd8
    long          FrameBetWasLastChanged;           // +0xdc
    long          FrameBetChangeStarted;            // +0xe0
    long          Mult;                             // +0xe4
    bool          BetIncreasing;                    // +0xe8
    bool          HasPrizeBeenGiven;                // +0xe9
    CTCTavernGame(CThing* thing);
    virtual void v00();
};

CTCTavernGame::CTCTavernGame(CThing* thing)
    : CTCBase(*thing)
    , LastRenderTime(0.0)
{
    TargetTime = 0;
    TargetRound = 0;
    HasPrizeBeenGiven = false;
    FrameBetWasLastChanged = 0;
    FrameBetChangeStarted = 0;
    BetHandles.Resize(5);
    MoneyHandles.Resize(5);
    WinningsHandles.Resize(5);
}
