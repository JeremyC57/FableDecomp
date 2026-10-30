// CQ_TentacleKrakenBossFightScript::DoMission() -- the Kraken boss quest: wait for the KrakenChamber level, take the one-off auto-save checkpoint, spawn the WatchForLeaving thread, run phase one, and when the hero has not failed, stop the music override and mark the mission succeeded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DF4E00, length 370 bytes (+ int3 padding), plain `ret`.
// Identity: Ego_r.pdb ?DoMission@CQ_TentacleKrakenBossFightScript@NScript@@QAEXXZ (the class's member list: DoMission,
// EndMission, Init, Main, PhaseOne, WatchForLeaving, WatchForTermination).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
// 0x00CB7940, operator new 0x00BFEA1A, CSpawnedFuncBase::CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450
// (CSpawnedFunc<CQ_TentacleKrakenBossFightScript> vtable 0x012DE524, 0x3C bytes), CScriptBase::AddSpawnedFunction
// 0x00CB7E50, WatchForLeaving 0x00DF4F80 (the stored member pointer; manifest label "WatchForHeroDeath" is propagated),
// PhaseOne 0x00DF5000 (manifest "DoMission_PrisonEscape" is propagated); through the game script interface (+0x40):
// NewScriptFrame (slot 7), IsLevelLoaded (11), StopOverrideMusic (697), AutoSaveCheckPoint (703).
// Strings: "KrakenChamber" 0x012DB928, "WatchForLeaving" 0x012D19EC, "" 0x0122D70E.
// Layout: retail = Ego_r.pdb - 0x14: MissionSucceeded +0x48, MissionFailed +0x49, KrakenAutoSaveCheckPoint +0x4A.
// Shape: the landed CV_KnotholeGladeGatesScript::PhaseOne (0x00EA1630) spawn model (the thread name lives only when
// operator new succeeded, so its destructor is guarded by a flag that VC7.1 zeroes at function entry), but here the
// region string "" is built AFTER operator new: the spawn goes through an inline wrapper taking `const char*` that
// builds an explicit CCharString temporary (`push eax` of the ctor result; a plain const-ref conversion gives
// `lea ecx ; push ecx` and swaps the two string slots); the level name is an argument temporary in the wait
// condition (`neg ; sbb ; inc` gives the negated bool).
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
class CQ_TentacleKrakenBossFightScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    bool                  MissionSucceeded;             // +0x48 (PDB +0x5c)
    bool                  MissionFailed;                // +0x49
    bool                  KrakenAutoSaveCheckPoint;     // +0x4a
    void DoMission();
    void PhaseOne();                                    // 0x00DF5000
    void WatchForLeaving();                             // 0x00DF4F80
};
void CQ_TentacleKrakenBossFightScript::DoMission()
{
    while (!GSI->IsLevelLoaded("KrakenChamber")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!KrakenAutoSaveCheckPoint) {
        if (IsActiveThreadTerminating())
            return;
        KrakenAutoSaveCheckPoint = true;
        GSI->AutoSaveCheckPoint();
    }
    AddSpawnedFunction(new CSpawnedFunc<CQ_TentacleKrakenBossFightScript>("WatchForLeaving", 0, this,
                                                                    &CQ_TentacleKrakenBossFightScript::WatchForLeaving), CCharString(""));
    PhaseOne();
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (!MissionFailed) {
        GSI->StopOverrideMusic(false);
        MissionSucceeded = true;
    }
}
}