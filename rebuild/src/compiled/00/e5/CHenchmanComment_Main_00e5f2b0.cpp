// CHenchmanComment::Main() -- body-guard henchman chatter: collects the "BodyGuard" things and notes which one is the CREATURE_BODY_GUARD itself; then every frame, while any other henchman is within 10 units and more than 6 seconds have passed, asks the body-guard script for a comment (stopping once one is made); the timer restarts whenever no henchman is near
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E5F2B0, length 783 bytes (+ int3 padding).
// Callees: IsActiveThreadTerminating 0x00F35B30, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CCharString(const
//   char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CBaseClass() 0x0099A2D0 / ~CBaseClass 0x0099A2E0, ~CScriptThing 0x004AA840,
//   CV_BodyGuardScript::MakeHenchmanComment 0x00E5EBD0; CScriptThing slots 2 GetDefName / 3 GetDataString,
//   Compare 0x00411570 (the inlined CCharString ==), CCounter operator> 0x00D90670 / operator=(float) 0x00D90550 and its inline
//   ctor (thing manager 0x0143E8F8 slot 653); GameCalls slots
//   7 NewScriptFrame, 75 GetAllThingsWithScriptName.
// Data: "BodyGuard" 0x012CB3F0, "CREATURE_BODY_GUARD" 0x012578FC, "" 0x0122D70E.
// Layout: Ego_r.pdb CHenchmanComment (40 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14,
//   PMasterData +0x18, HenchmanVector +0x1c (retail STLport vector, 12 bytes).
// Shape: the comment call sits behind `counter > 6.0f &&`, so its two temporaries carry construction flags; the counter
//   lives in its own block (so the spilled `this` of the first loop can share its slot) and one function-scope index
//   serves both loops (a per-loop index swaps the index/offset frame words).
class CCharString;
class C3DVector;
class CBaseClass { public:                                     // vtable 0x01231710
 virtual void v00();
 CBaseClass();                                           // 0x0099A2D0
};
struct CCountBlock { long References; };
struct CCountedPointer {                                // CCountedPointer<CScriptThing>: {Data, Info}
 void* Data;
 CCountBlock* Info;
 CCountedPointer() : Data(0), Info(0) { }
 CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->References; }
 CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580 (folded with CCountedPointer<CDiskFileWin32>::operator=)
};
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C {vptr, PImp_Data, PImp_Info}
 virtual void v00();
 virtual void v01();
 virtual CCharString GetDefName() const;                 // slot 2 (+0x08) 0x004AA8D0
 virtual CCharString GetDataString() const;              // slot 3 (+0x0c) 0x004AA900
 virtual void v04(); virtual void v05();
 virtual const C3DVector& GetPos() const;               // slot 6 (+0x18)
 virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
 virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54) 0x004AAB30
 virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66();
 virtual void SetFriendsWithEverythingFlag(bool on);       // slot 67 (+0x10c) 0x004AB060
 virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74();
 virtual bool IsAlive() const;                            // slot 75 (+0x12c) 0x004AB130
 CCountedPointer PImp;                                  // +0x04 Data, +0x08 Info
 struct InlineCopy { };                                 // tag: the inline copy retail expands at by-value call sites
 CScriptThing() { }
 CScriptThing(const CScriptThing& other);               // 0x004ABE90 (the out-of-line copy)
 CScriptThing(const CScriptThing& other, InlineCopy) : CBaseClass(), PImp(other.PImp) { }
 virtual ~CScriptThing();                                       // 0x004AA840
 CScriptThing& operator=(const CScriptThing& other);    // 0x008AB980
};
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase { public:virtual bool operator()()const=0;virtual CPredicateBase* Clone()const=0;virtual ~CPredicateBase(){}};
class CIsThingAlive:public CPredicateBase { public:
 CScriptThing Thing;
 __forceinline CIsThingAlive(const CScriptThing& t):Thing(t){}
 virtual bool operator()()const;virtual CPredicateBase* Clone()const;
 ~CIsThingAlive(){}
};
}
class C2DCoordI;
class C2DVector;
class C3DVector;
class CCharString;
class CRGBColour;
class CRGBFloatColour;
class CRightHandedSet;
struct CScreenFilterSThingByPass;
class CScriptGameResourceObjectMovieBase;
class CScriptGameResourceObjectScriptedThingBase;
class CThing;
class CTimer;
class CWideString;
template<class T> class CArray;
class CCounter;
namespace std {template<class T> class allocator;template<class T> struct less;template<class A,class B>struct pair;template<class T,class A> class vector;template<class T,class A> class list;template<class T,class P,class A> class set;}
class CHeroLogBook { public: class CHeroLogBookEntry { public: typedef long ECategory; }; };
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
struct GameCalls {virtual ~GameCalls();
virtual void Error(CCharString const &, CCharString const &, CCharString const &, unsigned long) const; //slot1 0x0088de00
virtual void TraceMessage(CCharString const &) const; //slot2 0x006e7d20
virtual void Validate(void) const; //slot3 0x006e72a0
virtual void SetDebugCameraType(long) const; //slot4 0x0088e0f0
virtual void DeactivateBoastUi(void) const; //slot5 0x008a0c60
virtual bool IsXbox(void) const; //slot6 0x0088e100
virtual void NewScriptFrame(void) const; //slot7 0x006e7410
virtual bool StartScriptingEntity(CScriptThing const &, CScriptGameResourceObjectScriptedThingBase &, EScriptAIPriority); //slot8 0x0089b5b0
virtual bool IsEntityUnderScriptedControl(CScriptThing const &) const; //slot9 0x008960c0
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
virtual bool MsgOnHeroPickedPocket(CScriptThing &) const; //slot53 0x00893ff0
virtual bool MsgOnHeroPickedLock(CScriptThing &) const; //slot54 0x00894080
virtual bool MsgOnFishingGameFinished(CScriptThing &) const; //slot55 0x00894110
virtual bool MsgOnTavernGameFinished(CScriptThing const &) const; //slot56 0x008941a0
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
virtual CScriptThing & GetHero(void); //slot70 0x00891ca0
virtual CScriptThing GetHeroTargetedThing(void) const; //slot71 0x0089a490
virtual CScriptThing GetThingWithScriptName(CCharString const &, CCharString const &) const; //slot73 0x008930f0
virtual CScriptThing GetThingWithScriptName(CCharString const &) const; //slot72 0x008a7d60
virtual CScriptThing GetRandomThingWithScriptName(CCharString const &) const; //slot74 0x008a7e00
virtual long GetAllThingsWithScriptName(CCharString const &, std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; //slot75 0x008a8570
virtual long GetAllCreaturesInAreaWithScriptName(CCharString const &, C3DVector const &, float, std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; //slot76 0x008a86c0
virtual CScriptThing GetNearestWithScriptName(CScriptThing const &, CCharString const &) const; //slot77 0x0089a510
virtual CScriptThing GetFurthestWithScriptName(CScriptThing const &, CCharString const &) const; //slot78 0x0089a6e0
virtual long GetAllThingsWithDefNameByDistanceFrom(CScriptThing const &, CCharString const &, std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; //slot79 0x008a9c40
virtual long GetAllThingsWithDefName(CCharString const &, std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; //slot80 0x008a8890
virtual CScriptThing GetNearestWithDefName(CScriptThing const &, CCharString const &) const; //slot81 0x0089a880
virtual CScriptThing GetFurthestWithDefName(CScriptThing const &, CCharString const &) const; //slot82 0x0089aa70
virtual CScriptThing GetThingWithUID(unsigned __int64) const; //slot83 0x00891d50
virtual long GetAllCreaturesExcludingHero(std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; //slot84 0x008a89d0
virtual long GetAllThingsInLevel(CCharString const &, std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; //slot85 0x008a8af0
virtual bool IsThingWithThisUIDAlive(unsigned __int64) const; //slot86 0x0088e260
virtual long RegisterTimer(void); //slot87 0x006e79e0
virtual void DeregisterTimer(long); //slot88 0x006e7690
virtual void SetTimer(long, long); //slot89 0x006e76e0
virtual long GetTimer(long) const; //slot90 0x006e7710
virtual CScriptThing CreateCreature(CCharString const &, C3DVector const &, CCharString const &, bool) const; //slot91 0x008a9100
virtual CScriptThing CreateCreatureNearby(CCharString const &, C3DVector const &, float, CCharString const &, bool) const; //slot92 0x0089f300
virtual CScriptThing CreateCreatureOnEntity(CCharString const &, CScriptThing const &, CCharString const &) const; //slot93 0x00894500
virtual CScriptThing TurnCreatureInto(CScriptThing const &, CCharString const &) const; //slot94 0x0089f4a0
virtual void SetCreatureCreationDelayFrames(long); //slot95 0x006e7d30
virtual void ResetCreatureCreationDelayFrames(void); //slot96 0x006e7d40
virtual CScriptThing CreateObject(CCharString const &, C3DVector const &, CCharString const &) const; //slot98 0x00894820
virtual CScriptThing CreateObject(CCharString const &, C3DVector const &, float, CCharString const &) const; //slot97 0x00894950
virtual CScriptThing CreateObjectOnEntity(CCharString const &, CScriptThing const &, CCharString const &) const; //slot99 0x008946e0
virtual CScriptThing CreateEffect(CCharString const &, CScriptThing const &, CCharString const &, CCharString const &, bool, bool) const; //slot101 0x0089f910
virtual CScriptThing CreateEffect(CCharString const &, C3DVector const &, CCharString const &, float, bool, bool) const; //slot100 0x0089f9e0
virtual CScriptThing CreateLight(C3DVector const &, CRGBColour const &, CCharString const &, float, float, bool) const; //slot102 0x0089faa0
virtual CScriptThing CreateExperienceOrb(C3DVector const &, long) const; //slot103 0x0089fbb0
virtual CScriptThing CreateExplosion(CCharString const &, C3DVector const &, C3DVector const *, CCharString) const; //slot104 0x0089fc80
virtual CScriptThing CreatePhysicalBarrier(float, C3DVector const &, C3DVector const *, CCharString) const; //slot105 0x0089fd20
virtual CScriptThing CreateRumble(C3DVector const &, float, float, CCharString) const; //slot106 0x0089fda0
virtual void ClearAllRumbles(void) const; //slot107 0x00892df0
virtual void RemoveThing(CScriptThing const &, bool, bool) const; //slot108 0x008910d0
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
virtual void GiveHeroItem(CScriptThing const &) const; //slot123 0x00898bb0
virtual bool GiveHeroItemsFromContainer(CScriptThing const &, bool) const; //slot124 0x00898c40
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
virtual bool IsReportedOrUnreportedCrimeKnown(CScriptThing const &) const; //slot136 0x008985a0
virtual void ConfiscateAllHeroItems(void) const; //slot137 0x008986d0
virtual void ConfiscateAllHeroWeapons(void) const; //slot138 0x008987e0
virtual void ConfiscateItemsOfTypeFromHero(CCharString const &) const; //slot139 0x00898840
virtual void ReturnAllConfiscatedItemsToHero(void) const; //slot140 0x00898940
virtual bool GiveHeroTutorial(ETutorialCategory) const; //slot141 0x00898a50
virtual void MakeHeroCarryItemInHand(CCharString const &) const; //slot143 0x00897e10
virtual void MakeHeroCarryItemInHand(CScriptThing const &, bool, bool) const; //slot142 0x00897fa0
virtual void AddTattooToHero(CCharString const &) const; //slot144 0x008981c0
virtual void GiveHeroAbility(EHeroAbility, bool) const; //slot145 0x0088fcc0
virtual bool IsPlayerZTargetingThing(CScriptThing const &) const; //slot146 0x00899b10
virtual bool IsPlayerCreatureBlocking(void) const; //slot147 0x00899ba0
virtual bool IsPlayerCreatureReadyToFireProjectileWeapon(float &) const; //slot148 0x00899c10
virtual long GetPlayerCreatureCombatMultiplier(void) const; //slot149 0x00899c90
virtual long GetPlayerCreatureCombatMultiplierRunningNumHits(void) const; //slot150 0x00899cf0
virtual void ResetPlayerCreatureCombatMultiplier(void) const; //slot151 0x00899d50
virtual bool IsPlayerCreatureFlourishEnabled(void) const; //slot152 0x00899db0
virtual void SetPlayerCreatureOnlyTarget(CScriptThing const &) const; //slot153 0x00899e10
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
virtual bool IsObjectInThingsPossession(CCharString const &, CScriptThing const &) const; //slot184 0x00897220
virtual bool IsHeroHandLampLit(void) const; //slot185 0x00898e60
virtual void SetHeroHandLampAsLit(bool) const; //slot186 0x00898ec0
virtual bool IsWearingClothingItem(CScriptThing const &, CCharString const &) const; //slot187 0x0089b2b0
virtual bool IsHeroNaked(void) const; //slot188 0x0089b330
virtual void RemoveHeroClothing(void) const; //slot189 0x0089b390
virtual void SetHeroAsWearing(CCharString const &) const; //slot190 0x0089b3f0
virtual void ChangeHeroHairstyle(CCharString const &) const; //slot191 0x0089b450
virtual void RemoveHeroHairstyle(void) const; //slot192 0x0089b4d0
virtual bool IsWearingHairstyle(CScriptThing &, CCharString const &) const; //slot193 0x0089b530
virtual bool IsPlayerCarryingItemOfType(CCharString const &) const; //slot194 0x008973a0
virtual bool IsPlayerWieldingWeapon(void) const; //slot195 0x00897480
virtual bool IsEntityWieldingWeapon(CScriptThing const &) const; //slot196 0x008974f0
virtual bool IsEntityWieldingMeleeWeapon(CScriptThing const &) const; //slot197 0x00890220
virtual bool IsEntityWieldingRangedWeapon(CScriptThing const &) const; //slot198 0x00890250
virtual bool GetPreviouslyWieldedMeleeWeaponName(CCharString &) const; //slot199 0x00897560
virtual bool GetPreviouslyWieldedRangedWeaponName(CCharString &) const; //slot200 0x00897630
virtual bool IsEntityAbleToAttack(CScriptThing const &) const; //slot201 0x00890280
virtual CScriptThing EntityGetThingInPrimarySlot(CScriptThing const &) const; //slot202 0x008976f0
virtual void ApplyHeroPenaltyForDeath(void) const; //slot203 0x008977f0
virtual void GiveHeroTitle(CCharString const &) const; //slot204 0x0088e110
virtual EHeroTitle GetHeroTitle(void) const; //slot205 0x00897880
virtual void EntitySetAsMarryable(CScriptThing const &, bool) const; //slot206 0x00894dd0
virtual void EntitySetAsAbleToRegionFollowWhenMarried(CScriptThing const &, bool) const; //slot207 0x008978e0
virtual void EntityForceMarriageToHero(CScriptThing const &, bool) const; //slot208 0x00897940
virtual bool IsEntityMarriedToHero(CScriptThing const &) const; //slot209 0x008979c0
virtual bool IsEntityMarriable(CScriptThing const &) const; //slot210 0x00897a30
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
virtual void GiveThingHeroRewardItem(CScriptThing const &, CCharString const &, CCharString const &) const; //slot222 0x00898ac0
virtual void GiveThingItemInHand(CScriptThing const &, CCharString const &, bool) const; //slot223 0x0088de10
virtual void GiveThingItemInSlot(CScriptThing const &, CCharString const &, CCharString const &) const; //slot224 0x00895190
virtual void GiveHeroExpression(CCharString const &, long, bool) const; //slot225 0x0088fc60
virtual bool HeroHasExpression(CCharString const &) const; //slot226 0x0088fc90
virtual bool IsHeroPerformingExpression(CCharString const &) const; //slot227 0x008953a0
virtual bool IsDPadButtonHeldForExpression(CCharString const &) const; //slot228 0x00895430
virtual void EntityFollowThing(CScriptThing const &, CScriptThing const &, float, bool) const; //slot229 0x008954a0
virtual void EntityStopFollowing(CScriptThing const &) const; //slot230 0x008a7980
virtual bool GetFollowingEntityList(CScriptThing const &, std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; //slot231 0x008a80f0
virtual bool GetHeroSummonedCreaturesList(std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; //slot232 0x008a8260
virtual bool GetPerceivingHeroEntityList(std::vector<CScriptThing, std::allocator<CScriptThing> > &) const; //slot233 0x008a8440
virtual bool IsEntityFollowingHero(CScriptThing const &) const; //slot234 0x008955c0
virtual void EntitySetAsAllowedToFollowHero(CScriptThing const &, bool) const; //slot235 0x0088de90
virtual void EntitySetAsAllowedToChangeRegionFollowingState(CScriptThing const &, bool) const; //slot236 0x0088ded0
virtual void EntitySetAsRespondingToFollowAndWaitExpressions(CScriptThing const &, bool) const; //slot237 0x0088df20
virtual void EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(CScriptThing const &, bool) const; //slot238 0x00895650
virtual void TeleportAllFollowersToHeroPosition(void) const; //slot239 0x008a7a70
virtual void EntityTeleportToHeroPosition(CScriptThing const &) const; //slot240 0x008956b0
virtual void SendEntityEvent(NEntityEvents::EEventType, CScriptThing const &, CScriptThing const &, CThing *) const; //slot241 0x008957c0
virtual float GetWaterHeightAtPosition(C3DVector const &) const; //slot242 0x0088df60
virtual bool IsFishingSpotEnabled(CScriptThing const &) const; //slot243 0x008958f0
virtual void DisableFishingSpot(CScriptThing const &) const; //slot244 0x00895950
virtual void UpdateFishWeight(CScriptThing const &) const; //slot245 0x008959b0
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
virtual float GetHealth(CScriptThing const &) const; //slot264 0x00890400
virtual void ModifyThingHealth(CScriptThing const &, float, bool) const; //slot265 0x00890430
virtual void EntitySetMaxHealth(CScriptThing const &, float, bool) const; //slot266 0x0088e1c0
virtual void SetThingAsKilled(CScriptThing const &) const; //slot267 0x0089a270
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
virtual CScriptThing GetMostRecentValidUsedTarget(void) const; //slot319 0x00891610
virtual CCharString GetMostRecentValidUsedTargetName(void) const; //slot320 0x00891680
virtual void DisplayQuestInfo(bool) const; //slot321 0x00891920
virtual void SetQuestInfoName(char const *) const; //slot322 0x00891940
virtual void SetQuestInfoText(char const *) const; //slot323 0x00891a00
virtual long AddQuestInfoBar(float, float, CRGBColour const &, CRGBColour const &, CCharString const &, CCharString const &, float) const; //slot324 0x00891ac0
virtual long AddQuestInfoBarHealth(CScriptThing const &, CRGBColour const &, CCharString const &, float) const; //slot325 0x00891ad0
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
virtual bool IsEntityPickPocketable(CScriptThing const &) const; //slot344 0x0088e220
virtual bool IsEntityPickLockable(CScriptThing const &) const; //slot345 0x0089a2d0
virtual bool IsEntityStealable(CScriptThing const &) const; //slot346 0x0089a330
virtual void EntitySetAsPickPocketed(CScriptThing const &) const; //slot347 0x00890470
virtual void EntitySetAsPickLocked(CScriptThing const &) const; //slot348 0x0089a390
virtual void EntitySetAsStolen(CScriptThing const &) const; //slot349 0x008904e0
virtual void MiniMapAddMarker(CScriptThing const &, CCharString const &) const; //slot350 0x00891f50
virtual void MiniMapSetMarkerGraphic(CScriptThing const &, CCharString const &) const; //slot351 0x00891fa0
virtual void MiniMapRemoveMarker(CScriptThing const &) const; //slot352 0x00891ff0
virtual void MiniMapRemoveAllMarkers(void) const; //slot353 0x00892030
virtual void MiniMapAllowRouteBetweenRegions(CCharString const &, CCharString const &, bool) const; //slot354 0x0088e3d0
virtual void MiniMapSetAsEnabled(bool) const; //slot355 0x0088e400
virtual void EntitySetAsHiddenOnMiniMap(CScriptThing const &, bool) const; //slot356 0x0088e410
virtual void SetHUDEnabled(bool) const; //slot357 0x00892050
virtual bool TextEntryExists(CCharString const &) const; //slot358 0x00892070
virtual CCharString GetValidTextEntryNameWithAttitude(CScriptThing const &, CCharString const &) const; //slot359 0x008920c0
virtual void SetThingHasInformation(CScriptThing const &, bool, bool, bool) const; //slot360 0x0088e440
virtual void ClearThingHasInformation(CScriptThing const &) const; //slot361 0x0088e480
virtual void EntitySetWillBeUsingNarrator(CScriptThing const &, CCharString const &) const; //slot362 0x0089b050
virtual void EntityResetAsPureAINarrator(CScriptThing const &) const; //slot363 0x0089b0b0
virtual long AddNewConversation(CScriptThing const &, bool, bool) const; //slot364 0x008906c0
virtual void AddPersonToConversation(long, CScriptThing const &) const; //slot365 0x00890710
virtual void AddLineToConversation(long, CCharString const &, bool, CScriptThing const &, CScriptThing const &) const; //slot366 0x00890750
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
virtual void CameraCircleAroundThing(CScriptThing const &, C3DVector const &, float) const; //slot404 0x00892b50
virtual void CameraCircleAroundPos(C3DVector const &, C3DVector const &, float) const; //slot405 0x00892c20
virtual void CameraMoveToPosAndLookAtPos(C3DVector const &, C3DVector const &, float) const; //slot406 0x00892410
virtual void CameraMoveToPosAndLookAtThing(C3DVector const &, CScriptThing const &, float) const; //slot407 0x008924b0
virtual void CameraMoveBetweenLookingAt(C3DVector const &, C3DVector const &, C3DVector const &, float, float) const; //slot409 0x00892530
virtual void CameraMoveBetweenLookingAt(C3DVector const &, C3DVector const &, CScriptThing const &, float, float) const; //slot408 0x008926d0
virtual void CameraMoveBetweenLookFromAndLookTo(C3DVector const &, C3DVector const &, C3DVector const &, C3DVector const &, float) const; //slot410 0x00892610
virtual void CameraUseCameraPoint(CCharString const &, CScriptThing const &, float, long, long) const; //slot414 0x008922f0
virtual void CameraUseCameraPoint(CScriptThing const &, CScriptThing const &, float, long, long) const; //slot413 0x00891070
virtual void CameraUseCameraPoint(CCharString const &, C3DVector const *, CRightHandedSet const *, float, long, long) const; //slot412 0x00892380
virtual void CameraUseCameraPoint(CScriptThing const &, C3DVector const *, CRightHandedSet const *, float, long, long) const; //slot411 0x0089e190
virtual void CameraDoConversation(CScriptThing const &, CScriptThing const &, ECameraOp, bool) const; //slot415 0x0088edc0
virtual void CameraDefault(void); //slot416 0x00890ff0
virtual void CameraResetToViewBehindHero(float); //slot417 0x00890fc0
virtual bool IsCameraInScriptedMode(void) const; //slot418 0x0088ecc0
virtual void CameraUseScreenEffect(float, float, float) const; //slot419 0x0088eda0
virtual void CameraCancelScreenEffect(void) const; //slot420 0x0088edb0
virtual bool IsCameraPosOnScreen(C3DVector const &) const; //slot421 0x006e7300
virtual float GetGameAngleXY(void) const; //slot422 0x006e7340
virtual void CameraEarthquakeIntensityAtPos(C3DVector const &, float, float) const; //slot423 0x0088ed20
virtual void CameraShake(float, float) const; //slot424 0x0088ece0
virtual void OpenDoor(CScriptThing const &) const; //slot425 0x00895db0
virtual void CloseDoor(CScriptThing const &) const; //slot426 0x00895de0
virtual void OpenHouseDoors(CScriptThing const &) const; //slot427 0x0088dfd0
virtual void CloseHouseDoors(CScriptThing const &) const; //slot428 0x0088e000
virtual void JamDoor(CScriptThing const &) const; //slot429 0x00895e10
virtual void SetDoorTriggerType(CScriptThing const &, EDoorTriggerType) const; //slot430 0x00895e70
virtual void OverrideAutomaticHouseLocking(CScriptThing const &, bool) const; //slot431 0x0088e030
virtual void SetHouseOwnedByPlayer(CScriptThing const &, bool, bool) const; //slot432 0x00895ed0
virtual void SetBuyableHouseAsScripted(CScriptThing const &, bool) const; //slot433 0x00896000
virtual bool IsChestOpen(CScriptThing const &) const; //slot434 0x00896330
virtual bool OpenChest(CScriptThing const &, bool) const; //slot435 0x00896390
virtual void CloseChest(CScriptThing const &) const; //slot436 0x00896660
virtual long GetNumberOfKeysNeededToUnlockChest(CScriptThing const &, CCharString &) const; //slot437 0x008966c0
virtual void DisplayLockedChestMessage(CScriptThing const &) const; //slot438 0x00896730
virtual void SetTrophyAsMountable(CScriptThing const &, bool) const; //slot439 0x00896790
virtual void SetVillageLimbo(CScriptThing const &, bool) const; //slot440 0x00896060
virtual void SetCreatureNotReload(CScriptThing const &) const; //slot441 0x0088fd90
virtual bool IsSleepingTime(CScriptThing const &) const; //slot442 0x00896210
virtual void EnableGuards(CScriptThing const &, bool) const; //slot443 0x00896270
virtual void EnableVillagerDefTypes(CScriptThing const &, bool, CCharString const &) const; //slot444 0x008962d0
virtual CScriptThing TryToRespawnDefNamed(CScriptThing const &, CCharString const &, C3DVector const &) const; //slot445 0x00896120
virtual void ClearHeroEnemyOfGuards(CScriptThing const &) const; //slot446 0x008961b0
virtual void SetThingAsUsable(CScriptThing const &, bool) const; //slot447 0x008967f0
virtual void SetThingHomeBuilding(CScriptThing const &, CScriptThing const &) const; //slot448 0x00892e30
virtual void SetVillageAttitude(CScriptThing const &, EScriptVillageAttitude) const; //slot449 0x0089fe20
virtual void AddCrimeCommitted(CScriptThing const &, ECrime, bool, CScriptThing const *, CScriptThing const *, EOpinionPostDeedType) const; //slot450 0x0089feb0
virtual void GiveThingBestEnemyTarget(CScriptThing const &, CScriptThing const &) const; //slot451 0x008a0260
virtual void ClearThingBestEnemyTarget(CScriptThing const &) const; //slot452 0x008a03f0
virtual void EntitySetInLimbo(CScriptThing const &, bool, bool) const; //slot453 0x0088fd00
virtual bool IsEntityInLimbo(CScriptThing const &) const; //slot454 0x0088fd60
virtual bool EntityGetShotStrikePos(CScriptThing const &, C3DVector &) const; //slot455 0x0089df70
virtual void EntitySetNegateAllHits(CScriptThing const &, bool) const; //slot456 0x00890e80
virtual void EntitySetEvadeAllHits(CScriptThing const &, bool) const; //slot457 0x00890ec0
virtual void EntitySetAbleToBeEngagedInCombat(CScriptThing const &, bool) const; //slot458 0x00890f00
virtual void EntitySetAlwaysBlockAttacksFromThing(CScriptThing const &, CScriptThing const &, bool) const; //slot459 0x0089dff0
virtual void EntitySetAttackThingImmediately(CScriptThing const &, CScriptThing const &, bool, bool) const; //slot460 0x008a8eb0
virtual void EntitySetCombatType(CScriptThing const &, CCharString const &) const; //slot461 0x0089e080
virtual void EntityResetCombatTypeToDefault(CScriptThing const &) const; //slot462 0x00890f40
virtual void EntitySetMaxNumberOfAttackers(CScriptThing const &, long) const; //slot463 0x0089e130
virtual void EntityClearMaxNumberOfAttackers(CScriptThing const &) const; //slot464 0x0088ec70
virtual void EntityAttachToScript(CScriptThing const &, CCharString const &) const; //slot465 0x00890690
virtual void EntitySetCombatAbility(CScriptThing const &, float) const; //slot466 0x0089aff0
virtual void EntitySetRangedTarget(CScriptThing const &, CScriptThing const &) const; //slot467 0x008a04c0
virtual void EntityClearRangedTarget(CScriptThing const &) const; //slot468 0x008a0540
virtual void EntitySetTargetable(CScriptThing const &, bool) const; //slot469 0x008a05a0
virtual void EntitySetTargetingType(CScriptThing const &, ETargetingType) const; //slot470 0x008a0600
virtual void EntitySetTargetingValidTargetWithoutLOS(CScriptThing const &, bool) const; //slot471 0x008a0660
virtual void EntityTeleportToThing(CScriptThing const &, CScriptThing const &, bool) const; //slot472 0x0088e540
virtual void EntityTeleportToPosition(CScriptThing const &, C3DVector const &, float, bool, bool) const; //slot473 0x0089b780
virtual void EntitySetFacingAngle(CScriptThing const &, float, bool) const; //slot474 0x0089bdf0
virtual void EntitySetFacingAngleTowardsThing(CScriptThing const &, CScriptThing const &, bool) const; //slot475 0x0088e620
virtual void EntitySetPerceptionVariables(CScriptThing const &, float, float, float) const; //slot476 0x0089bf80
virtual void SetThingPersistent(CScriptThing const &, bool) const; //slot477 0x0088e5a0
virtual void EntitySetThingAsWantingMoney(CScriptThing const &, long) const; //slot478 0x0089bb30
virtual void EntitySetAppearanceMorphSeed(CScriptThing const &, long) const; //slot479 0x008a0200
virtual void SetEntityAsRegionFollowing(CScriptThing const &, CScriptThing const &, bool) const; //slot480 0x008a0b10
virtual void SetEntityAsFollowingHeroThroughTeleporters(CScriptThing const &, bool) const; //slot481 0x0088f670
virtual void EntitySetAppearanceSeed(CScriptThing const &, unsigned long) const; //slot482 0x008a0bc0
virtual void EntityGetAppearanceSeed(CScriptThing const &, unsigned long &) const; //slot483 0x008a0c10
virtual void EntitySetAsForSale(CScriptThing const &, bool) const; //slot484 0x008a00e0
virtual void EntitySetStockItemPrice(CScriptThing const &, long) const; //slot485 0x008a0140
virtual long EntityGetStockItemPrice(CScriptThing const &) const; //slot486 0x008a01a0
virtual void EntityPlayObjectAnimation(CScriptThing const &, CCharString const &, bool) const; //slot487 0x0089bbc0
virtual void EntitySetMaxRunningSpeed(CScriptThing const &, float) const; //slot488 0x0088e750
virtual void EntitySetMaxWalkingSpeed(CScriptThing const &, float) const; //slot489 0x0088e7b0
virtual void EntityResetMaxRunningSpeed(CScriptThing const &) const; //slot490 0x0088e810
virtual void EntityResetMaxWalkingSpeed(CScriptThing const &) const; //slot491 0x0088e870
virtual void EntityAttachToVillage(CScriptThing const &, CScriptThing const &) const; //slot492 0x0089d3a0
virtual void EntitySetAsSittingOnFloor(CScriptThing const &, bool) const; //slot493 0x008a8c60
virtual void EntitySetAsHavingBoundHands(CScriptThing &, bool) const; //slot494 0x0089d420
virtual void EntitySetAsRemoveAllMovementBlockingModes(CScriptThing &) const; //slot495 0x0088e8d0
virtual void EntitySetAsScared(CScriptThing const &, bool) const; //slot496 0x0089d4c0
virtual void EntitySetAsDrunk(CScriptThing &, bool) const; //slot497 0x0089d560
virtual void EntityForceToLookAtThing(CScriptThing const &, CScriptThing const &) const; //slot498 0x0089d830
virtual void EntityForceToLookAtCamera(CScriptThing const &, bool) const; //slot499 0x0089d8b0
virtual void EntityForceToLookAtNothing(CScriptThing const &) const; //slot500 0x0089d910
virtual void EntityResetForceToLookAt(CScriptThing const &) const; //slot501 0x0089d970
virtual void EntitySetShotAccuracyPercentage(CScriptThing const &, long) const; //slot502 0x0088e5f0
virtual CScriptThing EntityGetStandingOnThing(CScriptThing const &) const; //slot503 0x00892220
virtual CScriptThing EntityGetStandingInsideBuilding(CScriptThing const &) const; //slot504 0x0089bc50
virtual void EntityDropGenericBox(CScriptThing const &) const; //slot505 0x0089d9d0
virtual void EntitySheatheWeapons(CScriptThing const &, bool) const; //slot506 0x00890ba0
virtual void EntityUnsheatheWeapons(CScriptThing const &, bool) const; //slot507 0x00890c10
virtual void EntityUnsheatheMeleeWeapon(CScriptThing const &, bool) const; //slot508 0x00890c80
virtual void EntityUnsheatheRangedWeapon(CScriptThing const &, bool) const; //slot509 0x00890d80
virtual void EntitySetAlpha(CScriptThing const &, float, bool) const; //slot510 0x0089da70
virtual void EntitySetAsDrawable(CScriptThing const &, bool) const; //slot511 0x0088e9f0
virtual void EntitySetCutsceneBehaviour(CScriptThing const &, ECutsceneBehaviour) const; //slot512 0x0088ea20
virtual ESex EntityGetSex(CScriptThing const &) const; //slot513 0x0088ea50
virtual void EntitySetAsAbleToWalkThroughSolidObjects(CScriptThing const &, bool) const; //slot514 0x0089dbb0
virtual void EntitySetAsRespondToHit(CScriptThing const &, bool) const; //slot515 0x0088ea90
virtual void EntitySetAsDamageable(CScriptThing const &, bool) const; //slot516 0x0088ead0
virtual void EntitySetAsKillable(CScriptThing const &, bool, bool) const; //slot517 0x0088eb30
virtual void EntitySetAsLocked(CScriptThing const &, bool) const; //slot518 0x0089dc10
virtual void EntityDecapitate(CScriptThing const &) const; //slot519 0x008a8e40
virtual void EntityGiveGold(CScriptThing const &, long) const; //slot520 0x0089dc70
virtual void EntitySetAllowBossPhaseChanges(CScriptThing const &, bool) const; //slot521 0x0089dcd0
virtual long EntityGetBossPhase(CScriptThing const &) const; //slot522 0x0089dd30
virtual void EntitySetBossPhase(CScriptThing const &, long) const; //slot523 0x0089dd90
virtual void EntityResetCreatureMode(CScriptThing const &) const; //slot524 0x0089ddf0
virtual bool EntitySetAsReceivingEvents(CScriptThing const &, bool) const; //slot525 0x00895010
virtual void EntitySetAsToAddToComboMultiplierWhenHit(CScriptThing const &, bool) const; //slot526 0x0088ebc0
virtual void EntitySetAsToAddToStatChangesWhenHit(CScriptThing const &, bool) const; //slot527 0x0088ebf0
virtual void EntityLeaveCombatStance(CScriptThing const &) const; //slot528 0x0089de50
virtual void EntitySetAsUseMovementInActions(CScriptThing const &, bool) const; //slot529 0x0088ec20
virtual void EntitySetAsDisplayingEmoteIcon(CScriptThing const &, bool) const; //slot530 0x0089deb0
virtual void EntitySetAsCollidableToThings(CScriptThing const &, bool) const; //slot531 0x00891120
virtual void EntityEnableGravity(CScriptThing const &, bool) const; //slot532 0x0088f4d0
virtual void EntitySetLightAsOn(CScriptThing const &, bool) const; //slot533 0x008a06c0
virtual void EntityFadeOut(CScriptThing const &, float) const; //slot534 0x008a0720
virtual void EntityFadeIn(CScriptThing const &, float) const; //slot535 0x008a07c0
virtual void EntityBeginLoadingAnimation(CScriptThing const &, CCharString const &) const; //slot536 0x0088f500
virtual void EntityBeginLoadingBasicAnimations(CScriptThing const &) const; //slot537 0x0088f530
virtual bool EntityCastForcePush(CScriptThing const &, bool) const; //slot538 0x008a9320
virtual void EntityCastLightningAtTarget(CScriptThing const &, CScriptThing const &) const; //slot539 0x0088f560
virtual void BeginLoadingMesh(CCharString const &) const; //slot540 0x00894aa0
virtual bool EntityWillTeleportToArea(CScriptThing const &, C3DVector, float, float) const; //slot541 0x008a93a0
virtual void EntityStartScreamerSuperAttackThing(CScriptThing const &, CScriptThing const &) const; //slot542 0x008a08a0
virtual void EntityEndScreamerSuperAttackThing(CScriptThing const &) const; //slot543 0x008a0950
virtual void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool); //slot544 0x0088ec60
virtual void SetLightColour(CScriptThing const &, CRGBColour const &) const; //slot545 0x0089df10
virtual void CreatureGeneratorSetFamily(CScriptThing const &, CCharString const &) const; //slot546 0x008a9020
virtual void CreatureGeneratorTrigger(CScriptThing const &) const; //slot547 0x0089e8e0
virtual void CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(CScriptThing const &, bool) const; //slot548 0x0089e950
virtual bool CreatureGeneratorIsDepleted(CScriptThing const &) const; //slot549 0x0089e9c0
virtual bool CreatureGeneratorIsDestroyed(CScriptThing const &) const; //slot550 0x0089ea30
virtual void CreatureGeneratorSetGeneratedCreatureScriptName(CScriptThing const &, CCharString const &) const; //slot551 0x0089eaa0
virtual void CreatureGeneratorSetNumTriggers(CScriptThing const &, long) const; //slot552 0x0089eb10
virtual long CreatureGeneratorGetNumGeneratedCreatures(CScriptThing const &) const; //slot553 0x0089eb80
virtual bool CreatureGeneratorAreAllCreaturesAlive(CScriptThing const &) const; //slot554 0x0089ebf0
virtual void CreatureGeneratorAddTriggerer(CScriptThing const &, CScriptThing const &) const; //slot555 0x0089ec60
virtual void CreatureGeneratorRemoveTriggerer(CScriptThing const &, CScriptThing const &) const; //slot556 0x0089ecc0
virtual void SetCreatureGeneratorEnabled(CScriptThing const &, bool) const; //slot557 0x0089ed20
virtual void SetCreatureGeneratorsEnabled(CCharString const &, bool) const; //slot558 0x00892cf0
virtual void SetCreatureGeneratorsEnabledDuringScript(CCharString const &, bool) const; //slot559 0x00892d70
virtual void SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup, bool) const; //slot560 0x0088f400
virtual bool IsCreatureGenerationEnabledForRegion(CCharString const &) const; //slot561 0x00892d80
virtual bool IsCreatureFlying(CScriptThing const &) const; //slot562 0x0089ed80
virtual void SetTeleporterAsActive(CScriptThing const &, bool) const; //slot563 0x0089ee00
virtual bool IsTeleporterActive(CScriptThing const &) const; //slot564 0x0089ee60
virtual void SetTeleportingAsActive(bool) const; //slot565 0x0088f410
virtual bool IsTeleportingActive(void) const; //slot566 0x0088f420
virtual void SetRegionExitAsActive(CScriptThing const &, bool) const; //slot567 0x0089eec0
virtual void SetRegionEntranceAsActive(CScriptThing const &, bool) const; //slot568 0x0089ef00
virtual void SetRegionTextDisplayAsActive(bool) const; //slot569 0x0088f430
virtual void SetHeroSleepingAsEnabled(bool) const; //slot570 0x0088f440
virtual bool IsHeroSleepingEnabled(void) const; //slot571 0x0088f450
virtual void SetExperienceSpendingAsEnabled(bool) const; //slot572 0x0088f460
virtual void SetMoralityChangingAsEnabled(bool) const; //slot573 0x0089ef30
virtual void SetSummonerDeathExplosionAffectsHero(bool) const; //slot574 0x0088f470
virtual CScriptThing GetNearestEnabledDiggingSpot(CScriptThing const &) const; //slot575 0x0089ef90
virtual bool IsDiggingSpotEnabled(CScriptThing const &) const; //slot576 0x0089f1e0
virtual bool IsDiggingSpotHidden(CScriptThing const &) const; //slot577 0x0089f240
virtual void SetDiggingSpotAsHidden(CScriptThing const &, bool) const; //slot578 0x0089f2a0
virtual bool CheckForCameraMessage(CCharString const &) const; //slot579 0x006e7920
virtual void WaitForCameraMessage(CCharString const &) const; //slot580 0x006e7820
virtual void SetThingAsConscious(CScriptThing const &, bool, CCharString const &) const; //slot581 0x008a9610
virtual void SetFireToThing(CScriptThing const &) const; //slot582 0x0089bcd0
virtual void ExtinguishFiresOnThing(CScriptThing const &) const; //slot583 0x0089bd30
virtual bool IsThingOnFire(CScriptThing const &) const; //slot584 0x0089bd90
virtual void AddItemToContainer(CScriptThing const &, CCharString const &) const; //slot585 0x0089e780
virtual void RemoveItemFromContainer(CScriptThing const &, CCharString const &) const; //slot586 0x0089e810
virtual void EntitySetDeathContainerAsEnabled(CScriptThing const &, bool) const; //slot587 0x0089e880
virtual void GetItemDefNamesFromContainer(CScriptThing const &, std::vector<CCharString, std::allocator<CCharString> > &) const; //slot588 0x008a8f20
virtual void SetCreatureBrain(CScriptThing const &, CCharString const &) const; //slot589 0x00894b90
virtual void EntitySetStategroupEnabled(CScriptThing const &, CCharString const &, bool) const; //slot590 0x0088f6b0
virtual void EntitySetAllStategroupsEnabled(CScriptThing const &, bool) const; //slot591 0x0088f6e0
virtual void EntitySetCombatEnabled(CScriptThing const &, bool) const; //slot592 0x0088f710
virtual void EntitySetSleepEnabled(CScriptThing const &, bool) const; //slot593 0x0088f7d0
virtual void EntitySetOpinionReactionsEnabled(CScriptThing const &, bool) const; //slot594 0x0088f810
virtual void EntitySetDeedReactionsEnabled(CScriptThing const &, bool) const; //slot595 0x0088f850
virtual void DebugGetAllTextEntriesForTargetedThing(std::set<unsigned long, std::less<unsigned long>, std::allocator<unsigned long> > &) const; //slot596 0x008a0cb0
virtual void EntitySetThingAsEnemyOfThing(CScriptThing const &, CScriptThing const &) const; //slot597 0x0089c000
virtual void EntityUnsetThingAsEnemyOfThing(CScriptThing const &, CScriptThing const &) const; //slot598 0x0089c080
virtual void EntitySetThingAsAllyOfThing(CScriptThing const &, CScriptThing const &) const; //slot599 0x008a7ec0
virtual void EntityUnsetThingAsAllyOfThing(CScriptThing const &, CScriptThing const &) const; //slot600 0x008a7ee0
virtual void EntitySetInFaction(CScriptThing const &, CCharString const &) const; //slot601 0x0089c190
virtual void SetFactionAsAlliedToFaction(CCharString const &, CCharString const &) const; //slot602 0x00890870
virtual void SetFactionAsNeutralToFaction(CCharString const &, CCharString const &) const; //slot603 0x008908c0
virtual void SetFactionAsEnemyToFaction(CCharString const &, CCharString const &) const; //slot604 0x00890910
virtual bool AreEntitiesEnemies(CScriptThing const &, CScriptThing const &) const; //slot605 0x0088e6f0
virtual EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType) const; //slot606 0x00890960
virtual void GetOpinionAttitudeAsString(EOpinionAttitudeType, CCharString &) const; //slot607 0x00890970
virtual EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(CScriptThing const &) const; //slot608 0x00890990
virtual void EntityGetOpinionAttitudeToPlayerAsString(CScriptThing const &, CCharString &) const; //slot609 0x008909c0
virtual float EntityGetOpinionOfPlayer(CScriptThing const &, EOpinion) const; //slot610 0x008909f0
virtual void EntitySetOpinionReactionMask(CScriptThing const &, long) const; //slot612 0x0089c1f0
virtual void EntitySetOpinionReactionMask(CScriptThing const &, CCharString const &) const; //slot611 0x0089c240
virtual void EntitySetOpinionDeedMask(CScriptThing const &, long) const; //slot614 0x0089c290
virtual void EntitySetOpinionDeedMask(CScriptThing const &, CCharString const &) const; //slot613 0x0089c2e0
virtual void EntitySetOpinionDeedTypeEnabled(CScriptThing const &, EOpinionDeedType, bool) const; //slot615 0x0089c330
virtual void EntitySetOpinionAttitudeEnabled(CScriptThing const &, EOpinionAttitudeType, bool) const; //slot616 0x0089c380
virtual void EntitySetOpinionReactionEnabled(CScriptThing const &, EOpinionReactionType, bool) const; //slot617 0x0089c3d0
virtual void EntitySetPersonalityOverride(CScriptThing const &, CCharString const &) const; //slot619 0x0089c420
virtual void EntitySetPersonalityOverride(CScriptThing const &, long) const; //slot618 0x0089c470
virtual void EntityClearPersonalityOverride(CScriptThing const &) const; //slot620 0x0089c4c0
virtual void EntitySetAsOpinionSource(CScriptThing const &, CCharString const &) const; //slot622 0x00890a20
virtual void EntitySetAsOpinionSource(CScriptThing const &, long) const; //slot621 0x00890a50
virtual void EntityUnsetAsOpinionSource(CScriptThing const &, bool) const; //slot623 0x00890a80
virtual void OpinionSourceSetAsExclusive(CScriptThing const &, bool) const; //slot624 0x00894e60
virtual void OpinionSourceSetAsAttentionGrabbing(CScriptThing const &, bool) const; //slot625 0x0089c510
virtual void EntityPostOpinionDeedToAll(CScriptThing const &, EOpinionDeedType) const; //slot626 0x0089c560
virtual void EntityPostOpinionDeedToRecipient(CScriptThing const &, EOpinionDeedType, CScriptThing const &) const; //slot627 0x0089c670
virtual void EntityPostOpinionDeedToRecipientVillage(CScriptThing const &, EOpinionDeedType, CScriptThing const &) const; //slot628 0x0089c710
virtual long EntityPostOpinionDeedKeepSearchingForWitnesses(CScriptThing const &, EOpinionDeedType, CScriptThing const &) const; //slot629 0x0089c7b0
virtual void RemoveOpinionDeedStillSearchingForWitnesses(CScriptThing const &, long) const; //slot630 0x0089c850
virtual bool IsDeedWitnessed(long) const; //slot631 0x0089c8a0
virtual bool CanThingBe_Seen_ByOtherThing(CScriptThing const &, CScriptThing const &) const; //slot632 0x0089cd10
virtual bool CanThingBe_NearlySeen_ByOtherThing(CScriptThing const &, CScriptThing const &) const; //slot633 0x0089cd30
virtual bool CanThingBe_Smelled_ByOtherThing(CScriptThing const &, CScriptThing const &) const; //slot634 0x0089cd50
virtual bool CanThingBe_Heard_ByOtherThing(CScriptThing const &, CScriptThing const &) const; //slot635 0x0089cd70
virtual bool IsThingAwareOfOtherThingInAnyWay(CScriptThing const &, CScriptThing const &) const; //slot636 0x0089ce70
virtual void EntitySetAsAwareOfThing(CScriptThing const &, CScriptThing const &) const; //slot637 0x0089d0e0
virtual void EntitySetSoundRadius(CScriptThing const &, float) const; //slot638 0x0089d160
virtual void EntitySetSmellRadius(CScriptThing const &, float) const; //slot639 0x0089d1c0
virtual void EntitySetSightRadius(CScriptThing const &, float) const; //slot640 0x0089d220
virtual void EntitySetExtendedSightRadius(CScriptThing const &, float) const; //slot641 0x0089d280
virtual void EntitySetGiveUpChaseRadius(CScriptThing const &, float) const; //slot642 0x0089d2e0
virtual float EntityGetHearingRadius(CScriptThing const &) const; //slot643 0x0089d340
virtual bool ManuallyTriggerTrap(CScriptThing const &) const; //slot644 0x00895090
virtual bool ManuallyResetTrap(CScriptThing const &) const; //slot645 0x008950e0
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
virtual void SetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing &, void *) const; //slot677 0x008a99f0
virtual void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing &) const; //slot678 0x00897150
virtual bool IsGiftRomantic(CCharString const &) const; //slot679 0x00894bf0
virtual bool IsGiftFriendly(CCharString const &) const; //slot680 0x00894c90
virtual bool IsGiftOffensive(CCharString const &) const; //slot681 0x00894d30
virtual bool IsThingABed(CScriptThing const &) const; //slot682 0x008a0e80
virtual bool IsThingAChest(CScriptThing const &) const; //slot683 0x008a0eb0
virtual bool IsThingADoor(CScriptThing const &) const; //slot684 0x008a0ee0
virtual bool IsThingSmashable(CScriptThing const &) const; //slot685 0x008a0f10
virtual bool IsThingSearchable(CScriptThing const &) const; //slot686 0x008a0f40
virtual void ApplyScriptBrush(CCharString const &) const; //slot687 0x0088f480
virtual void EnableDecals(bool) const; //slot688 0x0088f4a0
virtual unsigned long PlayCriteriaSoundOnThing(CScriptThing const &, CCharString const &) const; //slot689 0x008a09b0
virtual unsigned long PlaySoundOnThing(CScriptThing const &, CCharString const &) const; //slot690 0x008a0a20
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
virtual void SetReadableObjectText(CScriptThing const &, CWideString const &) const; //slot713 0x008a0f70
virtual void SetReadableObjectTextTag(CScriptThing const &, CCharString const &) const; //slot714 0x008a0fd0
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
virtual long GetStealDuration(CScriptThing const &) const; //slot740 0x008a1360
virtual void SetUseableByHero(CScriptThing const &, bool) const; //slot741 0x008a1830
virtual void SetOwnedByHero(CScriptThing const &, bool) const; //slot742 0x008a18f0
virtual void SetTavernTableAvailableForUse(CScriptThing const &, bool) const; //slot743 0x008a1890
virtual void SetIsThingTurncoatable(CScriptThing const &, bool) const; //slot744 0x008a1950
virtual void SetIsThingForcePushable(CScriptThing const &, bool) const; //slot745 0x0088f9f0
virtual void SetIsThingLightningable(CScriptThing const &, bool) const; //slot746 0x0088fa20
virtual void SetIsThingEpicSpellable(CScriptThing const &, bool) const; //slot747 0x0088fa50
virtual bool IsThingTurncoated(CScriptThing const &) const; //slot748 0x008a19b0
virtual void AddCreatureScriptedMode(CScriptThing const &, CCharString const &) const; //slot749 0x008a1a10
virtual void RemoveCreatureScriptedMode(CScriptThing const &) const; //slot750 0x008a1ab0
virtual void ForceShipsVisible(void) const; //slot751 0x008a7f60
virtual bool GetSleepingPositionAndOrientationFromBed(CScriptThing const &, CScriptThing const &, C3DVector &, C3DVector &); //slot752 0x008a1b10
virtual void SetBedAvailability(CScriptThing const &, bool); //slot753 0x008a1bf0
virtual void RepopulateVillage(CScriptThing const &) const; //slot754 0x008a1c50
virtual void SmashAllWindowsWithinRadiusOfPoint(C3DVector const &, float); //slot755 0x008a8010
virtual CScriptThing SetResidency(CScriptThing const &, bool); //slot756 0x008a1cc0
virtual void SetThankingPhrase(CScriptThing, unsigned long); //slot757 0x008a1e50
virtual unsigned long GetThankingPhrase(CScriptThing); //slot758 0x008a1f30
virtual void ResetThankingPhrase(CScriptThing); //slot759 0x008a2020
virtual void SetIgnoringPhrase(CScriptThing, unsigned long); //slot760 0x008a2100
virtual unsigned long GetIgnoringPhrase(CScriptThing); //slot761 0x008a21e0
virtual void ResetIgnoringPhrase(CScriptThing); //slot762 0x008a22d0
virtual void SetWanderCentrePoint(CScriptThing, C3DVector); //slot763 0x008a23b0
virtual C3DVector GetWanderCentrePoint(CScriptThing); //slot764 0x008a24c0
virtual void ResetWanderCentrePoint(CScriptThing); //slot765 0x008a2590
virtual void SetWanderMinDistance(CScriptThing, float); //slot766 0x008a2690
virtual float GetWanderMinDistance(CScriptThing); //slot767 0x008a2770
virtual void ResetWanderMinDistance(CScriptThing); //slot768 0x008a2870
virtual void SetWanderMaxDistance(CScriptThing, float); //slot769 0x008a2950
virtual float GetWanderMaxDistance(CScriptThing); //slot770 0x008a2a30
virtual void ResetWanderMaxDistance(CScriptThing); //slot771 0x008a2b30
virtual void SetGossipCounter(CScriptThing, long); //slot772 0x008a2c10
virtual long GetGossipCounter(CScriptThing); //slot773 0x008a2cf0
virtual void ResetGossipCounter(CScriptThing); //slot774 0x008a2de0
virtual void SetMaxGossipPhrase(CScriptThing, long); //slot775 0x008a2ec0
virtual long GetMaxGossipPhrase(CScriptThing); //slot776 0x008a2fa0
virtual void ResetMaxGossipPhrase(CScriptThing); //slot777 0x008a3090
virtual void SetWarningPhrase(CScriptThing, unsigned long); //slot778 0x008a3170
virtual unsigned long GetWarningPhrase(CScriptThing); //slot779 0x008a3250
virtual void ResetWarningPhrase(CScriptThing); //slot780 0x008a3340
virtual void SetBeerRequestPhrase(CScriptThing, unsigned long); //slot781 0x008a3420
virtual unsigned long GetBeerRequestPhrase(CScriptThing); //slot782 0x008a3500
virtual void ResetBeerRequestPhrase(CScriptThing); //slot783 0x008a35f0
virtual void SetScriptingStateGroup(CScriptThing, EScriptingStateGroups); //slot784 0x008a36d0
virtual EScriptingStateGroups GetScriptingStateGroup(CScriptThing); //slot785 0x008a37b0
virtual void ResetScriptingStateGroup(CScriptThing); //slot786 0x008a38a0
virtual void SetMaxHeroReactionDistance(CScriptThing, float); //slot787 0x008a3980
virtual float GetMaxHeroReactionDistance(CScriptThing); //slot788 0x008a3a60
virtual void ResetMaxHeroReactionDistance(CScriptThing); //slot789 0x008a3b60
virtual void SetActionFrequency(CScriptThing, long); //slot790 0x008a3c40
virtual long GetActionFrequency(CScriptThing); //slot791 0x008a3d20
virtual void ResetActionFrequency(CScriptThing); //slot792 0x008a3e10
virtual void SetActionFrequencyVariation(CScriptThing, float); //slot793 0x008a3ef0
virtual float GetActionFrequencyVariation(CScriptThing); //slot794 0x008a3fd0
virtual void ResetActionFrequencyVariation(CScriptThing); //slot795 0x008a40d0
virtual void SetAction(CScriptThing, CCharString); //slot796 0x008a41b0
virtual CCharString GetAction(CScriptThing); //slot797 0x008a42c0
virtual void ResetAction(CScriptThing); //slot798 0x008a4390
virtual void SetFaceHeroForAction(CScriptThing, bool); //slot799 0x008a4480
virtual bool GetFaceHeroForAction(CScriptThing); //slot800 0x008a4560
virtual void ResetFaceHeroForAction(CScriptThing); //slot801 0x008a4650
virtual void SetTargetName(CScriptThing, CCharString); //slot802 0x008a4730
virtual CCharString GetTargetName(CScriptThing); //slot803 0x008a4840
virtual void ResetTargetName(CScriptThing); //slot804 0x008a4910
virtual void SetFollowDistance(CScriptThing, float); //slot805 0x008a4a00
virtual float GetFollowDistance(CScriptThing); //slot806 0x008a4ae0
virtual void ResetFollowDistance(CScriptThing); //slot807 0x008a4be0
virtual void SetAttackHeroOnSight(CScriptThing, bool); //slot808 0x008a4cc0
virtual bool GetAttackHeroOnSight(CScriptThing); //slot809 0x008a4da0
virtual void ResetAttackHeroOnSight(CScriptThing); //slot810 0x008a4e90
virtual void SetTimeToSpendHarassingHero(CScriptThing, long); //slot811 0x008a4f70
virtual long GetTimeToSpendHarassingHero(CScriptThing); //slot812 0x008a5050
virtual void ResetTimeToSpendHarassingHero(CScriptThing); //slot813 0x008a5140
virtual void SetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing, float); //slot814 0x008a5220
virtual float GetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing); //slot815 0x008a5300
virtual void ResetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing); //slot816 0x008a5400
virtual void SetCombatNearbyBreakOffRange(CScriptThing, float); //slot817 0x008a54e0
virtual float GetCombatNearbyBreakOffRange(CScriptThing); //slot818 0x008a55c0
virtual void ResetCombatNearbyBreakOffRange(CScriptThing); //slot819 0x008a56c0
virtual void SetStealStealableItems(CScriptThing, bool); //slot820 0x008a57b0
virtual bool GetStealStealableItems(CScriptThing); //slot821 0x008a5890
virtual void ResetStealStealableItems(CScriptThing); //slot822 0x008a5980
virtual void SetRecoverStealableItems(CScriptThing, bool); //slot823 0x008a5a70
virtual bool GetRecoverStealableItems(CScriptThing); //slot824 0x008a5b50
virtual void ResetRecoverStealableItems(CScriptThing); //slot825 0x008a5c40
virtual void SetTakeStealableItemToRandomDestination(CScriptThing, bool); //slot826 0x008a5d30
virtual bool GetTakeStealableItemToRandomDestination(CScriptThing); //slot827 0x008a5e10
virtual void ResetTakeStealableItemToRandomDestination(CScriptThing); //slot828 0x008a5f00
virtual void SetKillSelfAndStealableItemAfterReachingDestination(CScriptThing, bool); //slot829 0x008a5ff0
virtual bool GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing); //slot830 0x008a60d0
virtual void ResetKillSelfAndStealableItemAfterReachingDestination(CScriptThing); //slot831 0x008a61c0
virtual void SetAllowedToFollow(CScriptThing, bool); //slot832 0x008a62b0
virtual bool GetAllowedToFollow(CScriptThing); //slot833 0x008a6390
virtual void ResetAllowedToFollow(CScriptThing); //slot834 0x008a6480
virtual void SetTableName(CScriptThing, CCharString); //slot835 0x008a6570
virtual CCharString GetTableName(CScriptThing); //slot836 0x008a6680
virtual void ResetTableName(CScriptThing); //slot837 0x008a6750
virtual void SetSeatName(CScriptThing, CCharString); //slot838 0x008a6840
virtual CCharString GetSeatName(CScriptThing); //slot839 0x008a6950
virtual void ResetSeatName(CScriptThing); //slot840 0x008a6a20
virtual void SetDisableHeadLooking(CScriptThing, bool); //slot841 0x008a6b10
virtual bool GetDisableHeadLooking(CScriptThing); //slot842 0x008a6bf0
virtual void ResetDisableHeadLooking(CScriptThing); //slot843 0x008a6ce0
virtual void SetIsPushableByHero(CScriptThing, bool); //slot844 0x008a6dd0
};
#include <string.h>
#pragma auto_inline(off)
long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
 for (;;) {
  char ca = *a;
  if (ca == 0 && *b == 0)
   return 0;
  char cb = *b;
  if (ca < cb)
   return -1;
  if (ca > cb)
   return 1;
  ++a;
  ++b;
 }
}
#pragma auto_inline(on)
#pragma pack(push, 1)
struct CBasicStringChar {                            // CBasicString<char>, 13 bytes
 char*         PData;                                // +0x00
 unsigned long StringLength;                         // +0x04
 unsigned long DataLength : 31;                      // +0x08
 bool          UseFastExtend : 1;                    // +0x0c
 bool operator==(const char* text) const { return Compare_00411570(PData, text) ? false : true; }
};
class CCharStringData { public:                             // 17 bytes
 CBasicStringChar Data;                              // +0x00
 long             NoRefs;                            // +0x0d
};
#pragma pack(pop)
class CCharString { public:
 CCharStringData* PStringData;
 CCharString(const char* text, long length = -1);   // 0x0099EBF0
 CCharString(const CCharString& other);             // 0x0099EC30
 ~CCharString();                                    // 0x0099EAE0
 __forceinline bool operator==(const char* text) const
 {
  if (PStringData == 0) {
   if (strcmp(text, "") == 0)
    return true;
   return false;
  }
  return PStringData->Data == text;
 }
};
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
class CBaseObject { public:
 void* m_pIntelligentPointer;
 CBaseObject();                                     // 0x0099A380
 virtual ~CBaseObject();
};
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C
 CCountedPointerWords PImp;
 CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
 virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
 bool IsScripting();                                // 0x00CD23B9
 void Release();                                    // 0x00CD2770
 void PlayAnimation(const CCharString& anim, bool waitForFinish, bool stayOnLastFrame, bool b3, bool b4, bool b5, bool b6, bool b7);   // slot 18, 0x007E73D0
 void PlayLoopingAnimation(const CCharString& anim, long loops, bool waitForFinish, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7);   // slot 20, 0x007E73F0
 void MoveToPosition(const C3DVector& pos, float speed, long moveType, bool a, bool b);   // 0x007E72F0
 bool IsPerformingScriptTask() const;               // 0x007E7450
 CScriptThing GetScriptThing() const;              // 0x007E7490
 void FollowThing(const CScriptThing& thing, float distance, bool run);   // 0x007E7320
 void ClearCommands();                              // 0x007E7360
};
extern bool g_bScriptAnimationFlag;                 // 0x01375748 (a global bool the script headers pass as an animation default)
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
namespace std {
template <> class vector<CScriptThing, allocator<CScriptThing> > {  // STLport vector (retail 12 bytes)
public:
 CScriptThing* _M_start;
 CScriptThing* _M_finish;
 CScriptThing* _M_end_of_storage;
 unsigned int size() const { return _M_finish - _M_start; }
 CScriptThing* begin() { return _M_start; }
 CScriptThing& operator[](unsigned int n) { return *(begin() + n); }
};
}
typedef long EHENCHMAN_COMMENT;
struct CV_BodyGuardScript {
 bool MakeHenchmanComment(const CCharString& text, long priority, EHENCHMAN_COMMENT type, const CScriptThing& target);   // 0x00E5EBD0
};
struct CThingManager_Calls {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
    virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
    virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175();
    virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183();
    virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191();
    virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207();
    virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215();
    virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223();
    virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231();
    virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239();
    virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247();
    virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255();
    virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263();
    virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271();
    virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279();
    virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287();
    virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295();
    virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303();
    virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311();
    virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319();
    virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327();
    virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335();
    virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343();
    virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351();
    virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359();
    virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366(); virtual void v367();
    virtual void v368(); virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375();
    virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379(); virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383();
    virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389(); virtual void v390(); virtual void v391();
    virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396(); virtual void v397(); virtual void v398(); virtual void v399();
    virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404(); virtual void v405(); virtual void v406(); virtual void v407();
    virtual void v408(); virtual void v409(); virtual void v410(); virtual void v411(); virtual void v412(); virtual void v413(); virtual void v414(); virtual void v415();
    virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420(); virtual void v421(); virtual void v422(); virtual void v423();
    virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428(); virtual void v429(); virtual void v430(); virtual void v431();
    virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437(); virtual void v438(); virtual void v439();
    virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445(); virtual void v446(); virtual void v447();
    virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451(); virtual void v452(); virtual void v453(); virtual void v454(); virtual void v455();
    virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459(); virtual void v460(); virtual void v461(); virtual void v462(); virtual void v463();
    virtual void v464(); virtual void v465(); virtual void v466(); virtual void v467(); virtual void v468(); virtual void v469(); virtual void v470(); virtual void v471();
    virtual void v472(); virtual void v473(); virtual void v474(); virtual void v475(); virtual void v476(); virtual void v477(); virtual void v478(); virtual void v479();
    virtual void v480(); virtual void v481(); virtual void v482(); virtual void v483(); virtual void v484(); virtual void v485(); virtual void v486(); virtual void v487();
    virtual void v488(); virtual void v489(); virtual void v490(); virtual void v491(); virtual void v492(); virtual void v493(); virtual void v494(); virtual void v495();
    virtual void v496(); virtual void v497(); virtual void v498(); virtual void v499(); virtual void v500(); virtual void v501(); virtual void v502(); virtual void v503();
    virtual void v504(); virtual void v505(); virtual void v506(); virtual void v507(); virtual void v508(); virtual void v509(); virtual void v510(); virtual void v511();
    virtual void v512(); virtual void v513(); virtual void v514(); virtual void v515(); virtual void v516(); virtual void v517(); virtual void v518(); virtual void v519();
    virtual void v520(); virtual void v521(); virtual void v522(); virtual void v523(); virtual void v524(); virtual void v525(); virtual void v526(); virtual void v527();
    virtual void v528(); virtual void v529(); virtual void v530(); virtual void v531(); virtual void v532(); virtual void v533(); virtual void v534(); virtual void v535();
    virtual void v536(); virtual void v537(); virtual void v538(); virtual void v539(); virtual void v540(); virtual void v541(); virtual void v542(); virtual void v543();
    virtual void v544(); virtual void v545(); virtual void v546(); virtual void v547(); virtual void v548(); virtual void v549(); virtual void v550(); virtual void v551();
    virtual void v552(); virtual void v553(); virtual void v554(); virtual void v555(); virtual void v556(); virtual void v557(); virtual void v558(); virtual void v559();
    virtual void v560(); virtual void v561(); virtual void v562(); virtual void v563(); virtual void v564(); virtual void v565(); virtual void v566(); virtual void v567();
    virtual void v568(); virtual void v569(); virtual void v570(); virtual void v571(); virtual void v572(); virtual void v573(); virtual void v574(); virtual void v575();
    virtual void v576(); virtual void v577(); virtual void v578(); virtual void v579(); virtual void v580(); virtual void v581(); virtual void v582(); virtual void v583();
    virtual void v584(); virtual void v585(); virtual void v586(); virtual void v587(); virtual void v588(); virtual void v589(); virtual void v590(); virtual void v591();
    virtual void v592(); virtual void v593(); virtual void v594(); virtual void v595(); virtual void v596(); virtual void v597(); virtual void v598(); virtual void v599();
    virtual void v600(); virtual void v601(); virtual void v602(); virtual void v603(); virtual void v604(); virtual void v605(); virtual void v606(); virtual void v607();
    virtual void v608(); virtual void v609(); virtual void v610(); virtual void v611(); virtual void v612(); virtual void v613(); virtual void v614(); virtual void v615();
    virtual void v616(); virtual void v617(); virtual void v618(); virtual void v619(); virtual void v620(); virtual void v621(); virtual void v622(); virtual void v623();
    virtual void v624(); virtual void v625(); virtual void v626(); virtual void v627(); virtual void v628(); virtual void v629(); virtual void v630(); virtual void v631();
    virtual void v632(); virtual void v633(); virtual void v634(); virtual void v635(); virtual void v636(); virtual void v637(); virtual void v638(); virtual void v639();
    virtual void v640(); virtual void v641(); virtual void v642(); virtual void v643(); virtual void v644(); virtual void v645(); virtual void v646(); virtual void v647();
    virtual void v648(); virtual void v649(); virtual void v650(); virtual void v651(); virtual void v652();
    virtual long GetWorldFrame();   // slot 653 (+0xa34)
};
extern CThingManager_Calls* g_pThingManager;         // 0x0143E8F8
class CCounter { public:                                    // Ego_r.pdb CCounter { long RelevantWorldFrame }
 long RelevantWorldFrame;
 CCounter() : RelevantWorldFrame(g_pThingManager->GetWorldFrame()) { }   // out of line 0x00CD4490
 bool operator>(float seconds) const;               // 0x00D90670
 CCounter& operator=(float seconds);                // 0x00D90550
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

struct CHenchmanComment : public CEntityScriptBase {
 void* VTable; GameCalls* Game; CScriptThing Me;
 CV_BodyGuardScript* ParentClass;                    // +0x14
 void* PMasterData;                                  // +0x18
 std::vector<CScriptThing, std::allocator<CScriptThing> > HenchmanVector;   // +0x1c
 void Main();
};
void CHenchmanComment::Main()
{
 Game->GetAllThingsWithScriptName("BodyGuard", HenchmanVector);
 if (HenchmanVector.size() == 0) {
  IsActiveThreadTerminating();
  return;
 }
 unsigned int bodyGuard = (unsigned int)-1;
 unsigned int i;
 for (i = 0; i < HenchmanVector.size(); ++i) {
  if (IsActiveThreadTerminating()) return;
  if (HenchmanVector[i].GetDefName() == "CREATURE_BODY_GUARD") {
   if (IsActiveThreadTerminating()) return;
   bodyGuard = i;
  }
 }
 if (IsActiveThreadTerminating()) return;
 {
 CCounter sinceComment;
 while (!IsActiveThreadTerminating()) {
  bool anyNear = false;
  for (i = 0; i < HenchmanVector.size(); ++i) {
   if (IsActiveThreadTerminating()) return;
   if (bodyGuard != i && IsDistanceBetweenThingsUnder(Me, HenchmanVector[i], 10.0f)) {
    if (IsActiveThreadTerminating()) return;
    anyNear = true;
    if (sinceComment > 6.0f && ParentClass->MakeHenchmanComment(Me.GetDataString(), 2, 2, CScriptThing())) {
     IsActiveThreadTerminating();
     return;
    }
   }
  }
  if (IsActiveThreadTerminating()) return;
  if (!anyNear) {
   if (IsActiveThreadTerminating()) return;
   sinceComment = 0.0f;
  }
  Game->NewScriptFrame();
 }
 }
}