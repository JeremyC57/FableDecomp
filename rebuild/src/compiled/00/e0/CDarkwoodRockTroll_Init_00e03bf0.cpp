// CDarkwoodRockTroll::Init -- reveal troll, update objective and install health bar and map marker.
// Retail00E03BF0 length278. Corrected full845 Game interface from root142 signature audit.
// No generated leaf/parent/colour headers. PDB parent ShownEarthTroll+85 is native+55 (script base smaller30).
// Native colour order B,G,R,A; red opaque. All called Game slot signatures retained.
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
#include "fable_string.h"
// Donor CThing is quarantined: donor Components+48/lifecycle groups+a1..a4 differ from retail Components+44/LifeFlags+91.
// Include its actual declaration for provenance; the portable native projection below retains retail field boundaries.
#define CThing CThingDonorLayout
#include "engine/_quarantine/CThing.h"
#undef CThing

struct C3DVector{float X,Y,Z;};
struct ComponentEntry{long Type;void* Component;};struct ComponentMap{ComponentEntry *Start,*Finish;ComponentEntry* LowerBound(const long&);}; //0040F020
struct ScriptMethods;
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components; unsigned char ComponentListsAndMetadata[0x45];unsigned char LifeFlags;}; //004C73D0,004C7E50
FABLE_STATIC_ASSERT(offsetof(CThingNative,UsingComponents)==0x20);
FABLE_STATIC_ASSERT(offsetof(CThingNative,Components)==0x44);
FABLE_STATIC_ASSERT(offsetof(CThingNative,LifeFlags)==0x91);
// Complete native CScriptThing virtual interface: all slots0..78, PDB signatures mapped to actual retail RTTI table1238C8C.
// Generated engine header flattens vptr; this projection restores its real virtual ABI, with member layout checked against that header.
namespace std {template<class T>class allocator;template<class T,class A>class vector;}
typedef long EAIStateGroupTypeABI;typedef long EScriptAIPriorityABI;typedef long EHeroAbilityABI; // enum ABI declarations only
struct CBaseClassNative{CBaseClassNative(){}virtual ~CBaseClassNative();}; //0099A2D0 ctor,0099A2E0 dtor,VT01231710
struct CCPPointerInfo{unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void* Data;}; // actual PDB3field layout, no generated header
struct CountedScriptImp{CScriptThing* Data;CCPPointerInfo* Info;CountedScriptImp():Data(0),Info(0){}CountedScriptImp(const CountedScriptImp& other):Data(other.Data),Info(other.Info){if(Info)++Info->RefCount;}~CountedScriptImp(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Data=0;Info=0;}};
struct ScriptMethods:CBaseClassNative{
 CountedScriptImp PImp;
 ScriptMethods(){}
 ScriptMethods(const ScriptMethods& other):CBaseClassNative(),PImp(other.PImp){}
 virtual ~ScriptMethods(){}
 CThingNative* GetThing() const {if(!PImp.Data)return 0;return reinterpret_cast<ScriptMethods*>(PImp.Data)->GetPThing();}
virtual const CCharString& GetName() const; //slot1 0x004AA8B0
virtual CCharString GetDefName() const; //slot2 0x004AA8D0
virtual CCharString GetDataString() const; //slot3 0x004AA900
virtual void SetDataString(const CCharString&); //slot4 0x004AA930
virtual C3DVector GetFocalPos() const; //slot5 0x004AA940
virtual const C3DVector& GetPos() const; //slot6 0x004AA980
virtual C3DVector GetHomePos() const; //slot7 0x004AA9A0
virtual CCharString GetCurrentMapName() const; //slot8 0x004AA9E0
virtual CCharString GetHomeMapName() const; //slot9 0x004AAA10
virtual float GetAngleXY() const; //slot10 0x004AAA40
virtual CThingNative* GetPThing() const; //slot11 0x004AAA60
virtual unsigned __int64 GetPThingUniqueID() const; //slot12 0x004AAA70
virtual EAIStateGroupTypeABI GetCurrentStateGroupType() const; //slot13 0x004AAA90
virtual EScriptAIPriorityABI GetCurrentScriptPriority() const; //slot14 0x004AAAA0
virtual bool IsSneaking() const; //slot15 0x004AAAB0
virtual bool IsAwareOfHero() const; //slot16 0x004AAAC0
virtual bool MsgIsTriggeredBy(const CCharString&) const; //slot17 0x004AAAD0
virtual bool MsgIsKilledBy(const CCharString&) const; //slot18 0x004AAAF0
virtual ScriptMethods MsgWhoKilledMe() const; //slot19 0x004ABE60
virtual bool MsgIsUsedBy(const CCharString&) const; //slot20 0x004AAB10
virtual bool MsgIsHitBy(const CCharString&) const; //slot21 0x004AAB30
virtual bool MsgIsHitByWithFlourish(const CCharString&) const; //slot22 0x004AAB50
virtual bool MsgIsHitByWithDecapitate(const CCharString&) const; //slot23 0x004AAB70
virtual bool MsgIsHitByWithWeapon(const CCharString&,const CCharString&) const; //slot24 0x004AAB90
virtual bool MsgIsHitByWithProjectileWeapon(const CCharString&,float&) const; //slot25 0x004AABB0
virtual ScriptMethods MsgWhoHitMe() const; //slot26 0x004ABEC0
virtual bool MsgIsTalkedToBy(const CCharString&) const; //slot27 0x004AABD0
virtual ScriptMethods MsgWhoTalkedToMe() const; //slot28 0x004ABF00
virtual bool MsgExpressionPerformedTo(CCharString&) const; //slot29 0x004AABF0
virtual ScriptMethods MsgWhoExpressedToMe() const; //slot30 0x004ABF40
virtual long MsgHowLongWasExpressionPerformed() const; //slot31 0x004AAC10
virtual bool MsgIsKnockedOutBy(const CCharString&) const; //slot32 0x004AAC20
virtual ScriptMethods MsgWhoKnockedMeOut() const; //slot33 0x004ABF80
virtual bool MsgReceivedMoney(long&) const; //slot34 0x004AAC40
virtual bool MsgIsPresentedWithItem(CCharString&) const; //slot35 0x004AAC60
virtual bool MsgReceivedInventoryItem() const; //slot36 0x004AAC80
virtual bool MsgPerformedSpecialAbility(EHeroAbilityABI) const; //slot37 0x004AACA0
virtual bool MsgPerformedAnySpecialAbility() const; //slot38 0x004AACC0
virtual bool MsgPerformedAnyAggressiveSpecialAbility() const; //slot39 0x004AACE0
virtual bool MsgPerformedAnyNonAggressiveSpecialAbility() const; //slot40 0x004AAD00
virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbilityABI,const CCharString&) const; //slot41 0x004AAD20
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
virtual bool IsEqualTo(const ScriptMethods&) const; //slot78 0x004AA8A0
};
FABLE_STATIC_ASSERT(sizeof(ScriptMethods)==sizeof(CScriptThing));
FABLE_STATIC_ASSERT(offsetof(ScriptMethods,PImp)==offsetof(CScriptThing,PImp_Data));
FABLE_STATIC_ASSERT(offsetof(ScriptMethods,PImp)+offsetof(CountedScriptImp,Info)==offsetof(CScriptThing,PImp_Info));

struct C2DCoordI;
struct C2DVector;
struct C3DVector;
struct CCharString;
struct CRGBColour;
struct CRGBFloatColour;
struct CRightHandedSet;
struct CScreenFilterSThingByPass;
struct CScriptGameResourceObjectMovieBase;
struct CScriptGameResourceObjectScriptedThingBase;
struct CThing;
struct CTimer;
struct CWideString;
template<class T> struct CArray;
struct CCounter;
namespace std {template<class T> class allocator;template<class T> struct less;template<class A,class B>struct pair;template<class T,class A> class vector;template<class T,class A> class list;template<class T,class P,class A> class set;}
struct CHeroLogBook { struct CHeroLogBookEntry { typedef long ECategory; }; };
typedef long ECameraOp;
typedef long EClothingCoversArea;
typedef long ECreatureGroup;
typedef long ECrime;
typedef long ECutsceneBehaviour;
typedef long EDoorTriggerType;
typedef long EGameAction;
typedef long EHeroAbility;
typedef long EHeroTitle;
typedef long EHeroTrainableStatType;
typedef long EMiniGameType;
typedef long EMorality;
typedef long EMusicSetType;
typedef long EOpinion;
typedef long EOpinionAttitudeType;
typedef long EOpinionDeedType;
typedef long EOpinionPostDeedType;
typedef long EOpinionReactionType;
typedef long EScriptAIPriority;
typedef long EScriptVillageAttitude;
typedef long EScriptingStateGroups;
typedef long ESex;
typedef long ETargetingType;
typedef long ETextGroupSelectionMethod;
typedef long ETutorialCategory;
namespace NEntityEvents {typedef long EEventType;}
struct GameMethods {virtual ~GameMethods();
virtual void Error(CCharString const &, CCharString const &, CCharString const &, unsigned long) const; //slot1 0x0088de00
virtual void TraceMessage(CCharString const &) const; //slot2 0x006e7d20
virtual void Validate(void) const; //slot3 0x006e72a0
virtual void SetDebugCameraType(long) const; //slot4 0x0088e0f0
virtual void DeactivateBoastUi(void) const; //slot5 0x008a0c60
virtual bool IsXbox(void) const; //slot6 0x0088e100
virtual void NewScriptFrame(void) const; //slot7 0x006e7410
virtual bool StartScriptingEntity(ScriptMethods const &, CScriptGameResourceObjectScriptedThingBase &, EScriptAIPriority); //slot8 0x0089b5b0
virtual bool IsEntityUnderScriptedControl(ScriptMethods const &) const; //slot9 0x008960c0
virtual bool IsActiveThreadTerminating(void) const; //slot10 0x006e71b0
virtual bool IsLevelLoaded(CCharString const &) const; //slot11 0x0088e2a0
virtual bool IsRegionLoaded(CCharString const &) const; //slot12 0x0088e300
virtual bool IsRegionLoadedAndPreloaded(CCharString const &) const; //slot13 0x00890550
virtual bool IsRegionDefLoaded(CCharString const &) const; //slot14 0x00891db0
virtual CCharString const & GetRegionName(void) const; //slot15 0x0088e340
virtual bool MsgIsLevelLoaded(CCharString const &) const; //slot16 0x008931b0
virtual bool MsgIsLevelUnloaded(CCharString const &) const; //slot17 0x00893240
virtual bool MsgOnLevelLoaded(std::list<CCharString, std::allocator<CCharString> > &) const; //slot18 0x0089ac10
virtual bool MsgOnLevelUnloaded(std::list<CCharString, std::allocator<CCharString> > &) const; //slot19 0x0089ac80
virtual bool MsgIsRegionLoaded(CCharString const &) const; //slot20 0x008932d0
virtual bool MsgIsRegionUnloaded(CCharString const &) const; //slot21 0x00893360
virtual bool MsgOnRegionLoaded(CCharString &) const; //slot22 0x008933f0
virtual bool MsgOnRegionUnloaded(CCharString &) const; //slot23 0x00893470
virtual bool MsgOnRegionPreunload(CCharString &) const; //slot24 0x008934f0
virtual bool MsgOnQuestCompleted(CCharString const &) const; //slot25 0x00893570
virtual bool MsgOnAnyQuestCompleted(CCharString &) const; //slot26 0x00893610
virtual bool MsgOnQuestFailed(CCharString const &) const; //slot27 0x00893690
virtual bool MsgOnQuestCompletedBeforeScreenShown(CCharString const &) const; //slot28 0x00893730
virtual bool MsgOnQuestFailedBeforeScreenShown(CCharString const &) const; //slot29 0x008937d0
virtual bool MsgOnQuestAccepted(CCharString &) const; //slot30 0x00893870
virtual bool MsgOnFeatAccepted(long &) const; //slot31 0x008938f0
virtual bool MsgIsBoastMade(long) const; //slot32 0x00893960
virtual bool MsgOnBoastMade(long &, CCharString &) const; //slot33 0x008939d0
virtual bool MsgOnBoastsMade(std::vector<std::pair<long, CCharString>, std::allocator<std::pair<long, CCharString> > > &) const; //slot34 0x008a9ae0
virtual void RemoveBoastMessage(void) const; //slot35 0x00893a50
virtual bool IsQuestStartScreenActive(CCharString &) const; //slot36 0x0089ad90
virtual bool MsgOnLeavingQuestStartScreen(CCharString &) const; //slot37 0x0089ae30
virtual bool MsgOnLeavingExperienceSpendingScreen(void) const; //slot38 0x00893ab0
virtual long MsgIsQuestionAnsweredYesOrNo(void) const; //slot39 0x00894300
virtual bool MsgIsGameInfoClickedPast(void) const; //slot40 0x00894370
virtual bool MsgIsTutorialClickedPast(void) const; //slot41 0x008943c0
virtual bool MsgIsActionModeButtonPressed(void) const; //slot42 0x00894440
virtual bool MsgOnExpressionPerformed(CCharString &) const; //slot43 0x00893b00
virtual bool MsgIsCutSceneSkipped(void) const; //slot44 0x00893ba0
virtual void RemoveAllCutSceneSkippedMessages(void) const; //slot45 0x0089af30
virtual bool MsgOnHeroHairTypeChanged(EClothingCoversArea, CCharString &) const; //slot46 0x00893c00
virtual bool MsgOnHeroUsedTeleporter(CCharString &) const; //slot47 0x00893d80
virtual bool MsgOnHeroUsedGuildSeal(void) const; //slot48 0x00893e20
virtual bool MsgOnGameSavedManually(void) const; //slot49 0x00893e70
virtual bool MsgOnHeroSlept(bool &) const; //slot50 0x00893ec0
virtual bool MsgOnHeroFiredRangedWeapon(void) const; //slot51 0x00893f30
virtual bool MsgOnHeroCastSpell(EHeroAbility &) const; //slot52 0x00893f80
virtual bool MsgOnHeroPickedPocket(ScriptMethods &) const; //slot53 0x00893ff0
virtual bool MsgOnHeroPickedLock(ScriptMethods &) const; //slot54 0x00894080
virtual bool MsgOnFishingGameFinished(ScriptMethods &) const; //slot55 0x00894110
virtual bool MsgOnTavernGameFinished(ScriptMethods const &) const; //slot56 0x008941a0
virtual bool MsgOnHeroRewardedWithItemsFrom(CCharString &) const; //slot57 0x00894250
virtual bool MsgOnChestOpeningCancelled(void) const; //slot58 0x008942c0
virtual void DontPopulateNextLoadedRegion(void) const; //slot59 0x0088e380
virtual void GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString &) const; //slot60 0x0088e390
virtual void GetWanderingPopulationScriptDefNameInRegion(CCharString const &, CCharString &) const; //slot61 0x00891e30
virtual bool IsHeroAllowedHenchmenInCurrentRegion(void) const; //slot62 0x00891ea0
virtual bool IsHeroAllowedHenchmenInRegion(CCharString const &) const; //slot63 0x00891ef0
virtual void PostAddScriptedEntities(void) const; //slot64 0x006e7460
virtual bool IsPlayerHoldingUnsheatheRangedWeaponButton(void) const; //slot65 0x008905b0
virtual bool IsPlayerHoldingLockTargetButton(void) const; //slot66 0x008905e0
virtual bool IsPlayerHoldingFireRangedWeaponButton(void) const; //slot67 0x00890610
virtual bool IsPlayerHoldingFirstPersonTargetingButton(void) const; //slot68 0x00890640
virtual bool IsHeroInProjectileWeaponMode(void) const; //slot69 0x00890670
virtual ScriptMethods & GetHero(void); //slot70 0x00891ca0
virtual ScriptMethods GetHeroTargetedThing(void) const; //slot71 0x0089a490
virtual ScriptMethods GetThingWithScriptName(CCharString const &, CCharString const &) const; //slot73 0x008930f0
virtual ScriptMethods GetThingWithScriptName(CCharString const &) const; //slot72 0x008a7d60
virtual ScriptMethods GetRandomThingWithScriptName(CCharString const &) const; //slot74 0x008a7e00
virtual long GetAllThingsWithScriptName(CCharString const &, std::vector<ScriptMethods, std::allocator<ScriptMethods> > &) const; //slot75 0x008a8570
virtual long GetAllCreaturesInAreaWithScriptName(CCharString const &, C3DVector const &, float, std::vector<ScriptMethods, std::allocator<ScriptMethods> > &) const; //slot76 0x008a86c0
virtual ScriptMethods GetNearestWithScriptName(ScriptMethods const &, CCharString const &) const; //slot77 0x0089a510
virtual ScriptMethods GetFurthestWithScriptName(ScriptMethods const &, CCharString const &) const; //slot78 0x0089a6e0
virtual long GetAllThingsWithDefNameByDistanceFrom(ScriptMethods const &, CCharString const &, std::vector<ScriptMethods, std::allocator<ScriptMethods> > &) const; //slot79 0x008a9c40
virtual long GetAllThingsWithDefName(CCharString const &, std::vector<ScriptMethods, std::allocator<ScriptMethods> > &) const; //slot80 0x008a8890
virtual ScriptMethods GetNearestWithDefName(ScriptMethods const &, CCharString const &) const; //slot81 0x0089a880
virtual ScriptMethods GetFurthestWithDefName(ScriptMethods const &, CCharString const &) const; //slot82 0x0089aa70
virtual ScriptMethods GetThingWithUID(unsigned __int64) const; //slot83 0x00891d50
virtual long GetAllCreaturesExcludingHero(std::vector<ScriptMethods, std::allocator<ScriptMethods> > &) const; //slot84 0x008a89d0
virtual long GetAllThingsInLevel(CCharString const &, std::vector<ScriptMethods, std::allocator<ScriptMethods> > &) const; //slot85 0x008a8af0
virtual bool IsThingWithThisUIDAlive(unsigned __int64) const; //slot86 0x0088e260
virtual long RegisterTimer(void); //slot87 0x006e79e0
virtual void DeregisterTimer(long); //slot88 0x006e7690
virtual void SetTimer(long, long); //slot89 0x006e76e0
virtual long GetTimer(long) const; //slot90 0x006e7710
virtual ScriptMethods CreateCreature(CCharString const &, C3DVector const &, CCharString const &, bool) const; //slot91 0x008a9100
virtual ScriptMethods CreateCreatureNearby(CCharString const &, C3DVector const &, float, CCharString const &, bool) const; //slot92 0x0089f300
virtual ScriptMethods CreateCreatureOnEntity(CCharString const &, ScriptMethods const &, CCharString const &) const; //slot93 0x00894500
virtual ScriptMethods TurnCreatureInto(ScriptMethods const &, CCharString const &) const; //slot94 0x0089f4a0
virtual void SetCreatureCreationDelayFrames(long); //slot95 0x006e7d30
virtual void ResetCreatureCreationDelayFrames(void); //slot96 0x006e7d40
virtual ScriptMethods CreateObject(CCharString const &, C3DVector const &, CCharString const &) const; //slot98 0x00894820
virtual ScriptMethods CreateObject(CCharString const &, C3DVector const &, float, CCharString const &) const; //slot97 0x00894950
virtual ScriptMethods CreateObjectOnEntity(CCharString const &, ScriptMethods const &, CCharString const &) const; //slot99 0x008946e0
virtual ScriptMethods CreateEffect(CCharString const &, ScriptMethods const &, CCharString const &, CCharString const &, bool, bool) const; //slot101 0x0089f910
virtual ScriptMethods CreateEffect(CCharString const &, C3DVector const &, CCharString const &, float, bool, bool) const; //slot100 0x0089f9e0
virtual ScriptMethods CreateLight(C3DVector const &, CRGBColour const &, CCharString const &, float, float, bool) const; //slot102 0x0089faa0
virtual ScriptMethods CreateExperienceOrb(C3DVector const &, long) const; //slot103 0x0089fbb0
virtual ScriptMethods CreateExplosion(CCharString const &, C3DVector const &, C3DVector const *, CCharString) const; //slot104 0x0089fc80
virtual ScriptMethods CreatePhysicalBarrier(float, C3DVector const &, C3DVector const *, CCharString) const; //slot105 0x0089fd20
virtual ScriptMethods CreateRumble(C3DVector const &, float, float, CCharString) const; //slot106 0x0089fda0
virtual void ClearAllRumbles(void) const; //slot107 0x00892df0
virtual void RemoveThing(ScriptMethods const &, bool, bool) const; //slot108 0x008910d0
virtual void ShowOnScreenMessage(CCharString const &, float) const; //slot111 0x00892780
virtual void ShowOnScreenMessage(CCharString const &, CCharString const &, CCharString const &, float) const; //slot110 0x008927b0
virtual void ShowOnScreenMessage(C2DVector const &, CCharString const &, CRGBColour const &, CCharString const &) const; //slot109 0x00892810
virtual void AddScreenMessage(CCharString const &, ETextGroupSelectionMethod) const; //slot112 0x00892850
virtual void AddScreenTitleMessage(CCharString const &, float, bool) const; //slot113 0x0089e4c0
virtual void GiveHeroYesNoQuestion(CCharString const &, CCharString const &, CCharString const &, CCharString const &, bool) const; //slot114 0x0089e540
virtual void DisplayGameInfo(CCharString const &) const; //slot115 0x008929d0
virtual void DisplayGameInfoText(CWideString const &) const; //slot116 0x00892a80
virtual bool IsSafeToDisplayGameInfo(void) const; //slot117 0x00892b30
virtual bool DisplayTutorial(ETutorialCategory) const; //slot118 0x0089e710
virtual bool IsTutorialSystemEnabled(void) const; //slot119 0x0088f3e0
virtual void GiveHeroWeapon(CCharString const &, bool) const; //slot120 0x0088e130
virtual void GiveHeroObject(CCharString const &, long, bool) const; //slot121 0x008902e0
virtual void SetWeaponAsHerosActiveWeapon(CCharString const &) const; //slot122 0x00898b30
virtual void GiveHeroItem(ScriptMethods const &) const; //slot123 0x00898bb0
virtual bool GiveHeroItemsFromContainer(ScriptMethods const &, bool) const; //slot124 0x00898c40
virtual void TakeObjectFromHero(CCharString const &) const; //slot125 0x00898db0
virtual void GiveHeroGold(long) const; //slot126 0x00898f40
virtual long GetHeroGold(void) const; //slot127 0x00898fa0
virtual void GiveHeroExperience(long) const; //slot128 0x00899000
virtual void SetHeroAbleToGainExperience(bool) const; //slot129 0x00899060
virtual void SheatheHeroWeapons(void) const; //slot130 0x008916a0
virtual void SetHeroWillAsUsable(bool) const; //slot131 0x00898340
virtual void SetHeroWeaponsAsUsable(bool) const; //slot132 0x00898410
virtual void SetWeaponOutCrimeEnabled(bool) const; //slot133 0x008984e0
virtual void SetGuardsIgnoreCrimes(bool) const; //slot134 0x00898540
virtual void RemoveAllHeroWeapons(void) const; //slot135 0x00898600
virtual bool IsReportedOrUnreportedCrimeKnown(ScriptMethods const &) const; //slot136 0x008985a0
virtual void ConfiscateAllHeroItems(void) const; //slot137 0x008986d0
virtual void ConfiscateAllHeroWeapons(void) const; //slot138 0x008987e0
virtual void ConfiscateItemsOfTypeFromHero(CCharString const &) const; //slot139 0x00898840
virtual void ReturnAllConfiscatedItemsToHero(void) const; //slot140 0x00898940
virtual bool GiveHeroTutorial(ETutorialCategory) const; //slot141 0x00898a50
virtual void MakeHeroCarryItemInHand(CCharString const &) const; //slot143 0x00897e10
virtual void MakeHeroCarryItemInHand(ScriptMethods const &, bool, bool) const; //slot142 0x00897fa0
virtual void AddTattooToHero(CCharString const &) const; //slot144 0x008981c0
virtual void GiveHeroAbility(EHeroAbility, bool) const; //slot145 0x0088fcc0
virtual bool IsPlayerZTargetingThing(ScriptMethods const &) const; //slot146 0x00899b10
virtual bool IsPlayerCreatureBlocking(void) const; //slot147 0x00899ba0
virtual bool IsPlayerCreatureReadyToFireProjectileWeapon(float &) const; //slot148 0x00899c10
virtual long GetPlayerCreatureCombatMultiplier(void) const; //slot149 0x00899c90
virtual long GetPlayerCreatureCombatMultiplierRunningNumHits(void) const; //slot150 0x00899cf0
virtual void ResetPlayerCreatureCombatMultiplier(void) const; //slot151 0x00899d50
virtual bool IsPlayerCreatureFlourishEnabled(void) const; //slot152 0x00899db0
virtual void SetPlayerCreatureOnlyTarget(ScriptMethods const &) const; //slot153 0x00899e10
virtual void ResetPlayerCreatureOnlyTarget(void) const; //slot154 0x00899e90
virtual void RespawnHero(bool) const; //slot155 0x0088e150
virtual void GiveHeroMorality(float) const; //slot156 0x00899ef0
virtual float GetHeroMorality(void) const; //slot157 0x00899f90
virtual EMorality GetHeroMoralityCategory(void) const; //slot158 0x0089a000
virtual void ChangeHeroMoralityDueToTheft(void) const; //slot159 0x0089a070
virtual void ChangeHeroMoralityDueToPicklock(void) const; //slot160 0x0089a0e0
virtual void GiveHeroRenownPoints(long) const; //slot161 0x008990c0
virtual long GetHeroRenownLevel(void) const; //slot162 0x00899140
virtual bool IsHeroRenownLevelFull(void) const; //slot163 0x008991a0
virtual void IncreaseHeroRenownLevel(void) const; //slot164 0x00899220
virtual long GetHeroStrengthLevel(void) const; //slot165 0x00899390
virtual long GetHeroSkillLevel(void) const; //slot166 0x00899400
virtual long GetHeroWillLevel(void) const; //slot167 0x00899470
virtual long GetHeroStatLevel(EHeroTrainableStatType) const; //slot168 0x00899290
virtual long GetHeroStatMax(EHeroTrainableStatType) const; //slot169 0x00899310
virtual void SetHeroAge(float) const; //slot170 0x008994e0
virtual float GetHeroAge(void) const; //slot171 0x00899560
virtual void SetHeroAsTeenager(bool) const; //slot172 0x008995d0
virtual void SetHeroAsApprentice(bool) const; //slot173 0x00899680
virtual float GetDistanceHeroCanBeHeardFrom(void) const; //slot174 0x00899700
virtual long GetHeroRoughExperienceLevel(void) const; //slot175 0x00899770
virtual long GetHeroExperienceAvailableToSpend(void) const; //slot176 0x008997e0
virtual float GetHeroFatness(void) const; //slot177 0x00899850
virtual float GetHeroScariness(void) const; //slot178 0x008998c0
virtual float GetHeroAttractiveness(void) const; //slot179 0x00899930
virtual float GetHeroWillEnergyLevel(void) const; //slot180 0x008999a0
virtual void SetHeroWillEnergyLevel(float) const; //slot181 0x00899a10
virtual void SetHeroWillEnergyAsAbleToRefill(bool) const; //slot182 0x00899a90
virtual long GetNumberOfItemsOfTypeInInventory(CCharString const &) const; //slot183 0x00897190
virtual bool IsObjectInThingsPossession(CCharString const &, ScriptMethods const &) const; //slot184 0x00897220
virtual bool IsHeroHandLampLit(void) const; //slot185 0x00898e60
virtual void SetHeroHandLampAsLit(bool) const; //slot186 0x00898ec0
virtual bool IsWearingClothingItem(ScriptMethods const &, CCharString const &) const; //slot187 0x0089b2b0
virtual bool IsHeroNaked(void) const; //slot188 0x0089b330
virtual void RemoveHeroClothing(void) const; //slot189 0x0089b390
virtual void SetHeroAsWearing(CCharString const &) const; //slot190 0x0089b3f0
virtual void ChangeHeroHairstyle(CCharString const &) const; //slot191 0x0089b450
virtual void RemoveHeroHairstyle(void) const; //slot192 0x0089b4d0
virtual bool IsWearingHairstyle(ScriptMethods &, CCharString const &) const; //slot193 0x0089b530
virtual bool IsPlayerCarryingItemOfType(CCharString const &) const; //slot194 0x008973a0
virtual bool IsPlayerWieldingWeapon(void) const; //slot195 0x00897480
virtual bool IsEntityWieldingWeapon(ScriptMethods const &) const; //slot196 0x008974f0
virtual bool IsEntityWieldingMeleeWeapon(ScriptMethods const &) const; //slot197 0x00890220
virtual bool IsEntityWieldingRangedWeapon(ScriptMethods const &) const; //slot198 0x00890250
virtual bool GetPreviouslyWieldedMeleeWeaponName(CCharString &) const; //slot199 0x00897560
virtual bool GetPreviouslyWieldedRangedWeaponName(CCharString &) const; //slot200 0x00897630
virtual bool IsEntityAbleToAttack(ScriptMethods const &) const; //slot201 0x00890280
virtual ScriptMethods EntityGetThingInPrimarySlot(ScriptMethods const &) const; //slot202 0x008976f0
virtual void ApplyHeroPenaltyForDeath(void) const; //slot203 0x008977f0
virtual void GiveHeroTitle(CCharString const &) const; //slot204 0x0088e110
virtual EHeroTitle GetHeroTitle(void) const; //slot205 0x00897880
virtual void EntitySetAsMarryable(ScriptMethods const &, bool) const; //slot206 0x00894dd0
virtual void EntitySetAsAbleToRegionFollowWhenMarried(ScriptMethods const &, bool) const; //slot207 0x008978e0
virtual void EntityForceMarriageToHero(ScriptMethods const &, bool) const; //slot208 0x00897940
virtual bool IsEntityMarriedToHero(ScriptMethods const &) const; //slot209 0x008979c0
virtual bool IsEntityMarriable(ScriptMethods const &) const; //slot210 0x00897a30
virtual bool GetHeroHasMarried(void) const; //slot211 0x00897aa0
virtual bool GetHeroHasCurrentMarriage(void) const; //slot212 0x00897b10
virtual bool GetHeroHasDivorcedMarriage(void) const; //slot213 0x00897b70
virtual bool GetHeroHasChildren(void) const; //slot214 0x00897bd0
virtual bool GetHeroHasMurderedWife(void) const; //slot215 0x00897c30
virtual bool IsHeroChild(void) const; //slot216 0x008902b0
virtual void CancelHeroTeleportEffects(void) const; //slot217 0x008a0820
virtual long GetNumberOfTimesHeroHasHadSex(void) const; //slot218 0x00897c90
virtual void SetNumberOfTimesHeroHasHadSex(long) const; //slot219 0x00897cf0
virtual void SetHeroAsHavingHadSex(bool) const; //slot220 0x00897d50
virtual void SetHeroAsHavingHadGaySex(bool) const; //slot221 0x00897db0
virtual void GiveThingHeroRewardItem(ScriptMethods const &, CCharString const &, CCharString const &) const; //slot222 0x00898ac0
virtual void GiveThingItemInHand(ScriptMethods const &, CCharString const &, bool) const; //slot223 0x0088de10
virtual void GiveThingItemInSlot(ScriptMethods const &, CCharString const &, CCharString const &) const; //slot224 0x00895190
virtual void GiveHeroExpression(CCharString const &, long, bool) const; //slot225 0x0088fc60
virtual bool HeroHasExpression(CCharString const &) const; //slot226 0x0088fc90
virtual bool IsHeroPerformingExpression(CCharString const &) const; //slot227 0x008953a0
virtual bool IsDPadButtonHeldForExpression(CCharString const &) const; //slot228 0x00895430
virtual void EntityFollowThing(ScriptMethods const &, ScriptMethods const &, float, bool) const; //slot229 0x008954a0
virtual void EntityStopFollowing(ScriptMethods const &) const; //slot230 0x008a7980
virtual bool GetFollowingEntityList(ScriptMethods const &, std::vector<ScriptMethods, std::allocator<ScriptMethods> > &) const; //slot231 0x008a80f0
virtual bool GetHeroSummonedCreaturesList(std::vector<ScriptMethods, std::allocator<ScriptMethods> > &) const; //slot232 0x008a8260
virtual bool GetPerceivingHeroEntityList(std::vector<ScriptMethods, std::allocator<ScriptMethods> > &) const; //slot233 0x008a8440
virtual bool IsEntityFollowingHero(ScriptMethods const &) const; //slot234 0x008955c0
virtual void EntitySetAsAllowedToFollowHero(ScriptMethods const &, bool) const; //slot235 0x0088de90
virtual void EntitySetAsAllowedToChangeRegionFollowingState(ScriptMethods const &, bool) const; //slot236 0x0088ded0
virtual void EntitySetAsRespondingToFollowAndWaitExpressions(ScriptMethods const &, bool) const; //slot237 0x0088df20
virtual void EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(ScriptMethods const &, bool) const; //slot238 0x00895650
virtual void TeleportAllFollowersToHeroPosition(void) const; //slot239 0x008a7a70
virtual void EntityTeleportToHeroPosition(ScriptMethods const &) const; //slot240 0x008956b0
virtual void SendEntityEvent(NEntityEvents::EEventType, ScriptMethods const &, ScriptMethods const &, CThing *) const; //slot241 0x008957c0
virtual float GetWaterHeightAtPosition(C3DVector const &) const; //slot242 0x0088df60
virtual bool IsFishingSpotEnabled(ScriptMethods const &) const; //slot243 0x008958f0
virtual void DisableFishingSpot(ScriptMethods const &) const; //slot244 0x00895950
virtual void UpdateFishWeight(ScriptMethods const &) const; //slot245 0x008959b0
virtual float GetBestFishWeight(CCharString const &) const; //slot246 0x00895a10
virtual void HeroGoFishing(bool) const; //slot247 0x00895a90
virtual long GetHeroFishingLevel(void) const; //slot248 0x00895b00
virtual void HeroGoDigging(void) const; //slot249 0x0088df80
virtual void HeroStopDigging(void) const; //slot250 0x0088dfa0
virtual void HeroPlayOracleMinigame(void) const; //slot251 0x00895b60
virtual bool IsHeroPlayingOracleMinigame(void) const; //slot252 0x00895bc0
virtual bool HasHeroWonOracleMinigame(void) const; //slot253 0x00895c30
virtual void HeroPlayFireheartMinigame(void) const; //slot254 0x00895c90
virtual void HeroQuitFireheartMinigame(void) const; //slot255 0x00895cf0
virtual bool HasHeroForceQuitFireheartMinigame(void) const; //slot256 0x00895d50
virtual float GetHeroHealth(void) const; //slot257 0x00890370
virtual float GetHeroHealthMax(void) const; //slot258 0x008903a0
virtual float GetHeroHealthPercentage(void) const; //slot259 0x008903d0
virtual long GetHeroWillEnergy(void) const; //slot260 0x0089a150
virtual long GetHeroWillEnergyMax(void) const; //slot261 0x0089a1b0
virtual long GetHeroWillEnergyPercentage(void) const; //slot262 0x0089a210
virtual void ChangeHeroHealthBy(float, bool, bool) const; //slot263 0x00891720
virtual float GetHealth(ScriptMethods const &) const; //slot264 0x00890400
virtual void ModifyThingHealth(ScriptMethods const &, float, bool) const; //slot265 0x00890430
virtual void EntitySetMaxHealth(ScriptMethods const &, float, bool) const; //slot266 0x0088e1c0
virtual void SetThingAsKilled(ScriptMethods const &) const; //slot267 0x0089a270
virtual void GiveHeroNewQuestObjective(CCharString const &, unsigned long) const; //slot268 0x008917f0
virtual void TellHeroQuestObjectiveCompleted(unsigned long) const; //slot269 0x00891800
virtual void TellHeroQuestObjectiveFailed(unsigned long) const; //slot270 0x00891810
virtual void AddQuestRegion(CCharString const &, CCharString const &) const; //slot271 0x00891820
virtual void SetQuestWorldMapOffset(CCharString const &, C2DCoordI const &) const; //slot272 0x00891860
virtual bool IsHeroOnQuest(void) const; //slot273 0x00891870
virtual void HeroReceiveMessageFromGuildMaster(CCharString const &, CCharString const &, bool, bool) const; //slot274 0x008918c0
virtual void SetGuildMasterMessages(bool) const; //slot275 0x0088e200
virtual void ActivateQuest(CCharString const &) const; //slot276 0x00892e80
virtual void ActivateMultipleQuests(CArray<CCharString> const &) const; //slot277 0x00892ea0
virtual void ActivateQuestWithoutLoadingResources(CCharString const &) const; //slot278 0x00892ec0
virtual void ActivateMultipleQuestsWithoutLoadingResources(CArray<CCharString> const &) const; //slot279 0x00892ee0
virtual void DeactivateQuest(CCharString const &, unsigned long) const; //slot280 0x00892f00
virtual void DeactivateQuestLater(CCharString const &, unsigned long) const; //slot281 0x00892f10
virtual void PrepareQuestsWhenFinalQuestIsActivated(CCharString const &) const; //slot282 0x00892f20
virtual void PrepareQuestsWhenFinalQuestIsCompleted(void) const; //slot283 0x00892f30
virtual bool IsQuestActive(CCharString const &) const; //slot284 0x00892f40
virtual bool IsQuestRegistered(CCharString const &) const; //slot285 0x00892f50
virtual bool IsQuestCompleted(CCharString const &) const; //slot286 0x00892f60
virtual bool IsQuestFailed(CCharString const &) const; //slot287 0x00892f70
virtual void SetQuestAsCompleted(CCharString const &, bool, bool, bool) const; //slot288 0x00892f80
virtual void SetQuestAsFailed(CCharString const &, bool, CWideString const &, bool) const; //slot289 0x00892fd0
virtual void SetQuestAsPersistent(CCharString const &, bool) const; //slot290 0x00893020
virtual CCharString const & GetExclusiveQuestScriptName(void) const; //slot291 0x00893030
virtual void AddQuestCard(CCharString const &, CCharString const &, bool, bool) const; //slot292 0x008913f0
virtual void RemoveQuestCardFromGuild(CCharString const &) const; //slot293 0x00891490
virtual void RemoveQuestCardFromHero(CCharString const &) const; //slot294 0x008914e0
virtual void GiveHeroQuestCardDirectly(CCharString const &, CCharString const &, bool) const; //slot295 0x008968c0
virtual void SetQuestCardObjective(CCharString const &, CCharString const &, CCharString const &, CCharString const &) const; //slot296 0x00896a30
virtual void SetQuestCardGoldReward(CCharString const &, long) const; //slot297 0x008914f0
virtual void SetQuestCardRenownReward(CCharString const &, long) const; //slot298 0x00891500
virtual void RemoveAllAvailableQuestCardsFromGuild(void) const; //slot299 0x00891510
virtual void FailAllActiveQuests(void) const; //slot300 0x00891520
virtual void GetAllActiveQuestInfo(std::vector<CCharString, std::allocator<CCharString> > &, std::vector<CCharString, std::allocator<CCharString> > &) const; //slot301 0x00891530
virtual void AddFeatCard(long, CCharString const &, CCharString const &) const; //slot302 0x00891540
virtual void AddBoast(CCharString const &, long, long, long, bool, CCharString const &, long) const; //slot303 0x00893060
virtual void RemoveBoast(long, CCharString const &) const; //slot304 0x00891580
virtual void SetBoastAsFailed(long, CCharString const &) const; //slot305 0x008915b0
virtual void SetBoastAsCompleted(long, CCharString const &) const; //slot306 0x008915e0
virtual bool IsBoastTaken(long, CCharString const &) const; //slot307 0x00891550
virtual void AddLogBookEntry(CWideString const &, CWideString const &, CWideString const &, CHeroLogBook::CHeroLogBookEntry::ECategory) const; //slot308 0x0088fe00
virtual void KickOffQuestStartScreen(CCharString const &, bool, bool) const; //slot309 0x00896b60
virtual void KickOffDeathScreen(void) const; //slot310 0x00896d20
virtual void KickOffCreditsScreen(CCharString &) const; //slot311 0x00896df0
virtual void SetPreferredQuickAccessItem(CCharString const &, long, long) const; //slot312 0x00896e60
virtual CCharString GetDeathRecoveryMarkerName(void) const; //slot313 0x00896ee0
virtual void SetDeathRecoveryMarkerName(CCharString const &) const; //slot314 0x00896f60
virtual void ResetDeathRecoveryMarkerNameToDefault(void) const; //slot315 0x00896fd0
virtual bool IsToFailQuestOnDeath(void) const; //slot316 0x00897030
virtual void SetWhetherToFailQuestOnDeath(bool) const; //slot317 0x00897090
virtual void ResetWhetherToFailQuestOnDeathToDefault(void) const; //slot318 0x008970f0
virtual ScriptMethods GetMostRecentValidUsedTarget(void) const; //slot319 0x00891610
virtual CCharString GetMostRecentValidUsedTargetName(void) const; //slot320 0x00891680
virtual void DisplayQuestInfo(bool) const; //slot321 0x00891920
virtual void SetQuestInfoName(char const *) const; //slot322 0x00891940
virtual void SetQuestInfoText(char const *) const; //slot323 0x00891a00
virtual long AddQuestInfoBar(float, float, CRGBColour const &, CRGBColour const &, CCharString const &, CCharString const &, float) const; //slot324 0x00891ac0
virtual long AddQuestInfoBarHealth(ScriptMethods const &, CRGBColour const &, CCharString const &, float) const; //slot325 0x00891ad0
virtual long AddQuestInfoTimer(CTimer const &, CCharString const &, float) const; //slot326 0x00891b20
virtual long AddQuestInfoCounter(CCounter const &, CCharString const &, float) const; //slot328 0x00891b30
virtual long AddQuestInfoCounter(CCharString const &, long, float) const; //slot327 0x00891b40
virtual long AddQuestInfoCounterList(CCharString const &, long, float) const; //slot329 0x00891b50
virtual long AddQuestInfoTick(CCharString const &, bool, float) const; //slot331 0x00891b60
virtual long AddQuestInfoTick(EGameAction, bool, float) const; //slot330 0x00891b70
virtual void UpdateQuestInfoBar(long, float, float, float) const; //slot332 0x00891b80
virtual void ChangeQuestInfoBarColour(long, CRGBColour const &, CRGBColour const &) const; //slot333 0x00891ba0
virtual void UpdateQuestInfoTimer(long, float) const; //slot334 0x00891bb0
virtual void UpdateQuestInfoCounter(long, long, long) const; //slot335 0x00891bd0
virtual void UpdateQuestInfoCounterList(long, long, long) const; //slot336 0x00891be0
virtual void UpdateQuestInfoTick(long, bool) const; //slot337 0x00891bf0
virtual void RemoveQuestInfoElement(long) const; //slot338 0x00891c10
virtual void RemoveAllQuestInfoElements(void) const; //slot339 0x00891c30
virtual void DisplayTime(bool) const; //slot340 0x0088e210
virtual void DisplayMoneyBag(bool) const; //slot341 0x00891c40
virtual void DisplayMiniGameInfo(bool, EMiniGameType) const; //slot342 0x00891c60
virtual void UpdateMiniGameInfoBar(float) const; //slot343 0x00891c80
virtual bool IsEntityPickPocketable(ScriptMethods const &) const; //slot344 0x0088e220
virtual bool IsEntityPickLockable(ScriptMethods const &) const; //slot345 0x0089a2d0
virtual bool IsEntityStealable(ScriptMethods const &) const; //slot346 0x0089a330
virtual void EntitySetAsPickPocketed(ScriptMethods const &) const; //slot347 0x00890470
virtual void EntitySetAsPickLocked(ScriptMethods const &) const; //slot348 0x0089a390
virtual void EntitySetAsStolen(ScriptMethods const &) const; //slot349 0x008904e0
virtual void MiniMapAddMarker(ScriptMethods const &, CCharString const &) const; //slot350 0x00891f50
virtual void MiniMapSetMarkerGraphic(ScriptMethods const &, CCharString const &) const; //slot351 0x00891fa0
virtual void MiniMapRemoveMarker(ScriptMethods const &) const; //slot352 0x00891ff0
virtual void MiniMapRemoveAllMarkers(void) const; //slot353 0x00892030
virtual void MiniMapAllowRouteBetweenRegions(CCharString const &, CCharString const &, bool) const; //slot354 0x0088e3d0
virtual void MiniMapSetAsEnabled(bool) const; //slot355 0x0088e400
virtual void EntitySetAsHiddenOnMiniMap(ScriptMethods const &, bool) const; //slot356 0x0088e410
virtual void SetHUDEnabled(bool) const; //slot357 0x00892050
virtual bool TextEntryExists(CCharString const &) const; //slot358 0x00892070
virtual CCharString GetValidTextEntryNameWithAttitude(ScriptMethods const &, CCharString const &) const; //slot359 0x008920c0
virtual void SetThingHasInformation(ScriptMethods const &, bool, bool, bool) const; //slot360 0x0088e440
virtual void ClearThingHasInformation(ScriptMethods const &) const; //slot361 0x0088e480
virtual void EntitySetWillBeUsingNarrator(ScriptMethods const &, CCharString const &) const; //slot362 0x0089b050
virtual void EntityResetAsPureAINarrator(ScriptMethods const &) const; //slot363 0x0089b0b0
virtual long AddNewConversation(ScriptMethods const &, bool, bool) const; //slot364 0x008906c0
virtual void AddPersonToConversation(long, ScriptMethods const &) const; //slot365 0x00890710
virtual void AddLineToConversation(long, CCharString const &, bool, ScriptMethods const &, ScriptMethods const &) const; //slot366 0x00890750
virtual void RemoveConversation(long, bool) const; //slot367 0x008907c0
virtual bool IsConversationActive(long) const; //slot368 0x008907d0
virtual void PlayAVIMovie(CCharString const &) const; //slot369 0x0088f890
virtual void StartMovieSequence(CCharString const &, CScriptGameResourceObjectMovieBase &); //slot370 0x0089b110
virtual void FixMovieSequenceCamera(bool) const; //slot371 0x0088e500
virtual void FadeScreenOutUntilNextCallToFadeScreenIn(float, float); //slot372 0x008907e0
virtual bool FadeScreenOut(float, float, CRGBColour); //slot373 0x00890820
virtual void FadeScreenIn(void); //slot374 0x0088e4c0
virtual bool IsScreenFadingOut(void) const; //slot375 0x0088e4d0
virtual void Pause(float) const; //slot376 0x0088e4f0
virtual void EndCutFade(void); //slot377 0x0088e520
virtual void EndLetterBox(void); //slot378 0x006e7290
virtual void PauseAllNonScriptedEntities(bool); //slot379 0x00890ab0
virtual void PauseAllEntities(bool); //slot380 0x00890b00
virtual void SetAllowScreenFadingOnNextRegionChange(bool); //slot381 0x0088e910
virtual void SetAllowScreenFadingIfAlreadyFaded(bool); //slot382 0x0088e920
virtual void SetToKeepHeroAbilitiesDuringCutscenes(bool) const; //slot383 0x00890b50
virtual void SetToDisplayTutorialsDuringCutscenes(bool) const; //slot384 0x0088e930
virtual void SetCutsceneMode(bool, bool) const; //slot385 0x0089d600
virtual bool IsInCutscene(void) const; //slot386 0x00892270
virtual void SetCutsceneSkippable(bool) const; //slot387 0x00892280
virtual void SetCutsceneSkippableWhilePaused(bool) const; //slot388 0x008922a0
virtual void SetCutsceneActionMode(bool, CCharString const &) const; //slot389 0x008922c0
virtual void PreloadNewScene(float) const; //slot390 0x00890b70
virtual void StartProgressDisplay(void) const; //slot391 0x0088e950
virtual void StopProgressDisplay(void) const; //slot392 0x0088e970
virtual void SetScreenMessagesEnabled(bool) const; //slot393 0x008922d0
virtual bool IsHeroControlledByPlayer(void) const; //slot394 0x0088e980
virtual bool IsInMovieSequence(void) const; //slot395 0x0088e9e0
virtual void CancelAbilitiesForCutscene(void) const; //slot396 0x0089d690
virtual void ResumeAbilitiesForCutscene(void) const; //slot397 0x0089d6f0
virtual void CancelUsingAbility(EHeroAbility) const; //slot398 0x0089d750
virtual void SetAbilityAvailability(EHeroAbility, bool) const; //slot399 0x0089d7c0
virtual void SetEnvironmentalEffectsAlwaysUpdate(bool) const; //slot400 0x00890b90
virtual void SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool) const; //slot401 0x00894ec0
virtual void RemoveDeadCreature(CCharString const &) const; //slot402 0x00894490
virtual void CameraSetCameraPreloadFlag(bool) const; //slot403 0x0088ec90
virtual void CameraCircleAroundThing(ScriptMethods const &, C3DVector const &, float) const; //slot404 0x00892b50
virtual void CameraCircleAroundPos(C3DVector const &, C3DVector const &, float) const; //slot405 0x00892c20
virtual void CameraMoveToPosAndLookAtPos(C3DVector const &, C3DVector const &, float) const; //slot406 0x00892410
virtual void CameraMoveToPosAndLookAtThing(C3DVector const &, ScriptMethods const &, float) const; //slot407 0x008924b0
virtual void CameraMoveBetweenLookingAt(C3DVector const &, C3DVector const &, C3DVector const &, float, float) const; //slot409 0x00892530
virtual void CameraMoveBetweenLookingAt(C3DVector const &, C3DVector const &, ScriptMethods const &, float, float) const; //slot408 0x008926d0
virtual void CameraMoveBetweenLookFromAndLookTo(C3DVector const &, C3DVector const &, C3DVector const &, C3DVector const &, float) const; //slot410 0x00892610
virtual void CameraUseCameraPoint(CCharString const &, ScriptMethods const &, float, long, long) const; //slot414 0x008922f0
virtual void CameraUseCameraPoint(ScriptMethods const &, ScriptMethods const &, float, long, long) const; //slot413 0x00891070
virtual void CameraUseCameraPoint(CCharString const &, C3DVector const *, CRightHandedSet const *, float, long, long) const; //slot412 0x00892380
virtual void CameraUseCameraPoint(ScriptMethods const &, C3DVector const *, CRightHandedSet const *, float, long, long) const; //slot411 0x0089e190
virtual void CameraDoConversation(ScriptMethods const &, ScriptMethods const &, ECameraOp, bool) const; //slot415 0x0088edc0
virtual void CameraDefault(void); //slot416 0x00890ff0
virtual void CameraResetToViewBehindHero(float); //slot417 0x00890fc0
virtual bool IsCameraInScriptedMode(void) const; //slot418 0x0088ecc0
virtual void CameraUseScreenEffect(float, float, float) const; //slot419 0x0088eda0
virtual void CameraCancelScreenEffect(void) const; //slot420 0x0088edb0
virtual bool IsCameraPosOnScreen(C3DVector const &) const; //slot421 0x006e7300
virtual float GetGameAngleXY(void) const; //slot422 0x006e7340
virtual void CameraEarthquakeIntensityAtPos(C3DVector const &, float, float) const; //slot423 0x0088ed20
virtual void CameraShake(float, float) const; //slot424 0x0088ece0
virtual void OpenDoor(ScriptMethods const &) const; //slot425 0x00895db0
virtual void CloseDoor(ScriptMethods const &) const; //slot426 0x00895de0
virtual void OpenHouseDoors(ScriptMethods const &) const; //slot427 0x0088dfd0
virtual void CloseHouseDoors(ScriptMethods const &) const; //slot428 0x0088e000
virtual void JamDoor(ScriptMethods const &) const; //slot429 0x00895e10
virtual void SetDoorTriggerType(ScriptMethods const &, EDoorTriggerType) const; //slot430 0x00895e70
virtual void OverrideAutomaticHouseLocking(ScriptMethods const &, bool) const; //slot431 0x0088e030
virtual void SetHouseOwnedByPlayer(ScriptMethods const &, bool, bool) const; //slot432 0x00895ed0
virtual void SetBuyableHouseAsScripted(ScriptMethods const &, bool) const; //slot433 0x00896000
virtual bool IsChestOpen(ScriptMethods const &) const; //slot434 0x00896330
virtual bool OpenChest(ScriptMethods const &, bool) const; //slot435 0x00896390
virtual void CloseChest(ScriptMethods const &) const; //slot436 0x00896660
virtual long GetNumberOfKeysNeededToUnlockChest(ScriptMethods const &, CCharString &) const; //slot437 0x008966c0
virtual void DisplayLockedChestMessage(ScriptMethods const &) const; //slot438 0x00896730
virtual void SetTrophyAsMountable(ScriptMethods const &, bool) const; //slot439 0x00896790
virtual void SetVillageLimbo(ScriptMethods const &, bool) const; //slot440 0x00896060
virtual void SetCreatureNotReload(ScriptMethods const &) const; //slot441 0x0088fd90
virtual bool IsSleepingTime(ScriptMethods const &) const; //slot442 0x00896210
virtual void EnableGuards(ScriptMethods const &, bool) const; //slot443 0x00896270
virtual void EnableVillagerDefTypes(ScriptMethods const &, bool, CCharString const &) const; //slot444 0x008962d0
virtual ScriptMethods TryToRespawnDefNamed(ScriptMethods const &, CCharString const &, C3DVector const &) const; //slot445 0x00896120
virtual void ClearHeroEnemyOfGuards(ScriptMethods const &) const; //slot446 0x008961b0
virtual void SetThingAsUsable(ScriptMethods const &, bool) const; //slot447 0x008967f0
virtual void SetThingHomeBuilding(ScriptMethods const &, ScriptMethods const &) const; //slot448 0x00892e30
virtual void SetVillageAttitude(ScriptMethods const &, EScriptVillageAttitude) const; //slot449 0x0089fe20
virtual void AddCrimeCommitted(ScriptMethods const &, ECrime, bool, ScriptMethods const *, ScriptMethods const *, EOpinionPostDeedType) const; //slot450 0x0089feb0
virtual void GiveThingBestEnemyTarget(ScriptMethods const &, ScriptMethods const &) const; //slot451 0x008a0260
virtual void ClearThingBestEnemyTarget(ScriptMethods const &) const; //slot452 0x008a03f0
virtual void EntitySetInLimbo(ScriptMethods const &, bool, bool) const; //slot453 0x0088fd00
virtual bool IsEntityInLimbo(ScriptMethods const &) const; //slot454 0x0088fd60
virtual bool EntityGetShotStrikePos(ScriptMethods const &, C3DVector &) const; //slot455 0x0089df70
virtual void EntitySetNegateAllHits(ScriptMethods const &, bool) const; //slot456 0x00890e80
virtual void EntitySetEvadeAllHits(ScriptMethods const &, bool) const; //slot457 0x00890ec0
virtual void EntitySetAbleToBeEngagedInCombat(ScriptMethods const &, bool) const; //slot458 0x00890f00
virtual void EntitySetAlwaysBlockAttacksFromThing(ScriptMethods const &, ScriptMethods const &, bool) const; //slot459 0x0089dff0
virtual void EntitySetAttackThingImmediately(ScriptMethods const &, ScriptMethods const &, bool, bool) const; //slot460 0x008a8eb0
virtual void EntitySetCombatType(ScriptMethods const &, CCharString const &) const; //slot461 0x0089e080
virtual void EntityResetCombatTypeToDefault(ScriptMethods const &) const; //slot462 0x00890f40
virtual void EntitySetMaxNumberOfAttackers(ScriptMethods const &, long) const; //slot463 0x0089e130
virtual void EntityClearMaxNumberOfAttackers(ScriptMethods const &) const; //slot464 0x0088ec70
virtual void EntityAttachToScript(ScriptMethods const &, CCharString const &) const; //slot465 0x00890690
virtual void EntitySetCombatAbility(ScriptMethods const &, float) const; //slot466 0x0089aff0
virtual void EntitySetRangedTarget(ScriptMethods const &, ScriptMethods const &) const; //slot467 0x008a04c0
virtual void EntityClearRangedTarget(ScriptMethods const &) const; //slot468 0x008a0540
virtual void EntitySetTargetable(ScriptMethods const &, bool) const; //slot469 0x008a05a0
virtual void EntitySetTargetingType(ScriptMethods const &, ETargetingType) const; //slot470 0x008a0600
virtual void EntitySetTargetingValidTargetWithoutLOS(ScriptMethods const &, bool) const; //slot471 0x008a0660
virtual void EntityTeleportToThing(ScriptMethods const &, ScriptMethods const &, bool) const; //slot472 0x0088e540
virtual void EntityTeleportToPosition(ScriptMethods const &, C3DVector const &, float, bool, bool) const; //slot473 0x0089b780
virtual void EntitySetFacingAngle(ScriptMethods const &, float, bool) const; //slot474 0x0089bdf0
virtual void EntitySetFacingAngleTowardsThing(ScriptMethods const &, ScriptMethods const &, bool) const; //slot475 0x0088e620
virtual void EntitySetPerceptionVariables(ScriptMethods const &, float, float, float) const; //slot476 0x0089bf80
virtual void SetThingPersistent(ScriptMethods const &, bool) const; //slot477 0x0088e5a0
virtual void EntitySetThingAsWantingMoney(ScriptMethods const &, long) const; //slot478 0x0089bb30
virtual void EntitySetAppearanceMorphSeed(ScriptMethods const &, long) const; //slot479 0x008a0200
virtual void SetEntityAsRegionFollowing(ScriptMethods const &, ScriptMethods const &, bool) const; //slot480 0x008a0b10
virtual void SetEntityAsFollowingHeroThroughTeleporters(ScriptMethods const &, bool) const; //slot481 0x0088f670
virtual void EntitySetAppearanceSeed(ScriptMethods const &, unsigned long) const; //slot482 0x008a0bc0
virtual void EntityGetAppearanceSeed(ScriptMethods const &, unsigned long &) const; //slot483 0x008a0c10
virtual void EntitySetAsForSale(ScriptMethods const &, bool) const; //slot484 0x008a00e0
virtual void EntitySetStockItemPrice(ScriptMethods const &, long) const; //slot485 0x008a0140
virtual long EntityGetStockItemPrice(ScriptMethods const &) const; //slot486 0x008a01a0
virtual void EntityPlayObjectAnimation(ScriptMethods const &, CCharString const &, bool) const; //slot487 0x0089bbc0
virtual void EntitySetMaxRunningSpeed(ScriptMethods const &, float) const; //slot488 0x0088e750
virtual void EntitySetMaxWalkingSpeed(ScriptMethods const &, float) const; //slot489 0x0088e7b0
virtual void EntityResetMaxRunningSpeed(ScriptMethods const &) const; //slot490 0x0088e810
virtual void EntityResetMaxWalkingSpeed(ScriptMethods const &) const; //slot491 0x0088e870
virtual void EntityAttachToVillage(ScriptMethods const &, ScriptMethods const &) const; //slot492 0x0089d3a0
virtual void EntitySetAsSittingOnFloor(ScriptMethods const &, bool) const; //slot493 0x008a8c60
virtual void EntitySetAsHavingBoundHands(ScriptMethods &, bool) const; //slot494 0x0089d420
virtual void EntitySetAsRemoveAllMovementBlockingModes(ScriptMethods &) const; //slot495 0x0088e8d0
virtual void EntitySetAsScared(ScriptMethods const &, bool) const; //slot496 0x0089d4c0
virtual void EntitySetAsDrunk(ScriptMethods &, bool) const; //slot497 0x0089d560
virtual void EntityForceToLookAtThing(ScriptMethods const &, ScriptMethods const &) const; //slot498 0x0089d830
virtual void EntityForceToLookAtCamera(ScriptMethods const &, bool) const; //slot499 0x0089d8b0
virtual void EntityForceToLookAtNothing(ScriptMethods const &) const; //slot500 0x0089d910
virtual void EntityResetForceToLookAt(ScriptMethods const &) const; //slot501 0x0089d970
virtual void EntitySetShotAccuracyPercentage(ScriptMethods const &, long) const; //slot502 0x0088e5f0
virtual ScriptMethods EntityGetStandingOnThing(ScriptMethods const &) const; //slot503 0x00892220
virtual ScriptMethods EntityGetStandingInsideBuilding(ScriptMethods const &) const; //slot504 0x0089bc50
virtual void EntityDropGenericBox(ScriptMethods const &) const; //slot505 0x0089d9d0
virtual void EntitySheatheWeapons(ScriptMethods const &, bool) const; //slot506 0x00890ba0
virtual void EntityUnsheatheWeapons(ScriptMethods const &, bool) const; //slot507 0x00890c10
virtual void EntityUnsheatheMeleeWeapon(ScriptMethods const &, bool) const; //slot508 0x00890c80
virtual void EntityUnsheatheRangedWeapon(ScriptMethods const &, bool) const; //slot509 0x00890d80
virtual void EntitySetAlpha(ScriptMethods const &, float, bool) const; //slot510 0x0089da70
virtual void EntitySetAsDrawable(ScriptMethods const &, bool) const; //slot511 0x0088e9f0
virtual void EntitySetCutsceneBehaviour(ScriptMethods const &, ECutsceneBehaviour) const; //slot512 0x0088ea20
virtual ESex EntityGetSex(ScriptMethods const &) const; //slot513 0x0088ea50
virtual void EntitySetAsAbleToWalkThroughSolidObjects(ScriptMethods const &, bool) const; //slot514 0x0089dbb0
virtual void EntitySetAsRespondToHit(ScriptMethods const &, bool) const; //slot515 0x0088ea90
virtual void EntitySetAsDamageable(ScriptMethods const &, bool) const; //slot516 0x0088ead0
virtual void EntitySetAsKillable(ScriptMethods const &, bool, bool) const; //slot517 0x0088eb30
virtual void EntitySetAsLocked(ScriptMethods const &, bool) const; //slot518 0x0089dc10
virtual void EntityDecapitate(ScriptMethods const &) const; //slot519 0x008a8e40
virtual void EntityGiveGold(ScriptMethods const &, long) const; //slot520 0x0089dc70
virtual void EntitySetAllowBossPhaseChanges(ScriptMethods const &, bool) const; //slot521 0x0089dcd0
virtual long EntityGetBossPhase(ScriptMethods const &) const; //slot522 0x0089dd30
virtual void EntitySetBossPhase(ScriptMethods const &, long) const; //slot523 0x0089dd90
virtual void EntityResetCreatureMode(ScriptMethods const &) const; //slot524 0x0089ddf0
virtual bool EntitySetAsReceivingEvents(ScriptMethods const &, bool) const; //slot525 0x00895010
virtual void EntitySetAsToAddToComboMultiplierWhenHit(ScriptMethods const &, bool) const; //slot526 0x0088ebc0
virtual void EntitySetAsToAddToStatChangesWhenHit(ScriptMethods const &, bool) const; //slot527 0x0088ebf0
virtual void EntityLeaveCombatStance(ScriptMethods const &) const; //slot528 0x0089de50
virtual void EntitySetAsUseMovementInActions(ScriptMethods const &, bool) const; //slot529 0x0088ec20
virtual void EntitySetAsDisplayingEmoteIcon(ScriptMethods const &, bool) const; //slot530 0x0089deb0
virtual void EntitySetAsCollidableToThings(ScriptMethods const &, bool) const; //slot531 0x00891120
virtual void EntityEnableGravity(ScriptMethods const &, bool) const; //slot532 0x0088f4d0
virtual void EntitySetLightAsOn(ScriptMethods const &, bool) const; //slot533 0x008a06c0
virtual void EntityFadeOut(ScriptMethods const &, float) const; //slot534 0x008a0720
virtual void EntityFadeIn(ScriptMethods const &, float) const; //slot535 0x008a07c0
virtual void EntityBeginLoadingAnimation(ScriptMethods const &, CCharString const &) const; //slot536 0x0088f500
virtual void EntityBeginLoadingBasicAnimations(ScriptMethods const &) const; //slot537 0x0088f530
virtual bool EntityCastForcePush(ScriptMethods const &, bool) const; //slot538 0x008a9320
virtual void EntityCastLightningAtTarget(ScriptMethods const &, ScriptMethods const &) const; //slot539 0x0088f560
virtual void BeginLoadingMesh(CCharString const &) const; //slot540 0x00894aa0
virtual bool EntityWillTeleportToArea(ScriptMethods const &, C3DVector, float, float) const; //slot541 0x008a93a0
virtual void EntityStartScreamerSuperAttackThing(ScriptMethods const &, ScriptMethods const &) const; //slot542 0x008a08a0
virtual void EntityEndScreamerSuperAttackThing(ScriptMethods const &) const; //slot543 0x008a0950
virtual void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool); //slot544 0x0088ec60
virtual void SetLightColour(ScriptMethods const &, CRGBColour const &) const; //slot545 0x0089df10
virtual void CreatureGeneratorSetFamily(ScriptMethods const &, CCharString const &) const; //slot546 0x008a9020
virtual void CreatureGeneratorTrigger(ScriptMethods const &) const; //slot547 0x0089e8e0
virtual void CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(ScriptMethods const &, bool) const; //slot548 0x0089e950
virtual bool CreatureGeneratorIsDepleted(ScriptMethods const &) const; //slot549 0x0089e9c0
virtual bool CreatureGeneratorIsDestroyed(ScriptMethods const &) const; //slot550 0x0089ea30
virtual void CreatureGeneratorSetGeneratedCreatureScriptName(ScriptMethods const &, CCharString const &) const; //slot551 0x0089eaa0
virtual void CreatureGeneratorSetNumTriggers(ScriptMethods const &, long) const; //slot552 0x0089eb10
virtual long CreatureGeneratorGetNumGeneratedCreatures(ScriptMethods const &) const; //slot553 0x0089eb80
virtual bool CreatureGeneratorAreAllCreaturesAlive(ScriptMethods const &) const; //slot554 0x0089ebf0
virtual void CreatureGeneratorAddTriggerer(ScriptMethods const &, ScriptMethods const &) const; //slot555 0x0089ec60
virtual void CreatureGeneratorRemoveTriggerer(ScriptMethods const &, ScriptMethods const &) const; //slot556 0x0089ecc0
virtual void SetCreatureGeneratorEnabled(ScriptMethods const &, bool) const; //slot557 0x0089ed20
virtual void SetCreatureGeneratorsEnabled(CCharString const &, bool) const; //slot558 0x00892cf0
virtual void SetCreatureGeneratorsEnabledDuringScript(CCharString const &, bool) const; //slot559 0x00892d70
virtual void SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup, bool) const; //slot560 0x0088f400
virtual bool IsCreatureGenerationEnabledForRegion(CCharString const &) const; //slot561 0x00892d80
virtual bool IsCreatureFlying(ScriptMethods const &) const; //slot562 0x0089ed80
virtual void SetTeleporterAsActive(ScriptMethods const &, bool) const; //slot563 0x0089ee00
virtual bool IsTeleporterActive(ScriptMethods const &) const; //slot564 0x0089ee60
virtual void SetTeleportingAsActive(bool) const; //slot565 0x0088f410
virtual bool IsTeleportingActive(void) const; //slot566 0x0088f420
virtual void SetRegionExitAsActive(ScriptMethods const &, bool) const; //slot567 0x0089eec0
virtual void SetRegionEntranceAsActive(ScriptMethods const &, bool) const; //slot568 0x0089ef00
virtual void SetRegionTextDisplayAsActive(bool) const; //slot569 0x0088f430
virtual void SetHeroSleepingAsEnabled(bool) const; //slot570 0x0088f440
virtual bool IsHeroSleepingEnabled(void) const; //slot571 0x0088f450
virtual void SetExperienceSpendingAsEnabled(bool) const; //slot572 0x0088f460
virtual void SetMoralityChangingAsEnabled(bool) const; //slot573 0x0089ef30
virtual void SetSummonerDeathExplosionAffectsHero(bool) const; //slot574 0x0088f470
virtual ScriptMethods GetNearestEnabledDiggingSpot(ScriptMethods const &) const; //slot575 0x0089ef90
virtual bool IsDiggingSpotEnabled(ScriptMethods const &) const; //slot576 0x0089f1e0
virtual bool IsDiggingSpotHidden(ScriptMethods const &) const; //slot577 0x0089f240
virtual void SetDiggingSpotAsHidden(ScriptMethods const &, bool) const; //slot578 0x0089f2a0
virtual bool CheckForCameraMessage(CCharString const &) const; //slot579 0x006e7920
virtual void WaitForCameraMessage(CCharString const &) const; //slot580 0x006e7820
virtual void SetThingAsConscious(ScriptMethods const &, bool, CCharString const &) const; //slot581 0x008a9610
virtual void SetFireToThing(ScriptMethods const &) const; //slot582 0x0089bcd0
virtual void ExtinguishFiresOnThing(ScriptMethods const &) const; //slot583 0x0089bd30
virtual bool IsThingOnFire(ScriptMethods const &) const; //slot584 0x0089bd90
virtual void AddItemToContainer(ScriptMethods const &, CCharString const &) const; //slot585 0x0089e780
virtual void RemoveItemFromContainer(ScriptMethods const &, CCharString const &) const; //slot586 0x0089e810
virtual void EntitySetDeathContainerAsEnabled(ScriptMethods const &, bool) const; //slot587 0x0089e880
virtual void GetItemDefNamesFromContainer(ScriptMethods const &, std::vector<CCharString, std::allocator<CCharString> > &) const; //slot588 0x008a8f20
virtual void SetCreatureBrain(ScriptMethods const &, CCharString const &) const; //slot589 0x00894b90
virtual void EntitySetStategroupEnabled(ScriptMethods const &, CCharString const &, bool) const; //slot590 0x0088f6b0
virtual void EntitySetAllStategroupsEnabled(ScriptMethods const &, bool) const; //slot591 0x0088f6e0
virtual void EntitySetCombatEnabled(ScriptMethods const &, bool) const; //slot592 0x0088f710
virtual void EntitySetSleepEnabled(ScriptMethods const &, bool) const; //slot593 0x0088f7d0
virtual void EntitySetOpinionReactionsEnabled(ScriptMethods const &, bool) const; //slot594 0x0088f810
virtual void EntitySetDeedReactionsEnabled(ScriptMethods const &, bool) const; //slot595 0x0088f850
virtual void DebugGetAllTextEntriesForTargetedThing(std::set<unsigned long, std::less<unsigned long>, std::allocator<unsigned long> > &) const; //slot596 0x008a0cb0
virtual void EntitySetThingAsEnemyOfThing(ScriptMethods const &, ScriptMethods const &) const; //slot597 0x0089c000
virtual void EntityUnsetThingAsEnemyOfThing(ScriptMethods const &, ScriptMethods const &) const; //slot598 0x0089c080
virtual void EntitySetThingAsAllyOfThing(ScriptMethods const &, ScriptMethods const &) const; //slot599 0x008a7ec0
virtual void EntityUnsetThingAsAllyOfThing(ScriptMethods const &, ScriptMethods const &) const; //slot600 0x008a7ee0
virtual void EntitySetInFaction(ScriptMethods const &, CCharString const &) const; //slot601 0x0089c190
virtual void SetFactionAsAlliedToFaction(CCharString const &, CCharString const &) const; //slot602 0x00890870
virtual void SetFactionAsNeutralToFaction(CCharString const &, CCharString const &) const; //slot603 0x008908c0
virtual void SetFactionAsEnemyToFaction(CCharString const &, CCharString const &) const; //slot604 0x00890910
virtual bool AreEntitiesEnemies(ScriptMethods const &, ScriptMethods const &) const; //slot605 0x0088e6f0
virtual EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType) const; //slot606 0x00890960
virtual void GetOpinionAttitudeAsString(EOpinionAttitudeType, CCharString &) const; //slot607 0x00890970
virtual EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(ScriptMethods const &) const; //slot608 0x00890990
virtual void EntityGetOpinionAttitudeToPlayerAsString(ScriptMethods const &, CCharString &) const; //slot609 0x008909c0
virtual float EntityGetOpinionOfPlayer(ScriptMethods const &, EOpinion) const; //slot610 0x008909f0
virtual void EntitySetOpinionReactionMask(ScriptMethods const &, long) const; //slot612 0x0089c1f0
virtual void EntitySetOpinionReactionMask(ScriptMethods const &, CCharString const &) const; //slot611 0x0089c240
virtual void EntitySetOpinionDeedMask(ScriptMethods const &, long) const; //slot614 0x0089c290
virtual void EntitySetOpinionDeedMask(ScriptMethods const &, CCharString const &) const; //slot613 0x0089c2e0
virtual void EntitySetOpinionDeedTypeEnabled(ScriptMethods const &, EOpinionDeedType, bool) const; //slot615 0x0089c330
virtual void EntitySetOpinionAttitudeEnabled(ScriptMethods const &, EOpinionAttitudeType, bool) const; //slot616 0x0089c380
virtual void EntitySetOpinionReactionEnabled(ScriptMethods const &, EOpinionReactionType, bool) const; //slot617 0x0089c3d0
virtual void EntitySetPersonalityOverride(ScriptMethods const &, CCharString const &) const; //slot619 0x0089c420
virtual void EntitySetPersonalityOverride(ScriptMethods const &, long) const; //slot618 0x0089c470
virtual void EntityClearPersonalityOverride(ScriptMethods const &) const; //slot620 0x0089c4c0
virtual void EntitySetAsOpinionSource(ScriptMethods const &, CCharString const &) const; //slot622 0x00890a20
virtual void EntitySetAsOpinionSource(ScriptMethods const &, long) const; //slot621 0x00890a50
virtual void EntityUnsetAsOpinionSource(ScriptMethods const &, bool) const; //slot623 0x00890a80
virtual void OpinionSourceSetAsExclusive(ScriptMethods const &, bool) const; //slot624 0x00894e60
virtual void OpinionSourceSetAsAttentionGrabbing(ScriptMethods const &, bool) const; //slot625 0x0089c510
virtual void EntityPostOpinionDeedToAll(ScriptMethods const &, EOpinionDeedType) const; //slot626 0x0089c560
virtual void EntityPostOpinionDeedToRecipient(ScriptMethods const &, EOpinionDeedType, ScriptMethods const &) const; //slot627 0x0089c670
virtual void EntityPostOpinionDeedToRecipientVillage(ScriptMethods const &, EOpinionDeedType, ScriptMethods const &) const; //slot628 0x0089c710
virtual long EntityPostOpinionDeedKeepSearchingForWitnesses(ScriptMethods const &, EOpinionDeedType, ScriptMethods const &) const; //slot629 0x0089c7b0
virtual void RemoveOpinionDeedStillSearchingForWitnesses(ScriptMethods const &, long) const; //slot630 0x0089c850
virtual bool IsDeedWitnessed(long) const; //slot631 0x0089c8a0
virtual bool CanThingBe_Seen_ByOtherThing(ScriptMethods const &, ScriptMethods const &) const; //slot632 0x0089cd10
virtual bool CanThingBe_NearlySeen_ByOtherThing(ScriptMethods const &, ScriptMethods const &) const; //slot633 0x0089cd30
virtual bool CanThingBe_Smelled_ByOtherThing(ScriptMethods const &, ScriptMethods const &) const; //slot634 0x0089cd50
virtual bool CanThingBe_Heard_ByOtherThing(ScriptMethods const &, ScriptMethods const &) const; //slot635 0x0089cd70
virtual bool IsThingAwareOfOtherThingInAnyWay(ScriptMethods const &, ScriptMethods const &) const; //slot636 0x0089ce70
virtual void EntitySetAsAwareOfThing(ScriptMethods const &, ScriptMethods const &) const; //slot637 0x0089d0e0
virtual void EntitySetSoundRadius(ScriptMethods const &, float) const; //slot638 0x0089d160
virtual void EntitySetSmellRadius(ScriptMethods const &, float) const; //slot639 0x0089d1c0
virtual void EntitySetSightRadius(ScriptMethods const &, float) const; //slot640 0x0089d220
virtual void EntitySetExtendedSightRadius(ScriptMethods const &, float) const; //slot641 0x0089d280
virtual void EntitySetGiveUpChaseRadius(ScriptMethods const &, float) const; //slot642 0x0089d2e0
virtual float EntityGetHearingRadius(ScriptMethods const &) const; //slot643 0x0089d340
virtual bool ManuallyTriggerTrap(ScriptMethods const &) const; //slot644 0x00895090
virtual bool ManuallyResetTrap(ScriptMethods const &) const; //slot645 0x008950e0
virtual void SetTimeOfDay(float) const; //slot646 0x0088fdc0
virtual long GetTimeOfDay(void) const; //slot647 0x0088fe30
virtual void SetTimeAsStopped(bool, long &) const; //slot648 0x0088fe80
virtual void FastForwardTimeTo(float, float) const; //slot649 0x0088fe90
virtual bool IsTimeOfDayBetween(long, long) const; //slot650 0x0088fe40
virtual long GetDayOfWeek(void) const; //slot651 0x0088fe50
virtual long GetDayCount(void) const; //slot652 0x0088fe70
virtual long GetWorldFrame(void) const; //slot653 0x0088e070
virtual long GetConstantFPS(void) const; //slot654 0x0088e080
virtual CCharString GetActiveQuestName(void) const; //slot655 0x00891880
virtual void TransitionToTheme(CCharString const &, float) const; //slot656 0x0088fee0
virtual void ResetToDefaultTheme(float) const; //slot657 0x0088ff10
virtual void TransitionToThemeAllInternals(CCharString const &, float) const; //slot658 0x0088ff30
virtual void ResetToDefaultThemeAllInternals(float) const; //slot659 0x0088ff80
virtual void TransitionToThemeExternals(CCharString const &, float) const; //slot660 0x0088ffb0
virtual void ResetToDefaultThemeExternals(float) const; //slot661 0x0088fff0
virtual void SetEnvironmentThemeWeightAllChannels(CCharString const &, float) const; //slot662 0x00890010
virtual void SetEnvironmentThemeWeightAllInternals(CCharString const &, float) const; //slot663 0x00890040
virtual void SetEnvironmentThemeWeightExternals(CCharString const &, float) const; //slot664 0x00890090
virtual void SetSoundThemesAsEnabledForRegion(CCharString const &, bool) const; //slot665 0x0088e0b0
virtual void SetAllSoundsAsMuted(bool) const; //slot666 0x0088e090
virtual void * RadialBlurFadeTo(float, C3DVector, float, C3DVector, float, float, float) const; //slot668 0x008900d0
virtual void * RadialBlurFadeTo(float, float, float, float, float, float, float) const; //slot667 0x00890110
virtual void RadialBlurFadeOut(float, void *) const; //slot669 0x00890150
virtual bool IsRadialBlurFadeActive(void) const; //slot670 0x00890170
virtual void CancelRadialBlurFade(void) const; //slot671 0x00890180
virtual void RadialBlurSetCenterWorldPos(void *, C3DVector const &) const; //slot672 0x008901a0
virtual void * DisplacementMonochromeEffectColourFadeTo(float, CRGBFloatColour const &) const; //slot673 0x008901c0
virtual void DisplacementMonochromeEffectColourFadeOut(float, void *) const; //slot674 0x008901e0
virtual void * ScreenFilterFadeTo(float, float, float, float, float, CRGBFloatColour const &, std::vector<CScreenFilterSThingByPass, std::allocator<CScreenFilterSThingByPass> > const &) const; //slot675 0x008a9880
virtual void ScreenFilterFadeOut(float, void *) const; //slot676 0x00890200
virtual void SetThingAndCarriedItemsNotAffectedByScreenFilter(ScriptMethods &, void *) const; //slot677 0x008a99f0
virtual void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(ScriptMethods &) const; //slot678 0x00897150
virtual bool IsGiftRomantic(CCharString const &) const; //slot679 0x00894bf0
virtual bool IsGiftFriendly(CCharString const &) const; //slot680 0x00894c90
virtual bool IsGiftOffensive(CCharString const &) const; //slot681 0x00894d30
virtual bool IsThingABed(ScriptMethods const &) const; //slot682 0x008a0e80
virtual bool IsThingAChest(ScriptMethods const &) const; //slot683 0x008a0eb0
virtual bool IsThingADoor(ScriptMethods const &) const; //slot684 0x008a0ee0
virtual bool IsThingSmashable(ScriptMethods const &) const; //slot685 0x008a0f10
virtual bool IsThingSearchable(ScriptMethods const &) const; //slot686 0x008a0f40
virtual void ApplyScriptBrush(CCharString const &) const; //slot687 0x0088f480
virtual void EnableDecals(bool) const; //slot688 0x0088f4a0
virtual unsigned long PlayCriteriaSoundOnThing(ScriptMethods const &, CCharString const &) const; //slot689 0x008a09b0
virtual unsigned long PlaySoundOnThing(ScriptMethods const &, CCharString const &) const; //slot690 0x008a0a20
virtual unsigned long PlaySoundAtPos(C3DVector const &, CCharString const &) const; //slot691 0x00891170
virtual unsigned long Play2DSound(CCharString const &) const; //slot692 0x0088f570
virtual bool IsSoundPlaying(unsigned long) const; //slot693 0x0088f610
virtual void EnableSounds(bool) const; //slot694 0x0088f620
virtual void StopSound(unsigned long) const; //slot695 0x0088f660
virtual void OverrideMusic(EMusicSetType, bool, bool) const; //slot696 0x00891220
virtual void StopOverrideMusic(bool) const; //slot697 0x008912a0
virtual void CacheMusicSet(EMusicSetType) const; //slot698 0x00891310
virtual void EnableDangerMusic(bool) const; //slot699 0x00891320
virtual bool IsDangerMusicEnabled(void) const; //slot700 0x00891330
virtual void StartCountdownTimer(float) const; //slot701 0x0088f8d0
virtual float GetCountdownTimer(void) const; //slot702 0x0088f900
virtual void AutoSaveCheckPoint(void) const; //slot703 0x0088f910
virtual void AutoSaveQuestStart(void) const; //slot704 0x0088f920
virtual void AutoSave(void) const; //slot705 0x0088f930
virtual void SetSavingAsEnabled(bool) const; //slot706 0x0088f940
virtual bool IsSavingEnabled(void) const; //slot707 0x0088f950
virtual void SetSaveGameMarkerPos(C3DVector const &) const; //slot708 0x0088f960
virtual void ResetToFrontEnd(void) const; //slot709 0x0088f970
virtual void SetGuildSealRecallLocation(C3DVector const &, float) const; //slot710 0x0088f980
virtual C3DVector GetGuildSealRecallPos(void) const; //slot711 0x0088f9b0
virtual float GetGuildSealRecallAngleXY(void) const; //slot712 0x0088f9e0
virtual void SetReadableObjectText(ScriptMethods const &, CWideString const &) const; //slot713 0x008a0f70
virtual void SetReadableObjectTextTag(ScriptMethods const &, CCharString const &) const; //slot714 0x008a0fd0
virtual CWideString GetFormattedString(CCharString const &, std::vector<CWideString, std::allocator<CWideString> > const &) const; //slot715 0x00891340
virtual CWideString GetTextString(CCharString const &) const; //slot716 0x008913a0
virtual void AddRumourCategory(CCharString const &) const; //slot717 0x008a9db0
virtual void AddNewRumourToCategory(CCharString const &, CCharString const &) const; //slot718 0x008a9dd0
virtual void RemoveRumourCategory(CCharString const &) const; //slot719 0x008a9e00
virtual void SetCategoryActivity(CCharString const &, bool) const; //slot720 0x008a96c0
virtual void AddGossipVillage(CCharString const &, CCharString const &) const; //slot721 0x008a9e30
virtual void AddGossipFactionToCategory(CCharString const &, CCharString const &) const; //slot722 0x008a9e60
virtual void SetIsGossipForPlayer(CCharString const &, bool) const; //slot724 0x008a96f0
virtual void SetIsGossipForPlayer(CCharString, bool); //slot723 0x008a97d0
virtual void UpdateOnlineScore_Archery(float) const; //slot725 0x008a1040
virtual void UpdateOnlineScore_ChickenKick(float) const; //slot726 0x008a10c0
virtual void UpdateOnlineScore_ChapelOrTemple(float) const; //slot727 0x008a1140
virtual void UpdateOnlineScore_FishingCompo(float) const; //slot728 0x008a11c0
virtual void UpdateScore_FishingCompetition(float) const; //slot729 0x008a1240
virtual float GetBestTimePairs(void) const; //slot730 0x008a1590
virtual float GetBestTimeSorting(void) const; //slot731 0x008a15f0
virtual long GetBestScoreBlackjack(void) const; //slot732 0x008a1650
virtual long GetBestScoreCoinGolfOakVale(void) const; //slot733 0x008a16b0
virtual long GetBestScoreCoinGolfSnowSpire(void) const; //slot734 0x008a1710
virtual long GetBestScoreShoveHaPenny(void) const; //slot735 0x008a1770
virtual float GetBestTimeGuessTheAddition(void) const; //slot736 0x008a17d0
virtual bool IsHeroInTavernGame(void) const; //slot737 0x008913c0
virtual long GetNumHousesOwned(void) const; //slot738 0x008a12a0
virtual void StartSneaking(void) const; //slot739 0x008a1300
virtual long GetStealDuration(ScriptMethods const &) const; //slot740 0x008a1360
virtual void SetUseableByHero(ScriptMethods const &, bool) const; //slot741 0x008a1830
virtual void SetOwnedByHero(ScriptMethods const &, bool) const; //slot742 0x008a18f0
virtual void SetTavernTableAvailableForUse(ScriptMethods const &, bool) const; //slot743 0x008a1890
virtual void SetIsThingTurncoatable(ScriptMethods const &, bool) const; //slot744 0x008a1950
virtual void SetIsThingForcePushable(ScriptMethods const &, bool) const; //slot745 0x0088f9f0
virtual void SetIsThingLightningable(ScriptMethods const &, bool) const; //slot746 0x0088fa20
virtual void SetIsThingEpicSpellable(ScriptMethods const &, bool) const; //slot747 0x0088fa50
virtual bool IsThingTurncoated(ScriptMethods const &) const; //slot748 0x008a19b0
virtual void AddCreatureScriptedMode(ScriptMethods const &, CCharString const &) const; //slot749 0x008a1a10
virtual void RemoveCreatureScriptedMode(ScriptMethods const &) const; //slot750 0x008a1ab0
virtual void ForceShipsVisible(void) const; //slot751 0x008a7f60
virtual bool GetSleepingPositionAndOrientationFromBed(ScriptMethods const &, ScriptMethods const &, C3DVector &, C3DVector &); //slot752 0x008a1b10
virtual void SetBedAvailability(ScriptMethods const &, bool); //slot753 0x008a1bf0
virtual void RepopulateVillage(ScriptMethods const &) const; //slot754 0x008a1c50
virtual void SmashAllWindowsWithinRadiusOfPoint(C3DVector const &, float); //slot755 0x008a8010
virtual ScriptMethods SetResidency(ScriptMethods const &, bool); //slot756 0x008a1cc0
virtual void SetThankingPhrase(ScriptMethods, unsigned long); //slot757 0x008a1e50
virtual unsigned long GetThankingPhrase(ScriptMethods); //slot758 0x008a1f30
virtual void ResetThankingPhrase(ScriptMethods); //slot759 0x008a2020
virtual void SetIgnoringPhrase(ScriptMethods, unsigned long); //slot760 0x008a2100
virtual unsigned long GetIgnoringPhrase(ScriptMethods); //slot761 0x008a21e0
virtual void ResetIgnoringPhrase(ScriptMethods); //slot762 0x008a22d0
virtual void SetWanderCentrePoint(ScriptMethods, C3DVector); //slot763 0x008a23b0
virtual C3DVector GetWanderCentrePoint(ScriptMethods); //slot764 0x008a24c0
virtual void ResetWanderCentrePoint(ScriptMethods); //slot765 0x008a2590
virtual void SetWanderMinDistance(ScriptMethods, float); //slot766 0x008a2690
virtual float GetWanderMinDistance(ScriptMethods); //slot767 0x008a2770
virtual void ResetWanderMinDistance(ScriptMethods); //slot768 0x008a2870
virtual void SetWanderMaxDistance(ScriptMethods, float); //slot769 0x008a2950
virtual float GetWanderMaxDistance(ScriptMethods); //slot770 0x008a2a30
virtual void ResetWanderMaxDistance(ScriptMethods); //slot771 0x008a2b30
virtual void SetGossipCounter(ScriptMethods, long); //slot772 0x008a2c10
virtual long GetGossipCounter(ScriptMethods); //slot773 0x008a2cf0
virtual void ResetGossipCounter(ScriptMethods); //slot774 0x008a2de0
virtual void SetMaxGossipPhrase(ScriptMethods, long); //slot775 0x008a2ec0
virtual long GetMaxGossipPhrase(ScriptMethods); //slot776 0x008a2fa0
virtual void ResetMaxGossipPhrase(ScriptMethods); //slot777 0x008a3090
virtual void SetWarningPhrase(ScriptMethods, unsigned long); //slot778 0x008a3170
virtual unsigned long GetWarningPhrase(ScriptMethods); //slot779 0x008a3250
virtual void ResetWarningPhrase(ScriptMethods); //slot780 0x008a3340
virtual void SetBeerRequestPhrase(ScriptMethods, unsigned long); //slot781 0x008a3420
virtual unsigned long GetBeerRequestPhrase(ScriptMethods); //slot782 0x008a3500
virtual void ResetBeerRequestPhrase(ScriptMethods); //slot783 0x008a35f0
virtual void SetScriptingStateGroup(ScriptMethods, EScriptingStateGroups); //slot784 0x008a36d0
virtual EScriptingStateGroups GetScriptingStateGroup(ScriptMethods); //slot785 0x008a37b0
virtual void ResetScriptingStateGroup(ScriptMethods); //slot786 0x008a38a0
virtual void SetMaxHeroReactionDistance(ScriptMethods, float); //slot787 0x008a3980
virtual float GetMaxHeroReactionDistance(ScriptMethods); //slot788 0x008a3a60
virtual void ResetMaxHeroReactionDistance(ScriptMethods); //slot789 0x008a3b60
virtual void SetActionFrequency(ScriptMethods, long); //slot790 0x008a3c40
virtual long GetActionFrequency(ScriptMethods); //slot791 0x008a3d20
virtual void ResetActionFrequency(ScriptMethods); //slot792 0x008a3e10
virtual void SetActionFrequencyVariation(ScriptMethods, float); //slot793 0x008a3ef0
virtual float GetActionFrequencyVariation(ScriptMethods); //slot794 0x008a3fd0
virtual void ResetActionFrequencyVariation(ScriptMethods); //slot795 0x008a40d0
virtual void SetAction(ScriptMethods, CCharString); //slot796 0x008a41b0
virtual CCharString GetAction(ScriptMethods); //slot797 0x008a42c0
virtual void ResetAction(ScriptMethods); //slot798 0x008a4390
virtual void SetFaceHeroForAction(ScriptMethods, bool); //slot799 0x008a4480
virtual bool GetFaceHeroForAction(ScriptMethods); //slot800 0x008a4560
virtual void ResetFaceHeroForAction(ScriptMethods); //slot801 0x008a4650
virtual void SetTargetName(ScriptMethods, CCharString); //slot802 0x008a4730
virtual CCharString GetTargetName(ScriptMethods); //slot803 0x008a4840
virtual void ResetTargetName(ScriptMethods); //slot804 0x008a4910
virtual void SetFollowDistance(ScriptMethods, float); //slot805 0x008a4a00
virtual float GetFollowDistance(ScriptMethods); //slot806 0x008a4ae0
virtual void ResetFollowDistance(ScriptMethods); //slot807 0x008a4be0
virtual void SetAttackHeroOnSight(ScriptMethods, bool); //slot808 0x008a4cc0
virtual bool GetAttackHeroOnSight(ScriptMethods); //slot809 0x008a4da0
virtual void ResetAttackHeroOnSight(ScriptMethods); //slot810 0x008a4e90
virtual void SetTimeToSpendHarassingHero(ScriptMethods, long); //slot811 0x008a4f70
virtual long GetTimeToSpendHarassingHero(ScriptMethods); //slot812 0x008a5050
virtual void ResetTimeToSpendHarassingHero(ScriptMethods); //slot813 0x008a5140
virtual void SetCombatNearbyEnemyFleeingBreakOffRange(ScriptMethods, float); //slot814 0x008a5220
virtual float GetCombatNearbyEnemyFleeingBreakOffRange(ScriptMethods); //slot815 0x008a5300
virtual void ResetCombatNearbyEnemyFleeingBreakOffRange(ScriptMethods); //slot816 0x008a5400
virtual void SetCombatNearbyBreakOffRange(ScriptMethods, float); //slot817 0x008a54e0
virtual float GetCombatNearbyBreakOffRange(ScriptMethods); //slot818 0x008a55c0
virtual void ResetCombatNearbyBreakOffRange(ScriptMethods); //slot819 0x008a56c0
virtual void SetStealStealableItems(ScriptMethods, bool); //slot820 0x008a57b0
virtual bool GetStealStealableItems(ScriptMethods); //slot821 0x008a5890
virtual void ResetStealStealableItems(ScriptMethods); //slot822 0x008a5980
virtual void SetRecoverStealableItems(ScriptMethods, bool); //slot823 0x008a5a70
virtual bool GetRecoverStealableItems(ScriptMethods); //slot824 0x008a5b50
virtual void ResetRecoverStealableItems(ScriptMethods); //slot825 0x008a5c40
virtual void SetTakeStealableItemToRandomDestination(ScriptMethods, bool); //slot826 0x008a5d30
virtual bool GetTakeStealableItemToRandomDestination(ScriptMethods); //slot827 0x008a5e10
virtual void ResetTakeStealableItemToRandomDestination(ScriptMethods); //slot828 0x008a5f00
virtual void SetKillSelfAndStealableItemAfterReachingDestination(ScriptMethods, bool); //slot829 0x008a5ff0
virtual bool GetKillSelfAndStealableItemAfterReachingDestination(ScriptMethods); //slot830 0x008a60d0
virtual void ResetKillSelfAndStealableItemAfterReachingDestination(ScriptMethods); //slot831 0x008a61c0
virtual void SetAllowedToFollow(ScriptMethods, bool); //slot832 0x008a62b0
virtual bool GetAllowedToFollow(ScriptMethods); //slot833 0x008a6390
virtual void ResetAllowedToFollow(ScriptMethods); //slot834 0x008a6480
virtual void SetTableName(ScriptMethods, CCharString); //slot835 0x008a6570
virtual CCharString GetTableName(ScriptMethods); //slot836 0x008a6680
virtual void ResetTableName(ScriptMethods); //slot837 0x008a6750
virtual void SetSeatName(ScriptMethods, CCharString); //slot838 0x008a6840
virtual CCharString GetSeatName(ScriptMethods); //slot839 0x008a6950
virtual void ResetSeatName(ScriptMethods); //slot840 0x008a6a20
virtual void SetDisableHeadLooking(ScriptMethods, bool); //slot841 0x008a6b10
virtual bool GetDisableHeadLooking(ScriptMethods); //slot842 0x008a6bf0
virtual void ResetDisableHeadLooking(ScriptMethods); //slot843 0x008a6ce0
virtual void SetIsPushableByHero(ScriptMethods, bool); //slot844 0x008a6dd0
};

struct CRGBColour {unsigned char B,G,R,A;CRGBColour(unsigned char r,unsigned char g,unsigned char b,unsigned char a){R=r;G=g;B=b;A=a;}};
struct TraderEscortNative {unsigned char ScriptState[0x55];bool ShownEarthTroll;};
struct CDarkwoodRockTroll {void* VTable;GameMethods* Game;ScriptMethods Me;TraderEscortNative* ParentClass;void* PMasterData;long HealthBarID;void Init();};
void CDarkwoodRockTroll::Init(){
 ParentClass->ShownEarthTroll=true;
 {CCharString third("BarrowFields",-1),second("BarrowFields",-1),first("TEXT_QUEST_DARKWOOD_TRADER_ESCORT_OBJECTIVE_04",-1);
 Game->SetQuestCardObjective(Game->GetActiveQuestName(),first,second,third);}
 Game->DisplayQuestInfo(true);
 {CCharString icon("HUD_QUEST_ICON_EARTH_TROLL",-1);
 HealthBarID=Game->AddQuestInfoBarHealth(Me,CRGBColour(255,0,0,255),icon,1.0f);}
 {CCharString marker("HUD_ORB_QUEST_CORE",-1);Game->MiniMapAddMarker(Me,marker);}
}