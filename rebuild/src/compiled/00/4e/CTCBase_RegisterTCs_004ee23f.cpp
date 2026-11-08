// CTCBase::RegisterTCs() -- rebuild the thing-component class registry: empty it, run the component classes' static setup, register every component class (name, allocator, driver type) and the game's definition classes, then sort the registry by name
// Leaf: ?RegisterTCs@CTCBase@@SIXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EE23F, 44,806 bytes, one `ret` at 0x004F9144; public static __fastcall, no arguments. A
//   size-optimised TU (unaligned entry packed against UninitFillN's `ret 4` at 0x004EE23C, CHeroExperienceDef::Copy follows at
//   0x004F9145): optimize("s"), which gives retail's `mov esi,offset RegisteredTCs ; mov ecx,esi` and `or edi,-1 ; push edi` forms.
// Identity: Ego_r.pdb ?RegisterTCs@CTCBase@@SIXXZ (donor 0x0050C7CD, right after CHeroExperienceDef::Alloc, as retail sits right before
//   CHeroExperienceDef::Copy) and FableWin.pdb (0x01B846AD); its only caller is CMainGameComponent::Init (0x00418585). The map it fills
//   is CTCBase::RegisteredTCs (0x013BA89C; Ego_r.pdb / FableWin.pdb "dynamic initializer for 'CTCBase::RegisteredTCs'"), the one
//   CTCBase::GetPTCInfo 0x00686F60 / AllocateTC 0x00686F10 / Clone 0x00686FA0 search. The catalog's Init_RegisterGameComponents is
//   descriptive.
// What it does, in retail order:
//   - RegisteredTCs.clear() (STLport vector::erase(begin, end) 0x004E1B5D);
//   - the static setup of eight classes (StatRegisterClass of CTCCreatureGenerator 0x00749D60, CTCCombat 0x007330F0,
//     CTCHeroRangedAttacks 0x007899C0, CTCHaste 0x007FFFA0, CTCCreatureNavigation 0x006CBA00, CTCCameraPointDefinitionBase 0x0075C0E0,
//     NHitLocations::CHitLocation 0x008162F0, CTCTargetingPlayer 0x00802370);
//   - 340 component registrations: the class-type name (StatGetClassTypeName: expanded `CCharString(literal, -1)` 0x0099EBF0 at
//     the first 41, the class's out-of-line copy from 0x004EF823 on), CTCBase::CRegisteredTCInfo(Alloc, StatGetDriverType(), name)
//     (0x004D2EF0; StatGetDriverType is CTCBase's 0x006869C0, or a driver's own constant pushed as an immediate), then
//     CVectorMap::Insert(name, info) expanded: pair<CCharString, CRegisteredTCInfo>(key, value) 0x004D9D2F, push_back 0x004E40C3, the
//     pair destroyed (expanded at the first 41, ~pair 0x004D6902 after), Dirty = true; then the info and the name destroyed
//     (CCharString::~CCharString 0x0099EAE0);
//   - interleaved with them, 111 definition-class registrations: CGameDefinitionManager::Get() 0x0044C6B0 .AddDefClass 0x009B0AC0 of a
//     CDefClassInit("<DefClass>", <DefClass>::Alloc) (constructor expanded at the first 8, 0x0042DAE0 after);
//   - CTCCreatureModeManager::RegisterModes 0x0073B130, then RegisteredTCs.Update(): Sort 0x004EBACE when Dirty.
// Inline budget: the 41 / 41 / 8 expanded-then-called split above is VC7.1's caller-size budget, reproduced by this plain source
//   with no flag or pragma.
// Frame: 0x2FB4 bytes, one slot per temporary (340 x (name 4 + info 12 + pair 16) + 111 x (name 4 + init 8)): VC7.1 shares no
//   temporary slot in a function this size, in retail as here.
// Models: engine/CTCBase.h (FABLE_RETAIL_CTCBASE_REGISTRY: CRegisteredTCInfo, StatGetDriverType, RegisterTCs, RegisteredTCs),
//   engine/CVectorMap.h (_INSERT_KEYVALUE + _INSERT_VALUE_TYPE, _SORT), engine/CDefinitionManager.h (_ADDDEFCLASS),
//   engine/CGameDefinitionManager.h, engine/CDefClassInit.h (new), engine/NHitLocations_CHitLocation.h (_STATREGISTERCLASS),
//   fable_string.h, stlport/fable_containers.h.
// The 451 registered classes are declared below with only their registration interface (no layout); 136 of them also have an engine
//   header, whose generated layouts do not declare these statics and do not compile together in one TU (see NOTES.md, header debt).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_REGISTRY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE
#define FABLE_RETAIL_CVECTORMAP_SORT
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_ADDDEFCLASS
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CHITLOCATION_STATREGISTERCLASS
#include "engine/CTCBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDefClassInit.h"
#include "engine/NHitLocations_CHitLocation.h"

class CThing;
class CDefClassBase;

// The registration interface of a thing-component class (declared in each class in the game's headers): its allocator and its
// class-type name. VC7.1 expands StatGetClassTypeName at the first 41 registrations and calls its out-of-line copy after that.
#define TC_REGISTRATION_INTERFACE(TCClass) \
    static CTCBase* __fastcall Alloc(CThing& owner); \
    static CCharString __fastcall StatGetClassTypeName() { return #TCClass; }
// The registration interface of a definition class: its allocator.
#define DEF_CLASS_REGISTRATION_INTERFACE(DefClass) \
    static CDefClassBase* __fastcall Alloc();

// The classes RegisterTCs names, in registration order: thing-component classes (drivers return their own driver type, the rest
// inherit CTCBase::StatGetDriverType's 0), CTCCameraPointDefinitionBase (static setup only; NHitLocations::CHitLocation comes from
// its engine header), then the definition classes. Only the registration interface is declared, no layout and no intermediate
// bases: in retail (RTTI) the components derive from CTCBase directly (208) or through CTCActionUseBase, CTCDriverBase and the
// like, and the definition classes from CSubDefClassBase (100), CParentDefClassBase (10) or CTavernGameDef (1).
class CTCHeroMorph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroMorph) };   // Alloc 0x004D28BB
class CTCSimpleAppearanceMorph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSimpleAppearanceMorph) };   // Alloc 0x004D2A14
class CTCAtmosPlayer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAtmosPlayer) };   // Alloc 0x004D642D
class CTCRandomAppearanceMorph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCRandomAppearanceMorph) };   // Alloc 0x004D2A44
class CTCHighlightItem : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHighlightItem) };   // Alloc 0x004D4548
class CTCSmokeGenerator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSmokeGenerator) };   // Alloc 0x004D28EB
class CTCTimeAppearanceFade : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTimeAppearanceFade) };   // Alloc 0x004D4114
class CTCPhysicsLight : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhysicsLight) };   // Alloc 0x004D294B
class CTCPhysicsStandard : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhysicsStandard) };   // Alloc 0x004D297B
class CTCPhysicsControlled : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhysicsControlled) };   // Alloc 0x004D29AE
class CTCCreatureNavigation : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureNavigation) static void __fastcall StatRegisterClass(); };   // Alloc 0x004D291B, StatRegisterClass 0x006CBA00
class CTCPhysicsNavigator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhysicsNavigator) };   // Alloc 0x004D29E1
class CTCTargetingAI : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTargetingAI) };   // Alloc 0x004E0047
class CTCTargetingPlayer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTargetingPlayer) static void __fastcall StatRegisterClass(); };   // Alloc 0x004E3783, StatRegisterClass 0x00802370
class CTCTargeted : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTargeted) };   // Alloc 0x004DBF1D
class CTCSpecialAbilities : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSpecialAbilities) };   // Alloc 0x004D2D98
class CTCUndeadSoul : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCUndeadSoul) };   // Alloc 0x004D7AE9
class CTCSoundPlayer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSoundPlayer) };   // Alloc 0x004D30EE
class CTCTalk : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTalk) };   // Alloc 0x004D2E0A
class CTCInventory : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventory) };   // Alloc 0x004D2E74
class CTCInventoryClothing : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryClothing) };   // Alloc 0x004E8967
class CTCInventoryWeapons : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryWeapons) };   // Alloc 0x004D2EBA
class CTCInventoryAbilities : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryAbilities) };   // Alloc 0x004D300F
class CTCInventoryMagic : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryMagic) };   // Alloc 0x004D2F46
class CTCInventoryStats : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryStats) };   // Alloc 0x004D2FDC
class CTCInventoryExperience : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryExperience) };   // Alloc 0x004D3042
class CTCInventoryTrade : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryTrade) };   // Alloc 0x004D3075
class CTCInventoryQuests : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryQuests) };   // Alloc 0x004D30A8
class CTCInventoryMap : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryMap) };   // Alloc 0x004D2F13
class CTCInventoryItem : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryItem) };   // Alloc 0x004DC5C7
class CTCCreatureExpression : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureExpression) };   // Alloc 0x004D2AA4
class CTCLook : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCLook) };   // Alloc 0x004D38F3
class CTCActionUseTorch : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseTorch) };   // Alloc 0x004DBF92
class CTCActionUseScriptedHook : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseScriptedHook) };   // Alloc 0x004D2AD4
class CTCActionUseSign : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseSign) };   // Alloc 0x004DBFCB
class CTCActionUseReadable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseReadable) };   // Alloc 0x004DC004
class CTCActionUsePickUp : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUsePickUp) };   // Alloc 0x004DC065
class CTCActionUsePickUpGenericBox : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUsePickUpGenericBox) };   // Alloc 0x004DC09E
class CTCActionUsePutInInventory : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUsePutInInventory) };   // Alloc 0x004DC0D7
class CTCActionUseMapTable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseMapTable) };   // Alloc 0x004DC110
class CTCActionUseSearch : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseSearch) };   // Alloc 0x004DC149
class CTCActionUseBlowHorn : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseBlowHorn) };   // Alloc 0x004DC182, name 0x004D2BFF
class CTCActionUseBuyOrSteal : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseBuyOrSteal) };   // Alloc 0x004D2C12, name 0x004D2C2F
class CTCActionUseWindow : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseWindow) };   // Alloc 0x004DC1BB, name 0x004D2C42
class CTCActionUseAcceptBoast : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseAcceptBoast) };   // Alloc 0x004DC1F4, name 0x004D2C55
class CTCActionUseStartExtrapolationMorph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseStartExtrapolationMorph) };   // Alloc 0x004DC22D, name 0x004D2C68
class CTCActionUseBed : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseBed) };   // Alloc 0x004DC266, name 0x004D2C7B
class CTCActionUseKickChicken : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseKickChicken) };   // Alloc 0x004DC283, name 0x004D2C8E
class CTCCarriedActionUseLamp : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseLamp) };   // Alloc 0x004DC2D8, name 0x004D2CA1
class CTCCarriedActionUseDrop : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseDrop) };   // Alloc 0x004DC311, name 0x004D2CB4
class CTCCarriedActionUseDropGenericBox : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseDropGenericBox) };   // Alloc 0x004DC34A, name 0x004D2CC7
class CTCCarriedActionUseGive : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseGive) };   // Alloc 0x004DC383, name 0x004D2CDA
class CTCCarriedActionUseGiveBeer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseGiveBeer) };   // Alloc 0x004DC3BC, name 0x004D2CED
class CTCCarriedActionUseGoldBag : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseGoldBag) };   // Alloc 0x004DC3F5, name 0x004D2D00
class CTCCarriedActionUseShow : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseShow) };   // Alloc 0x004DC42E, name 0x004D2D13
class CTCCarriedActionUseTrophy : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseTrophy) };   // Alloc 0x004DC54B, name 0x004D2D72
class CTCCarriedActionUsePotion : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUsePotion) };   // Alloc 0x004DC4D9, name 0x004D2D4C
class CTCCarriedActionUseFood : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseFood) };   // Alloc 0x004DC467, name 0x004D2D26
class CTCCarriedActionUseFish : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseFish) };   // Alloc 0x004DC4A0, name 0x004D2D39
class CTCActionUseSwitch : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseSwitch) };   // Alloc 0x004DC58E, name 0x004D2D85
class CTCScriptedControl : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCScriptedControl) };   // Alloc 0x004D311E, name 0x004D313B
class CTCMeshAutomaticEntityCreator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCMeshAutomaticEntityCreator) };   // Alloc 0x004D7B22, name 0x004D314E
class CTCThingOwner : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCThingOwner) };   // Alloc 0x004E0105, name 0x004D318D
class CTCVillage : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCVillage) };   // Alloc 0x004D31A0, name 0x004D31C0
class CTCVillageMember : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCVillageMember) };   // Alloc 0x004DC620, name 0x004D31D8
class CTCBuyableHouse : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBuyableHouse) };   // Alloc 0x004D31EB, name 0x004D3208
class CTCActionUseBuyHouse : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseBuyHouse) };   // Alloc 0x004D321B, name 0x004D3238
class CTCDummyVillager : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDummyVillager) };   // Alloc 0x004D3261, name 0x004D327E
class CTCWife : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWife) };   // Alloc 0x004D32A7, name 0x004D32C7
class CTCJackShield : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCJackShield) };   // Alloc 0x004D3350, name 0x004D336D
class CTCJackPillar : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCJackPillar) };   // Alloc 0x004D3380, name 0x004D339D
class CTCDoor : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDoor) };   // Alloc 0x004D32DA, name 0x004D32F7
class CTCLight : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCLight) };   // Alloc 0x004D3320, name 0x004D333D
class CTCSpotLight : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSpotLight) };   // Alloc 0x004D33DC, name 0x004D33F9
class CTCClock : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCClock) };   // Alloc 0x004D3422, name 0x004D343F
class CTCHero : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHero) };   // Alloc 0x004D3468, name 0x004D3485
class CTCWeatherVane : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWeatherVane) };   // Alloc 0x004D7D45, name 0x004D3498
class CTCSingleUserActionPoints : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSingleUserActionPoints) };   // Alloc 0x004E23E2, name 0x004D34DA
class CTCNamedUserActionPoints : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCNamedUserActionPoints) };   // Alloc 0x004E23FF, name 0x004D34ED
class CTCExclusiveUse : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCExclusiveUse) };   // Alloc 0x004DAF2E, name 0x004D3500
class CTCRotate : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCRotate) };   // Alloc 0x004D7A90, name 0x004D2B17
class CTCCombat : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCombat) static void __fastcall StatRegisterClass(); };   // Alloc 0x004D3513, name 0x004D3533, StatRegisterClass 0x007330F0
class CTCCreatedEntity : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatedEntity) };   // Alloc 0x004DAF4B, name 0x004D3576
class CTCOwnedEntity : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCOwnedEntity) };   // Alloc 0x004DAF68, name 0x004D3589
class CTCCreatureModeManager : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureModeManager) static void __fastcall RegisterModes(); };   // Alloc 0x004D3546, name 0x004D3563, RegisterModes 0x0073B130
class CTCPerceiveThing : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPerceiveThing) };   // Alloc 0x004D359C, name 0x004D35B9
class CTCBed : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBed) };   // Alloc 0x004D35E2, name 0x004D35FF
class CTCStealth : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCStealth) };   // Alloc 0x004D3612, name 0x004D362F
class CTCNoise : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCNoise) };   // Alloc 0x004D3658, name 0x004D3675
class CTCCupboard : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCupboard) };   // Alloc 0x004D7F42, name 0x004D3688
class CTCTrophy : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTrophy) };   // Alloc 0x004D36CE, name 0x004D36EB
class CTCMapwho : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCMapwho) };   // Alloc 0x004D3750, name 0x004D376D
class CTCCreatureGenerator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureGenerator) static void __fastcall StatRegisterClass(); };   // Alloc 0x004D3793, name 0x004D37B3, StatRegisterClass 0x00749D60
class CTCChest : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCChest) };   // Alloc 0x004D37F6, name 0x004D3813
class CTCSearchableContainer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSearchableContainer) };   // Alloc 0x004E746D, name 0x004D38CD
class CTCActionUseChest : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseChest) };   // Alloc 0x004DC681, name 0x004D3844
class CTCDiscovery : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDiscovery) };   // Alloc 0x004D3857, name 0x004D3874
class CTCExplodingObject : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCExplodingObject) };   // Alloc 0x004D3887, name 0x004D38A4
class CTCContainerRewardHero : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCContainerRewardHero) };   // Alloc 0x004D37C6, name 0x004D37E3
class CTCEntityEvents : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCEntityEvents) };   // Alloc 0x004E48F6, name 0x004D38E0
class CTCHeroStats : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroStats) };   // Alloc 0x004D2FA9, name 0x004D2FC9
class CTCHeroRememberCrimes : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroRememberCrimes) };   // Alloc 0x004D393C, name 0x004D3959
class CTCWound : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWound) };   // Alloc 0x004D396C, name 0x004D3989
class CTCPhysicsRigidBody : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhysicsRigidBody) };   // Alloc 0x004D399C, name 0x004D39BC
class CTCDCameraPoint : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDCameraPoint) static unsigned long __fastcall StatGetDriverType() { return 20; } };   // Alloc 0x004DAFCA, name 0x004D39CF
class CTCCameraPointFixedPoint : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointFixedPoint) };   // Alloc 0x004D81E6, name 0x004D3A60
class CTCCameraPointBuilding : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointBuilding) };   // Alloc 0x004DAFE7, name 0x004D39EB
class CTCCameraPointFixedDolly : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointFixedDolly) };   // Alloc 0x004D8225, name 0x004D3AA2
class CTCCameraPointTrack : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointTrack) };   // Alloc 0x004D8264, name 0x004D3AE1
class CTCCameraPointGeneralCase : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointGeneralCase) };   // Alloc 0x004D82A3, name 0x004D3B2E
class CTCCameraPointDead : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointDead) };   // Alloc 0x004D82E2, name 0x004D3B8C
class CTCCameraPointScripted : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointScripted) };   // Alloc 0x004D3BCB, name 0x004D3BEB
class CTCCameraPointScriptedSpline : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointScriptedSpline) };   // Alloc 0x004D3BFE, name 0x004D3C1E
class CTCShapeManager : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShapeManager) };   // Alloc 0x004D3C31, name 0x004D3C4E
class CTCEmoteIcon : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCEmoteIcon) };   // Alloc 0x004D3C61, name 0x004D3C7E
class CTCWeapon : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWeapon) };   // Alloc 0x004D3C91, name 0x004D3CAE
class CTCCarrying : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarrying) };   // Alloc 0x004E7DFC, name 0x004D3D56
class CTCCarryable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarryable) };   // Alloc 0x004D8321, name 0x004D3CFB
class CTCEnemy : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCEnemy) };   // Alloc 0x004D3D69, name 0x004D3D86
class CTCBandit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBandit) };   // Alloc 0x004D3D99, name 0x004D3DB6
class CTCDParticleEmitter : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDParticleEmitter) static unsigned long __fastcall StatGetDriverType() { return 19; } };   // Alloc 0x004D3DDF, name 0x004D3DFC
class CTCWeaponTrail : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWeaponTrail) };   // Alloc 0x004D3E13, name 0x004D3E30
class CTCSolidToThing : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSolidToThing) };   // Alloc 0x004D3E43, name 0x004D3E60
class CTCSmashableWindow : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSmashableWindow) };   // Alloc 0x004DE34E, name 0x004D3E73
class CTCSmashableBarrier : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSmashableBarrier) };   // Alloc 0x004DC6FE, name 0x004D3E86
class CTCDegradableBarrier : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDegradableBarrier) };   // Alloc 0x004D3E99, name 0x004D3EB6
class CTCHeroOpinionDeedLog : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroOpinionDeedLog) };   // Alloc 0x004D3F50, name 0x004D3F70
class CTCCreatureOpinionOfHero : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureOpinionOfHero) };   // Alloc 0x004D3F9D, name 0x004D3FBD
class CTCOpinionSource : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCOpinionSource) };   // Alloc 0x004D3FD0, name 0x004D3FF0
class CTCHeroPodium : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroPodium) };   // Alloc 0x004D4003, name 0x004D4020
class CTCShop : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShop) };   // Alloc 0x004D407C, name 0x004D4099
class CTCBlacksmiths : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBlacksmiths) };   // Alloc 0x004D8449, name 0x004D40AC
class CTCStockItem : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCStockItem) };   // Alloc 0x004DE414, name 0x004D40EB
class CTCGift : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGift) };   // Alloc 0x004D850E, name 0x004D415A
class CTCFamilyMember : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFamilyMember) };   // Alloc 0x004D41AF, name 0x004D41CC
class CTCDNavigationSeed : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDNavigationSeed) static unsigned long __fastcall StatGetDriverType() { return 21; } };   // Alloc 0x004D858D, name 0x004D41DF
class CTCDHeroSuit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDHeroSuit) static unsigned long __fastcall StatGetDriverType() { return 22; } };   // Alloc 0x004E272A, name 0x004D4232
class CTCDNone : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDNone) static unsigned long __fastcall StatGetDriverType() { return 23; } };   // Alloc 0x004D83A0, name 0x004D3EF9
class CTCDInternalMarker : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDInternalMarker) static unsigned long __fastcall StatGetDriverType() { return 12; } };   // Alloc 0x004D85C6, name 0x004D4249
class CTCDRegionEntrance : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDRegionEntrance) static unsigned long __fastcall StatGetDriverType() { return 24; } };   // Alloc 0x004D85FF, name 0x004D429B
class CTCDRegionExit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDRegionExit) static unsigned long __fastcall StatGetDriverType() { return 25; } };   // Alloc 0x004D42F2, name 0x004D430F
class CTCActivationReceptorChest : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActivationReceptorChest) };   // Alloc 0x004E2791, name 0x004D4326
class CTCActivationReceptorCreatureGenerator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActivationReceptorCreatureGenerator) };   // Alloc 0x004E0827, name 0x004D4339
class CTCActivationReceptorDoor : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActivationReceptorDoor) };   // Alloc 0x004E088E, name 0x004D4380
class CTCActivationReceptorRelay : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActivationReceptorRelay) };   // Alloc 0x004E08C7, name 0x004D4393
class CTCActivationTrigger : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActivationTrigger) };   // Alloc 0x004D8638, name 0x004D43A6
class CTCHeroExperience : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroExperience) };   // Alloc 0x004D2F79, name 0x004D2F96
class CTCDExperienceOrb : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDExperienceOrb) static unsigned long __fastcall StatGetDriverType() { return 26; } };   // Alloc 0x004D434C, name 0x004D4369
namespace NGraphicAppearance {
class CTCGraphicAppearance : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearance) };   // Alloc 0x004D44E8, name 0x004D4505
class CTCGraphicAppearanceAnimatedMesh : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearanceAnimatedMesh) };   // Alloc 0x004D43F8, name 0x004D4415
class CTCGraphicAppearanceStaticMesh : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearanceStaticMesh) };   // Alloc 0x004D4428, name 0x004D4445
class CTCGraphicAppearanceStaticMeshFadeOutOnProximity : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearanceStaticMeshFadeOutOnProximity) };   // Alloc 0x004D4458, name 0x004D4475
class CTCGraphicAppearanceSprite : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearanceSprite) };   // Alloc 0x004D4488, name 0x004D44A5
class CTCGraphicAppearanceReplaceableMesh : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearanceReplaceableMesh) };   // Alloc 0x004D4D71, name 0x004D4D8E
class CTCGraphicAppearanceMultipleStaticMeshes : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearanceMultipleStaticMeshes) };   // Alloc 0x004D5E44, name 0x004D5E61
}
class CTCAnimationComplex : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAnimationComplex) };   // Alloc 0x004D44B8, name 0x004D44D5
class CTCHeroAttachableAppearanceModifiers : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroAttachableAppearanceModifiers) };   // Alloc 0x004D4518, name 0x004D4535
class CTCCreatureItemLevitationHero : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureItemLevitationHero) };   // Alloc 0x004E09C7, name 0x004D45A1
class CTCCreatureItemLevitationNymph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureItemLevitationNymph) };   // Alloc 0x004E0984, name 0x004D458E
class CTCCreatureHive : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureHive) };   // Alloc 0x004D86B7, name 0x004D45B4
class CTCHeroCentre : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroCentre) };   // Alloc 0x004D45FA, name 0x004D4617
class CTCHeroCentreDoorMarker : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroCentreDoorMarker) };   // Alloc 0x004D4640, name 0x004D465D
class CTCHeroGuide : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroGuide) };   // Alloc 0x004D4670, name 0x004D468D
class CTCQuestCard : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCQuestCard) };   // Alloc 0x004D2E44, name 0x004D2E61
class CTCFlammable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFlammable) };   // Alloc 0x004D46A0, name 0x004D46BD
class CTCBoastingPodium : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBoastingPodium) };   // Alloc 0x004E7E19, name 0x004D46D0
class CTCBoastingPosition : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBoastingPosition) };   // Alloc 0x004D46F9, name 0x004D4716
class CTCBoastingArea : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBoastingArea) };   // Alloc 0x004D4729, name 0x004D4746
class CTCBoastingCrowdControl : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBoastingCrowdControl) };   // Alloc 0x004D4759, name 0x004D4776
class CTCCreatureGroupBoastingCrowd : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureGroupBoastingCrowd) };   // Alloc 0x004D479C, name 0x004D47B9
class CTCVolumeContainmentTracker : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCVolumeContainmentTracker) };   // Alloc 0x004D47CC, name 0x004D47E9
class CTCDeadShotInThingTracker : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDeadShotInThingTracker) };   // Alloc 0x004D6336, name 0x004D6353
namespace NThingShot {
class CTCShotDefinitionNymphFun : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionNymphFun) };   // Alloc 0x004DEACF, name 0x004D5401
class CTCShotDefinitionQueenHornetShot : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionQueenHornetShot) };   // Alloc 0x004D5414, name 0x004D5434
class CTCShotDefinitionSpreadShotProjectile : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionSpreadShotProjectile) };   // Alloc 0x004DEA93, name 0x004D53EE
class CTCShotDefinitionProjectile : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionProjectile) };   // Alloc 0x004DEA57, name 0x004D53A8
class CTCShotDefinitionThrow : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionThrow) };   // Alloc 0x004D8D9C, name 0x004D5447
class CTCShotDefinitionThrowPredictive : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionThrowPredictive) };   // Alloc 0x004D8DD8, name 0x004D549F
class CTCShotDefinitionProjectilePredictive : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionProjectilePredictive) };   // Alloc 0x004DEB0B, name 0x004D54DA
class CTCShotDefinitionTearInGround : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionTearInGround) };   // Alloc 0x004D8E14, name 0x004D54ED
class CTCShotDefinitionForcePushBall : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionForcePushBall) };   // Alloc 0x004DEB47, name 0x004D5529
class CTCShotDefinitionDragonFireball : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionDragonFireball) };   // Alloc 0x004D8E50, name 0x004D5562
class CTCShotDefinitionMultiArrow : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionMultiArrow) };   // Alloc 0x004D53BB, name 0x004D53DB
class CTCShotDefinitionDrainLife : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionDrainLife) };   // Alloc 0x004DCB3C, name 0x004D554F
class CTCShotDefinitionFireballSpell : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionFireballSpell) };   // Alloc 0x004DEB83, name 0x004D553C
}
class CTCBurntEffect : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBurntEffect) };   // Alloc 0x004D58C6, name 0x004D58E3
class CTCElectrocutedEffect : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCElectrocutedEffect) };   // Alloc 0x004D58F6, name 0x004D5913
class CTCCreatureTintEffect : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureTintEffect) };   // Alloc 0x004D5926, name 0x004D5943
class CTCSkeletalMorph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSkeletalMorph) };   // Alloc 0x004D47FC, name 0x004D4819
class CTCGatherPointBuilding : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGatherPointBuilding) };   // Alloc 0x004E28F1, name 0x004D4901
class CTCGatherPointRadialArea : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGatherPointRadialArea) };   // Alloc 0x004E292A, name 0x004D4914
class CTCSwitchableNavigation : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSwitchableNavigation) };   // Alloc 0x004D877C, name 0x004D482C
class CTCRegionFollower : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCRegionFollower) };   // Alloc 0x004D48D1, name 0x004D48EE
class CTCDayOrNightOnlySupport : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDayOrNightOnlySupport) };   // Alloc 0x004D4927, name 0x004D4944
class CTCMeleeTrap : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCMeleeTrap) };   // Alloc 0x004D4987, name 0x004D49A4
class CTCProjectileTrap : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCProjectileTrap) };   // Alloc 0x004D4957, name 0x004D4974
class CTCHitLocations : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHitLocations) };   // Alloc 0x004E7E46, name 0x004D49B7
class CTCMoneyReceiver : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCMoneyReceiver) };   // Alloc 0x004D87DF, name 0x004D49CA
class CTCHeroRangedAttacks : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroRangedAttacks) static void __fastcall StatRegisterClass(); };   // Alloc 0x004D4A13, name 0x004D4A30, StatRegisterClass 0x007899C0
class CTCParticleAttacher : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCParticleAttacher) };   // Alloc 0x004E0BBD, name 0x004D4A43
class CTCAnimatingObject : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAnimatingObject) };   // Alloc 0x004E0C1E, name 0x004D4A56
class CTCBerserk : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBerserk) };   // Alloc 0x004D4A69, name 0x004D4A86
class CTCDivineWrath : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDivineWrath) };   // Alloc 0x004D4A99, name 0x004D4AB9
class CTCUnholyPower : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCUnholyPower) };   // Alloc 0x004D4ACC, name 0x004D4AEC
class CTCHaste : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHaste) static void __fastcall StatRegisterClass(); };   // Alloc 0x004E2B4B, name 0x004D4AFF, StatRegisterClass 0x007FFFA0
class CTCForcePushed : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCForcePushed) };   // Alloc 0x004D4B12, name 0x004D4B2F
class CTCRegionDisplay : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCRegionDisplay) };   // Alloc 0x004D4B42, name 0x004D4B5F
class CTCExpression : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCExpression) };   // Alloc 0x004DC78F, name 0x004D4B72
class CTCCarriedActionUseExpression : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseExpression) };   // Alloc 0x004DC7E8, name 0x004D4B9B
class CTCMultiStrike : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCMultiStrike) };   // Alloc 0x004D5896, name 0x004D58B3
class CTCMultiArrow : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCMultiArrow) };   // Alloc 0x004D5866, name 0x004D5883
class CTCDrainLife : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDrainLife) };   // Alloc 0x004D5600, name 0x004D561D
class CTCHealLife : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHealLife) };   // Alloc 0x004D5630, name 0x004D564D
class CTCGhostSword : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGhostSword) };   // Alloc 0x004D5660, name 0x004D567D
class CTCEffectOnDie : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCEffectOnDie) };   // Alloc 0x004D5D53, name 0x004D5D70
class CTCWillResponse : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWillResponse) };   // Alloc 0x004D645D, name 0x004D647A
class CTCFireballSpell : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFireballSpell) };   // Alloc 0x004D5723, name 0x004D5743
class CTCTurncoatSpell : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTurncoatSpell) };   // Alloc 0x004D5690, name 0x004D56B0
class CTCTurncoat : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTurncoat) };   // Alloc 0x004D56C3, name 0x004D56E0
class CTCSummonableCreature : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSummonableCreature) };   // Alloc 0x004D4C0E, name 0x004D4C2B
class CTCShopKeeper : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShopKeeper) };   // Alloc 0x004D4BAE, name 0x004D4BCB
class CTCSummonSpell : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSummonSpell) };   // Alloc 0x004D4BDE, name 0x004D4BFB
class CTCBulletTime : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBulletTime) };   // Alloc 0x004D4C58, name 0x004D4C75
class CTCPhysicalShieldSpell : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhysicalShieldSpell) };   // Alloc 0x004D4C88, name 0x004D4CA5
class CTCWillLightning : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWillLightning) };   // Alloc 0x004D4CB8, name 0x004D4CD8
class CTCWillEnflame : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWillEnflame) };   // Alloc 0x004D4CEB, name 0x004D4D08
class CTCForcePushPower : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCForcePushPower) };   // Alloc 0x004D4D1B, name 0x004D4D38
class CTCPreCalculatedNavigationRoute : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPreCalculatedNavigationRoute) };   // Alloc 0x004E2B8E, name 0x004D4D4B
class CTCCreatureGroup : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureGroup) };   // Alloc 0x004E75A0, name 0x004D4789
class CTCBattleCharge : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBattleCharge) };   // Alloc 0x004D4DA1, name 0x004D4DC1
class CTCAssassinRush : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAssassinRush) };   // Alloc 0x004D4DD4, name 0x004D4DF4
class CTCCreatureGroupTag : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureGroupTag) };   // Alloc 0x004E762D, name 0x004D4D5E
class CTCAIScratchpad : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAIScratchpad) };   // Alloc 0x004D4E20, name 0x004D4E40
class CTCSinglePersonOccupiable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSinglePersonOccupiable) };   // Alloc 0x004DB0AD, name 0x004D4E53
class CTCBoss : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBoss) };   // Alloc 0x004D4E7C, name 0x004D4E99
class CTCRockTrollShield : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCRockTrollShield) };   // Alloc 0x004D4EAC, name 0x004D4EC9
class CTCCombatLeader : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCombatLeader) };   // Alloc 0x004D8942, name 0x004D4EDC
class CTCFishingSpot : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFishingSpot) };   // Alloc 0x004D4F21, name 0x004D4F3E
class CTCFishing : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFishing) };   // Alloc 0x004D4F51, name 0x004D4F6E
class CTCStealableItemLocation : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCStealableItemLocation) };   // Alloc 0x004D897B, name 0x004D4F81
class CTCGuard : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGuard) };   // Alloc 0x004D5013, name 0x004D5030
class CTCInterestingToVillagers : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInterestingToVillagers) };   // Alloc 0x004DC821, name 0x004D4FEA
class CTCCarriedActionUseActivateQuest : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseActivateQuest) };   // Alloc 0x004DC936, name 0x004D5043
class CTCWaterWader : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWaterWader) };   // Alloc 0x004D5070, name 0x004D508D
class CTCCrateStack : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCrateStack) };   // Alloc 0x004D50B6, name 0x004D50D3
class CTCOverheadDisplay : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCOverheadDisplay) };   // Alloc 0x004D50E6, name 0x004D5103
class CTCTavernTable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernTable) };   // Alloc 0x004D5142, name 0x004D515F
class CTCTavernBar : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernBar) };   // Alloc 0x004D5172, name 0x004D518F
class CTCTavernTankard : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernTankard) };   // Alloc 0x004DB124, name 0x004D51A2
class CTCTavernJug : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernJug) };   // Alloc 0x004D8BA8, name 0x004D51B5
class CTCTavern : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavern) };   // Alloc 0x004D5215, name 0x004D5232
class CTCObjectAugmentations : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCObjectAugmentations) };   // Alloc 0x004D3CC1, name 0x004D3CDE
class CTCFollowed : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFollowed) };   // Alloc 0x004D5245, name 0x004D5262
class CTCTrespasser : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTrespasser) };   // Alloc 0x004D52A5, name 0x004D52C2
class CTCTrespassable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTrespassable) };   // Alloc 0x004D5275, name 0x004D5292
class CTCGossip : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGossip) };   // Alloc 0x004DB141, name 0x004D52D5
class CTCHitOnCollision : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHitOnCollision) };   // Alloc 0x004D56F3, name 0x004D5710
class CTCDrunkenness : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDrunkenness) };   // Alloc 0x004D52FE, name 0x004D531B
class CTCOnHeroPush : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCOnHeroPush) };   // Alloc 0x004D8CD7, name 0x004D532E
class CTCInGameMenu : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInGameMenu) };   // Alloc 0x004D559D, name 0x004D55BD
class CTCElectrocuteOnTouch : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCElectrocuteOnTouch) };   // Alloc 0x004D6039, name 0x004D6056
class CTCCoopSpirit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCoopSpirit) };   // Alloc 0x004D55D0, name 0x004D55ED
class CTCGeneratesExperienceOnKilling : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGeneratesExperienceOnKilling) };   // Alloc 0x004D8E8C, name 0x004D5756
class CTCCreatureGeneratorCreator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureGeneratorCreator) };   // Alloc 0x004D5797, name 0x004D57B4
class CTCHeroReceiveItems : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroReceiveItems) };   // Alloc 0x004D57C7, name 0x004D57E7
class CTCActionUseActivateQuest : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseActivateQuest) };   // Alloc 0x004DCB5C, name 0x004D57FA
class CTCGold : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGold) };   // Alloc 0x004D580D, name 0x004D582A
class CTCTargetingSpirit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTargetingSpirit) };   // Alloc 0x004E1006, name 0x004D5853
class CTCCreatureHitNotification : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureHitNotification) };   // Alloc 0x004EA695, name 0x004D5956
class CTCAICreatureWillPowerIndicator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAICreatureWillPowerIndicator) };   // Alloc 0x004D5E14, name 0x004D5E31
class CTCTavernGameShoveHaPenny : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameShoveHaPenny) };   // Alloc 0x004D5995, name 0x004D59B5
class CTCTavernGameCoinGolf : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameCoinGolf) };   // Alloc 0x004D59DE, name 0x004D59FE
class CTCTavernGameCardSorting : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameCardSorting) };   // Alloc 0x004D5A11, name 0x004D5A31
class CTCTavernGameCardPairs : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameCardPairs) };   // Alloc 0x004D5A44, name 0x004D5A64
class CTCTavernGameSpotTheAddition : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameSpotTheAddition) };   // Alloc 0x004D5A77, name 0x004D5A97
class CTCTavernGameCardBlackjack : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameCardBlackjack) };   // Alloc 0x004D5AAA, name 0x004D5ACA
class CTCGamePlayer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGamePlayer) };   // Alloc 0x004DB1EA, name 0x004D5ADD
class CTCDecapitation : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDecapitation) };   // Alloc 0x004DCB95, name 0x004D5AF0
class CTCTavernGameMaster : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameMaster) };   // Alloc 0x004D908D, name 0x004D5B19
class CTCParticleAttachmentTracker : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCParticleAttachmentTracker) };   // Alloc 0x004E1244, name 0x004D5B5E
class CTCCreatureFlyOnSpline : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureFlyOnSpline) };   // Alloc 0x004D5B71, name 0x004D5B8E
class CTCCreatureFlyInCircles : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureFlyInCircles) };   // Alloc 0x004D5BA1, name 0x004D5BBE
class CTCWallMount : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWallMount) };   // Alloc 0x004D5BD1, name 0x004D5BEE
class CTCWallMountEffects : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWallMountEffects) };   // Alloc 0x004D5C01, name 0x004D5C1E
class CTCFish : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFish) };   // Alloc 0x004D5C47, name 0x004D5C64
class CTCTeleporter : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTeleporter) };   // Alloc 0x004D5C8D, name 0x004D5CAA
class CTCActionUseWallMount : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseWallMount) };   // Alloc 0x004DCBEE, name 0x004D5CD3
class CTCDExplosion : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDExplosion) static unsigned long __fastcall StatGetDriverType() { return 27; } };   // Alloc 0x004E3EA8, name 0x004D5CE6
class CTCCarriedActionUseWallMount : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseWallMount) };   // Alloc 0x004DC512, name 0x004D2D5F
class CTCCarriedActionUseFishingRod : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseFishingRod) };   // Alloc 0x004DCC27, name 0x004D5CFD
class CTCCarriedActionUseSpade : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseSpade) };   // Alloc 0x004DCC60, name 0x004D5D10
class CTCResurrectionItem : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCResurrectionItem) };   // Alloc 0x004DCC99, name 0x004D5D99
class CTCKraken : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCKraken) };   // Alloc 0x004E13FE, name 0x004D5DC2
class CTCKrakenTentacle : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCKrakenTentacle) };   // Alloc 0x004DCCF2, name 0x004D5DEB
class CTCHeroSpecialMovement : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroSpecialMovement) };   // Alloc 0x004D5D23, name 0x004D5D40
class CTCInfoDisplay : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInfoDisplay) };   // Alloc 0x004D5E74, name 0x004D5E91
class CTCIdleScheduler : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCIdleScheduler) };   // Alloc 0x004E323C, name 0x004D5EA4
class CTCCarriedActionUseRead : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseRead) };   // Alloc 0x004DCD6B, name 0x004D5EB7
class CTCAmbientBird : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAmbientBird) };   // Alloc 0x004D92E8, name 0x004D5EE8
class CTCTextureDecal : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTextureDecal) };   // Alloc 0x004D5F35, name 0x004D5F52
class CTCFishingRod : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFishingRod) };   // Alloc 0x004D5F65, name 0x004D5F82
class CTCBoastUI : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBoastUI) };   // Alloc 0x004D5FAB, name 0x004D5FC8
class CTCThingUIDDummyForSerialisation : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCThingUIDDummyForSerialisation) };   // Alloc 0x004D3EC9, name 0x004D3EE6
class CTCDRumble : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDRumble) static unsigned long __fastcall StatGetDriverType() { return 28; } };   // Alloc 0x004DCDC4, name 0x004D5FF2
class CTCDPhysicalObstruction : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDPhysicalObstruction) static unsigned long __fastcall StatGetDriverType() { return 29; } };   // Alloc 0x004E15D3, name 0x004D5FDB
class CTCDiggingSpot : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDiggingSpot) };   // Alloc 0x004D6009, name 0x004D6026
class CTCDiceGame : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDiceGame) };   // Alloc 0x004DCE1D, name 0x004D6069
class CTCPlot : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPlot) };   // Alloc 0x004DCE3A, name 0x004D608F
class CTCSleep : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSleep) };   // Alloc 0x004D9367, name 0x004D60D2
class CTCFineDialogue : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFineDialogue) };   // Alloc 0x004D60A2, name 0x004D60BF
class CTCBob : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBob) };   // Alloc 0x004D48A1, name 0x004D48BE
class CTCSoundAtmosphereVillage : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSoundAtmosphereVillage) };   // Alloc 0x004DF1CC, name 0x004D6122
class CTCQuickAccessMenu : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCQuickAccessMenu) };   // Alloc 0x004D6135, name 0x004D6152
class CTCNymph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCNymph) };   // Alloc 0x004E3FEF, name 0x004D617B
class CTCAISummon : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAISummon) };   // Alloc 0x004E181D, name 0x004D61A4
class CTCWobble : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWobble) };   // Alloc 0x004D942C, name 0x004D61B7
class CTCCameraCollision : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraCollision) };   // Alloc 0x004D6209, name 0x004D6226
class CTCFadeOutAndIn : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFadeOutAndIn) };   // Alloc 0x004D624F, name 0x004D626C
class CTCFlashOnHit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFlashOnHit) };   // Alloc 0x004D94AB, name 0x004D627F
class CTCQuestCompletionUI : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCQuestCompletionUI) };   // Alloc 0x004D62ED, name 0x004D630D
class CTCCreditsUI : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreditsUI) };   // Alloc 0x004D64B6, name 0x004D64D3
class CTCDestroyThingOnActionFinish : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDestroyThingOnActionFinish) };   // Alloc 0x004D950E, name 0x004D6366
class CTCObstacle : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCObstacle) };   // Alloc 0x004D9580, name 0x004D63E8
class CTCFlourishTarget : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFlourishTarget) };   // Alloc 0x004DB49D, name 0x004D64A3
class CTCBetting : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBetting) };   // Alloc 0x004D64FC, name 0x004D6519
class CTCLifeTimer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCLifeTimer) };   // Alloc 0x004D652C, name 0x004D6549
class CTCFireRangedWeaponOnLeavingStateGroup : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFireRangedWeaponOnLeavingStateGroup) };   // Alloc 0x004D9723, name 0x004D655C
class CTCAvoidRegionExit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAvoidRegionExit) };   // Alloc 0x004DB4BA, name 0x004D65A9
class CTCOracleMinigame : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCOracleMinigame) };   // Alloc 0x004D65BC, name 0x004D65DC
class CTCFireheartMinigame : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFireheartMinigame) };   // Alloc 0x004D6605, name 0x004D6625
class CTCDestroyThingOnSpecialAbilityFinish : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDestroyThingOnSpecialAbilityFinish) };   // Alloc 0x004D9547, name 0x004D63A7
class CTCLightningOrb : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCLightningOrb) };   // Alloc 0x004DCEA4, name 0x004D6687
class CTCLightningNode : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCLightningNode) };   // Alloc 0x004E7666, name 0x004D6674
class CTCHasName : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHasName) };   // Alloc 0x004DCF15, name 0x004D66B0
class CTCHeroOnlineScoreboard : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroOnlineScoreboard) };   // Alloc 0x004D66DA, name 0x004D66F7
class CTCPhotojournalCapture : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhotojournalCapture) };   // Alloc 0x004D670A, name 0x004D6727
class CTCDExplosiveTrail : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDExplosiveTrail) static unsigned long __fastcall StatGetDriverType() { return 30; } };   // Alloc 0x004E400C, name 0x004D66C3
class CTCCarriedActionUseAbility : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseAbility) };   // Alloc 0x004DCF6E, name 0x004D673A
class CTCAbsorbHits : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAbsorbHits) };   // Alloc 0x004D990E, name 0x004D674D
class CTCGeneratedCreature : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGeneratedCreature) };   // Alloc 0x004DAF85, name 0x004D3780
class CTCCameraPointDefinitionBase : public CTCBase { public: static void __fastcall StatRegisterClass(); };   // StatRegisterClass 0x0075C0E0

class CHeroMorphDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroMorphDef) };   // Alloc 0x004E4219
class CHighlightItemDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHighlightItemDef) };   // Alloc 0x004D8671
class CSmokeGeneratorDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSmokeGeneratorDef) };   // Alloc 0x004DA82B
class CTimeAppearanceFadeDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTimeAppearanceFadeDef) };   // Alloc 0x004D84C8
class CCreatureNavigationDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCreatureNavigationDef) };   // Alloc 0x004DA871
class CInventoryItemDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CInventoryItemDef) };   // Alloc 0x0044F644
class CLookDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CLookDef) };   // Alloc 0x004D80E4
class CReadableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CReadableDef) };   // Alloc 0x004DAA0E
class CVillageDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CVillageDef) };   // Alloc 0x004E213B
class CVillageMemberDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CVillageMemberDef) };   // Alloc 0x004DA7AD
class CBuyableHouseDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBuyableHouseDef) };   // Alloc 0x004E0148
class CBuyHouseDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBuyHouseDef) };   // Alloc 0x004D7B5B
class CWifeDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CWifeDef) };   // Alloc 0x004D7BA1
class CDoorDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CDoorDef) };   // Alloc 0x004D7BE7
class CLightDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CLightDef) };   // Alloc 0x004D7C73
class CSpotLightDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpotLightDef) };   // Alloc 0x004D7CB9
class CClockDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CClockDef) };   // Alloc 0x004E4477
class CHeroDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroDef) };   // Alloc 0x004D7CFF
class CCreatureModeDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCreatureModeDef) };   // Alloc 0x004E0B4B
class CPerceivedThingDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CPerceivedThingDef) };   // Alloc 0x004D7EB6
class CBedDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBedDef) };   // Alloc 0x004DA7F3
class CStealthDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CStealthDef) };   // Alloc 0x004D7EFC
class CTrophyDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTrophyDef) };   // Alloc 0x004D7F7B
class CCreatureGeneratorDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCreatureGeneratorDef) };   // Alloc 0x004E0513
class CChestDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CChestDef) };   // Alloc 0x004D805C
class CExplodingObjectDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CExplodingObjectDef) };   // Alloc 0x004D809E
class CContainerRewardHeroDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CContainerRewardHeroDef) };   // Alloc 0x004E3C81
class CWeaponDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CWeaponDef) };   // Alloc 0x004E3D15
class CCarryingDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCarryingDef) };   // Alloc 0x004DFE62
class CCarryableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCarryableDef) };   // Alloc 0x004DA767
class CEnemyDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CEnemyDef) };   // Alloc 0x004D835A
class COpinionOfHeroDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COpinionOfHeroDef) };   // Alloc 0x004D83D9
class CShopDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CShopDef) };   // Alloc 0x004E26BC
class CStockItemDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CStockItemDef) };   // Alloc 0x004D8482
class CGiftDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CGiftDef) };   // Alloc 0x004D8547
class CHeroSuitDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroSuitDef) };   // Alloc 0x004E2809
class CHeroExperienceDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroExperienceDef) };   // Alloc 0x004EBAE7
class CExperienceDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CExperienceDef) };   // Alloc 0x004E27AE
class CReplaceableMeshDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CReplaceableMeshDef) };   // Alloc 0x004E60D8
class CMultiStaticMeshDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CMultiStaticMeshDef) };   // Alloc 0x004E31FA
class CHeroCentreDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroCentreDef) };   // Alloc 0x004D86F0
class CQuestCardDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CQuestCardDef) };   // Alloc 0x004E2333
class CFlammableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFlammableDef) };   // Alloc 0x004E3DC3
class CBoastingPodiumDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBoastingPodiumDef) };   // Alloc 0x004D8736
class CTCVolumeContainmentTrackerDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTCVolumeContainmentTrackerDef) };   // Alloc 0x004D94C8
class CThingDrainLifeShotDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CThingDrainLifeShotDef) };   // Alloc 0x004D8D56
class CFireballSpellLevelDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFireballSpellLevelDef) };   // Alloc 0x004D8D10
class CSkeletalMorphDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSkeletalMorphDef) };   // Alloc 0x004E3DD9
class CTrapDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTrapDef) };   // Alloc 0x004E5CF2
class CParticleAttacherDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CParticleAttacherDef) };   // Alloc 0x004E2AFA
class CAnimatingObjectDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CAnimatingObjectDef) };   // Alloc 0x004EBA6E
class CExpressionSubDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CExpressionSubDef) };   // Alloc 0x004D8818
class CWillResponseDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CWillResponseDef) };   // Alloc 0x004D9629
class CTurncoatDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTurncoatDef) };   // Alloc 0x004E0F9C
class CSummonableCreatureDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSummonableCreatureDef) };   // Alloc 0x004D885E
class CAIScratchpadDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CAIScratchpadDef) };   // Alloc 0x004D4E07
class COccupiableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COccupiableDef) };   // Alloc 0x004D88FC
class CBossDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBossDef) };   // Alloc 0x004E0D4C
class CFishingDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFishingDef) };   // Alloc 0x004E0DB9
class CGuardDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CGuardDef) };   // Alloc 0x004D89EC
class CInterestingToVillagersDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CInterestingToVillagersDef) };   // Alloc 0x004D89B4
class CActivateQuestDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CActivateQuestDef) };   // Alloc 0x004D8A32
class CCrateStackDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCrateStackDef) };   // Alloc 0x004D8A6A
class COverheadDisplayDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COverheadDisplayDef) };   // Alloc 0x004D8AB0
class CTavernTableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernTableDef) };   // Alloc 0x004D8AF6
class CTavernDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernDef) };   // Alloc 0x004D8BE1
class CObjectAugmentationsDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CObjectAugmentationsDef) };   // Alloc 0x004EC526
class CDrunkennessDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CDrunkennessDef) };   // Alloc 0x004D8C91
class CGoldDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CGoldDef) };   // Alloc 0x004D8EC5
class CAICreatureWillPowerIndicatorDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CAICreatureWillPowerIndicatorDef) };   // Alloc 0x004D926A
class CKickableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CKickableDef) };   // Alloc 0x004D7C2D
class CTavernGameDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernGameDef) };   // Alloc 0x004E2D3B
class CTavernGameCardBaseDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernGameCardBaseDef) };   // Alloc 0x004E2DB2
class CTavernGameCoinBaseDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernGameCoinBaseDef) };   // Alloc 0x004D8F51
class CTavernGameShoveHaPennyDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernGameShoveHaPennyDef) };   // Alloc 0x004E2D70
class CTavernGameCoinGolfDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernGameCoinGolfDef) };   // Alloc 0x004D8F97
class CTavernGameSpotTheAdditionDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernGameSpotTheAdditionDef) };   // Alloc 0x004E11C3
class CDecapitationDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CDecapitationDef) };   // Alloc 0x004D9047
class CCoinGameObstacleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCoinGameObstacleDef) };   // Alloc 0x004D8F0B
class CWallMountEffectsDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CWallMountEffectsDef) };   // Alloc 0x004D90C6
class CFishDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFishDef) };   // Alloc 0x004D910C
class CTeleporterDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTeleporterDef) };   // Alloc 0x004D9152
class CExplosionDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CExplosionDef) };   // Alloc 0x004E3096
class CResurrectionItemDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CResurrectionItemDef) };   // Alloc 0x004D91DE
class CKrakenDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CKrakenDef) };   // Alloc 0x004E13AD
class CKrakenTentacleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CKrakenTentacleDef) };   // Alloc 0x004D9224
class CHeroSpecialMovementDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroSpecialMovementDef) };   // Alloc 0x004D9198
class CIdleSchedulerDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CIdleSchedulerDef) };   // Alloc 0x004E6232
class CCarriedReadableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCarriedReadableDef) };   // Alloc 0x004D92B0
class CJackOfBladesBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CJackOfBladesBattleDef) };   // Alloc 0x004E4748
class CScorpionKingBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CScorpionKingBattleDef) };   // Alloc 0x004E47BF
class CThunderBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CThunderBattleDef) };   // Alloc 0x004E4624
class CWhisperBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CWhisperBattleDef) };   // Alloc 0x004E4667
class CWaspQueenBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CWaspQueenBattleDef) };   // Alloc 0x004E46A4
class CMazeBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CMazeBattleDef) };   // Alloc 0x004E45CE
class CTrollBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTrollBattleDef) };   // Alloc 0x004E4833
class CBalverineBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBalverineBattleDef) };   // Alloc 0x004E4883
class CAreaOfEffectAttackDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CAreaOfEffectAttackDef) };   // Alloc 0x004E6CF3
class CFishingRodDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFishingRodDef) };   // Alloc 0x004D9321
class CRumbleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CRumbleDef) };   // Alloc 0x004E3290
class CShipDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CShipDef) };   // Alloc 0x004D8799
class CShopItemDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CShopItemDef) };   // Alloc 0x004D8411
class CSoundAtmospheresDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSoundAtmospheresDef) };   // Alloc 0x004E32E3
class CNymphDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CNymphDef) };   // Alloc 0x004D93A0
class CSummonDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSummonDef) };   // Alloc 0x004D93E6
class CCameraCollisionDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCameraCollisionDef) };   // Alloc 0x004D9465
class CBettingDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBettingDef) };   // Alloc 0x004D96DD
class COracleMinigameDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COracleMinigameDef) };   // Alloc 0x004D97E9
class CFireheartMinigameDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFireheartMinigameDef) };   // Alloc 0x004D982F
class CLightningOrbDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CLightningOrbDef) };   // Alloc 0x004D9882
class CHasNameDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHasNameDef) };   // Alloc 0x004D98C8

// One component registration: the class-type name keys the registry entry and is stored in it too.
#define REGISTER_TC(TCClass) \
    { \
        CCharString name = TCClass::StatGetClassTypeName(); \
        RegisteredTCs.Insert(name, CRegisteredTCInfo(TCClass::Alloc, TCClass::StatGetDriverType(), name)); \
    }

// One definition-class registration with the game's definition manager, under the class's own name.
#define REGISTER_DEF_CLASS(DefClass) \
    CGameDefinitionManager::Get().AddDefClass(CDefClassInit(#DefClass, DefClass::Alloc))

void __fastcall CTCBase::RegisterTCs()
{
    RegisteredTCs.clear();

    // the component classes' own static setup
    CTCCreatureGenerator::StatRegisterClass();
    CTCCombat::StatRegisterClass();
    CTCHeroRangedAttacks::StatRegisterClass();
    CTCHaste::StatRegisterClass();
    CTCCreatureNavigation::StatRegisterClass();
    CTCCameraPointDefinitionBase::StatRegisterClass();
    NHitLocations::CHitLocation::StatRegisterClass();
    CTCTargetingPlayer::StatRegisterClass();

    // every component class, each followed by the definition classes that belong with it
    REGISTER_TC(CTCHeroMorph);
    REGISTER_DEF_CLASS(CHeroMorphDef);
    REGISTER_TC(CTCSimpleAppearanceMorph);
    REGISTER_TC(CTCAtmosPlayer);
    REGISTER_TC(CTCRandomAppearanceMorph);
    REGISTER_TC(CTCHighlightItem);
    REGISTER_DEF_CLASS(CHighlightItemDef);
    REGISTER_TC(CTCSmokeGenerator);
    REGISTER_DEF_CLASS(CSmokeGeneratorDef);
    REGISTER_TC(CTCTimeAppearanceFade);
    REGISTER_DEF_CLASS(CTimeAppearanceFadeDef);
    REGISTER_TC(CTCPhysicsLight);
    REGISTER_TC(CTCPhysicsStandard);
    REGISTER_TC(CTCPhysicsControlled);
    REGISTER_TC(CTCCreatureNavigation);
    REGISTER_DEF_CLASS(CCreatureNavigationDef);
    REGISTER_TC(CTCPhysicsNavigator);
    REGISTER_TC(CTCTargetingAI);
    REGISTER_TC(CTCTargetingPlayer);
    REGISTER_TC(CTCTargeted);
    REGISTER_TC(CTCSpecialAbilities);
    REGISTER_TC(CTCUndeadSoul);
    REGISTER_TC(CTCSoundPlayer);
    REGISTER_TC(CTCTalk);
    REGISTER_TC(CTCInventory);
    REGISTER_TC(CTCInventoryClothing);
    REGISTER_TC(CTCInventoryWeapons);
    REGISTER_TC(CTCInventoryAbilities);
    REGISTER_TC(CTCInventoryMagic);
    REGISTER_TC(CTCInventoryStats);
    REGISTER_TC(CTCInventoryExperience);
    REGISTER_TC(CTCInventoryTrade);
    REGISTER_TC(CTCInventoryQuests);
    REGISTER_TC(CTCInventoryMap);
    REGISTER_TC(CTCInventoryItem);
    REGISTER_DEF_CLASS(CInventoryItemDef);
    REGISTER_TC(CTCCreatureExpression);
    REGISTER_TC(CTCLook);
    REGISTER_DEF_CLASS(CLookDef);
    REGISTER_TC(CTCActionUseTorch);
    REGISTER_TC(CTCActionUseScriptedHook);
    REGISTER_TC(CTCActionUseSign);
    REGISTER_TC(CTCActionUseReadable);
    REGISTER_DEF_CLASS(CReadableDef);
    REGISTER_TC(CTCActionUsePickUp);
    REGISTER_TC(CTCActionUsePickUpGenericBox);
    REGISTER_TC(CTCActionUsePutInInventory);
    REGISTER_TC(CTCActionUseMapTable);
    REGISTER_TC(CTCActionUseSearch);
    REGISTER_TC(CTCActionUseBlowHorn);
    REGISTER_TC(CTCActionUseBuyOrSteal);
    REGISTER_TC(CTCActionUseWindow);
    REGISTER_TC(CTCActionUseAcceptBoast);
    REGISTER_TC(CTCActionUseStartExtrapolationMorph);
    REGISTER_TC(CTCActionUseBed);
    REGISTER_TC(CTCActionUseKickChicken);
    REGISTER_TC(CTCCarriedActionUseLamp);
    REGISTER_TC(CTCCarriedActionUseDrop);
    REGISTER_TC(CTCCarriedActionUseDropGenericBox);
    REGISTER_TC(CTCCarriedActionUseGive);
    REGISTER_TC(CTCCarriedActionUseGiveBeer);
    REGISTER_TC(CTCCarriedActionUseGoldBag);
    REGISTER_TC(CTCCarriedActionUseShow);
    REGISTER_TC(CTCCarriedActionUseTrophy);
    REGISTER_TC(CTCCarriedActionUsePotion);
    REGISTER_TC(CTCCarriedActionUseFood);
    REGISTER_TC(CTCCarriedActionUseFish);
    REGISTER_TC(CTCActionUseSwitch);
    REGISTER_TC(CTCScriptedControl);
    REGISTER_TC(CTCMeshAutomaticEntityCreator);
    REGISTER_TC(CTCThingOwner);
    REGISTER_TC(CTCVillage);
    REGISTER_DEF_CLASS(CVillageDef);
    REGISTER_TC(CTCVillageMember);
    REGISTER_DEF_CLASS(CVillageMemberDef);
    REGISTER_TC(CTCBuyableHouse);
    REGISTER_DEF_CLASS(CBuyableHouseDef);
    REGISTER_TC(CTCActionUseBuyHouse);
    REGISTER_DEF_CLASS(CBuyHouseDef);
    REGISTER_TC(CTCDummyVillager);
    REGISTER_TC(CTCWife);
    REGISTER_DEF_CLASS(CWifeDef);
    REGISTER_TC(CTCJackShield);
    REGISTER_TC(CTCJackPillar);
    REGISTER_TC(CTCDoor);
    REGISTER_DEF_CLASS(CDoorDef);
    REGISTER_TC(CTCLight);
    REGISTER_DEF_CLASS(CLightDef);
    REGISTER_TC(CTCSpotLight);
    REGISTER_DEF_CLASS(CSpotLightDef);
    REGISTER_TC(CTCClock);
    REGISTER_DEF_CLASS(CClockDef);
    REGISTER_TC(CTCHero);
    REGISTER_DEF_CLASS(CHeroDef);
    REGISTER_TC(CTCWeatherVane);
    REGISTER_TC(CTCSingleUserActionPoints);
    REGISTER_TC(CTCNamedUserActionPoints);
    REGISTER_TC(CTCExclusiveUse);
    REGISTER_TC(CTCRotate);
    REGISTER_TC(CTCCombat);
    REGISTER_TC(CTCCreatedEntity);
    REGISTER_TC(CTCOwnedEntity);
    REGISTER_TC(CTCCreatureModeManager);
    REGISTER_DEF_CLASS(CCreatureModeDef);
    REGISTER_TC(CTCPerceiveThing);
    REGISTER_DEF_CLASS(CPerceivedThingDef);
    REGISTER_TC(CTCBed);
    REGISTER_DEF_CLASS(CBedDef);
    REGISTER_TC(CTCStealth);
    REGISTER_DEF_CLASS(CStealthDef);
    REGISTER_TC(CTCNoise);
    REGISTER_TC(CTCCupboard);
    REGISTER_TC(CTCTrophy);
    REGISTER_DEF_CLASS(CTrophyDef);
    REGISTER_TC(CTCMapwho);
    REGISTER_TC(CTCCreatureGenerator);
    REGISTER_DEF_CLASS(CCreatureGeneratorDef);
    REGISTER_TC(CTCChest);
    REGISTER_DEF_CLASS(CChestDef);
    REGISTER_TC(CTCSearchableContainer);
    REGISTER_TC(CTCActionUseChest);
    REGISTER_TC(CTCDiscovery);
    REGISTER_TC(CTCExplodingObject);
    REGISTER_DEF_CLASS(CExplodingObjectDef);
    REGISTER_TC(CTCContainerRewardHero);
    REGISTER_DEF_CLASS(CContainerRewardHeroDef);
    REGISTER_TC(CTCEntityEvents);
    REGISTER_TC(CTCHeroStats);
    REGISTER_TC(CTCHeroRememberCrimes);
    REGISTER_TC(CTCWound);
    REGISTER_TC(CTCPhysicsRigidBody);
    REGISTER_TC(CTCDCameraPoint);
    REGISTER_TC(CTCCameraPointFixedPoint);
    REGISTER_TC(CTCCameraPointBuilding);
    REGISTER_TC(CTCCameraPointFixedDolly);
    REGISTER_TC(CTCCameraPointTrack);
    REGISTER_TC(CTCCameraPointGeneralCase);
    REGISTER_TC(CTCCameraPointDead);
    REGISTER_TC(CTCCameraPointScripted);
    REGISTER_TC(CTCCameraPointScriptedSpline);
    REGISTER_TC(CTCShapeManager);
    REGISTER_TC(CTCEmoteIcon);
    REGISTER_TC(CTCWeapon);
    REGISTER_DEF_CLASS(CWeaponDef);
    REGISTER_TC(CTCCarrying);
    REGISTER_DEF_CLASS(CCarryingDef);
    REGISTER_TC(CTCCarryable);
    REGISTER_DEF_CLASS(CCarryableDef);
    REGISTER_TC(CTCEnemy);
    REGISTER_DEF_CLASS(CEnemyDef);
    REGISTER_TC(CTCBandit);
    REGISTER_TC(CTCDParticleEmitter);
    REGISTER_TC(CTCWeaponTrail);
    REGISTER_TC(CTCSolidToThing);
    REGISTER_TC(CTCSmashableWindow);
    REGISTER_TC(CTCSmashableBarrier);
    REGISTER_TC(CTCDegradableBarrier);
    REGISTER_TC(CTCHeroOpinionDeedLog);
    REGISTER_TC(CTCCreatureOpinionOfHero);
    REGISTER_DEF_CLASS(COpinionOfHeroDef);
    REGISTER_TC(CTCOpinionSource);
    REGISTER_TC(CTCHeroPodium);
    REGISTER_TC(CTCShop);
    REGISTER_DEF_CLASS(CShopDef);
    REGISTER_TC(CTCBlacksmiths);
    REGISTER_TC(CTCStockItem);
    REGISTER_DEF_CLASS(CStockItemDef);
    REGISTER_TC(CTCGift);
    REGISTER_DEF_CLASS(CGiftDef);
    REGISTER_TC(CTCFamilyMember);
    REGISTER_TC(CTCDNavigationSeed);
    REGISTER_TC(CTCDHeroSuit);
    REGISTER_DEF_CLASS(CHeroSuitDef);
    REGISTER_TC(CTCDNone);
    REGISTER_TC(CTCDInternalMarker);
    REGISTER_TC(CTCDRegionEntrance);
    REGISTER_TC(CTCDRegionExit);
    REGISTER_TC(CTCActivationReceptorChest);
    REGISTER_TC(CTCActivationReceptorCreatureGenerator);
    REGISTER_TC(CTCActivationReceptorDoor);
    REGISTER_TC(CTCActivationReceptorRelay);
    REGISTER_TC(CTCActivationTrigger);
    REGISTER_TC(CTCHeroExperience);
    REGISTER_DEF_CLASS(CHeroExperienceDef);
    REGISTER_TC(CTCDExperienceOrb);
    REGISTER_DEF_CLASS(CExperienceDef);
    REGISTER_TC(NGraphicAppearance::CTCGraphicAppearance);
    REGISTER_TC(NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh);
    REGISTER_TC(NGraphicAppearance::CTCGraphicAppearanceStaticMesh);
    REGISTER_TC(NGraphicAppearance::CTCGraphicAppearanceStaticMeshFadeOutOnProximity);
    REGISTER_TC(NGraphicAppearance::CTCGraphicAppearanceSprite);
    REGISTER_TC(NGraphicAppearance::CTCGraphicAppearanceReplaceableMesh);
    REGISTER_DEF_CLASS(CReplaceableMeshDef);
    REGISTER_TC(NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes);
    REGISTER_DEF_CLASS(CMultiStaticMeshDef);
    REGISTER_TC(CTCAnimationComplex);
    REGISTER_TC(CTCHeroAttachableAppearanceModifiers);
    REGISTER_TC(CTCCreatureItemLevitationHero);
    REGISTER_TC(CTCCreatureItemLevitationNymph);
    REGISTER_TC(CTCCreatureHive);
    REGISTER_TC(CTCHeroCentre);
    REGISTER_DEF_CLASS(CHeroCentreDef);
    REGISTER_TC(CTCHeroCentreDoorMarker);
    REGISTER_TC(CTCHeroGuide);
    REGISTER_TC(CTCQuestCard);
    REGISTER_DEF_CLASS(CQuestCardDef);
    REGISTER_TC(CTCFlammable);
    REGISTER_DEF_CLASS(CFlammableDef);
    REGISTER_TC(CTCBoastingPodium);
    REGISTER_DEF_CLASS(CBoastingPodiumDef);
    REGISTER_TC(CTCBoastingPosition);
    REGISTER_TC(CTCBoastingArea);
    REGISTER_TC(CTCBoastingCrowdControl);
    REGISTER_TC(CTCCreatureGroupBoastingCrowd);
    REGISTER_TC(CTCVolumeContainmentTracker);
    REGISTER_DEF_CLASS(CTCVolumeContainmentTrackerDef);
    REGISTER_TC(CTCDeadShotInThingTracker);
    REGISTER_TC(NThingShot::CTCShotDefinitionNymphFun);
    REGISTER_TC(NThingShot::CTCShotDefinitionQueenHornetShot);
    REGISTER_TC(NThingShot::CTCShotDefinitionSpreadShotProjectile);
    REGISTER_TC(NThingShot::CTCShotDefinitionProjectile);
    REGISTER_TC(NThingShot::CTCShotDefinitionThrow);
    REGISTER_TC(NThingShot::CTCShotDefinitionThrowPredictive);
    REGISTER_TC(NThingShot::CTCShotDefinitionProjectilePredictive);
    REGISTER_TC(NThingShot::CTCShotDefinitionTearInGround);
    REGISTER_TC(NThingShot::CTCShotDefinitionForcePushBall);
    REGISTER_TC(NThingShot::CTCShotDefinitionDragonFireball);
    REGISTER_TC(NThingShot::CTCShotDefinitionMultiArrow);
    REGISTER_TC(NThingShot::CTCShotDefinitionDrainLife);
    REGISTER_DEF_CLASS(CThingDrainLifeShotDef);
    REGISTER_TC(NThingShot::CTCShotDefinitionFireballSpell);
    REGISTER_DEF_CLASS(CFireballSpellLevelDef);
    REGISTER_TC(CTCBurntEffect);
    REGISTER_TC(CTCElectrocutedEffect);
    REGISTER_TC(CTCCreatureTintEffect);
    REGISTER_TC(CTCSkeletalMorph);
    REGISTER_DEF_CLASS(CSkeletalMorphDef);
    REGISTER_TC(CTCGatherPointBuilding);
    REGISTER_TC(CTCGatherPointRadialArea);
    REGISTER_TC(CTCSwitchableNavigation);
    REGISTER_TC(CTCRegionFollower);
    REGISTER_TC(CTCDayOrNightOnlySupport);
    REGISTER_TC(CTCMeleeTrap);
    REGISTER_DEF_CLASS(CTrapDef);
    REGISTER_TC(CTCProjectileTrap);
    REGISTER_TC(CTCHitLocations);
    REGISTER_TC(CTCMoneyReceiver);
    REGISTER_TC(CTCHeroRangedAttacks);
    REGISTER_TC(CTCParticleAttacher);
    REGISTER_DEF_CLASS(CParticleAttacherDef);
    REGISTER_TC(CTCAnimatingObject);
    REGISTER_DEF_CLASS(CAnimatingObjectDef);
    REGISTER_TC(CTCBerserk);
    REGISTER_TC(CTCDivineWrath);
    REGISTER_TC(CTCUnholyPower);
    REGISTER_TC(CTCHaste);
    REGISTER_TC(CTCForcePushed);
    REGISTER_TC(CTCRegionDisplay);
    REGISTER_TC(CTCExpression);
    REGISTER_DEF_CLASS(CExpressionSubDef);
    REGISTER_TC(CTCCarriedActionUseExpression);
    REGISTER_TC(CTCMultiStrike);
    REGISTER_TC(CTCMultiArrow);
    REGISTER_TC(CTCDrainLife);
    REGISTER_TC(CTCHealLife);
    REGISTER_TC(CTCGhostSword);
    REGISTER_TC(CTCEffectOnDie);
    REGISTER_TC(CTCWillResponse);
    REGISTER_DEF_CLASS(CWillResponseDef);
    REGISTER_TC(CTCFireballSpell);
    REGISTER_TC(CTCTurncoatSpell);
    REGISTER_TC(CTCTurncoat);
    REGISTER_DEF_CLASS(CTurncoatDef);
    REGISTER_TC(CTCSummonableCreature);
    REGISTER_DEF_CLASS(CSummonableCreatureDef);
    REGISTER_TC(CTCShopKeeper);
    REGISTER_TC(CTCSummonSpell);
    REGISTER_TC(CTCBulletTime);
    REGISTER_TC(CTCPhysicalShieldSpell);
    REGISTER_TC(CTCWillLightning);
    REGISTER_TC(CTCWillEnflame);
    REGISTER_TC(CTCForcePushPower);
    REGISTER_TC(CTCPreCalculatedNavigationRoute);
    REGISTER_TC(CTCCreatureGroup);
    REGISTER_TC(CTCBattleCharge);
    REGISTER_TC(CTCAssassinRush);
    REGISTER_TC(CTCCreatureGroupTag);
    REGISTER_TC(CTCAIScratchpad);
    REGISTER_DEF_CLASS(CAIScratchpadDef);
    REGISTER_TC(CTCSinglePersonOccupiable);
    REGISTER_DEF_CLASS(COccupiableDef);
    REGISTER_TC(CTCBoss);
    REGISTER_DEF_CLASS(CBossDef);
    REGISTER_TC(CTCRockTrollShield);
    REGISTER_TC(CTCCombatLeader);
    REGISTER_TC(CTCFishingSpot);
    REGISTER_TC(CTCFishing);
    REGISTER_DEF_CLASS(CFishingDef);
    REGISTER_TC(CTCStealableItemLocation);
    REGISTER_TC(CTCGuard);
    REGISTER_DEF_CLASS(CGuardDef);
    REGISTER_TC(CTCInterestingToVillagers);
    REGISTER_DEF_CLASS(CInterestingToVillagersDef);
    REGISTER_TC(CTCCarriedActionUseActivateQuest);
    REGISTER_DEF_CLASS(CActivateQuestDef);
    REGISTER_TC(CTCWaterWader);
    REGISTER_TC(CTCCrateStack);
    REGISTER_DEF_CLASS(CCrateStackDef);
    REGISTER_TC(CTCOverheadDisplay);
    REGISTER_DEF_CLASS(COverheadDisplayDef);
    REGISTER_TC(CTCTavernTable);
    REGISTER_DEF_CLASS(CTavernTableDef);
    REGISTER_TC(CTCTavernBar);
    REGISTER_TC(CTCTavernTankard);
    REGISTER_TC(CTCTavernJug);
    REGISTER_TC(CTCTavern);
    REGISTER_DEF_CLASS(CTavernDef);
    REGISTER_TC(CTCObjectAugmentations);
    REGISTER_DEF_CLASS(CObjectAugmentationsDef);
    REGISTER_TC(CTCFollowed);
    REGISTER_TC(CTCTrespasser);
    REGISTER_TC(CTCTrespassable);
    REGISTER_TC(CTCGossip);
    REGISTER_TC(CTCHitOnCollision);
    REGISTER_TC(CTCDrunkenness);
    REGISTER_DEF_CLASS(CDrunkennessDef);
    REGISTER_TC(CTCOnHeroPush);
    REGISTER_TC(CTCInGameMenu);
    REGISTER_TC(CTCElectrocuteOnTouch);
    REGISTER_TC(CTCCoopSpirit);
    REGISTER_TC(CTCGeneratesExperienceOnKilling);
    REGISTER_TC(CTCCreatureGeneratorCreator);
    REGISTER_TC(CTCHeroReceiveItems);
    REGISTER_TC(CTCActionUseActivateQuest);
    REGISTER_TC(CTCGold);
    REGISTER_DEF_CLASS(CGoldDef);
    REGISTER_TC(CTCTargetingSpirit);
    REGISTER_TC(CTCCreatureHitNotification);
    REGISTER_TC(CTCAICreatureWillPowerIndicator);
    REGISTER_DEF_CLASS(CAICreatureWillPowerIndicatorDef);
    REGISTER_DEF_CLASS(CKickableDef);
    REGISTER_DEF_CLASS(CTavernGameDef);
    REGISTER_DEF_CLASS(CTavernGameCardBaseDef);
    REGISTER_DEF_CLASS(CTavernGameCoinBaseDef);
    REGISTER_TC(CTCTavernGameShoveHaPenny);
    REGISTER_DEF_CLASS(CTavernGameShoveHaPennyDef);
    REGISTER_TC(CTCTavernGameCoinGolf);
    REGISTER_DEF_CLASS(CTavernGameCoinGolfDef);
    REGISTER_TC(CTCTavernGameCardSorting);
    REGISTER_TC(CTCTavernGameCardPairs);
    REGISTER_TC(CTCTavernGameSpotTheAddition);
    REGISTER_DEF_CLASS(CTavernGameSpotTheAdditionDef);
    REGISTER_TC(CTCTavernGameCardBlackjack);
    REGISTER_TC(CTCGamePlayer);
    REGISTER_TC(CTCDecapitation);
    REGISTER_DEF_CLASS(CDecapitationDef);
    REGISTER_TC(CTCTavernGameMaster);
    REGISTER_DEF_CLASS(CCoinGameObstacleDef);
    REGISTER_TC(CTCParticleAttachmentTracker);
    REGISTER_TC(CTCCreatureFlyOnSpline);
    REGISTER_TC(CTCCreatureFlyInCircles);
    REGISTER_TC(CTCWallMount);
    REGISTER_TC(CTCWallMountEffects);
    REGISTER_DEF_CLASS(CWallMountEffectsDef);
    REGISTER_TC(CTCFish);
    REGISTER_DEF_CLASS(CFishDef);
    REGISTER_TC(CTCTeleporter);
    REGISTER_DEF_CLASS(CTeleporterDef);
    REGISTER_TC(CTCActionUseWallMount);
    REGISTER_TC(CTCDExplosion);
    REGISTER_DEF_CLASS(CExplosionDef);
    REGISTER_TC(CTCCarriedActionUseWallMount);
    REGISTER_TC(CTCCarriedActionUseFishingRod);
    REGISTER_TC(CTCCarriedActionUseSpade);
    REGISTER_TC(CTCResurrectionItem);
    REGISTER_DEF_CLASS(CResurrectionItemDef);
    REGISTER_TC(CTCKraken);
    REGISTER_DEF_CLASS(CKrakenDef);
    REGISTER_TC(CTCKrakenTentacle);
    REGISTER_DEF_CLASS(CKrakenTentacleDef);
    REGISTER_TC(CTCHeroSpecialMovement);
    REGISTER_DEF_CLASS(CHeroSpecialMovementDef);
    REGISTER_TC(CTCInfoDisplay);
    REGISTER_TC(CTCIdleScheduler);
    REGISTER_DEF_CLASS(CIdleSchedulerDef);
    REGISTER_TC(CTCCarriedActionUseRead);
    REGISTER_DEF_CLASS(CCarriedReadableDef);
    REGISTER_TC(CTCAmbientBird);
    REGISTER_DEF_CLASS(CJackOfBladesBattleDef);
    REGISTER_DEF_CLASS(CScorpionKingBattleDef);
    REGISTER_DEF_CLASS(CThunderBattleDef);
    REGISTER_DEF_CLASS(CWhisperBattleDef);
    REGISTER_DEF_CLASS(CWaspQueenBattleDef);
    REGISTER_DEF_CLASS(CMazeBattleDef);
    REGISTER_DEF_CLASS(CTrollBattleDef);
    REGISTER_DEF_CLASS(CBalverineBattleDef);
    REGISTER_TC(CTCTextureDecal);
    REGISTER_DEF_CLASS(CAreaOfEffectAttackDef);
    REGISTER_TC(CTCFishingRod);
    REGISTER_DEF_CLASS(CFishingRodDef);
    REGISTER_TC(CTCBoastUI);
    REGISTER_TC(CTCThingUIDDummyForSerialisation);
    REGISTER_TC(CTCDRumble);
    REGISTER_DEF_CLASS(CRumbleDef);
    REGISTER_TC(CTCDPhysicalObstruction);
    REGISTER_TC(CTCDiggingSpot);
    REGISTER_TC(CTCDiceGame);
    REGISTER_TC(CTCPlot);
    REGISTER_TC(CTCSleep);
    REGISTER_TC(CTCFineDialogue);
    REGISTER_TC(CTCBob);
    REGISTER_DEF_CLASS(CShipDef);
    REGISTER_DEF_CLASS(CShopItemDef);
    REGISTER_DEF_CLASS(CSoundAtmospheresDef);
    REGISTER_TC(CTCSoundAtmosphereVillage);
    REGISTER_TC(CTCQuickAccessMenu);
    REGISTER_TC(CTCNymph);
    REGISTER_DEF_CLASS(CNymphDef);
    REGISTER_TC(CTCAISummon);
    REGISTER_DEF_CLASS(CSummonDef);
    REGISTER_TC(CTCWobble);
    REGISTER_TC(CTCCameraCollision);
    REGISTER_DEF_CLASS(CCameraCollisionDef);
    REGISTER_TC(CTCFadeOutAndIn);
    REGISTER_TC(CTCFlashOnHit);
    REGISTER_TC(CTCQuestCompletionUI);
    REGISTER_TC(CTCCreditsUI);
    REGISTER_TC(CTCDestroyThingOnActionFinish);
    REGISTER_TC(CTCObstacle);
    REGISTER_TC(CTCFlourishTarget);
    REGISTER_TC(CTCBetting);
    REGISTER_DEF_CLASS(CBettingDef);
    REGISTER_TC(CTCLifeTimer);
    REGISTER_TC(CTCFireRangedWeaponOnLeavingStateGroup);
    REGISTER_TC(CTCAvoidRegionExit);
    REGISTER_TC(CTCOracleMinigame);
    REGISTER_DEF_CLASS(COracleMinigameDef);
    REGISTER_TC(CTCFireheartMinigame);
    REGISTER_DEF_CLASS(CFireheartMinigameDef);
    REGISTER_TC(CTCDestroyThingOnSpecialAbilityFinish);
    REGISTER_TC(CTCLightningOrb);
    REGISTER_DEF_CLASS(CLightningOrbDef);
    REGISTER_TC(CTCLightningNode);
    REGISTER_TC(CTCHasName);
    REGISTER_DEF_CLASS(CHasNameDef);
    REGISTER_TC(CTCHeroOnlineScoreboard);
    REGISTER_TC(CTCPhotojournalCapture);
    REGISTER_TC(CTCDExplosiveTrail);
    REGISTER_TC(CTCCarriedActionUseAbility);
    REGISTER_TC(CTCAbsorbHits);
    REGISTER_TC(CTCGeneratedCreature);

    CTCCreatureModeManager::RegisterModes();
    RegisteredTCs.Update();
}