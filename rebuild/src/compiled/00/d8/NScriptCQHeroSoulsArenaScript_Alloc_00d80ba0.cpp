// NScript::CQ_HeroSoulsArenaScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- script factory: `new CQ_HeroSoulsArenaScript(data, game)` with the constructor inlined
// Leaf: ?Alloc@CQ_HeroSoulsArenaScript
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D80BA0, 92 bytes, static __fastcall (data in ecx, game in edx). PDB:
// ?Alloc@CQ_HeroSoulsArenaScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z.
// Callees: operator new 0x00BFEA1A (0x68 bytes), CScriptBase::CScriptBase 0x00CB8110, g_pGameScriptInterface (0x0143E8F8)
// ->RegisterTimer (slot 87, +0x15C) for the GuardChatTimer member. Vtable 0x012D3994.
// Inline constructor: CLevelScriptBase<T> (PDB: CScriptBase + { CGameScriptInterfaceBase* Game; T* MasterScriptData; })
// stores Game +0x40 and the data +0x44 before the class's own vtable store (a novtable intermediate base, as in the
// landed quest-script ctors, e.g. CQ_HangingTreeEvilScript 0x00D68F00), then GuardChatTimer (+0x58) and the empty
// Rounds vector (+0x5C..+0x64). CreatureCount, the seven bools and ArenaLevelReached stay uninitialised.
// Layout: PDB (Ego_r) offsets minus 0x14 (script base; CreatureCount PDB +0x5C -> retail +0x48) and a 12-byte STLport
// vector (PDB 16): retail sizeof 0x68 (`push 68h`), PDB 128.
// Shape: the Rounds vector must be the real STLport _Vector_base (_M_start, _M_finish) + _STLP_alloc_proxy end-of-storage
// built from an allocator() temporary: with a flat three-pointer vector VC pops edi before the three zero stores,
// retail pops it between the second and third (92 v 92, scheduling only).
// The interface model (88 virtual slots of vtable 0x01260F0C, simplified signatures) is the landed one.
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
struct CTimer {
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
class CHeroSoulsRoundDef;
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
namespace NScript {
struct CEmptyData;
class CQ_HeroSoulsArenaScript : public CLevelScriptBase<CEmptyData> {
public:
    long CreatureCount;                                 // +0x48
    bool PlayedArenaCS;                                 // +0x4c
    bool PlayerHitSummoner1;
    bool PlayerHitSummoner2;
    bool PlayerAttacksGuard;
    bool JackTauntNearHOH;
    bool FirstWaveAttack;
    bool PlayedHoHCS;                                   // +0x52
    long ArenaLevelReached;                             // +0x54
    CTimer GuardChatTimer;                              // +0x58
    vector<CHeroSoulsRoundDef> Rounds;                  // +0x5c
    CQ_HeroSoulsArenaScript(CScriptDataBase* data, CGameScriptInterface& game) : CLevelScriptBase<CEmptyData>(data, game) { }
    virtual ~CQ_HeroSoulsArenaScript();
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterface& game);
};
CScriptBase* __fastcall CQ_HeroSoulsArenaScript::Alloc(CScriptDataBase* data, CGameScriptInterface& game)
{
    return new CQ_HeroSoulsArenaScript(data, game);
}
}