// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test checks an unrelated local struct; header comment describes another function; names CBaseObject ctor 0x99a380 as CBaseIntelligentPointer and vtable 0x127094c (CScriptGameResourceObjectScriptedThingBase) as CIntelligentThing)
// NScript::CV_BordelloScript::CV_BordelloScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base and
//   interface/data pair, vtable 0x012E38B0, the BordelloVillage handle, the four seh_ entity handles and the empty
//   cut-scene argument map
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E398F0, `ret 8`. Identity: its factory 0x00E46A00 (`push 0C0h`) is registered at
//   0x00CD8539 under "S_VB" / "V_Bordello"; the manifest label "CQ_HangingTreeGoodScript" is propagated (that PDB class has
//   no such members) and the old header text described CQ_WizardBattleScript.
// Callees: CScriptBase::CScriptBase 0x00CB8110, CBaseClass::CBaseClass 0x0099A2D0 (CScriptThing base; vtable 0x01238C8C =
//   CScriptThing), CBaseObject::CBaseObject 0x0099A380 (CScriptGameResourceObjectScriptedThingBase base; vtable 0x0127094C
//   = CScriptGameResourceObjectScriptedThingBase), CRT malloc 0x00BFEA0E (the map's 0x18-byte header).
// Leaf: ??0CV_BordelloScript
// Layout: PDB (Ego_r) names; retail = PDB - 0x14 (script base): flags/counters +0x48..+0x5b, BordelloVillage +0x5c,
//   WearingWhoreWig..BeerSetToDPad +0x68..+0x73, seh_Boss +0x74, seh_Guard +0x84, seh_Madam +0x94, seh_Whore +0xa4,
//   csargs (12-byte STLport map) +0xb4; size 0xc0. Model shared with the landed CV_ChickenKickingScript 0x00E62830.
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
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
struct CBaseClass { virtual ~CBaseClass(); CBaseClass(); };   // 0x0099A2D0 (vtable 0x01231710 = CBaseClass)
template<class T> struct CCountedPointer { T* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
struct CScriptThing : CBaseClass {                      // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;                 // +0x04
    CScriptThing() : CBaseClass(), PImp() { }
    virtual ~CScriptThing();
};
struct CBaseObject {                                    // CBaseIntelligentPointer root, vtable 0x0129A7D8
    virtual ~CBaseObject();
    void* IntelligentPointer;                           // +0x04
    CBaseObject();                                      // 0x0099A380
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // 16 bytes, vtable 0x0127094C
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;   // +0x08
    CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
    virtual ~CScriptGameResourceObjectScriptedThingBase();
};
struct _Rb_tree_node_base { char _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right; };
template<unsigned int NodeSize> struct _Rb_tree_base {
    _Rb_tree_node_base* _M_header;
    _Rb_tree_base() : _M_header(0) { _M_header = (_Rb_tree_node_base*)malloc(NodeSize); }
};
template<unsigned int NodeSize> struct CRetailMap : _Rb_tree_base<NodeSize> {   // STLport map (12 bytes in retail)
    unsigned long _M_node_count;
    unsigned char _M_key_compare;
    CRetailMap() : _M_node_count(0) { _M_empty_initialize(); }
    void _M_empty_initialize() {
        this->_M_header->_M_color = 0; this->_M_header->_M_parent = 0;
        this->_M_header->_M_left = this->_M_header; this->_M_header->_M_right = this->_M_header;
    }
};
namespace NScript {
struct CV_BordelloScript : CQuestScriptObject {
    bool HeroPartying, HeroTricking, MagicianSleeping, CutscenePlaying, PlayerOwned, BecomeNunnery, PlayerHasLeftRegion,
         HeroFoundDeeds, HeroFoundDeedsLocation;        // +0x48..+0x50
    long ClientsAlive;                                  // +0x54
    bool ClientInUse[3];                                // +0x58
    CScriptThing BordelloVillage;                       // +0x5c
    bool WearingWhoreWig;                               // +0x68
    bool HadSex;                                        // +0x69
    long BeersDrunk;                                    // +0x6c
    bool BeerSetToDPad;                                 // +0x70
    CScriptGameResourceObjectScriptedThingBase seh_Boss;   // +0x74
    CScriptGameResourceObjectScriptedThingBase seh_Guard;  // +0x84
    CScriptGameResourceObjectScriptedThingBase seh_Madam;  // +0x94
    CScriptGameResourceObjectScriptedThingBase seh_Whore;  // +0xa4
    CRetailMap<0x18> csargs;                            // +0xb4 map<CCharString,CCharString>
    virtual ~CV_BordelloScript();
    CV_BordelloScript(void* data, CGameScriptInterface* game);
};
CV_BordelloScript::CV_BordelloScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}
}