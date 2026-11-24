// 00e4fa20 NScript::CV_BeardyBaldyScript::Main
// Leaf: ?Main@CV_BeardyBaldyScript@NScript@@UAEXXZ
// (NF344, 2026-11-24) /d2inlT69 dropped: on the current headers the object is identical without it (every function COMDAT, bytes and relocations; NF340 /d2inl audit).
// Inline-budget: expand the first IsAlive and both string-equality layers. The second IsAlive stays a call.
#define FABLE_RETAIL_BEARDY_ORIGINAL_TEXT_COMPARISON_PASS27
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CV_BEARDYBALDYSCRIPT_MAIN_PASS27
#define FABLE_RETAIL_CV_BEARDYBALDYSCRIPT_WATCHFORQUESTFINISHED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CBASICSTRING_COMPARE_BODY
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#include <string.h>
#if defined(_MSC_VER)
#pragma intrinsic(strcmp)
#endif
#include "fable_string.h"
#include "engine/CCountedPointer.h"
namespace NScript { class CPredicateBase; }
template<> CCountedPointer<const NScript::CPredicateBase>::~CCountedPointer();
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
inline CScriptThing::~CScriptThing() { }
FABLE_FORCEINLINE bool CScriptThing::IsAlive() const { return !PImp.IsNull() && PImp->IsAlive(); }
#include "engine/CV_BeardyBaldyScript.h"
#include "engine/TRetVoid.h"
void NScript::CV_BeardyBaldyScript::Main()
{
    TRetVoid _Fail_Val;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BeardyBaldyScript>("BB_BeardyBaldyMan", this, &CBB_BeardyBaldyMan::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    CCharString regionName;
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!Game->MsgOnRegionLoaded(regionName)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (regionName == "BowerstoneSlumsWarehouses") {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            break;
        }
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForQuestFinished", 0, this, &CV_BeardyBaldyScript::WatchForQuestFinished), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForQuestCardConditions", 0, this, &CV_BeardyBaldyScript::WatchForQuestCardConditions), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForBarberLadyDeath", 0, this, &CV_BeardyBaldyScript::WatchForBarberLadyDeath), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForBeardyBaldyDeath", 0, this, &CV_BeardyBaldyScript::WatchForBeardyBaldyDeath), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForNewHairdo", 0, this, &CV_BeardyBaldyScript::WatchForNewHairdo), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForNewBeard", 0, this, &CV_BeardyBaldyScript::WatchForNewBeard), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BeardyBaldyScript>("WatchForNewTash", 0, this, &CV_BeardyBaldyScript::WatchForNewTash), CCharString(""));
    if (!QuestActivated) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        QuestActivated = true;
    }
    GoTalkToBeardyBaldy();
    while (!QuestFinished && (!QuestComplete || Game->IsRegionLoaded("BowerstoneSlumsWarehouses"))) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    CScriptThing beardyBaldyMan = Game->GetThingWithScriptName("BB_BeardyBaldyMan");
    CScriptThing barberLady = Game->GetNearestWithScriptName(Game->GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER");
    if (barberLady.IsAlive()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->MiniMapRemoveMarker(barberLady);
        Game->ClearThingHasInformation(barberLady);
    }
    // Inline-budget: the second IsAlive is a call at 00E50030.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    if (beardyBaldyMan.IsAlive()) {
#ifdef _MSC_VER
#pragma inline_depth()
#endif
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->MiniMapRemoveMarker(beardyBaldyMan);
        Game->ClearThingHasInformation(beardyBaldyMan);
    }
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, false, false);
    Game->RemoveQuestCardFromHero(Game->GetActiveQuestName());
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}