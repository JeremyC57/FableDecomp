// Behaviour test for 005ec8a0: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGame::CTCTavernGame(CThing* thing) -- the tavern-game component: CTCBase(thing), empty UI-component and primitive-handle vectors, null fonts and game master, cleared target and bet timers, five-slot bet / money / winnings handle vectors
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
class CThing;
class CThingAICreature;
class CFontBank;
class CCPPointerInfo;
class CEnginePrimitiveHandle;
namespace NUISystem { class CComponent; }

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

template <class T> class CCountedPointer { public:         // Data + Info (8 bytes)
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};

class CBaseObject;
struct CBaseIntelligentPointer {                    // 8 bytes: vptr + item
    virtual void v00();
    CBaseObject* PItem;
    CBaseIntelligentPointer();                      // 0x00A01B10
};
struct CIntelligentPointerAICreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingAICreature>, vtable 0x0123E7E0
    virtual void v00();
    CIntelligentPointerAICreature() { }
};

class CTCBase { public:
    virtual void v00();
    CThing*       ParentThing;                      // +0x04
    bool          ToBeRemoved;                      // +0x08
    bool          IsInGlobalUpdateTCs;              // +0x09
    unsigned char _pad0a[2];
    CTCBase(CThing& thing);                         // 0x00686800 (Ego_r.pdb takes the reference)
};

typedef void (__cdecl* TTavernGameFunc)();

class CTCTavernGame : public CTCBase { public:                    // vtable 0x012560BC
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

// ---- stubs and driver ----
#include <new>
void CTCBase::v00() {}
CTCBase::CTCBase(CThing& thing) : ParentThing(&thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
void CBaseIntelligentPointer::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() : PItem(0) {}
void CIntelligentPointerAICreature::v00() {}
void CTCTavernGame::v00() {}
static const void* g_resized[3]; static long g_counts[3]; static int g_resizes;
void CHandleVector::Resize(long count) { if (g_resizes < 3) { g_resized[g_resizes] = this; g_counts[g_resizes] = count; } ++g_resizes; }

static double g_buffer[(sizeof(CTCTavernGame) + 7) / 8];

int main()
{
    std::memset(g_buffer, 0xcd, sizeof(g_buffer));
    CThing* thing = (CThing*)0x1234;
    CTCTavernGame* g = new (g_buffer) CTCTavernGame(thing);
    if (g->ParentThing != thing || g->UIComponents._M_start || g->UIComponents._M_finish || g->UIComponents._M_end_of_storage.m_data
        || g->LastRenderTime != 0.0) { std::printf("005ec8a0_TEST FAIL (base)\n"); return 1; }
    CHandleVector* v[5] = { &g->Handles, &g->BoxHandles, &g->BetHandles, &g->MoneyHandles, &g->WinningsHandles };
    for (int i = 0; i < 5; ++i)
        if (v[i]->_M_start || v[i]->_M_finish || v[i]->_M_end_of_storage.m_data) { std::printf("005ec8a0_TEST FAIL (vector %d)\n", i); return 1; }
    if (g->PScoreFont.Data || g->PScoreFont.Info || g->PTargetFont.Data || g->PTargetFont.Info || g->PStatsFont.Data || g->PStatsFont.Info
        || g->PGameMaster.PItem || g->TargetTime != 0.0f || g->TargetRound || g->HasPrizeBeenGiven || g->FrameBetWasLastChanged
        || g->FrameBetChangeStarted) { std::printf("005ec8a0_TEST FAIL (members)\n"); return 1; }
    if (g_resizes != 3 || g_resized[0] != &g->BetHandles || g_resized[1] != &g->MoneyHandles || g_resized[2] != &g->WinningsHandles
        || g_counts[0] != 5 || g_counts[1] != 5 || g_counts[2] != 5) { std::printf("005ec8a0_TEST FAIL (resize)\n"); return 1; }
    // members the constructor leaves alone keep their bytes
    unsigned char untouched;
    std::memcpy(&untouched, &g->GameWon, 1);
    if (untouched != 0xcd) { std::printf("005ec8a0_TEST FAIL (untouched)\n"); return 1; }
    std::printf("005ec8a0_TEST PASS\n");
    return 0;
}