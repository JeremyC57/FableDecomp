// NScript::CQ_PrisonEscapeRescueMotherScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game) -- script factory: `new CQ_PrisonEscapeRescueMotherScript(data, game)` with the constructor inlined (registered at 0x00CD71EB as "S_QPERM" / "Q_PrisonEscapeRescueMother")
// Leaf: ?Alloc@CQ_PrisonEscapeRescueMotherScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterface@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DDACB0, 74 bytes, static __fastcall (data in ecx, game in edx). PDB:
//   ?Alloc@CQ_PrisonEscapeRescueMotherScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z.
// Callees: operator new 0x00BFEA1A (0x80 bytes), CScriptBase::CScriptBase 0x00CB8110, g_pGameScriptInterface (0x0143E8F8)->RegisterTimer for RunningTimer. Vtable 0x012DB86C.
// Inline constructor: only RunningTimer (+0x7C) has a constructor; the longs and bools stay uninitialised.
// Layout: PDB (Ego_r) offsets minus 0x14 (the retail script base is 0x14 bytes smaller), STLport vectors 12 bytes (PDB 16).
// Model: the landed quest-script Alloc (NScript::CQ_HeroSoulsArenaScript::Alloc 0x00D80BA0): CLevelScriptBase<T> is the
//   `__declspec(novtable)` argument-carrying base (Game +0x40, MasterScriptData +0x44, stored before the class's own vtable
//   store), the CGameScriptInterface model is the landed 88-slot one (RegisterTimer = slot 87, +0x15C).
class CGameScriptInterface { public:
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
class CScriptDataBase;
class CScriptBase { public:                                    // 0x40 bytes, the quest-script root
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04..+0x3f
    CScriptBase();                                      // 0x00CB8110
};
template <class TData>
struct __declspec(novtable) CLevelScriptBase : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    TData*                MasterScriptData;             // +0x44
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterface& game) : CScriptBase(), Game(&game), MasterScriptData((TData*)data) { }
};
class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_PrisonEscapeRescueMotherScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long CurrentTask;                                   // +0x48
    long TasksDone[3];                                  // +0x4c
    long TargetNumber[3];                               // +0x58
    bool HeroAgreesToTask;                              // +0x64
    bool HeroHasFoundWeapons;
    bool MotherCellDoorOpen;
    bool MotherFollowing;
    bool MotherDied;
    bool WatchingHero;
    bool PrisonCutsceneFinished;
    bool RaceStarting;                                  // +0x6b
    long YearsPassedInCell;                             // +0x6c
    long PrisonerConversationIndex;                     // +0x70
    long ConversationIndex;                             // +0x74
    bool HeroEscapedCell;                               // +0x78
    bool AlarmSounded;
    bool MotherRescuedFromPrisonAutoSaveCheckPoint;     // +0x7a
    CTimer RunningTimer;                                // +0x7c
    CQ_PrisonEscapeRescueMotherScript(CScriptDataBase* data, CGameScriptInterface& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_PrisonEscapeRescueMotherScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterface& game);
};
CScriptBase* __fastcall CQ_PrisonEscapeRescueMotherScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game)
{
    return new CQ_PrisonEscapeRescueMotherScript(data, game);
}
}