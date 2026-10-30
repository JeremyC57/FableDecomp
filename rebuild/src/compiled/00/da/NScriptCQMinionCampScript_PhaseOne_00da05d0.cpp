// CQ_MinionCampScript::PhaseOne() -- the minion camp's first phase: wait for the intro, start the first-wave control thread, wait for Briar to reach the altar and start the spawn-point control thread, then hold the minion cap at three while the chant runs until the portal is sealed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA05D0, length 585 bytes.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable
//   0x012D6CAC), CScriptBase::AddSpawnedFunction 0x00CB7E50, RunFirstWaveControl 0x00DA0820, RunSpawnPointControl
//   0x00DA0C40; the game script interface's NewScriptFrame (slot 7); the timer service (global 0x0143E8F8) GetTimer
//   (slot 90) through an inline CTimer::GetTime.
// Layout: the landed CQ_MinionCampScript ctor 0x00D9E210 (retail = Ego_r.pdb - 0x14).
// Shape: the landed CinemaTest Main spawn model (flag word in ebx guarding the name temporaries).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
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
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(Index); }
};
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
public:
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&);   // 0x00CB7E50
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {
class CQ_MinionCampScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool QuestStartScreenShown;                         // +0x48
    bool IntroDone;                                     // +0x49
    bool BriarRoseAtAltar;                              // +0x4a
    bool MinionsSummoned;                               // +0x4b
    bool PortalSealed;                                  // +0x4c
    bool MissionFailed;                                 // +0x4d
    long NumberOfMinions;                               // +0x50
    long MaxNumberOfMinions;                            // +0x54
    float MinMinionSpawnPeriod;                         // +0x58
    float MaxMinionSpawnPeriod;                         // +0x5c
    CTimer ChantTimer;                                  // +0x60
    long ChantTimerBar;                                 // +0x64
    bool ChantTimerPaused;                              // +0x68
    long PausedTime;                                    // +0x6c
    CTimer MinionSpawnTimer;                            // +0x70
    long CurrentStage;                                  // +0x74 (MissionStage)
    void PhaseOne();
    void RunFirstWaveControl();                         // 0x00DA0820
    void RunSpawnPointControl();                        // 0x00DA0C40
};
void CQ_MinionCampScript::PhaseOne()
{
    while (!IntroDone && !PortalSealed && !MissionFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CurrentStage = 2;
    SpawnFunction(new CSpawnedFunc<CQ_MinionCampScript>("RunFirstWaveControl", 0, this, &CQ_MinionCampScript::RunFirstWaveControl));
    while (!BriarRoseAtAltar && !PortalSealed && !MissionFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (BriarRoseAtAltar) {
        if (IsActiveThreadTerminating())
            return;
        CurrentStage = 3;
        SpawnFunction(new CSpawnedFunc<CQ_MinionCampScript>("RunSpawnPointControl", 0, this, &CQ_MinionCampScript::RunSpawnPointControl));
    }
    while (!PortalSealed && !MissionFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ChantTimer.GetTime() == 0x30) {
            if (IsActiveThreadTerminating())
                return;
            MaxNumberOfMinions = 3;
        }
        if (ChantTimer.GetTime() == 0x18) {
            if (IsActiveThreadTerminating())
                return;
            MaxNumberOfMinions = 3;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionFailed)
        return;
    if (IsActiveThreadTerminating())
        return;
    CurrentStage = 4;
}
}