// CQ_HangingTreeGoodScript::ReassignLeader() -- refill the guard list from the "HTG_Guard" things and make the first guard the party leader if he lives, otherwise the hero
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D74F80, length 213 bytes; called from DoMission (0x00D74430) and 0x00D76030.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0, CScriptThing::operator= 0x008AB980, CScriptThing's virtual destructor (slot 0, with 0) and IsAlive (slot 75),
// and through the game script interface (+0x40): GetHero (slot 70), GetAllThingsWithScriptName (slot 75).
// Layout (Ego_r.pdb - 0x14, retail 12-byte vectors): PartySuccessfullySetUp +0x49, MissionSucceeded +0x4a,
// MissionFailed +0x4b, ReassignLeaderNow +0x4d, Leader +0x68, CurrentEnemyVector +0xa0, CurrentGuardVector +0xac.
// Shape: three early-out tests; the guard vector is emptied with the inline pop_back loop (the `lea esi` reference to
// the vector is kept for the lookup and front()); the guard name is a named CCharString local destroyed right after the
// lookup; each arm re-tests the thread before assigning the leader.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:                       // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    bool empty() const { return _M_start == _M_finish; }
    T& front() { return *_M_start; }
    void pop_back() { --_M_finish; _M_finish->~T(); }
};
}
using std::vector;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

namespace NScript {
class CQ_HangingTreeGoodScript : public CQuestScriptBase {
public:
    bool LeaderSpokenTo;                                 // +0x48
    bool PartySuccessfullySetUp;                         // +0x49
    bool MissionSucceeded;                               // +0x4a
    bool MissionFailed;                                  // +0x4b
    bool SwitchingRegions;                               // +0x4c
    bool ReassignLeaderNow;                              // +0x4d
    unsigned char _pad4e[0x68 - 0x4e];
    CScriptThing Leader;                                 // +0x68
    CScriptThing Prisoner;                               // +0x74
    CScriptThing PartySpawnPoint;                        // +0x80
    CScriptThing ActiveWayPoint;                         // +0x8c
    CCharString  CurrentLevel;                           // +0x98
    CCharString  ReasonForFailing;                       // +0x9c
    vector<CScriptThing> CurrentEnemyVector;             // +0xa0
    vector<CScriptThing> CurrentGuardVector;             // +0xac
    void ReassignLeader();
};
} // namespace NScript
using NScript::CQ_HangingTreeGoodScript;

void NScript::CQ_HangingTreeGoodScript::ReassignLeader()
{
    if (MissionSucceeded || MissionFailed || IsActiveThreadTerminating())
        return;
    vector<CScriptThing>& guards = CurrentGuardVector;
    while (!guards.empty())
        guards.pop_back();
    {
        CCharString name("HTG_Guard");
        GSI->GetAllThingsWithScriptName(name, guards);
    }
    if (guards.front().IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Leader = guards.front();
        ReassignLeaderNow = false;
        PartySuccessfullySetUp = true;
    } else {
        if (IsActiveThreadTerminating())
            return;
        Leader = GSI->GetHero();
        PartySuccessfullySetUp = true;
    }
}