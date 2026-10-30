// CQ_HangingTreeEvilScript::MonitorEnemies() -- script thread: wait until every current enemy is dead (then flag AllEnemiesDead); if the hero switches regions first, wait for the switch to finish, re-collect the HTE_AssaultGuard enemies and, if any still live, restart this monitor as a fresh thread
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D6BE20, length 369 bytes, plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, AreAllThingsInVectorDead (__fastcall, 0x00CBED00),
// vector<CScriptThing>::erase(iterator, iterator) 0x008AC9D0, CCharString(const char*, long) 0x0099EBF0 /
// ~CCharString 0x0099EAE0, operator new 0x00BFEA1A, CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable
// 0x012D1FE8), CScriptBase::AddSpawnedFunction 0x00CB7E50; through the quest's interface (+0x40): NewScriptFrame
// (slot 7), GetAllThingsWithScriptName (75).
// Strings: "HTE_AssaultGuard" 0x012D2064, "MonitorEnemies" 0x012D22F0, "" 0x0122D70E.
// Layout: retail = Ego_r.pdb - 0x14 (as the landed BanditYells 0x00D69CD0): SwitchingRegions +0x51, AllEnemiesDead
// +0x53, CurrentEnemyVector (STLport, 12 bytes) +0xAC.
// Shape: the region-switch arm sits inside the wait loop and ends the function (laid out after the main return);
// the respawn uses the Kraken DoMission (0x00DF4E00) inline `const char*` spawn wrapper (thread name guarded by an
// entry-zeroed flag). The thread spawns THIS function, and VC7.1 then refuses to inline the CSpawnedFunc ctor and the
// wrapper (both emitted out of line, the frame shrinks to `push ecx`) unless they are `__forceinline`.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
namespace std {
template <class T> class allocator;
template <class T, class A = allocator<T> > class vector {
public:                      // STLport vector (retail 12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                        // 0x008AC9D0 for CScriptThing
    void clear() { erase(begin(), end()); }
};
}
using std::vector;
bool __fastcall AreAllThingsInVectorDead(const vector<CScriptThing>& things);   // 0x00CBED00

void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_HangingTreeEvilScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x51 - 0x48];
    bool                  SwitchingRegions;             // +0x51 (PDB +0x65)
    bool                  RemoveBandits;                // +0x52
    bool                  AllEnemiesDead;               // +0x53
    unsigned char         _pad54[0xac - 0x54];
    vector<CScriptThing>  CurrentEnemyVector;           // +0xac (PDB +0xc0)
    void MonitorEnemies();
};
void CQ_HangingTreeEvilScript::MonitorEnemies()
{
    AllEnemiesDead = false;
    while (!AreAllThingsInVectorDead(CurrentEnemyVector)) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (SwitchingRegions) {
            if (IsActiveThreadTerminating())
                return;
            while (SwitchingRegions) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            CurrentEnemyVector.clear();
            AllEnemiesDead = true;
            GSI->GetAllThingsWithScriptName("HTE_AssaultGuard", CurrentEnemyVector);
            if (AreAllThingsInVectorDead(CurrentEnemyVector))
                return;
            if (IsActiveThreadTerminating())
                return;
            AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeEvilScript>("MonitorEnemies", 0, this, &CQ_HangingTreeEvilScript::MonitorEnemies), CCharString(""));
            return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    AllEnemiesDead = true;
}
}