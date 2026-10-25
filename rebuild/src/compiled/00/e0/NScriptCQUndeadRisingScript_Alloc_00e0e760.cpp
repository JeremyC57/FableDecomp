// NScript::CQ_UndeadRisingScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game) -- script factory: `new CQ_UndeadRisingScript(data, game)` with the constructor inlined (registered at 0x00CD79B0 as "S_QUR" / "Q_UndeadRising")
// Leaf: ?Alloc@CQ_UndeadRisingScript@NScript@@SIPAUCScriptBase@@PAUCScriptDataBase@@AAUCGameScriptInterface@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0E760, 77 bytes, static __fastcall (data in ecx, game in edx). PDB:
//   ?Alloc@CQ_UndeadRisingScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z.
// Callees: operator new 0x00BFEA1A (0x78 bytes), CScriptBase::CScriptBase 0x00CB8110. Vtable 0x012DFD3C.
// Inline constructor: the two empty CScriptThing vectors (+0x60, +0x6C, six zero words from one zeroed register).
// Layout: PDB (Ego_r) offsets minus 0x14 (the retail script base is 0x14 bytes smaller), STLport vectors 12 bytes (PDB 16).
// Model: the landed quest-script Alloc (NScript::CQ_HeroSoulsArenaScript::Alloc 0x00D80BA0): CLevelScriptBase<T> is the
//   `__declspec(novtable)` argument-carrying base (Game +0x40, MasterScriptData +0x44, stored before the class's own vtable
//   store), the CGameScriptInterface model is the landed 88-slot one (RegisterTimer = slot 87, +0x15C).
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
struct CScriptDataBase;
struct CScriptBase {                                    // 0x40 bytes, the quest-script root
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
template <class T> struct allocator {                  // STLport allocator<T>: empty
    allocator() { }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T*                    _M_start;
    T*                    _M_finish;
    _STLP_alloc_proxy<T>  _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
template <class T> struct vector : _Vector_base<T> {   // STLport vector<T>, 12 bytes in retail
    vector() : _Vector_base<T>(allocator<T>()) { }
};
class CScriptThing;
namespace NScript {
struct CQ_SunnyvaleMasterData;
class CQ_UndeadRisingScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    unsigned long number_of_zombie_tokens_in_right_area;   // +0x48
    unsigned long number_of_boss_kills_in_right_area;      // +0x4c
    bool HeroReachedCentre;                             // +0x50
    bool BossCutscenePlayed;
    bool MissionSucceeded;
    bool MissionFailed;                                 // +0x53
    bool LightsActivated[8];                            // +0x54
    long quest_status;                                  // +0x5c (an unnamed enum in the PDB)
    vector<CScriptThing> activation_points;             // +0x60
    vector<CScriptThing> boss_points;                   // +0x6c
    CQ_UndeadRisingScript(CScriptDataBase* data, CGameScriptInterface& game) : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game) { }
    virtual ~CQ_UndeadRisingScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterface& game);
};
CScriptBase* __fastcall CQ_UndeadRisingScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game)
{
    return new CQ_UndeadRisingScript(data, game);
}
}