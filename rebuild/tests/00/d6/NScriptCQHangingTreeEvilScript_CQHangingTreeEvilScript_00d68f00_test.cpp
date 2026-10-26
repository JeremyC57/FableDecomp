// Behaviour test for retail 0x00d68f00: the candidate source below is embedded verbatim (from src/00d68f00.cpp),
// followed by dependency stubs/spies and a main that asserts on the observable effects.
// ===== candidate source =====
// NScript::CQ_HangingTreeEvilScript::CQ_HangingTreeEvilScript(void* data, CGameScriptInterface* game) -- quest-script ctor:
//   script base and interface/data pair, vtable 0x012D1F98, ReasonForFailing, the four CScriptThing handles, the mission
//   timer, CurrentLevel and three empty vectors
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D68F00, `ret 8`. Identity: its factory 0x00D6ED10 (`push 0D0h`) is registered at
//   0x00CD6694 under "S_QHTE" / "Q_HangingTreeEvil". Callees: CScriptBase::CScriptBase 0x00CB8110, CCharString() 0x0099E4B0,
//   CBaseClass::CBaseClass 0x0099A2D0, g_pGameScriptInterface->RegisterTimer (slot 87).
// Leaf: ??0CQ_HangingTreeEvilScript
// Layout: PDB (Ego_r) names; retail = PDB - 0x14 (script base), and the three STLport vectors are 12 bytes (PDB 16):
//   bools +0x48..+0x58, ReasonForFailing +0x5c, HTE_State..CarryOverGuards +0x60..+0x70, Executioner +0x74, Prisoner +0x80,
//   PartySpawnPoint +0x8c, BanditExitPoint +0x98, MissionTimer +0xa4, CurrentLevel +0xa8, vectors +0xac/+0xb8/+0xc4;
//   size 0xd0. Model shared with the landed quest-script ctors (CV_ChickenKickingScript 0x00E62830).
//   Local model: engine/CGameScriptInterface.h is data-only (no virtual slots).
// CGameScriptInterface: the first 88 virtual slots of vtable 0x01260F0C, named from the slot labels (rtti-vtables.tsv +
//   manifest); only the slot position matters here, so the signatures are simplified (void, no parameters) except
//   RegisterTimer (slot 87, returns the new timer index). Overloaded names carry a _2 suffix.
struct CGameScriptInterface {
    virtual ~CGameScriptInterface();                    // slot 0 (deleting destructor)
    virtual void Error();
    virtual void TraceMessage();
    virtual void Validate();
    virtual void SetDebugCameraType();
    virtual void DeactivateBoastUi();
    virtual void IsXbox();
    virtual void NewScriptFrame();
    virtual void StartScriptingEntity();
    virtual void IsEntityUnderScriptedControl();
    virtual void IsActiveThreadTerminating();
    virtual void IsLevelLoaded();
    virtual void IsRegionLoaded();
    virtual void IsRegionLoadedAndPreloaded();
    virtual void IsRegionDefLoaded();
    virtual void GetRegionName();
    virtual void MsgIsLevelLoaded();
    virtual void MsgIsLevelUnloaded();
    virtual void MsgOnLevelLoaded();
    virtual void MsgOnLevelUnloaded();
    virtual void MsgIsRegionLoaded();
    virtual void MsgIsRegionUnloaded();
    virtual void MsgOnRegionLoaded();
    virtual void MsgOnRegionUnloaded();
    virtual void MsgOnRegionPreunload();
    virtual void MsgOnQuestCompleted();
    virtual void MsgOnAnyQuestCompleted();
    virtual void MsgOnQuestFailed();
    virtual void MsgOnQuestCompletedBeforeScreenShown();
    virtual void MsgOnQuestFailedBeforeScreenShown();
    virtual void MsgOnQuestAccepted();
    virtual void MsgOnFeatAccepted();
    virtual void MsgIsBoastMade();
    virtual void MsgOnBoastMade();
    virtual void MsgOnBoastsMade();
    virtual void RemoveBoastMessage();
    virtual void IsQuestStartScreenActive();
    virtual void MsgOnLeavingQuestStartScreen();
    virtual void MsgOnLeavingExperienceSpendingScreen();
    virtual void MsgIsQuestionAnsweredYesOrNo();
    virtual void MsgIsGameInfoClickedPast();
    virtual void MsgIsTutorialClickedPast();
    virtual void MsgIsActionModeButtonPressed();
    virtual void MsgOnExpressionPerformed();
    virtual void MsgIsCutSceneSkipped();
    virtual void RemoveAllCutSceneSkippedMessages();
    virtual void MsgOnHeroHairTypeChanged();
    virtual void MsgOnHeroUsedTeleporter();
    virtual void MsgOnHeroUsedGuildSeal();
    virtual void MsgOnGameSavedManually();
    virtual void MsgOnHeroSlept();
    virtual void MsgOnHeroFiredRangedWeapon();
    virtual void MsgOnHeroCastSpell();
    virtual void MsgOnHeroPickedPocket();
    virtual void MsgOnHeroPickedLock();
    virtual void MsgOnFishingGameFinished();
    virtual void MsgOnTavernGameFinished();
    virtual void MsgOnHeroRewardedWithItemsFrom();
    virtual void MsgOnChestOpeningCancelled();
    virtual void DontPopulateNextLoadedRegion();
    virtual void GetWanderingPopulationScriptDefNameInCurrentRegion();
    virtual void GetWanderingPopulationScriptDefNameInRegion();
    virtual void IsHeroAllowedHenchmenInCurrentRegion();
    virtual void IsHeroAllowedHenchmenInRegion();
    virtual void PostAddScriptedEntities();
    virtual void IsPlayerHoldingUnsheatheRangedWeaponButton();
    virtual void IsPlayerHoldingLockTargetButton();
    virtual void IsPlayerHoldingFireRangedWeaponButton();
    virtual void IsPlayerHoldingFirstPersonTargetingButton();
    virtual void IsHeroInProjectileWeaponMode();
    virtual void GetHero();
    virtual void GetHeroTargetedThing();
    virtual void GetThingWithScriptName();
    virtual void GetThingWithScriptName_2();
    virtual void GetRandomThingWithScriptName();
    virtual void GetAllThingsWithScriptName();
    virtual void GetAllCreaturesInAreaWithScriptName();
    virtual void GetNearestWithScriptName();
    virtual void GetFurthestWithScriptName();
    virtual void GetAllThingsWithDefNameByDistanceFrom();
    virtual void GetAllThingsWithDefName();
    virtual void GetNearestWithDefName();
    virtual void GetFurthestWithDefName();
    virtual void GetThingWithUID();
    virtual void GetAllCreaturesExcludingHero();
    virtual void GetAllThingsInLevel();
    virtual void IsThingWithThisUIDAlive();
    virtual long RegisterTimer();                       // slot 87 (+0x15c)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CScriptBase {                                    // 0x40 bytes, the quest-script root
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04..+0x3f
    CScriptBase();                                      // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base (landed model)
    CGameScriptInterface* Game;                         // +0x40 (second argument)
    void*                 Data;                         // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
struct CTimer {                                         // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
struct CCharString { void* PStringData; CCharString(); };   // default ctor 0x0099E4B0
struct CBaseClass { virtual ~CBaseClass(); CBaseClass(); };   // 0x0099A2D0 (vtable 0x01231710 = CBaseClass)
template<class T> struct CCountedPointer { T* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
struct CScriptThing : CBaseClass {                      // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    CScriptThing() : CBaseClass(), PImp() { }
    virtual ~CScriptThing();
};
template<class T> struct CRetailVector { T* m_first; T* m_last; T* m_end; CRetailVector() : m_first(0), m_last(0), m_end(0) { } };   // STLport vector (12 bytes)
namespace NScript {
struct CQ_HangingTreeEvilScript : CQuestScriptObject {
    bool IntroCutscenePlayed, BanditsStarted, BanditKilled, PartySuccessfullySetUp, MissionSucceeded, MissionFailed,
         BanditsLeftRegion, SpawnHere, AtWayPoint, SwitchingRegions, RemoveBandits, AllEnemiesDead, QuestStartScreenShown,
         BanditYelledRecently, ExecutionerInterrupted, AudienceLeggit, ShouldFollowHero;   // +0x48..+0x58
    CCharString ReasonForFailing;                       // +0x5c
    long HTE_State;                                     // +0x60
    long CurrentBandits;                                // +0x64
    long BanditsAtNextRegion;                           // +0x68
    long TimerGUIIndex;                                 // +0x6c
    long CarryOverGuards;                               // +0x70
    CScriptThing Executioner;                           // +0x74
    CScriptThing Prisoner;                              // +0x80
    CScriptThing PartySpawnPoint;                       // +0x8c
    CScriptThing BanditExitPoint;                       // +0x98
    CTimer MissionTimer;                                // +0xa4
    CCharString CurrentLevel;                           // +0xa8
    CRetailVector<CScriptThing> CurrentEnemyVector;     // +0xac
    CRetailVector<CScriptThing> CurrentBanditVector;    // +0xb8
    CRetailVector<CCharString> AreasSpawnedIn;          // +0xc4
    virtual ~CQ_HangingTreeEvilScript();
    CQ_HangingTreeEvilScript(void* data, CGameScriptInterface* game);
};
CQ_HangingTreeEvilScript::CQ_HangingTreeEvilScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}
}

// ===== harness =====
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <new>
/* every base slot needs a body because the spy below pulls in CGameScriptInterface's vtable */
CGameScriptInterface::~CGameScriptInterface() {}
long CGameScriptInterface::RegisterTimer() { return -1; }
void CGameScriptInterface::Error() {}
void CGameScriptInterface::TraceMessage() {}
void CGameScriptInterface::Validate() {}
void CGameScriptInterface::SetDebugCameraType() {}
void CGameScriptInterface::DeactivateBoastUi() {}
void CGameScriptInterface::IsXbox() {}
void CGameScriptInterface::NewScriptFrame() {}
void CGameScriptInterface::StartScriptingEntity() {}
void CGameScriptInterface::IsEntityUnderScriptedControl() {}
void CGameScriptInterface::IsActiveThreadTerminating() {}
void CGameScriptInterface::IsLevelLoaded() {}
void CGameScriptInterface::IsRegionLoaded() {}
void CGameScriptInterface::IsRegionLoadedAndPreloaded() {}
void CGameScriptInterface::IsRegionDefLoaded() {}
void CGameScriptInterface::GetRegionName() {}
void CGameScriptInterface::MsgIsLevelLoaded() {}
void CGameScriptInterface::MsgIsLevelUnloaded() {}
void CGameScriptInterface::MsgOnLevelLoaded() {}
void CGameScriptInterface::MsgOnLevelUnloaded() {}
void CGameScriptInterface::MsgIsRegionLoaded() {}
void CGameScriptInterface::MsgIsRegionUnloaded() {}
void CGameScriptInterface::MsgOnRegionLoaded() {}
void CGameScriptInterface::MsgOnRegionUnloaded() {}
void CGameScriptInterface::MsgOnRegionPreunload() {}
void CGameScriptInterface::MsgOnQuestCompleted() {}
void CGameScriptInterface::MsgOnAnyQuestCompleted() {}
void CGameScriptInterface::MsgOnQuestFailed() {}
void CGameScriptInterface::MsgOnQuestCompletedBeforeScreenShown() {}
void CGameScriptInterface::MsgOnQuestFailedBeforeScreenShown() {}
void CGameScriptInterface::MsgOnQuestAccepted() {}
void CGameScriptInterface::MsgOnFeatAccepted() {}
void CGameScriptInterface::MsgIsBoastMade() {}
void CGameScriptInterface::MsgOnBoastMade() {}
void CGameScriptInterface::MsgOnBoastsMade() {}
void CGameScriptInterface::RemoveBoastMessage() {}
void CGameScriptInterface::IsQuestStartScreenActive() {}
void CGameScriptInterface::MsgOnLeavingQuestStartScreen() {}
void CGameScriptInterface::MsgOnLeavingExperienceSpendingScreen() {}
void CGameScriptInterface::MsgIsQuestionAnsweredYesOrNo() {}
void CGameScriptInterface::MsgIsGameInfoClickedPast() {}
void CGameScriptInterface::MsgIsTutorialClickedPast() {}
void CGameScriptInterface::MsgIsActionModeButtonPressed() {}
void CGameScriptInterface::MsgOnExpressionPerformed() {}
void CGameScriptInterface::MsgIsCutSceneSkipped() {}
void CGameScriptInterface::RemoveAllCutSceneSkippedMessages() {}
void CGameScriptInterface::MsgOnHeroHairTypeChanged() {}
void CGameScriptInterface::MsgOnHeroUsedTeleporter() {}
void CGameScriptInterface::MsgOnHeroUsedGuildSeal() {}
void CGameScriptInterface::MsgOnGameSavedManually() {}
void CGameScriptInterface::MsgOnHeroSlept() {}
void CGameScriptInterface::MsgOnHeroFiredRangedWeapon() {}
void CGameScriptInterface::MsgOnHeroCastSpell() {}
void CGameScriptInterface::MsgOnHeroPickedPocket() {}
void CGameScriptInterface::MsgOnHeroPickedLock() {}
void CGameScriptInterface::MsgOnFishingGameFinished() {}
void CGameScriptInterface::MsgOnTavernGameFinished() {}
void CGameScriptInterface::MsgOnHeroRewardedWithItemsFrom() {}
void CGameScriptInterface::MsgOnChestOpeningCancelled() {}
void CGameScriptInterface::DontPopulateNextLoadedRegion() {}
void CGameScriptInterface::GetWanderingPopulationScriptDefNameInCurrentRegion() {}
void CGameScriptInterface::GetWanderingPopulationScriptDefNameInRegion() {}
void CGameScriptInterface::IsHeroAllowedHenchmenInCurrentRegion() {}
void CGameScriptInterface::IsHeroAllowedHenchmenInRegion() {}
void CGameScriptInterface::PostAddScriptedEntities() {}
void CGameScriptInterface::IsPlayerHoldingUnsheatheRangedWeaponButton() {}
void CGameScriptInterface::IsPlayerHoldingLockTargetButton() {}
void CGameScriptInterface::IsPlayerHoldingFireRangedWeaponButton() {}
void CGameScriptInterface::IsPlayerHoldingFirstPersonTargetingButton() {}
void CGameScriptInterface::IsHeroInProjectileWeaponMode() {}
void CGameScriptInterface::GetHero() {}
void CGameScriptInterface::GetHeroTargetedThing() {}
void CGameScriptInterface::GetThingWithScriptName() {}
void CGameScriptInterface::GetThingWithScriptName_2() {}
void CGameScriptInterface::GetRandomThingWithScriptName() {}
void CGameScriptInterface::GetAllThingsWithScriptName() {}
void CGameScriptInterface::GetAllCreaturesInAreaWithScriptName() {}
void CGameScriptInterface::GetNearestWithScriptName() {}
void CGameScriptInterface::GetFurthestWithScriptName() {}
void CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom() {}
void CGameScriptInterface::GetAllThingsWithDefName() {}
void CGameScriptInterface::GetNearestWithDefName() {}
void CGameScriptInterface::GetFurthestWithDefName() {}
void CGameScriptInterface::GetThingWithUID() {}
void CGameScriptInterface::GetAllCreaturesExcludingHero() {}
void CGameScriptInterface::GetAllThingsInLevel() {}
void CGameScriptInterface::IsThingWithThisUIDAlive() {}
static int g_timers;
struct TGsi : CGameScriptInterface { virtual long RegisterTimer() { return 100 + g_timers++; } };   /* slot-87 spy */
static TGsi g_gsi;
CGameScriptInterface* g_pGameScriptInterface = &g_gsi;
static int g_scriptbase;
CScriptBase::~CScriptBase() {}
CScriptBase::CScriptBase() { ++g_scriptbase; }
static int g_strings, g_baseclass;
CCharString::CCharString() : PStringData(0) { ++g_strings; }
CBaseClass::~CBaseClass() {}
CBaseClass::CBaseClass() { ++g_baseclass; }
CScriptThing::~CScriptThing() {}
NScript::CQ_HangingTreeEvilScript::~CQ_HangingTreeEvilScript() {}
typedef NScript::CQ_HangingTreeEvilScript Q;
static unsigned char g_buf[sizeof(Q)];
int main() {
    memset(g_buf, 0xcd, sizeof g_buf);
    void* data = (void*)0x5150; Q* q = new (g_buf) Q(data, &g_gsi);
    int ok = sizeof(Q) == 0xd0 && offsetof(Q, ReasonForFailing) == 0x5c && offsetof(Q, Executioner) == 0x74 && offsetof(Q, MissionTimer) == 0xa4
          && offsetof(Q, CurrentLevel) == 0xa8 && offsetof(Q, AreasSpawnedIn) == 0xc4;
    ok = ok && g_scriptbase == 1 && q->Data == data && q->Game == &g_gsi && g_strings == 2 && g_baseclass == 4 && g_timers == 1 && q->MissionTimer.Index == 100;
    CScriptThing* t[4] = { &q->Executioner, &q->Prisoner, &q->PartySpawnPoint, &q->BanditExitPoint };
    for (int i = 0; ok && i < 4; ++i) ok = t[i]->PImp.Data == 0 && t[i]->PImp.Info == 0 && *(void**)t[i] == *(void**)t[0] && *(void**)t[i] != 0;
    ok = ok && !q->CurrentEnemyVector.m_first && !q->CurrentBanditVector.m_last && !q->AreasSpawnedIn.m_end;
    ok = ok && q->HTE_State == (long)0xcdcdcdcd && *(unsigned char*)&q->IntroCutscenePlayed == 0xcd;      /* plain members untouched */
    CScriptBase* b = q; ok = ok && *(void**)b != *(void**)t[0];                                  /* own vtable installed */
    printf(ok ? "00d68f00_TEST PASS\n" : "00d68f00_TEST FAIL\n"); return ok ? 0 : 1;
}