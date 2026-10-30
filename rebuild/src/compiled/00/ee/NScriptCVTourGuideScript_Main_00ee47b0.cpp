// NScript::CV_TourGuideScript::Main() -- the Tour Guide quest main: bind the TourGuideGuide / TourGuideFollower entity scripts, spawn the WatchForGuideKilled and WatchForClosingTime threads, wait for QuestFinished, then deactivate the active quest
// Leaf: ?Main@CV_TourGuideScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE47B0, 576 bytes, thiscall, plain `ret`.
// Identity (proven): slot 2 (Main) of the RTTI-less level-script vtable 0x012F46B8 [dtor 0x00EE6C30, RegisterMain 0x00EE4710
//   (landed NScript::CV_TourGuideScript::RegisterMain), Main 0x00EE47B0, Init 0x00EE42A0, OnPersist 0x00EE5720 (landed
//   CV_TourGuideScript::OnPersist)], stored by the ctor at 0x00EE420A; the row is referenced only from 0x012F46C0. Ego_r.pdb
//   ?Main@CV_TourGuideScript@NScript@@UAEXXZ. The binding factories are the landed CTourGuideGuide::Alloc 0x00EE4BB0 /
//   CTourGuideFollower::Alloc 0x00EE4CF0 and the spawned threads the landed WatchForGuideKilled 0x00EE4A70 /
//   WatchForClosingTime 0x00EE4A00 (each paired with its own name string). Catalog label sub_00ee47b0.
// Callees: operator new 0x00BFEA1A, CCharString(const char*, long) 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CScriptBase::AddEntityScriptBinding 0x00CB8230 / PostAddScriptedEntities 0x00CB8930 / AddSpawnedFunction 0x00CB7E50 /
//   IsActiveThreadTerminating 0x00CB7940, CSpawnedFuncBase ctor 0x00CDD450; vtables CEntityScriptBinding<CV_TourGuideScript>
//   0x012F5104, CSpawnedFunc<CV_TourGuideScript> 0x012F50AC; Game slots 7 NewScriptFrame, 64 PostAddScriptedEntities, 281
//   DeactivateQuestLater, 655 GetActiveQuestName.
// Literals: "TourGuideGuide" 0x012F510C, "TourGuideFollower" 0x012F50F0, "WatchForGuideKilled" 0x012F50DC,
//   "WatchForClosingTime" 0x012F50C8, "" 0x0122D70E.
// Layout: retail NScript headers (CLevelScriptBase 0x48); CV_TourGuideScript members at Ego_r.pdb - 0x14 (WaypointData is 12
//   bytes in both builds): WaypointInfo[18] +0x48, TourGuideKilled +0x120, AllFollowersDead +0x121, QuestFinished +0x122.
// Shape: the landed CQS_GuardianSisterInfo2_SisterInBanditCampScript::Main 0x00E268C0 / CV_IntroductionToTrophiesScript::Main
//   0x00E9C4B0 forms (bindings with conditional name temporaries, the __forceinline SpawnFunction helper, `while (!flag)`).
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int bytes);         // 0x00BFEA1A (MSVCR71)
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CGameScriptInterfaceBase.h"
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"

#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_TourGuideScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CTourGuideGuide {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TourGuideScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EE4BB0
    };
    class CTourGuideFollower {
    public:
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_TourGuideScript& parent, CScriptDataBase* data, const CScriptThing& thing);   // 0x00EE4CF0
    };
    unsigned char WaypointInfo[18][0xc];                // +0x48 WaypointData[18]
    bool TourGuideKilled;                               // +0x120
    bool AllFollowersDead;                              // +0x121
    bool QuestFinished;                                 // +0x122
    virtual void Main();
    void WatchForClosingTime();                         // 0x00EE4A00
    void WatchForGuideKilled();                         // 0x00EE4A70
    // Source-level spawn helper of the landed quest rows (no retail symbol).
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};

void CV_TourGuideScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TourGuideScript>("TourGuideGuide", this, &CTourGuideGuide::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TourGuideScript>("TourGuideFollower", this, &CTourGuideFollower::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    SpawnFunction(new CSpawnedFunc<CV_TourGuideScript>("WatchForGuideKilled", 0, this, &CV_TourGuideScript::WatchForGuideKilled));
    SpawnFunction(new CSpawnedFunc<CV_TourGuideScript>("WatchForClosingTime", 0, this, &CV_TourGuideScript::WatchForClosingTime));
    while (!QuestFinished) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}   // namespace NScript