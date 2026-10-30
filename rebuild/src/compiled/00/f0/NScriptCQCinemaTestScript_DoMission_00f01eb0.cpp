// CQ_CinemaTestScript::DoMission() -- the cinema test quest: set the first quest objective, wait for the ArenaHallOfHeroes level, spawn the WatchForLeaving thread, run phase one, and end the mission unless it failed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F01EB0, length 372 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb ?DoMission@CQ_CinemaTestScript@NScript@@QAEXXZ; the body is the CQ_TentacleKrakenBossFightScript
// DoMission (0x00DF4E00) shape.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
// 0x00CB7940, operator new 0x00BFEA1A, CSpawnedFuncBase::CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450
// (CSpawnedFunc<CQ_CinemaTestScript> vtable 0x012F7314, 0x3C bytes), CScriptBase::AddSpawnedFunction 0x00CB7E50,
// WatchForLeaving 0x00F02030 (the stored member pointer), PhaseOne 0x00F02170, EndMission 0x00F02100; through the
// game script interface (+0x40): NewScriptFrame (slot 7), IsLevelLoaded (11), GiveHeroNewQuestObjective (268).
// Strings: "first objective" 0x012D19FC, "ArenaHallOfHeroes" 0x012C7820, "WatchForLeaving" 0x012D19EC, "" 0x0122D70E.
// Layout: retail = Ego_r.pdb - 0x14: MissionSucceeded +0x48, MissionFailed +0x49, MissionOver +0x4A.
// Shape: the Kraken DoMission spawn model (thread name guarded by an entry-zeroed flag, the region "" built after
// operator new through an inline `const char*` wrapper with an explicit CCharString temporary); string arguments are
// argument temporaries.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                  // +0x34
    T* PClass;                                          // +0x38
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {
class CQ_CinemaTestScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    bool                  MissionSucceeded;             // +0x48 (PDB +0x5c)
    bool                  MissionFailed;                // +0x49
    bool                  MissionOver;                  // +0x4a
    void DoMission();
    void PhaseOne();                                    // 0x00F02170
    void EndMission();                                  // 0x00F02100
    void WatchForLeaving();                             // 0x00F02030
};
void CQ_CinemaTestScript::DoMission()
{
    GSI->GiveHeroNewQuestObjective("first objective", true);
    while (!GSI->IsLevelLoaded("ArenaHallOfHeroes")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    SpawnFunction(new CSpawnedFunc<CQ_CinemaTestScript>("WatchForLeaving", 0, this, &CQ_CinemaTestScript::WatchForLeaving));
    PhaseOne();
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (!MissionFailed)
        EndMission();
}
}