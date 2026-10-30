// CM_FishermanWaspSpawn::Main() -- wait for intro and spawn wasp unless saved or thread terminating
// Native00E7B970 length198. String verified through NUL; script definition field598.
#define CScriptThing CScriptThingDonor
#include "engine/CScriptThing.h"
#undef CScriptThing
#define CGameScriptInterface CGameScriptInterfaceDonor
#include "engine/CGameScriptInterface.h"
#undef CGameScriptInterface
#define CCharString CCharStringDonor
#include "fable_string.h"
#undef CCharString
// Actual flattened donor headers included; genuine virtual interfaces follow.
// CCharString donor omits the native owning copy constructor, restored below.
class CThing;
typedef long EAIStateGroupTypeABI;typedef long EScriptAIPriorityABI;
namespace std {template<class T>class allocator;template<class T,class A>class list;template<class T,class A>class vector;template<class T,class U>struct pair;}
enum EScriptAIPriority { ScriptPriorityUnspecified=0 };
enum EClothingCoversArea { ClothingAreaUnspecified=0 };
enum EHeroAbility { HeroAbilityUnspecified=0 };
#ifdef _MSC_VER
typedef unsigned __int64 ScriptUID;
#else
typedef unsigned long long ScriptUID;
#endif
class C3DVector { public:float X,Y,Z;};
class CScriptGameResourceObjectScriptedThingBase;
class CCharString { public:char* Data; CCharString(const char*,long=-1); CCharString(const CCharString&); ~CCharString();}; // 0099EBF0/0099EAE0
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CCPPointerInfo { public:unsigned long References;void (__fastcall *Delete)(void*);void* Data;};
void operator delete(void*);
class CScriptThing : public CBaseClass { public:
 CScriptThing* PImp_Data; CCPPointerInfo* PImp_Info;
 CScriptThing();
 CScriptThing(const CScriptThing&); //004ABE90 owning-copy constructor
 virtual CCharString const& GetName()const; // slot1 004AA8B0
 virtual CCharString GetDefName()const; // slot2 004AA8D0
 virtual CCharString GetDataString()const; // slot3 004AA900
 virtual void SetDataString(CCharString const&); // slot4 004AA930
 virtual C3DVector GetFocalPos()const; // slot5 004AA940
 virtual C3DVector const& GetPos()const; // slot6 004AA980
virtual C3DVector GetHomePos() const; //slot7 0x004AA9A0
virtual CCharString GetCurrentMapName() const; //slot8 0x004AA9E0
virtual CCharString GetHomeMapName() const; //slot9 0x004AAA10
virtual float GetAngleXY() const; //slot10 0x004AAA40
virtual CThing* GetPThing() const; //slot11 0x004AAA60
virtual ScriptUID GetPThingUniqueID() const; //slot12 0x004AAA70
virtual EAIStateGroupTypeABI GetCurrentStateGroupType() const; //slot13 0x004AAA90
virtual EScriptAIPriorityABI GetCurrentScriptPriority() const; //slot14 0x004AAAA0
virtual bool IsSneaking() const; //slot15 0x004AAAB0
virtual bool IsAwareOfHero() const; //slot16 0x004AAAC0
virtual bool MsgIsTriggeredBy(const CCharString&) const; //slot17 0x004AAAD0
virtual bool MsgIsKilledBy(const CCharString&) const; //slot18 0x004AAAF0
virtual CScriptThing MsgWhoKilledMe() const; //slot19 0x004ABE60
virtual bool MsgIsUsedBy(const CCharString&) const; //slot20 0x004AAB10
virtual bool MsgIsHitBy(const CCharString&) const; //slot21 0x004AAB30
virtual bool MsgIsHitByWithFlourish(const CCharString&) const; //slot22 0x004AAB50
virtual bool MsgIsHitByWithDecapitate(const CCharString&) const; //slot23 0x004AAB70
virtual bool MsgIsHitByWithWeapon(const CCharString&,const CCharString&) const; //slot24 0x004AAB90
virtual bool MsgIsHitByWithProjectileWeapon(const CCharString&,float&) const; //slot25 0x004AABB0
virtual CScriptThing MsgWhoHitMe() const; //slot26 0x004ABEC0
virtual bool MsgIsTalkedToBy(const CCharString&) const; //slot27 0x004AABD0
virtual CScriptThing MsgWhoTalkedToMe() const; //slot28 0x004ABF00
virtual bool MsgExpressionPerformedTo(CCharString&) const; //slot29 0x004AABF0
virtual CScriptThing MsgWhoExpressedToMe() const; //slot30 0x004ABF40
virtual long MsgHowLongWasExpressionPerformed() const; //slot31 0x004AAC10
virtual bool MsgIsKnockedOutBy(const CCharString&) const; //slot32 0x004AAC20
virtual CScriptThing MsgWhoKnockedMeOut() const; //slot33 0x004ABF80
virtual bool MsgReceivedMoney(long&) const; //slot34 0x004AAC40
virtual bool MsgIsPresentedWithItem(CCharString&) const; //slot35 0x004AAC60
virtual bool MsgReceivedInventoryItem() const; //slot36 0x004AAC80
virtual bool MsgPerformedSpecialAbility(EHeroAbility) const; //slot37 0x004AACA0
virtual bool MsgPerformedAnySpecialAbility() const; //slot38 0x004AACC0
virtual bool MsgPerformedAnyAggressiveSpecialAbility() const; //slot39 0x004AACE0
virtual bool MsgPerformedAnyNonAggressiveSpecialAbility() const; //slot40 0x004AAD00
virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility,const CCharString&) const; //slot41 0x004AAD20
virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString&) const; //slot42 0x004AAD40
virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString&) const; //slot43 0x004AAD60
virtual bool MsgIsHitByAnyNonAggressiveSpecialAbilityFrom(const CCharString&) const; //slot44 0x004AAD80
virtual bool MsgOpenedChest() const; //slot45 0x004AADA0
virtual bool MsgHitEnemyWithMeleeWeapon() const; //slot46 0x004AADC0
virtual bool MsgHitFriendWithMeleeWeapon() const; //slot47 0x004AADE0
virtual bool MsgHitEnemyWithRangedWeapon() const; //slot48 0x004AAE00
virtual bool MsgHitFriendWithRangedWeapon() const; //slot49 0x004AAE20
virtual bool MsgHitEnemyWithBareHands() const; //slot50 0x004AAE40
virtual bool MsgHitFriendWithBareHands() const; //slot51 0x004AAE60
virtual bool MsgAttackedWithMeleeWeaponWithoutHittingAnything() const; //slot52 0x004AAE80
virtual bool MsgAttackedWithBareHandsWithoutHittingAnything() const; //slot53 0x004AAEA0
virtual bool MsgTalkedToAnyone() const; //slot54 0x004AAEC0
virtual bool MsgGetThingsKilled(std::vector<unsigned long,std::allocator<unsigned long> >&) const; //slot55 0x004AAEE0
virtual bool MsgPerformedFlourish() const; //slot56 0x004AAF00
virtual bool MsgPerformedSuccessfulFlourish() const; //slot57 0x004AAF20
virtual bool MsgOnMoralityChanged(long&) const; //slot58 0x004AAF40
virtual bool MsgOnCutSceneAnimEvent(CCharString&) const; //slot59 0x004AAF60
virtual bool MsgIsKicked() const; //slot60 0x004AAF80
virtual bool IsUnconscious() const; //slot61 0x004AAFA0
virtual bool IsUsable() const; //slot62 0x004AAFC0
virtual bool IsBeingCarriedBy(const CCharString&) const; //slot63 0x004AAFE0
virtual bool IsOpenDoor() const; //slot64 0x004AB000
virtual bool IsSummonedCreature() const; //slot65 0x004AB020
virtual void SetAsUsable(bool); //slot66 0x004AB040
virtual void SetFriendsWithEverythingFlag(bool); //slot67 0x004AB060
virtual bool GetActivationTriggerStatus() const; //slot68 0x004AB080
virtual void SetActivationTriggerStatus(bool); //slot69 0x004AB0A0
virtual void SetToKillOnLevelUnload(bool); //slot70 0x004AB0C0
virtual void UpdateThingAttachment(); //slot71 0x004AB0E0
virtual void IncrementScriptCounter(); //slot72 0x004AB0F0
virtual void DecrementScriptCounter(); //slot73 0x004AB100
virtual long GetScriptCounter(); //slot74 0x004AB110
virtual bool IsAlive() const; //slot75 0x004AB130
virtual bool IsDead() const; //slot76 0x004AA890
virtual bool IsNull() const; //slot77 0x004AB150
virtual bool IsEqualTo(const CScriptThing&) const; //slot78 0x004AA8A0
 virtual ~CScriptThing(); //004AA840
};
// Native CGameScriptInterface virtual prefix: all100 entries use recovered PDB
// signatures and native raw VT01260F0C order. No fabricated padding slots.
class CGameScriptInterface { public:
virtual ~CGameScriptInterface(); // slot0 0x006e7d50
virtual void Error(CCharString const &, CCharString const &, CCharString const &, unsigned long) const; // slot1 0x0088de00
virtual void TraceMessage(CCharString const &) const; // slot2 0x006e7d20
virtual void Validate(void) const; // slot3 0x006e72a0
virtual void SetDebugCameraType(long) const; // slot4 0x0088e0f0
virtual void DeactivateBoastUi(void) const; // slot5 0x008a0c60
virtual bool IsXbox(void) const; // slot6 0x0088e100
virtual void NewScriptFrame(void) const; // slot7 0x006e7410
virtual bool StartScriptingEntity(CScriptThing const &, CScriptGameResourceObjectScriptedThingBase &, enum EScriptAIPriority); // slot8 0x0089b5b0
virtual bool IsEntityUnderScriptedControl(CScriptThing const &) const; // slot9 0x008960c0
virtual bool IsActiveThreadTerminating(void) const; // slot10 0x006e71b0
virtual bool IsLevelLoaded(CCharString const &) const; // slot11 0x0088e2a0
virtual bool IsRegionLoaded(CCharString const &) const; // slot12 0x0088e300
virtual bool IsRegionLoadedAndPreloaded(CCharString const &) const; // slot13 0x00890550
virtual bool IsRegionDefLoaded(CCharString const &) const; // slot14 0x00891db0
virtual CCharString const & GetRegionName(void) const; // slot15 0x0088e340
virtual bool MsgIsLevelLoaded(CCharString const &) const; // slot16 0x008931b0
virtual bool MsgIsLevelUnloaded(CCharString const &) const; // slot17 0x00893240
virtual bool MsgOnLevelLoaded(std::list<CCharString, std::allocator<CCharString> > &) const; // slot18 0x0089ac10
virtual bool MsgOnLevelUnloaded(std::list<CCharString, std::allocator<CCharString> > &) const; // slot19 0x0089ac80
virtual bool MsgIsRegionLoaded(CCharString const &) const; // slot20 0x008932d0
virtual bool MsgIsRegionUnloaded(CCharString const &) const; // slot21 0x00893360
virtual bool MsgOnRegionLoaded(CCharString &) const; // slot22 0x008933f0
virtual bool MsgOnRegionUnloaded(CCharString &) const; // slot23 0x00893470
virtual bool MsgOnRegionPreunload(CCharString &) const; // slot24 0x008934f0
virtual bool MsgOnQuestCompleted(CCharString const &) const; // slot25 0x00893570
virtual bool MsgOnAnyQuestCompleted(CCharString &) const; // slot26 0x00893610
virtual bool MsgOnQuestFailed(CCharString const &) const; // slot27 0x00893690
virtual bool MsgOnQuestCompletedBeforeScreenShown(CCharString const &) const; // slot28 0x00893730
virtual bool MsgOnQuestFailedBeforeScreenShown(CCharString const &) const; // slot29 0x008937d0
virtual bool MsgOnQuestAccepted(CCharString &) const; // slot30 0x00893870
virtual bool MsgOnFeatAccepted(long &) const; // slot31 0x008938f0
virtual bool MsgIsBoastMade(long) const; // slot32 0x00893960
virtual bool MsgOnBoastMade(long &, CCharString &) const; // slot33 0x008939d0
virtual bool MsgOnBoastsMade(std::vector<std::pair<long, CCharString>, std::allocator<std::pair<long, CCharString> > > &) const; // slot34 0x008a9ae0
virtual void RemoveBoastMessage(void) const; // slot35 0x00893a50
virtual bool IsQuestStartScreenActive(CCharString &) const; // slot36 0x0089ad90
virtual bool MsgOnLeavingQuestStartScreen(CCharString &) const; // slot37 0x0089ae30
virtual bool MsgOnLeavingExperienceSpendingScreen(void) const; // slot38 0x00893ab0
virtual long MsgIsQuestionAnsweredYesOrNo(void) const; // slot39 0x00894300
virtual bool MsgIsGameInfoClickedPast(void) const; // slot40 0x00894370
virtual bool MsgIsTutorialClickedPast(void) const; // slot41 0x008943c0
virtual bool MsgIsActionModeButtonPressed(void) const; // slot42 0x00894440
virtual bool MsgOnExpressionPerformed(CCharString &) const; // slot43 0x00893b00
virtual bool MsgIsCutSceneSkipped(void) const; // slot44 0x00893ba0
virtual void RemoveAllCutSceneSkippedMessages(void) const; // slot45 0x0089af30
virtual bool MsgOnHeroHairTypeChanged(enum EClothingCoversArea, CCharString &) const; // slot46 0x00893c00
virtual bool MsgOnHeroUsedTeleporter(CCharString &) const; // slot47 0x00893d80
virtual bool MsgOnHeroUsedGuildSeal(void) const; // slot48 0x00893e20
virtual bool MsgOnGameSavedManually(void) const; // slot49 0x00893e70
virtual bool MsgOnHeroSlept(bool &) const; // slot50 0x00893ec0
virtual bool MsgOnHeroFiredRangedWeapon(void) const; // slot51 0x00893f30
virtual bool MsgOnHeroCastSpell(enum EHeroAbility &) const; // slot52 0x00893f80
virtual bool MsgOnHeroPickedPocket(CScriptThing &) const; // slot53 0x00893ff0
virtual bool MsgOnHeroPickedLock(CScriptThing &) const; // slot54 0x00894080
virtual bool MsgOnFishingGameFinished(CScriptThing &) const; // slot55 0x00894110
virtual bool MsgOnTavernGameFinished(CScriptThing const &) const; // slot56 0x008941a0
virtual bool MsgOnHeroRewardedWithItemsFrom(CCharString &) const; // slot57 0x00894250
virtual bool MsgOnChestOpeningCancelled(void) const; // slot58 0x008942c0
virtual void DontPopulateNextLoadedRegion(void) const; // slot59 0x0088e380
virtual void GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString &) const; // slot60 0x0088e390
virtual void GetWanderingPopulationScriptDefNameInRegion(CCharString const &, CCharString &) const; // slot61 0x00891e30
virtual bool IsHeroAllowedHenchmenInCurrentRegion(void) const; // slot62 0x00891ea0
virtual bool IsHeroAllowedHenchmenInRegion(CCharString const &) const; // slot63 0x00891ef0
virtual void PostAddScriptedEntities(void) const; // slot64 0x006e7460
virtual bool IsPlayerHoldingUnsheatheRangedWeaponButton(void) const; // slot65 0x008905b0
virtual bool IsPlayerHoldingLockTargetButton(void) const; // slot66 0x008905e0
virtual bool IsPlayerHoldingFireRangedWeaponButton(void) const; // slot67 0x00890610
virtual bool IsPlayerHoldingFirstPersonTargetingButton(void) const; // slot68 0x00890640
virtual bool IsHeroInProjectileWeaponMode(void) const; // slot69 0x00890670
virtual CScriptThing & GetHero(void); // slot70 0x00891ca0
virtual CScriptThing GetHeroTargetedThing(void) const; // slot71 0x0089a490
virtual CScriptThing GetThingWithScriptName(CCharString const &, CCharString const &) const; // slot73 0x008930f0
virtual CScriptThing GetThingWithScriptName(CCharString const &) const; // slot72 0x008a7d60
virtual CScriptThing GetRandomThingWithScriptName(CCharString const &) const; // slot74 0x008a7e00
virtual long GetAllThingsWithScriptName(CCharString const &, std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; // slot75 0x008a8570
virtual long GetAllCreaturesInAreaWithScriptName(CCharString const &, C3DVector const &, float, std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; // slot76 0x008a86c0
virtual CScriptThing GetNearestWithScriptName(CScriptThing const &, CCharString const &) const; // slot77 0x0089a510
virtual CScriptThing GetFurthestWithScriptName(CScriptThing const &, CCharString const &) const; // slot78 0x0089a6e0
virtual long GetAllThingsWithDefNameByDistanceFrom(CScriptThing const &, CCharString const &, std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; // slot79 0x008a9c40
virtual long GetAllThingsWithDefName(CCharString const &, std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; // slot80 0x008a8890
virtual CScriptThing GetNearestWithDefName(CScriptThing const &, CCharString const &) const; // slot81 0x0089a880
virtual CScriptThing GetFurthestWithDefName(CScriptThing const &, CCharString const &) const; // slot82 0x0089aa70
virtual CScriptThing GetThingWithUID(ScriptUID) const; // slot83 0x00891d50
virtual long GetAllCreaturesExcludingHero(std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; // slot84 0x008a89d0
virtual long GetAllThingsInLevel(CCharString const &, std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; // slot85 0x008a8af0
virtual bool IsThingWithThisUIDAlive(ScriptUID) const; // slot86 0x0088e260
virtual long RegisterTimer(void); // slot87 0x006e79e0
virtual void DeregisterTimer(long); // slot88 0x006e7690
virtual void SetTimer(long, long); // slot89 0x006e76e0
virtual long GetTimer(long) const; // slot90 0x006e7710
virtual CScriptThing CreateCreature(CCharString const &, C3DVector const &, CCharString const &, bool) const; // slot91 0x008a9100
virtual CScriptThing CreateCreatureNearby(CCharString const &, C3DVector const &, float, CCharString const &, bool) const; // slot92 0x0089f300
virtual CScriptThing CreateCreatureOnEntity(CCharString const &, CScriptThing const &, CCharString const &) const; // slot93 0x00894500
virtual CScriptThing TurnCreatureInto(CScriptThing const &, CCharString const &) const; // slot94 0x0089f4a0
virtual void SetCreatureCreationDelayFrames(long); // slot95 0x006e7d30
virtual void ResetCreatureCreationDelayFrames(void); // slot96 0x006e7d40
virtual CScriptThing CreateObject(CCharString const &, C3DVector const &, CCharString const &) const; // slot98 0x00894820
virtual CScriptThing CreateObject(CCharString const &, C3DVector const &, float, CCharString const &) const; // slot97 0x00894950
virtual CScriptThing CreateObjectOnEntity(CCharString const &, CScriptThing const &, CCharString const &) const; // slot99 0x008946e0
};
class CScriptBase; class CPersistContext;
class CEntityScriptBase { public:
 virtual ~CEntityScriptBase();
 virtual void Main();
 virtual void Init();
 virtual CScriptBase* GetParentScript()const;
 virtual void OnPersist(CPersistContext&);
 virtual void OnPredicateFail();
 virtual void OnInterrupted();
 CGameScriptInterface* Game;
 CScriptThing Me;
 bool IsActiveThreadTerminating()const; // 00F35B30: thread termination only
};
struct CV_FishermanScript {unsigned char BaseScript[0x48];bool IntroPlayed;bool HeroHasSavedFishermanFromWasps;}; // donor5C/5D, retail48/49
class CDefString { public:void* Data;operator CCharString()const;}; // 00415D70
class CScriptDef { public:unsigned char BeforeWaspType[0x598];CDefString F_WaspType;}; // donor5E4,retail598
extern CScriptDef* g_ScriptDef; // 0143E90C
struct CM_FishermanWaspSpawn : CEntityScriptBase {
 CV_FishermanScript* ParentClass;void* PMasterData;
 virtual void Main();
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CScriptThing)==sizeof(CScriptThingDonor));
FABLE_STATIC_ASSERT(sizeof(CEntityScriptBase)==20);
FABLE_STATIC_ASSERT(sizeof(CM_FishermanWaspSpawn)==28);
FABLE_STATIC_ASSERT(offsetof(CM_FishermanWaspSpawn,ParentClass)==0x14);
FABLE_STATIC_ASSERT(sizeof(void (CM_FishermanWaspSpawn::*)())==4);
#endif
void CM_FishermanWaspSpawn::Main(){
 while(!ParentClass->IntroPlayed){Game->NewScriptFrame();if(IsActiveThreadTerminating())return;}
 if(IsActiveThreadTerminating())return;
 if(!ParentClass->HeroHasSavedFishermanFromWasps && !IsActiveThreadTerminating()){
 CCharString name("FishermanWasp");
 Game->CreateCreature(g_ScriptDef->F_WaspType,Me.GetPos(),name,false);
 }
}