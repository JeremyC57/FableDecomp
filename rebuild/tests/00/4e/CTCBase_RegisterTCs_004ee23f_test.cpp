// Behaviour test for CTCBase::RegisterTCs (Fable.exe 0x004EE23F): the same registration code over a portable model of the registry
// (CVectorMap<CCharString, CTCBase::CRegisteredTCInfo>: clear / Insert = push_back(value_type(key, value)) + Dirty / Update = Sort
// when Dirty), the definition manager and the classes' static entry points. Checks: the eight static setups run first and in retail
// order; the registry is emptied first, then holds the 340 retail component classes sorted by name, each entry storing its name, its
// class's allocator (which builds a component of that class) and its driver type (CTCBase's 0, or the 13 drivers' own constants);
// the 111 definition classes reach AddDefClass in retail order, interleaved after the right component, each with its own allocator;
// RegisterModes runs after the last registration and before the sort; a second run rebuilds the same registry.
// Plain C++98 (VC7.1 /Od and g++).
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif

class CThing { };

// CCharString model: a fixed buffer is enough for the class names (the longest is 48 characters).
class CCharString {
public:
    CCharString(const char* text) { std::strcpy(Text, text); }
    CCharString(const CCharString& other) { std::strcpy(Text, other.Text); }
    CCharString& operator=(const CCharString& other) { std::strcpy(Text, other.Text); return *this; }
    bool operator<(const CCharString& other) const { return std::strcmp(Text, other.Text) < 0; }   // CBasicString<char>::operator< order
    bool operator==(const char* text) const { return std::strcmp(Text, text) == 0; }
    const char* GetAsCharArray() const { return Text; }
private:
    char Text[64];
};

// The setup calls RegisterTCs makes, in order, and what RegisterModes saw.
static const char* SetupCalls[32];
static int NumSetupCalls = 0;
static void NoteSetupCall(const char* name) { if (NumSetupCalls < 32) SetupCalls[NumSetupCalls++] = name; }

class CTCBase;
static CTCBase* NewTC(const char* className, CThing& owner);

class CTCBase {
public:
    class CRegisteredTCInfo {
    public:
        CRegisteredTCInfo(CTCBase* (__fastcall* allocFunc)(CThing&), unsigned long driverType, const CCharString& name)
            : AllocFunc(allocFunc), DriverType(driverType), Name(name) { }
        CTCBase*      (__fastcall* AllocFunc)(CThing&);
        unsigned long DriverType;
        CCharString   Name;
    };
    // CVectorMap<CCharString, CRegisteredTCInfo> model (a sorted vector of pairs with a Dirty flag).
    class CRegistry {
    public:
        struct value_type {
            value_type(const CCharString& key, const CRegisteredTCInfo& value) : first(key), second(value) { }
            CCharString first;
            CRegisteredTCInfo second;
        };
        CRegistry() : Count(0), Dirty(false) { }
        void clear() { for (int i = 0; i < Count; ++i) delete Data[i]; Count = 0; }
        void push_back(const value_type& x) { if (Count < 512) { Data[Count] = new value_type(x); ++Count; } }
        void Insert(const CCharString& key, const CRegisteredTCInfo& value) { push_back(value_type(key, value)); Dirty = true; }
        void Sort() {
            for (int i = 1; i < Count; ++i)
                for (int j = i; j > 0 && Data[j]->first < Data[j - 1]->first; --j) { value_type* t = Data[j]; Data[j] = Data[j - 1]; Data[j - 1] = t; }
            Dirty = false;
        }
        void Update() { if (Dirty) Sort(); }
        value_type* Data[512];
        int Count;
        bool Dirty;
    };
    CTCBase() : ClassName(0) { }
    static unsigned long __fastcall StatGetDriverType() { return 0; }
    static void __fastcall RegisterTCs();
    static CRegistry RegisteredTCs;
    const char* ClassName;                 // test only: the class whose Alloc built this component
};
CTCBase::CRegistry CTCBase::RegisteredTCs;

static CTCBase TCPool[1024];
static int NumTCs = 0;
static CTCBase* NewTC(const char* className, CThing&) { CTCBase* tc = &TCPool[NumTCs++ % 1024]; tc->ClassName = className; return tc; }

class CDefClassBase {
public:
    const char* ClassName;
};
static CDefClassBase DefPool[512];
static int NumDefs = 0;
static CDefClassBase* NewDef(const char* className) { CDefClassBase* def = &DefPool[NumDefs++ % 512]; def->ClassName = className; return def; }

class CDefClassInit {
public:
    typedef CDefClassBase* (__fastcall* TAllocFunc)();
    CDefClassInit(const CCharString& name, TAllocFunc allocFunc) : Name(name), AllocFunc(allocFunc) { }
    CCharString Name;
    TAllocFunc  AllocFunc;
};

// The definition manager records each AddDefClass and how many components were registered before it.
class CGameDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get() { static CGameDefinitionManager manager; return manager; }
    CGameDefinitionManager() : Count(0) { }
    void AddDefClass(const CDefClassInit& init) {
        if (Count < 512) { Names[Count] = new CCharString(init.Name); Allocs[Count] = init.AllocFunc; TCsBefore[Count] = CTCBase::RegisteredTCs.Count; ++Count; }
    }
    CCharString* Names[512];
    CDefClassInit::TAllocFunc Allocs[512];
    int TCsBefore[512];
    int Count;
};

#define TC_REGISTRATION_INTERFACE(TCClass) \
    static CTCBase* __fastcall Alloc(CThing& owner) { return NewTC(#TCClass, owner); } \
    static CCharString __fastcall StatGetClassTypeName() { return #TCClass; }
#define DEF_CLASS_REGISTRATION_INTERFACE(DefClass) \
    static CDefClassBase* __fastcall Alloc() { return NewDef(#DefClass); }

static int ModesSawCount = -1;
static bool ModesSawDirty = false;
class CTCHeroMorph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroMorph) };
class CTCSimpleAppearanceMorph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSimpleAppearanceMorph) };
class CTCAtmosPlayer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAtmosPlayer) };
class CTCRandomAppearanceMorph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCRandomAppearanceMorph) };
class CTCHighlightItem : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHighlightItem) };
class CTCSmokeGenerator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSmokeGenerator) };
class CTCTimeAppearanceFade : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTimeAppearanceFade) };
class CTCPhysicsLight : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhysicsLight) };
class CTCPhysicsStandard : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhysicsStandard) };
class CTCPhysicsControlled : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhysicsControlled) };
class CTCCreatureNavigation : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureNavigation) static void __fastcall StatRegisterClass() { NoteSetupCall("CTCCreatureNavigation::StatRegisterClass"); } };
class CTCPhysicsNavigator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhysicsNavigator) };
class CTCTargetingAI : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTargetingAI) };
class CTCTargetingPlayer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTargetingPlayer) static void __fastcall StatRegisterClass() { NoteSetupCall("CTCTargetingPlayer::StatRegisterClass"); } };
class CTCTargeted : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTargeted) };
class CTCSpecialAbilities : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSpecialAbilities) };
class CTCUndeadSoul : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCUndeadSoul) };
class CTCSoundPlayer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSoundPlayer) };
class CTCTalk : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTalk) };
class CTCInventory : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventory) };
class CTCInventoryClothing : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryClothing) };
class CTCInventoryWeapons : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryWeapons) };
class CTCInventoryAbilities : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryAbilities) };
class CTCInventoryMagic : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryMagic) };
class CTCInventoryStats : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryStats) };
class CTCInventoryExperience : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryExperience) };
class CTCInventoryTrade : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryTrade) };
class CTCInventoryQuests : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryQuests) };
class CTCInventoryMap : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryMap) };
class CTCInventoryItem : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInventoryItem) };
class CTCCreatureExpression : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureExpression) };
class CTCLook : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCLook) };
class CTCActionUseTorch : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseTorch) };
class CTCActionUseScriptedHook : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseScriptedHook) };
class CTCActionUseSign : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseSign) };
class CTCActionUseReadable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseReadable) };
class CTCActionUsePickUp : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUsePickUp) };
class CTCActionUsePickUpGenericBox : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUsePickUpGenericBox) };
class CTCActionUsePutInInventory : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUsePutInInventory) };
class CTCActionUseMapTable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseMapTable) };
class CTCActionUseSearch : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseSearch) };
class CTCActionUseBlowHorn : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseBlowHorn) };
class CTCActionUseBuyOrSteal : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseBuyOrSteal) };
class CTCActionUseWindow : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseWindow) };
class CTCActionUseAcceptBoast : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseAcceptBoast) };
class CTCActionUseStartExtrapolationMorph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseStartExtrapolationMorph) };
class CTCActionUseBed : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseBed) };
class CTCActionUseKickChicken : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseKickChicken) };
class CTCCarriedActionUseLamp : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseLamp) };
class CTCCarriedActionUseDrop : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseDrop) };
class CTCCarriedActionUseDropGenericBox : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseDropGenericBox) };
class CTCCarriedActionUseGive : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseGive) };
class CTCCarriedActionUseGiveBeer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseGiveBeer) };
class CTCCarriedActionUseGoldBag : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseGoldBag) };
class CTCCarriedActionUseShow : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseShow) };
class CTCCarriedActionUseTrophy : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseTrophy) };
class CTCCarriedActionUsePotion : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUsePotion) };
class CTCCarriedActionUseFood : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseFood) };
class CTCCarriedActionUseFish : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseFish) };
class CTCActionUseSwitch : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseSwitch) };
class CTCScriptedControl : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCScriptedControl) };
class CTCMeshAutomaticEntityCreator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCMeshAutomaticEntityCreator) };
class CTCThingOwner : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCThingOwner) };
class CTCVillage : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCVillage) };
class CTCVillageMember : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCVillageMember) };
class CTCBuyableHouse : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBuyableHouse) };
class CTCActionUseBuyHouse : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseBuyHouse) };
class CTCDummyVillager : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDummyVillager) };
class CTCWife : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWife) };
class CTCJackShield : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCJackShield) };
class CTCJackPillar : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCJackPillar) };
class CTCDoor : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDoor) };
class CTCLight : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCLight) };
class CTCSpotLight : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSpotLight) };
class CTCClock : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCClock) };
class CTCHero : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHero) };
class CTCWeatherVane : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWeatherVane) };
class CTCSingleUserActionPoints : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSingleUserActionPoints) };
class CTCNamedUserActionPoints : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCNamedUserActionPoints) };
class CTCExclusiveUse : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCExclusiveUse) };
class CTCRotate : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCRotate) };
class CTCCombat : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCombat) static void __fastcall StatRegisterClass() { NoteSetupCall("CTCCombat::StatRegisterClass"); } };
class CTCCreatedEntity : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatedEntity) };
class CTCOwnedEntity : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCOwnedEntity) };
class CTCCreatureModeManager : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureModeManager) static void __fastcall RegisterModes() { NoteSetupCall("CTCCreatureModeManager::RegisterModes"); ModesSawCount = RegisteredTCs.Count; ModesSawDirty = RegisteredTCs.Dirty; } };
class CTCPerceiveThing : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPerceiveThing) };
class CTCBed : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBed) };
class CTCStealth : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCStealth) };
class CTCNoise : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCNoise) };
class CTCCupboard : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCupboard) };
class CTCTrophy : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTrophy) };
class CTCMapwho : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCMapwho) };
class CTCCreatureGenerator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureGenerator) static void __fastcall StatRegisterClass() { NoteSetupCall("CTCCreatureGenerator::StatRegisterClass"); } };
class CTCChest : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCChest) };
class CTCSearchableContainer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSearchableContainer) };
class CTCActionUseChest : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseChest) };
class CTCDiscovery : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDiscovery) };
class CTCExplodingObject : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCExplodingObject) };
class CTCContainerRewardHero : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCContainerRewardHero) };
class CTCEntityEvents : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCEntityEvents) };
class CTCHeroStats : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroStats) };
class CTCHeroRememberCrimes : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroRememberCrimes) };
class CTCWound : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWound) };
class CTCPhysicsRigidBody : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhysicsRigidBody) };
class CTCDCameraPoint : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDCameraPoint) static unsigned long __fastcall StatGetDriverType() { return 20; } };
class CTCCameraPointFixedPoint : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointFixedPoint) };
class CTCCameraPointBuilding : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointBuilding) };
class CTCCameraPointFixedDolly : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointFixedDolly) };
class CTCCameraPointTrack : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointTrack) };
class CTCCameraPointGeneralCase : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointGeneralCase) };
class CTCCameraPointDead : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointDead) };
class CTCCameraPointScripted : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointScripted) };
class CTCCameraPointScriptedSpline : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraPointScriptedSpline) };
class CTCShapeManager : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShapeManager) };
class CTCEmoteIcon : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCEmoteIcon) };
class CTCWeapon : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWeapon) };
class CTCCarrying : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarrying) };
class CTCCarryable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarryable) };
class CTCEnemy : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCEnemy) };
class CTCBandit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBandit) };
class CTCDParticleEmitter : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDParticleEmitter) static unsigned long __fastcall StatGetDriverType() { return 19; } };
class CTCWeaponTrail : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWeaponTrail) };
class CTCSolidToThing : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSolidToThing) };
class CTCSmashableWindow : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSmashableWindow) };
class CTCSmashableBarrier : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSmashableBarrier) };
class CTCDegradableBarrier : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDegradableBarrier) };
class CTCHeroOpinionDeedLog : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroOpinionDeedLog) };
class CTCCreatureOpinionOfHero : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureOpinionOfHero) };
class CTCOpinionSource : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCOpinionSource) };
class CTCHeroPodium : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroPodium) };
class CTCShop : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShop) };
class CTCBlacksmiths : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBlacksmiths) };
class CTCStockItem : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCStockItem) };
class CTCGift : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGift) };
class CTCFamilyMember : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFamilyMember) };
class CTCDNavigationSeed : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDNavigationSeed) static unsigned long __fastcall StatGetDriverType() { return 21; } };
class CTCDHeroSuit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDHeroSuit) static unsigned long __fastcall StatGetDriverType() { return 22; } };
class CTCDNone : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDNone) static unsigned long __fastcall StatGetDriverType() { return 23; } };
class CTCDInternalMarker : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDInternalMarker) static unsigned long __fastcall StatGetDriverType() { return 12; } };
class CTCDRegionEntrance : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDRegionEntrance) static unsigned long __fastcall StatGetDriverType() { return 24; } };
class CTCDRegionExit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDRegionExit) static unsigned long __fastcall StatGetDriverType() { return 25; } };
class CTCActivationReceptorChest : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActivationReceptorChest) };
class CTCActivationReceptorCreatureGenerator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActivationReceptorCreatureGenerator) };
class CTCActivationReceptorDoor : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActivationReceptorDoor) };
class CTCActivationReceptorRelay : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActivationReceptorRelay) };
class CTCActivationTrigger : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActivationTrigger) };
class CTCHeroExperience : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroExperience) };
class CTCDExperienceOrb : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDExperienceOrb) static unsigned long __fastcall StatGetDriverType() { return 26; } };
namespace NGraphicAppearance {
class CTCGraphicAppearance : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearance) };
class CTCGraphicAppearanceAnimatedMesh : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearanceAnimatedMesh) };
class CTCGraphicAppearanceStaticMesh : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearanceStaticMesh) };
class CTCGraphicAppearanceStaticMeshFadeOutOnProximity : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearanceStaticMeshFadeOutOnProximity) };
class CTCGraphicAppearanceSprite : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearanceSprite) };
class CTCGraphicAppearanceReplaceableMesh : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearanceReplaceableMesh) };
class CTCGraphicAppearanceMultipleStaticMeshes : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGraphicAppearanceMultipleStaticMeshes) };
}
class CTCAnimationComplex : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAnimationComplex) };
class CTCHeroAttachableAppearanceModifiers : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroAttachableAppearanceModifiers) };
class CTCCreatureItemLevitationHero : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureItemLevitationHero) };
class CTCCreatureItemLevitationNymph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureItemLevitationNymph) };
class CTCCreatureHive : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureHive) };
class CTCHeroCentre : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroCentre) };
class CTCHeroCentreDoorMarker : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroCentreDoorMarker) };
class CTCHeroGuide : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroGuide) };
class CTCQuestCard : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCQuestCard) };
class CTCFlammable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFlammable) };
class CTCBoastingPodium : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBoastingPodium) };
class CTCBoastingPosition : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBoastingPosition) };
class CTCBoastingArea : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBoastingArea) };
class CTCBoastingCrowdControl : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBoastingCrowdControl) };
class CTCCreatureGroupBoastingCrowd : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureGroupBoastingCrowd) };
class CTCVolumeContainmentTracker : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCVolumeContainmentTracker) };
class CTCDeadShotInThingTracker : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDeadShotInThingTracker) };
namespace NThingShot {
class CTCShotDefinitionNymphFun : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionNymphFun) };
class CTCShotDefinitionQueenHornetShot : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionQueenHornetShot) };
class CTCShotDefinitionSpreadShotProjectile : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionSpreadShotProjectile) };
class CTCShotDefinitionProjectile : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionProjectile) };
class CTCShotDefinitionThrow : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionThrow) };
class CTCShotDefinitionThrowPredictive : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionThrowPredictive) };
class CTCShotDefinitionProjectilePredictive : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionProjectilePredictive) };
class CTCShotDefinitionTearInGround : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionTearInGround) };
class CTCShotDefinitionForcePushBall : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionForcePushBall) };
class CTCShotDefinitionDragonFireball : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionDragonFireball) };
class CTCShotDefinitionMultiArrow : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionMultiArrow) };
class CTCShotDefinitionDrainLife : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionDrainLife) };
class CTCShotDefinitionFireballSpell : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShotDefinitionFireballSpell) };
}
class CTCBurntEffect : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBurntEffect) };
class CTCElectrocutedEffect : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCElectrocutedEffect) };
class CTCCreatureTintEffect : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureTintEffect) };
class CTCSkeletalMorph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSkeletalMorph) };
class CTCGatherPointBuilding : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGatherPointBuilding) };
class CTCGatherPointRadialArea : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGatherPointRadialArea) };
class CTCSwitchableNavigation : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSwitchableNavigation) };
class CTCRegionFollower : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCRegionFollower) };
class CTCDayOrNightOnlySupport : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDayOrNightOnlySupport) };
class CTCMeleeTrap : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCMeleeTrap) };
class CTCProjectileTrap : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCProjectileTrap) };
class CTCHitLocations : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHitLocations) };
class CTCMoneyReceiver : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCMoneyReceiver) };
class CTCHeroRangedAttacks : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroRangedAttacks) static void __fastcall StatRegisterClass() { NoteSetupCall("CTCHeroRangedAttacks::StatRegisterClass"); } };
class CTCParticleAttacher : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCParticleAttacher) };
class CTCAnimatingObject : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAnimatingObject) };
class CTCBerserk : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBerserk) };
class CTCDivineWrath : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDivineWrath) };
class CTCUnholyPower : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCUnholyPower) };
class CTCHaste : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHaste) static void __fastcall StatRegisterClass() { NoteSetupCall("CTCHaste::StatRegisterClass"); } };
class CTCForcePushed : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCForcePushed) };
class CTCRegionDisplay : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCRegionDisplay) };
class CTCExpression : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCExpression) };
class CTCCarriedActionUseExpression : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseExpression) };
class CTCMultiStrike : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCMultiStrike) };
class CTCMultiArrow : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCMultiArrow) };
class CTCDrainLife : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDrainLife) };
class CTCHealLife : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHealLife) };
class CTCGhostSword : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGhostSword) };
class CTCEffectOnDie : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCEffectOnDie) };
class CTCWillResponse : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWillResponse) };
class CTCFireballSpell : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFireballSpell) };
class CTCTurncoatSpell : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTurncoatSpell) };
class CTCTurncoat : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTurncoat) };
class CTCSummonableCreature : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSummonableCreature) };
class CTCShopKeeper : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCShopKeeper) };
class CTCSummonSpell : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSummonSpell) };
class CTCBulletTime : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBulletTime) };
class CTCPhysicalShieldSpell : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhysicalShieldSpell) };
class CTCWillLightning : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWillLightning) };
class CTCWillEnflame : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWillEnflame) };
class CTCForcePushPower : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCForcePushPower) };
class CTCPreCalculatedNavigationRoute : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPreCalculatedNavigationRoute) };
class CTCCreatureGroup : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureGroup) };
class CTCBattleCharge : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBattleCharge) };
class CTCAssassinRush : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAssassinRush) };
class CTCCreatureGroupTag : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureGroupTag) };
class CTCAIScratchpad : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAIScratchpad) };
class CTCSinglePersonOccupiable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSinglePersonOccupiable) };
class CTCBoss : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBoss) };
class CTCRockTrollShield : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCRockTrollShield) };
class CTCCombatLeader : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCombatLeader) };
class CTCFishingSpot : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFishingSpot) };
class CTCFishing : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFishing) };
class CTCStealableItemLocation : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCStealableItemLocation) };
class CTCGuard : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGuard) };
class CTCInterestingToVillagers : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInterestingToVillagers) };
class CTCCarriedActionUseActivateQuest : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseActivateQuest) };
class CTCWaterWader : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWaterWader) };
class CTCCrateStack : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCrateStack) };
class CTCOverheadDisplay : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCOverheadDisplay) };
class CTCTavernTable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernTable) };
class CTCTavernBar : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernBar) };
class CTCTavernTankard : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernTankard) };
class CTCTavernJug : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernJug) };
class CTCTavern : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavern) };
class CTCObjectAugmentations : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCObjectAugmentations) };
class CTCFollowed : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFollowed) };
class CTCTrespasser : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTrespasser) };
class CTCTrespassable : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTrespassable) };
class CTCGossip : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGossip) };
class CTCHitOnCollision : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHitOnCollision) };
class CTCDrunkenness : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDrunkenness) };
class CTCOnHeroPush : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCOnHeroPush) };
class CTCInGameMenu : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInGameMenu) };
class CTCElectrocuteOnTouch : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCElectrocuteOnTouch) };
class CTCCoopSpirit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCoopSpirit) };
class CTCGeneratesExperienceOnKilling : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGeneratesExperienceOnKilling) };
class CTCCreatureGeneratorCreator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureGeneratorCreator) };
class CTCHeroReceiveItems : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroReceiveItems) };
class CTCActionUseActivateQuest : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseActivateQuest) };
class CTCGold : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGold) };
class CTCTargetingSpirit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTargetingSpirit) };
class CTCCreatureHitNotification : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureHitNotification) };
class CTCAICreatureWillPowerIndicator : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAICreatureWillPowerIndicator) };
class CTCTavernGameShoveHaPenny : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameShoveHaPenny) };
class CTCTavernGameCoinGolf : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameCoinGolf) };
class CTCTavernGameCardSorting : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameCardSorting) };
class CTCTavernGameCardPairs : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameCardPairs) };
class CTCTavernGameSpotTheAddition : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameSpotTheAddition) };
class CTCTavernGameCardBlackjack : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameCardBlackjack) };
class CTCGamePlayer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGamePlayer) };
class CTCDecapitation : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDecapitation) };
class CTCTavernGameMaster : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTavernGameMaster) };
class CTCParticleAttachmentTracker : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCParticleAttachmentTracker) };
class CTCCreatureFlyOnSpline : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureFlyOnSpline) };
class CTCCreatureFlyInCircles : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreatureFlyInCircles) };
class CTCWallMount : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWallMount) };
class CTCWallMountEffects : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWallMountEffects) };
class CTCFish : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFish) };
class CTCTeleporter : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTeleporter) };
class CTCActionUseWallMount : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCActionUseWallMount) };
class CTCDExplosion : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDExplosion) static unsigned long __fastcall StatGetDriverType() { return 27; } };
class CTCCarriedActionUseWallMount : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseWallMount) };
class CTCCarriedActionUseFishingRod : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseFishingRod) };
class CTCCarriedActionUseSpade : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseSpade) };
class CTCResurrectionItem : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCResurrectionItem) };
class CTCKraken : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCKraken) };
class CTCKrakenTentacle : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCKrakenTentacle) };
class CTCHeroSpecialMovement : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroSpecialMovement) };
class CTCInfoDisplay : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCInfoDisplay) };
class CTCIdleScheduler : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCIdleScheduler) };
class CTCCarriedActionUseRead : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseRead) };
class CTCAmbientBird : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAmbientBird) };
class CTCTextureDecal : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCTextureDecal) };
class CTCFishingRod : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFishingRod) };
class CTCBoastUI : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBoastUI) };
class CTCThingUIDDummyForSerialisation : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCThingUIDDummyForSerialisation) };
class CTCDRumble : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDRumble) static unsigned long __fastcall StatGetDriverType() { return 28; } };
class CTCDPhysicalObstruction : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDPhysicalObstruction) static unsigned long __fastcall StatGetDriverType() { return 29; } };
class CTCDiggingSpot : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDiggingSpot) };
class CTCDiceGame : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDiceGame) };
class CTCPlot : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPlot) };
class CTCSleep : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSleep) };
class CTCFineDialogue : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFineDialogue) };
class CTCBob : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBob) };
class CTCSoundAtmosphereVillage : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCSoundAtmosphereVillage) };
class CTCQuickAccessMenu : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCQuickAccessMenu) };
class CTCNymph : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCNymph) };
class CTCAISummon : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAISummon) };
class CTCWobble : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCWobble) };
class CTCCameraCollision : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCameraCollision) };
class CTCFadeOutAndIn : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFadeOutAndIn) };
class CTCFlashOnHit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFlashOnHit) };
class CTCQuestCompletionUI : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCQuestCompletionUI) };
class CTCCreditsUI : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCreditsUI) };
class CTCDestroyThingOnActionFinish : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDestroyThingOnActionFinish) };
class CTCObstacle : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCObstacle) };
class CTCFlourishTarget : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFlourishTarget) };
class CTCBetting : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCBetting) };
class CTCLifeTimer : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCLifeTimer) };
class CTCFireRangedWeaponOnLeavingStateGroup : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFireRangedWeaponOnLeavingStateGroup) };
class CTCAvoidRegionExit : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAvoidRegionExit) };
class CTCOracleMinigame : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCOracleMinigame) };
class CTCFireheartMinigame : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCFireheartMinigame) };
class CTCDestroyThingOnSpecialAbilityFinish : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDestroyThingOnSpecialAbilityFinish) };
class CTCLightningOrb : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCLightningOrb) };
class CTCLightningNode : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCLightningNode) };
class CTCHasName : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHasName) };
class CTCHeroOnlineScoreboard : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCHeroOnlineScoreboard) };
class CTCPhotojournalCapture : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCPhotojournalCapture) };
class CTCDExplosiveTrail : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCDExplosiveTrail) static unsigned long __fastcall StatGetDriverType() { return 30; } };
class CTCCarriedActionUseAbility : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCCarriedActionUseAbility) };
class CTCAbsorbHits : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCAbsorbHits) };
class CTCGeneratedCreature : public CTCBase { public: TC_REGISTRATION_INTERFACE(CTCGeneratedCreature) };
class CTCCameraPointDefinitionBase : public CTCBase { public: static void __fastcall StatRegisterClass() { NoteSetupCall("CTCCameraPointDefinitionBase::StatRegisterClass"); } };
namespace NHitLocations {
class CHitLocation { public: static void __fastcall StatRegisterClass() { NoteSetupCall("CHitLocation::StatRegisterClass"); } };
}

class CHeroMorphDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroMorphDef) };
class CHighlightItemDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHighlightItemDef) };
class CSmokeGeneratorDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSmokeGeneratorDef) };
class CTimeAppearanceFadeDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTimeAppearanceFadeDef) };
class CCreatureNavigationDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCreatureNavigationDef) };
class CInventoryItemDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CInventoryItemDef) };
class CLookDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CLookDef) };
class CReadableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CReadableDef) };
class CVillageDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CVillageDef) };
class CVillageMemberDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CVillageMemberDef) };
class CBuyableHouseDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBuyableHouseDef) };
class CBuyHouseDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBuyHouseDef) };
class CWifeDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CWifeDef) };
class CDoorDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CDoorDef) };
class CLightDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CLightDef) };
class CSpotLightDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSpotLightDef) };
class CClockDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CClockDef) };
class CHeroDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroDef) };
class CCreatureModeDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCreatureModeDef) };
class CPerceivedThingDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CPerceivedThingDef) };
class CBedDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBedDef) };
class CStealthDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CStealthDef) };
class CTrophyDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTrophyDef) };
class CCreatureGeneratorDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCreatureGeneratorDef) };
class CChestDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CChestDef) };
class CExplodingObjectDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CExplodingObjectDef) };
class CContainerRewardHeroDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CContainerRewardHeroDef) };
class CWeaponDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CWeaponDef) };
class CCarryingDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCarryingDef) };
class CCarryableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCarryableDef) };
class CEnemyDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CEnemyDef) };
class COpinionOfHeroDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COpinionOfHeroDef) };
class CShopDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CShopDef) };
class CStockItemDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CStockItemDef) };
class CGiftDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CGiftDef) };
class CHeroSuitDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroSuitDef) };
class CHeroExperienceDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroExperienceDef) };
class CExperienceDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CExperienceDef) };
class CReplaceableMeshDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CReplaceableMeshDef) };
class CMultiStaticMeshDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CMultiStaticMeshDef) };
class CHeroCentreDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroCentreDef) };
class CQuestCardDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CQuestCardDef) };
class CFlammableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFlammableDef) };
class CBoastingPodiumDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBoastingPodiumDef) };
class CTCVolumeContainmentTrackerDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTCVolumeContainmentTrackerDef) };
class CThingDrainLifeShotDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CThingDrainLifeShotDef) };
class CFireballSpellLevelDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFireballSpellLevelDef) };
class CSkeletalMorphDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSkeletalMorphDef) };
class CTrapDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTrapDef) };
class CParticleAttacherDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CParticleAttacherDef) };
class CAnimatingObjectDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CAnimatingObjectDef) };
class CExpressionSubDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CExpressionSubDef) };
class CWillResponseDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CWillResponseDef) };
class CTurncoatDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTurncoatDef) };
class CSummonableCreatureDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSummonableCreatureDef) };
class CAIScratchpadDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CAIScratchpadDef) };
class COccupiableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COccupiableDef) };
class CBossDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBossDef) };
class CFishingDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFishingDef) };
class CGuardDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CGuardDef) };
class CInterestingToVillagersDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CInterestingToVillagersDef) };
class CActivateQuestDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CActivateQuestDef) };
class CCrateStackDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCrateStackDef) };
class COverheadDisplayDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COverheadDisplayDef) };
class CTavernTableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernTableDef) };
class CTavernDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernDef) };
class CObjectAugmentationsDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CObjectAugmentationsDef) };
class CDrunkennessDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CDrunkennessDef) };
class CGoldDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CGoldDef) };
class CAICreatureWillPowerIndicatorDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CAICreatureWillPowerIndicatorDef) };
class CKickableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CKickableDef) };
class CTavernGameDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernGameDef) };
class CTavernGameCardBaseDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernGameCardBaseDef) };
class CTavernGameCoinBaseDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernGameCoinBaseDef) };
class CTavernGameShoveHaPennyDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernGameShoveHaPennyDef) };
class CTavernGameCoinGolfDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernGameCoinGolfDef) };
class CTavernGameSpotTheAdditionDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTavernGameSpotTheAdditionDef) };
class CDecapitationDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CDecapitationDef) };
class CCoinGameObstacleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCoinGameObstacleDef) };
class CWallMountEffectsDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CWallMountEffectsDef) };
class CFishDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFishDef) };
class CTeleporterDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTeleporterDef) };
class CExplosionDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CExplosionDef) };
class CResurrectionItemDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CResurrectionItemDef) };
class CKrakenDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CKrakenDef) };
class CKrakenTentacleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CKrakenTentacleDef) };
class CHeroSpecialMovementDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHeroSpecialMovementDef) };
class CIdleSchedulerDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CIdleSchedulerDef) };
class CCarriedReadableDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCarriedReadableDef) };
class CJackOfBladesBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CJackOfBladesBattleDef) };
class CScorpionKingBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CScorpionKingBattleDef) };
class CThunderBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CThunderBattleDef) };
class CWhisperBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CWhisperBattleDef) };
class CWaspQueenBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CWaspQueenBattleDef) };
class CMazeBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CMazeBattleDef) };
class CTrollBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CTrollBattleDef) };
class CBalverineBattleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBalverineBattleDef) };
class CAreaOfEffectAttackDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CAreaOfEffectAttackDef) };
class CFishingRodDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFishingRodDef) };
class CRumbleDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CRumbleDef) };
class CShipDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CShipDef) };
class CShopItemDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CShopItemDef) };
class CSoundAtmospheresDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSoundAtmospheresDef) };
class CNymphDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CNymphDef) };
class CSummonDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CSummonDef) };
class CCameraCollisionDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CCameraCollisionDef) };
class CBettingDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CBettingDef) };
class COracleMinigameDef { public: DEF_CLASS_REGISTRATION_INTERFACE(COracleMinigameDef) };
class CFireheartMinigameDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CFireheartMinigameDef) };
class CLightningOrbDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CLightningOrbDef) };
class CHasNameDef { public: DEF_CLASS_REGISTRATION_INTERFACE(CHasNameDef) };

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

// The retail table (Fable.exe 0x004EE23F, in source order): component classes with their driver types, definition classes,
// setup calls.
struct SExpectedTC { const char* Name; unsigned long DriverType; };
static const SExpectedTC ExpectedTCs[] = {
    { "CTCHeroMorph", 0 },
    { "CTCSimpleAppearanceMorph", 0 },
    { "CTCAtmosPlayer", 0 },
    { "CTCRandomAppearanceMorph", 0 },
    { "CTCHighlightItem", 0 },
    { "CTCSmokeGenerator", 0 },
    { "CTCTimeAppearanceFade", 0 },
    { "CTCPhysicsLight", 0 },
    { "CTCPhysicsStandard", 0 },
    { "CTCPhysicsControlled", 0 },
    { "CTCCreatureNavigation", 0 },
    { "CTCPhysicsNavigator", 0 },
    { "CTCTargetingAI", 0 },
    { "CTCTargetingPlayer", 0 },
    { "CTCTargeted", 0 },
    { "CTCSpecialAbilities", 0 },
    { "CTCUndeadSoul", 0 },
    { "CTCSoundPlayer", 0 },
    { "CTCTalk", 0 },
    { "CTCInventory", 0 },
    { "CTCInventoryClothing", 0 },
    { "CTCInventoryWeapons", 0 },
    { "CTCInventoryAbilities", 0 },
    { "CTCInventoryMagic", 0 },
    { "CTCInventoryStats", 0 },
    { "CTCInventoryExperience", 0 },
    { "CTCInventoryTrade", 0 },
    { "CTCInventoryQuests", 0 },
    { "CTCInventoryMap", 0 },
    { "CTCInventoryItem", 0 },
    { "CTCCreatureExpression", 0 },
    { "CTCLook", 0 },
    { "CTCActionUseTorch", 0 },
    { "CTCActionUseScriptedHook", 0 },
    { "CTCActionUseSign", 0 },
    { "CTCActionUseReadable", 0 },
    { "CTCActionUsePickUp", 0 },
    { "CTCActionUsePickUpGenericBox", 0 },
    { "CTCActionUsePutInInventory", 0 },
    { "CTCActionUseMapTable", 0 },
    { "CTCActionUseSearch", 0 },
    { "CTCActionUseBlowHorn", 0 },
    { "CTCActionUseBuyOrSteal", 0 },
    { "CTCActionUseWindow", 0 },
    { "CTCActionUseAcceptBoast", 0 },
    { "CTCActionUseStartExtrapolationMorph", 0 },
    { "CTCActionUseBed", 0 },
    { "CTCActionUseKickChicken", 0 },
    { "CTCCarriedActionUseLamp", 0 },
    { "CTCCarriedActionUseDrop", 0 },
    { "CTCCarriedActionUseDropGenericBox", 0 },
    { "CTCCarriedActionUseGive", 0 },
    { "CTCCarriedActionUseGiveBeer", 0 },
    { "CTCCarriedActionUseGoldBag", 0 },
    { "CTCCarriedActionUseShow", 0 },
    { "CTCCarriedActionUseTrophy", 0 },
    { "CTCCarriedActionUsePotion", 0 },
    { "CTCCarriedActionUseFood", 0 },
    { "CTCCarriedActionUseFish", 0 },
    { "CTCActionUseSwitch", 0 },
    { "CTCScriptedControl", 0 },
    { "CTCMeshAutomaticEntityCreator", 0 },
    { "CTCThingOwner", 0 },
    { "CTCVillage", 0 },
    { "CTCVillageMember", 0 },
    { "CTCBuyableHouse", 0 },
    { "CTCActionUseBuyHouse", 0 },
    { "CTCDummyVillager", 0 },
    { "CTCWife", 0 },
    { "CTCJackShield", 0 },
    { "CTCJackPillar", 0 },
    { "CTCDoor", 0 },
    { "CTCLight", 0 },
    { "CTCSpotLight", 0 },
    { "CTCClock", 0 },
    { "CTCHero", 0 },
    { "CTCWeatherVane", 0 },
    { "CTCSingleUserActionPoints", 0 },
    { "CTCNamedUserActionPoints", 0 },
    { "CTCExclusiveUse", 0 },
    { "CTCRotate", 0 },
    { "CTCCombat", 0 },
    { "CTCCreatedEntity", 0 },
    { "CTCOwnedEntity", 0 },
    { "CTCCreatureModeManager", 0 },
    { "CTCPerceiveThing", 0 },
    { "CTCBed", 0 },
    { "CTCStealth", 0 },
    { "CTCNoise", 0 },
    { "CTCCupboard", 0 },
    { "CTCTrophy", 0 },
    { "CTCMapwho", 0 },
    { "CTCCreatureGenerator", 0 },
    { "CTCChest", 0 },
    { "CTCSearchableContainer", 0 },
    { "CTCActionUseChest", 0 },
    { "CTCDiscovery", 0 },
    { "CTCExplodingObject", 0 },
    { "CTCContainerRewardHero", 0 },
    { "CTCEntityEvents", 0 },
    { "CTCHeroStats", 0 },
    { "CTCHeroRememberCrimes", 0 },
    { "CTCWound", 0 },
    { "CTCPhysicsRigidBody", 0 },
    { "CTCDCameraPoint", 20 },
    { "CTCCameraPointFixedPoint", 0 },
    { "CTCCameraPointBuilding", 0 },
    { "CTCCameraPointFixedDolly", 0 },
    { "CTCCameraPointTrack", 0 },
    { "CTCCameraPointGeneralCase", 0 },
    { "CTCCameraPointDead", 0 },
    { "CTCCameraPointScripted", 0 },
    { "CTCCameraPointScriptedSpline", 0 },
    { "CTCShapeManager", 0 },
    { "CTCEmoteIcon", 0 },
    { "CTCWeapon", 0 },
    { "CTCCarrying", 0 },
    { "CTCCarryable", 0 },
    { "CTCEnemy", 0 },
    { "CTCBandit", 0 },
    { "CTCDParticleEmitter", 19 },
    { "CTCWeaponTrail", 0 },
    { "CTCSolidToThing", 0 },
    { "CTCSmashableWindow", 0 },
    { "CTCSmashableBarrier", 0 },
    { "CTCDegradableBarrier", 0 },
    { "CTCHeroOpinionDeedLog", 0 },
    { "CTCCreatureOpinionOfHero", 0 },
    { "CTCOpinionSource", 0 },
    { "CTCHeroPodium", 0 },
    { "CTCShop", 0 },
    { "CTCBlacksmiths", 0 },
    { "CTCStockItem", 0 },
    { "CTCGift", 0 },
    { "CTCFamilyMember", 0 },
    { "CTCDNavigationSeed", 21 },
    { "CTCDHeroSuit", 22 },
    { "CTCDNone", 23 },
    { "CTCDInternalMarker", 12 },
    { "CTCDRegionEntrance", 24 },
    { "CTCDRegionExit", 25 },
    { "CTCActivationReceptorChest", 0 },
    { "CTCActivationReceptorCreatureGenerator", 0 },
    { "CTCActivationReceptorDoor", 0 },
    { "CTCActivationReceptorRelay", 0 },
    { "CTCActivationTrigger", 0 },
    { "CTCHeroExperience", 0 },
    { "CTCDExperienceOrb", 26 },
    { "CTCGraphicAppearance", 0 },
    { "CTCGraphicAppearanceAnimatedMesh", 0 },
    { "CTCGraphicAppearanceStaticMesh", 0 },
    { "CTCGraphicAppearanceStaticMeshFadeOutOnProximity", 0 },
    { "CTCGraphicAppearanceSprite", 0 },
    { "CTCGraphicAppearanceReplaceableMesh", 0 },
    { "CTCGraphicAppearanceMultipleStaticMeshes", 0 },
    { "CTCAnimationComplex", 0 },
    { "CTCHeroAttachableAppearanceModifiers", 0 },
    { "CTCCreatureItemLevitationHero", 0 },
    { "CTCCreatureItemLevitationNymph", 0 },
    { "CTCCreatureHive", 0 },
    { "CTCHeroCentre", 0 },
    { "CTCHeroCentreDoorMarker", 0 },
    { "CTCHeroGuide", 0 },
    { "CTCQuestCard", 0 },
    { "CTCFlammable", 0 },
    { "CTCBoastingPodium", 0 },
    { "CTCBoastingPosition", 0 },
    { "CTCBoastingArea", 0 },
    { "CTCBoastingCrowdControl", 0 },
    { "CTCCreatureGroupBoastingCrowd", 0 },
    { "CTCVolumeContainmentTracker", 0 },
    { "CTCDeadShotInThingTracker", 0 },
    { "CTCShotDefinitionNymphFun", 0 },
    { "CTCShotDefinitionQueenHornetShot", 0 },
    { "CTCShotDefinitionSpreadShotProjectile", 0 },
    { "CTCShotDefinitionProjectile", 0 },
    { "CTCShotDefinitionThrow", 0 },
    { "CTCShotDefinitionThrowPredictive", 0 },
    { "CTCShotDefinitionProjectilePredictive", 0 },
    { "CTCShotDefinitionTearInGround", 0 },
    { "CTCShotDefinitionForcePushBall", 0 },
    { "CTCShotDefinitionDragonFireball", 0 },
    { "CTCShotDefinitionMultiArrow", 0 },
    { "CTCShotDefinitionDrainLife", 0 },
    { "CTCShotDefinitionFireballSpell", 0 },
    { "CTCBurntEffect", 0 },
    { "CTCElectrocutedEffect", 0 },
    { "CTCCreatureTintEffect", 0 },
    { "CTCSkeletalMorph", 0 },
    { "CTCGatherPointBuilding", 0 },
    { "CTCGatherPointRadialArea", 0 },
    { "CTCSwitchableNavigation", 0 },
    { "CTCRegionFollower", 0 },
    { "CTCDayOrNightOnlySupport", 0 },
    { "CTCMeleeTrap", 0 },
    { "CTCProjectileTrap", 0 },
    { "CTCHitLocations", 0 },
    { "CTCMoneyReceiver", 0 },
    { "CTCHeroRangedAttacks", 0 },
    { "CTCParticleAttacher", 0 },
    { "CTCAnimatingObject", 0 },
    { "CTCBerserk", 0 },
    { "CTCDivineWrath", 0 },
    { "CTCUnholyPower", 0 },
    { "CTCHaste", 0 },
    { "CTCForcePushed", 0 },
    { "CTCRegionDisplay", 0 },
    { "CTCExpression", 0 },
    { "CTCCarriedActionUseExpression", 0 },
    { "CTCMultiStrike", 0 },
    { "CTCMultiArrow", 0 },
    { "CTCDrainLife", 0 },
    { "CTCHealLife", 0 },
    { "CTCGhostSword", 0 },
    { "CTCEffectOnDie", 0 },
    { "CTCWillResponse", 0 },
    { "CTCFireballSpell", 0 },
    { "CTCTurncoatSpell", 0 },
    { "CTCTurncoat", 0 },
    { "CTCSummonableCreature", 0 },
    { "CTCShopKeeper", 0 },
    { "CTCSummonSpell", 0 },
    { "CTCBulletTime", 0 },
    { "CTCPhysicalShieldSpell", 0 },
    { "CTCWillLightning", 0 },
    { "CTCWillEnflame", 0 },
    { "CTCForcePushPower", 0 },
    { "CTCPreCalculatedNavigationRoute", 0 },
    { "CTCCreatureGroup", 0 },
    { "CTCBattleCharge", 0 },
    { "CTCAssassinRush", 0 },
    { "CTCCreatureGroupTag", 0 },
    { "CTCAIScratchpad", 0 },
    { "CTCSinglePersonOccupiable", 0 },
    { "CTCBoss", 0 },
    { "CTCRockTrollShield", 0 },
    { "CTCCombatLeader", 0 },
    { "CTCFishingSpot", 0 },
    { "CTCFishing", 0 },
    { "CTCStealableItemLocation", 0 },
    { "CTCGuard", 0 },
    { "CTCInterestingToVillagers", 0 },
    { "CTCCarriedActionUseActivateQuest", 0 },
    { "CTCWaterWader", 0 },
    { "CTCCrateStack", 0 },
    { "CTCOverheadDisplay", 0 },
    { "CTCTavernTable", 0 },
    { "CTCTavernBar", 0 },
    { "CTCTavernTankard", 0 },
    { "CTCTavernJug", 0 },
    { "CTCTavern", 0 },
    { "CTCObjectAugmentations", 0 },
    { "CTCFollowed", 0 },
    { "CTCTrespasser", 0 },
    { "CTCTrespassable", 0 },
    { "CTCGossip", 0 },
    { "CTCHitOnCollision", 0 },
    { "CTCDrunkenness", 0 },
    { "CTCOnHeroPush", 0 },
    { "CTCInGameMenu", 0 },
    { "CTCElectrocuteOnTouch", 0 },
    { "CTCCoopSpirit", 0 },
    { "CTCGeneratesExperienceOnKilling", 0 },
    { "CTCCreatureGeneratorCreator", 0 },
    { "CTCHeroReceiveItems", 0 },
    { "CTCActionUseActivateQuest", 0 },
    { "CTCGold", 0 },
    { "CTCTargetingSpirit", 0 },
    { "CTCCreatureHitNotification", 0 },
    { "CTCAICreatureWillPowerIndicator", 0 },
    { "CTCTavernGameShoveHaPenny", 0 },
    { "CTCTavernGameCoinGolf", 0 },
    { "CTCTavernGameCardSorting", 0 },
    { "CTCTavernGameCardPairs", 0 },
    { "CTCTavernGameSpotTheAddition", 0 },
    { "CTCTavernGameCardBlackjack", 0 },
    { "CTCGamePlayer", 0 },
    { "CTCDecapitation", 0 },
    { "CTCTavernGameMaster", 0 },
    { "CTCParticleAttachmentTracker", 0 },
    { "CTCCreatureFlyOnSpline", 0 },
    { "CTCCreatureFlyInCircles", 0 },
    { "CTCWallMount", 0 },
    { "CTCWallMountEffects", 0 },
    { "CTCFish", 0 },
    { "CTCTeleporter", 0 },
    { "CTCActionUseWallMount", 0 },
    { "CTCDExplosion", 27 },
    { "CTCCarriedActionUseWallMount", 0 },
    { "CTCCarriedActionUseFishingRod", 0 },
    { "CTCCarriedActionUseSpade", 0 },
    { "CTCResurrectionItem", 0 },
    { "CTCKraken", 0 },
    { "CTCKrakenTentacle", 0 },
    { "CTCHeroSpecialMovement", 0 },
    { "CTCInfoDisplay", 0 },
    { "CTCIdleScheduler", 0 },
    { "CTCCarriedActionUseRead", 0 },
    { "CTCAmbientBird", 0 },
    { "CTCTextureDecal", 0 },
    { "CTCFishingRod", 0 },
    { "CTCBoastUI", 0 },
    { "CTCThingUIDDummyForSerialisation", 0 },
    { "CTCDRumble", 28 },
    { "CTCDPhysicalObstruction", 29 },
    { "CTCDiggingSpot", 0 },
    { "CTCDiceGame", 0 },
    { "CTCPlot", 0 },
    { "CTCSleep", 0 },
    { "CTCFineDialogue", 0 },
    { "CTCBob", 0 },
    { "CTCSoundAtmosphereVillage", 0 },
    { "CTCQuickAccessMenu", 0 },
    { "CTCNymph", 0 },
    { "CTCAISummon", 0 },
    { "CTCWobble", 0 },
    { "CTCCameraCollision", 0 },
    { "CTCFadeOutAndIn", 0 },
    { "CTCFlashOnHit", 0 },
    { "CTCQuestCompletionUI", 0 },
    { "CTCCreditsUI", 0 },
    { "CTCDestroyThingOnActionFinish", 0 },
    { "CTCObstacle", 0 },
    { "CTCFlourishTarget", 0 },
    { "CTCBetting", 0 },
    { "CTCLifeTimer", 0 },
    { "CTCFireRangedWeaponOnLeavingStateGroup", 0 },
    { "CTCAvoidRegionExit", 0 },
    { "CTCOracleMinigame", 0 },
    { "CTCFireheartMinigame", 0 },
    { "CTCDestroyThingOnSpecialAbilityFinish", 0 },
    { "CTCLightningOrb", 0 },
    { "CTCLightningNode", 0 },
    { "CTCHasName", 0 },
    { "CTCHeroOnlineScoreboard", 0 },
    { "CTCPhotojournalCapture", 0 },
    { "CTCDExplosiveTrail", 30 },
    { "CTCCarriedActionUseAbility", 0 },
    { "CTCAbsorbHits", 0 },
    { "CTCGeneratedCreature", 0 },
};
struct SExpectedDef { const char* Name; int TCsBefore; };
static const SExpectedDef ExpectedDefs[] = {
    { "CHeroMorphDef", 1 },
    { "CHighlightItemDef", 5 },
    { "CSmokeGeneratorDef", 6 },
    { "CTimeAppearanceFadeDef", 7 },
    { "CCreatureNavigationDef", 11 },
    { "CInventoryItemDef", 30 },
    { "CLookDef", 32 },
    { "CReadableDef", 36 },
    { "CVillageDef", 64 },
    { "CVillageMemberDef", 65 },
    { "CBuyableHouseDef", 66 },
    { "CBuyHouseDef", 67 },
    { "CWifeDef", 69 },
    { "CDoorDef", 72 },
    { "CLightDef", 73 },
    { "CSpotLightDef", 74 },
    { "CClockDef", 75 },
    { "CHeroDef", 76 },
    { "CCreatureModeDef", 85 },
    { "CPerceivedThingDef", 86 },
    { "CBedDef", 87 },
    { "CStealthDef", 88 },
    { "CTrophyDef", 91 },
    { "CCreatureGeneratorDef", 93 },
    { "CChestDef", 94 },
    { "CExplodingObjectDef", 98 },
    { "CContainerRewardHeroDef", 99 },
    { "CWeaponDef", 116 },
    { "CCarryingDef", 117 },
    { "CCarryableDef", 118 },
    { "CEnemyDef", 119 },
    { "COpinionOfHeroDef", 128 },
    { "CShopDef", 131 },
    { "CStockItemDef", 133 },
    { "CGiftDef", 134 },
    { "CHeroSuitDef", 137 },
    { "CHeroExperienceDef", 147 },
    { "CExperienceDef", 148 },
    { "CReplaceableMeshDef", 154 },
    { "CMultiStaticMeshDef", 155 },
    { "CHeroCentreDef", 161 },
    { "CQuestCardDef", 164 },
    { "CFlammableDef", 165 },
    { "CBoastingPodiumDef", 166 },
    { "CTCVolumeContainmentTrackerDef", 171 },
    { "CThingDrainLifeShotDef", 184 },
    { "CFireballSpellLevelDef", 185 },
    { "CSkeletalMorphDef", 189 },
    { "CTrapDef", 195 },
    { "CParticleAttacherDef", 200 },
    { "CAnimatingObjectDef", 201 },
    { "CExpressionSubDef", 208 },
    { "CWillResponseDef", 216 },
    { "CTurncoatDef", 219 },
    { "CSummonableCreatureDef", 220 },
    { "CAIScratchpadDef", 233 },
    { "COccupiableDef", 234 },
    { "CBossDef", 235 },
    { "CFishingDef", 239 },
    { "CGuardDef", 241 },
    { "CInterestingToVillagersDef", 242 },
    { "CActivateQuestDef", 243 },
    { "CCrateStackDef", 245 },
    { "COverheadDisplayDef", 246 },
    { "CTavernTableDef", 247 },
    { "CTavernDef", 251 },
    { "CObjectAugmentationsDef", 252 },
    { "CDrunkennessDef", 258 },
    { "CGoldDef", 267 },
    { "CAICreatureWillPowerIndicatorDef", 270 },
    { "CKickableDef", 270 },
    { "CTavernGameDef", 270 },
    { "CTavernGameCardBaseDef", 270 },
    { "CTavernGameCoinBaseDef", 270 },
    { "CTavernGameShoveHaPennyDef", 271 },
    { "CTavernGameCoinGolfDef", 272 },
    { "CTavernGameSpotTheAdditionDef", 275 },
    { "CDecapitationDef", 278 },
    { "CCoinGameObstacleDef", 279 },
    { "CWallMountEffectsDef", 284 },
    { "CFishDef", 285 },
    { "CTeleporterDef", 286 },
    { "CExplosionDef", 288 },
    { "CResurrectionItemDef", 292 },
    { "CKrakenDef", 293 },
    { "CKrakenTentacleDef", 294 },
    { "CHeroSpecialMovementDef", 295 },
    { "CIdleSchedulerDef", 297 },
    { "CCarriedReadableDef", 298 },
    { "CJackOfBladesBattleDef", 299 },
    { "CScorpionKingBattleDef", 299 },
    { "CThunderBattleDef", 299 },
    { "CWhisperBattleDef", 299 },
    { "CWaspQueenBattleDef", 299 },
    { "CMazeBattleDef", 299 },
    { "CTrollBattleDef", 299 },
    { "CBalverineBattleDef", 299 },
    { "CAreaOfEffectAttackDef", 300 },
    { "CFishingRodDef", 301 },
    { "CRumbleDef", 304 },
    { "CShipDef", 311 },
    { "CShopItemDef", 311 },
    { "CSoundAtmospheresDef", 311 },
    { "CNymphDef", 314 },
    { "CSummonDef", 315 },
    { "CCameraCollisionDef", 317 },
    { "CBettingDef", 325 },
    { "COracleMinigameDef", 329 },
    { "CFireheartMinigameDef", 330 },
    { "CLightningOrbDef", 332 },
    { "CHasNameDef", 334 },
};
static const char* const ExpectedSetupCalls[] = {
    "CTCCreatureGenerator::StatRegisterClass",
    "CTCCombat::StatRegisterClass",
    "CTCHeroRangedAttacks::StatRegisterClass",
    "CTCHaste::StatRegisterClass",
    "CTCCreatureNavigation::StatRegisterClass",
    "CTCCameraPointDefinitionBase::StatRegisterClass",
    "CHitLocation::StatRegisterClass",
    "CTCTargetingPlayer::StatRegisterClass",
    "CTCCreatureModeManager::RegisterModes",
};
static const int NumExpectedTCs = sizeof(ExpectedTCs) / sizeof(ExpectedTCs[0]);
static const int NumExpectedDefs = sizeof(ExpectedDefs) / sizeof(ExpectedDefs[0]);
static const int NumExpectedSetupCalls = sizeof(ExpectedSetupCalls) / sizeof(ExpectedSetupCalls[0]);

static int Fail(const char* what)
{
    std::printf("004ee23f_TEST FAIL (%s)\n", what);
    return 1;
}

// One full check of the registry and of the definition classes registered since `defsBefore`.
static const char* CheckRun(int defsBefore)
{
    CTCBase::CRegistry& registry = CTCBase::RegisteredTCs;
    if (registry.Count != NumExpectedTCs) return "component count";
    if (registry.Dirty) return "registry left dirty";
    for (int i = 1; i < registry.Count; ++i)
        if (!(registry.Data[i - 1]->first < registry.Data[i]->first)) return "registry not sorted by name";
    CThing owner;
    for (int e = 0; e < NumExpectedTCs; ++e) {
        int found = -1;
        for (int i = 0; i < registry.Count; ++i)
            if (registry.Data[i]->first == ExpectedTCs[e].Name) { found = i; break; }
        if (found < 0) return "component missing";
        const CTCBase::CRegisteredTCInfo& info = registry.Data[found]->second;
        if (!(info.Name == ExpectedTCs[e].Name)) return "entry name";
        if (info.DriverType != ExpectedTCs[e].DriverType) return "driver type";
        CTCBase* tc = info.AllocFunc(owner);
        if (tc == 0 || std::strcmp(tc->ClassName, ExpectedTCs[e].Name) != 0) return "allocator class";
    }
    CGameDefinitionManager& defs = CGameDefinitionManager::Get();
    if (defs.Count - defsBefore != NumExpectedDefs) return "definition class count";
    for (int d = 0; d < NumExpectedDefs; ++d) {
        int k = defsBefore + d;
        if (!(*defs.Names[k] == ExpectedDefs[d].Name)) return "definition class order";
        if (defs.TCsBefore[k] != ExpectedDefs[d].TCsBefore) return "definition class position";
        CDefClassBase* def = defs.Allocs[k]();
        if (def == 0 || std::strcmp(def->ClassName, ExpectedDefs[d].Name) != 0) return "definition allocator";
    }
    if (NumSetupCalls != NumExpectedSetupCalls) return "setup call count";
    for (int c = 0; c < NumSetupCalls; ++c)
        if (std::strcmp(SetupCalls[c], ExpectedSetupCalls[c]) != 0) return "setup call order";
    if (ModesSawCount != NumExpectedTCs || !ModesSawDirty) return "RegisterModes not between the registrations and the sort";
    return 0;
}

int main()
{
    // a stale entry from an earlier run must go
    CThing owner;
    CTCBase::RegisteredTCs.Insert("CTCStale", CTCBase::CRegisteredTCInfo(0, 99, "CTCStale"));
    CTCBase::RegisterTCs();
    const char* failure = CheckRun(0);
    if (failure) return Fail(failure);
    for (int i = 0; i < CTCBase::RegisteredTCs.Count; ++i)
        if (CTCBase::RegisteredTCs.Data[i]->first == "CTCStale") return Fail("registry not cleared");
    // a second run rebuilds the same registry (and registers the definition classes again)
    NumSetupCalls = 0; ModesSawCount = -1; ModesSawDirty = false;
    CTCBase::RegisterTCs();
    failure = CheckRun(NumExpectedDefs);
    if (failure) return Fail(failure);
    (void)owner;
    std::printf("004ee23f_TEST PASS\n");
    return 0;
}