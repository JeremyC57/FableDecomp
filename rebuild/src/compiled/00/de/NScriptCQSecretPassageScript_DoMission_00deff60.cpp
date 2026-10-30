// CQ_SecretPassageScript::DoMission() -- wait until KrakenChamber is loaded, show the quest start screen once, spawn the WatchForLeaving and BossBlockingPrisonPassage threads, turn teleporting off, run PhaseOne and end the mission unless it failed
// Leaf: DoMission
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DEFF60, 531 bytes (544 with int3 padding).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940,
//   operator new 0x00BFEA1A, CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450 (CSpawnedFunc vtable
//   0x012DDC54), CScriptBase::AddSpawnedFunction 0x00CB7E50, WatchForLeaving 0x00DF0180 (manifest label
//   "CGlobal_WatchForHeroDeathScript::WatchForHeroDeath" is propagated; the thread is spawned as "WatchForLeaving"),
//   BossBlockingPrisonPassage 0x00DF0200, PhaseOne 0x00DF06A0, EndMission 0x00DF0A50; CGameScriptInterface slots 7
//   NewScriptFrame (+0x1c), 12 IsRegionLoaded (+0x30), 309 KickOffQuestStartScreen (+0x4d4), 565 SetTeleportingAsActive
//   (+0x8d4).
// Layout (retail = Ego_r PDB -0x14, as the landed CQ_SecretPassageScript sources): MissionFailed +0x49,
//   QuestStartScreenShown +0x4F; spawn wrapper as the landed CV_ChapelOfEvilScript::DoMission 0x00E6A7F0.
// Shape: WAIT_UNTIL(loaded) is `while (!loaded)` (neg/sbb/inc); the two thread-name temporaries are guarded by the
//   entry-zeroed flag word (bits 1 and 2).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
template <class T> struct CSpawnedFunc : CSpawnedFuncBase {
    void (T::*Func)();                                   // +0x34
    T* PClass;                                           // +0x38
    __forceinline CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                    // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CQ_SecretPassageScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Data;                          // +0x44
    bool                  MissionSucceeded;              // +0x48
    bool                  MissionFailed;                 // +0x49
    bool                  MissionOver;                   // +0x4A
    bool                  MotherFound;                   // +0x4B
    bool                  DoorsBlocked;                  // +0x4C
    bool                  DoorsBeingDone;                // +0x4D
    bool                  OkayToSpawn;                   // +0x4E
    bool                  QuestStartScreenShown;         // +0x4F
    void WatchForLeaving();                              // 0x00DF0180
    void BossBlockingPrisonPassage();                    // 0x00DF0200
    void PhaseOne();                                     // 0x00DF06A0
    void EndMission();                                   // 0x00DF0A50
    void DoMission();
};
} // namespace NScript
using NScript::CQ_SecretPassageScript;

void NScript::CQ_SecretPassageScript::DoMission()
{
    while (!GSI->IsRegionLoaded("KrakenChamber")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!QuestStartScreenShown) {
        if (IsActiveThreadTerminating())
            return;
        GSI->KickOffQuestStartScreen("Q_SecretPassage", true, false);
        QuestStartScreenShown = true;
    }
    SpawnFunction(new CSpawnedFunc<CQ_SecretPassageScript>("WatchForLeaving", 0, this, &NScript::CQ_SecretPassageScript::WatchForLeaving));
    SpawnFunction(new CSpawnedFunc<CQ_SecretPassageScript>("BossBlockingPrisonPassage", 0, this, &NScript::CQ_SecretPassageScript::BossBlockingPrisonPassage));
    GSI->SetTeleportingAsActive(false);
    PhaseOne();
    GSI->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (!MissionFailed)
        EndMission();
}