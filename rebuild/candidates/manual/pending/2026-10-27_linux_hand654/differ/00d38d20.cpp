// CQ_FireHeartScript::RunVillagerComments() -- until the mission succeeds, every 25 seconds pick at random one villager whose squared distance to the hero is at most 25 and flag him to speak
// Grade: DIFFER(529v526) -- block layout: retail lays the natural-end exit copy (inline ~vector free + DeregisterTimer) right after the final re-test and the merged return copy after it; VC7.1 here lays them the other way round (same ten exits, same two merged groups); plus `lea ebp,[ebx+eax]` vs `mov ebp,[esi+0x110] ; add ebp,ebx`. Next: a CFG-level lever for which merged exit is laid first (see probe/00d38d20.log; 21 probes).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D38D20, length 526 bytes.
// Leaf: ?RunVillagerComments@CQ_FireHeartScript@NScript@@QAEXXZ
// Callees: vector<long>(size_type) 0x00D38F30 (out of line), free 0x00BFEA14 (inline ~vector copies), rand 0x00BFEB16,
//   GetSquaredDistanceBetweenThings 0x00CBE583 (__fastcall, no fsqrt; the 25.0f constant at 0x012C26F0 is 5 squared),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; Game (+0x40) slots 7 NewScriptFrame, 70 GetHero; the timer service
//   0x0143E8F8 slots 87..90 through the inline CTimer (engine/CTimer.h).
// Layout: the landed CQ_FireHeartScript ctor 0x00D34470: MissionSucceeded +0x49, NumberOfVillagers +0x84, ShouldVillagerSpeak
//   +0xEC (retail vector<bool> is a plain byte vector), Villagers +0x110.
extern "C" void __cdecl free(void* p);             // 0x00BFEA14
extern "C" int __cdecl rand();                     // 0x00BFEB16
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptBase.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE583
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    explicit vector(unsigned int n);               // 0x00D38F30 for long
    ~vector() { if (_M_start) free(_M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
namespace NScript {
class CQ_FireHeartScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    bool                  QuestStartScreenShown;        // +0x48
    bool                  MissionSucceeded;             // +0x49
    unsigned char         _pad4a[0x84 - 0x4a];
    long                  NumberOfVillagers;            // +0x84
    unsigned char         _pad88[0xec - 0x88];
    std::vector<bool>     ShouldVillagerSpeak;          // +0xec
    unsigned char         _padf8[0x110 - 0xf8];
    std::vector<CScriptThing> Villagers;                // +0x110
    void RunVillagerComments();
};
}
void NScript::CQ_FireHeartScript::RunVillagerComments()
{
    CTimer commentTimer;
    std::vector<long> candidates(NumberOfVillagers);
    while (!MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        commentTimer = 25;
        while (commentTimer.GetTime() != 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        long count;
        do {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            count = 0;
            for (long i = 0; i < NumberOfVillagers; ++i) {
                if (IsActiveThreadTerminating())
                    return;
                if (GetSquaredDistanceBetweenThings(Game->GetHero(), Villagers[i]) <= 25.0f) {
                    if (IsActiveThreadTerminating())
                        return;
                    candidates[count] = i;
                    count++;
                }
            }
            if (IsActiveThreadTerminating())
                return;
        } while (count <= 0);
        if (IsActiveThreadTerminating())
            return;
        ShouldVillagerSpeak[candidates[rand() % count]] = true;
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}
