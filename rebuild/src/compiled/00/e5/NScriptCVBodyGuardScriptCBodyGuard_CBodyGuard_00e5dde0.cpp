// NScript::CV_BodyGuardScript::CBodyGuard::CBodyGuard(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& me) -- a henchman's entity script: entity-script base over the quest's interface and Me, the parent, the anger and attacked-by-summoners timers, and the master data last
// Leaf: ??0CBodyGuard@CV_BodyGuardScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E5DDE0, 90 bytes, thiscall, `ret 0Ch`, returns this.
// Identity (proven): stores the CBodyGuard entity vtable 0x012E701C (site 0x00E5DE01), whose slot 5 is the landed
//   CBodyGuard::OnPredicateFail 0x00E5FB80 (hand508); only caller the "BodyGuard" binding's Alloc 0x00E5DD60
//   (call 0x00E5DD81). FableWin.pdb ??0CBodyGuard@CV_BodyGuardScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z.
//   Label correction: the manifest row "CDarkwoodTrader::CDarkwoodTrader" is propagated.
// Callees: CScriptThing::CScriptThing(const CScriptThing&) 0x004ABE90, g_pGameScriptInterface (0x0143E8F8)->RegisterTimer
//   (slot 87, +0x15C) for each CTimer.
// Layout: Ego_r.pdb CBodyGuard (60 bytes, entity scripts keep their PDB offsets): CEntityScriptBase {vptr 0x012C3224,
//   Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, HenchManType +0x1c, BrainState +0x20, CurrentHealthState +0x24,
//   PreviousHealthState +0x28, FreakedOut +0x2c (all uninitialised), AngerTimer +0x30, AttackedBySummonersTimer +0x34,
//   MyHealthBarID +0x38.
// Shape: the landed CFisticuffsMember constructor 0x00E83920 model (Game a plain base initialiser stored before the base
//   vtable, the derived vtable before ParentClass, the timers, PMasterData a body assignment stored last).
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
class CBaseClass { public:                           // vtable 0x01231710
    virtual void v00();
    CBaseClass();                             // 0x0099A2D0
};
struct CCountedPointerWords {
    void* Data;
    void* Info;
    CCountedPointerWords() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass { public:            // vtable 0x01238C8C, 12 bytes (PImp Data +4, Info +8)
    CCountedPointerWords PImp;                // +0x04
    virtual void v00();
    CScriptThing(const CScriptThing& other);  // 0x004ABE90
};
class CEntityScriptBase { public:                    // vtable 0x012C3224, 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;               // +0x04 (a plain initialiser: stored before the vtable)
    CScriptThing          Me;                 // +0x08
    CEntityScriptBase(const CScriptThing& thing, CGameScriptInterface* game) : Game(game), Me(thing) { }
};
class CTimer { public:                               // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BodyGuardScript {                              // the owning level script: only Game is read
public:
    unsigned char         _pad00[0x40];                 // CScriptBase
    CGameScriptInterface* Game;                         // +0x40 (CLevelScriptBase<T>::Game)
    class CBodyGuard;
};
class CV_BodyGuardScript::CBodyGuard : public CEntityScriptBase {   // vtable 0x012E701C
public:
    CV_BodyGuardScript*     ParentClass;                // +0x14
    CQ_SunnyvaleMasterData* PMasterData;                // +0x18
    long                    HenchManType;               // +0x1c
    long                    BrainState;                 // +0x20
    long                    CurrentHealthState;         // +0x24
    long                    PreviousHealthState;        // +0x28
    bool                    FreakedOut;                 // +0x2c
    CTimer                  AngerTimer;                 // +0x30
    CTimer                  AttackedBySummonersTimer;   // +0x34
    long                    MyHealthBarID;              // +0x38
    virtual void v00();
    CBodyGuard(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& me);
};
CV_BodyGuardScript::CBodyGuard::CBodyGuard(CV_BodyGuardScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(me, parent.Game), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
}