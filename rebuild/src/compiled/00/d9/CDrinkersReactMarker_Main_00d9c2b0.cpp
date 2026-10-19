// CDrinkersReactMarker::Main -- await hero or named Whisper within4, then clear the parent flag
// Retail00D9C2B0,269 bytes. PDB CQ_HobbeToothContestScript owner; parentflag5A meaning unproven.
// Native IsWithinDistance CBE2FF is fastcall(two references,float stack), checks both alive then squared distance.
// Game slots70 GetHero(ref),72 GetThingWithScriptName(value),7 NewScriptFrame; F35B30 termination.
// Literal12D6788 Q_HobbeToothWhisper. Actual Thing/Game/string headers; real79slot Thing interface.
// Leaf: Main
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "fable_string.h"
struct CThingNative;struct C3DVector{float X,Y,Z;};
// Restored genuine virtual ABI for by-value CScriptThing; all79 native slots named.
namespace std {template<class T>class allocator;template<class T,class A>class vector;}
typedef long EAIStateGroupTypeABI;typedef long EScriptAIPriorityABI;typedef long EHeroAbilityABI; // enum ABI declarations only
struct CBaseClassNative{CBaseClassNative(){}virtual ~CBaseClassNative();}; //0099A2D0 ctor,0099A2E0 dtor,VT01231710
struct CCPPointerInfo{unsigned long RefCount;void(__fastcall*DeleteFunc)(void*);void* Data;}; // actual PDB3field layout, no generated header
struct CountedScriptImp{CScriptThing* Data;CCPPointerInfo* Info;CountedScriptImp():Data(0),Info(0){}CountedScriptImp(const CountedScriptImp& other):Data(other.Data),Info(other.Info){if(Info)++Info->RefCount;}~CountedScriptImp(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Data=0;Info=0;}};
struct ScriptMethods:CBaseClassNative{
 CountedScriptImp PImp;
 ScriptMethods(){}
 __forceinline ScriptMethods(const CScriptThing& other):CBaseClassNative(){PImp.Data=other.PImp_Data;PImp.Info=other.PImp_Info;if(PImp.Info)++PImp.Info->RefCount;}
 ScriptMethods(const ScriptMethods& other):CBaseClassNative(),PImp(other.PImp){}
 virtual ~ScriptMethods(){} //native inline vptr reset, counted release, CBaseClass dtor99A2E0
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


bool __fastcall IsWithinDistance(const ScriptMethods&,const CScriptThing&,float); //00CBE2FF
namespace std {template<class T,class A>class list;template<class A,class B>struct pair;}
struct CScriptGameResourceObjectScriptedThingBase;typedef long EClothingCoversArea;
// Actual narrow-string wrapper restores the missing one-argument default-length surface, no extra fields.
struct ImplicitCharString:CCharString {ImplicitCharString(const char* text):CCharString(text,-1){}};
// Virtual prefix view only: native Game fields are in the actual included header.
FABLE_STATIC_ASSERT(sizeof(ImplicitCharString)==sizeof(CCharString));
// Slot72 actual signature is const CCharString&; field-free derived const-reference
// projection restores implicit conversion through genuine ctor99EBF0(text,-1).
struct GameMethods:CBaseClassNative{
virtual void Error(CCharString const &, CCharString const &, CCharString const &, unsigned long) const; //slot1 0x0088de00
virtual void TraceMessage(CCharString const &) const; //slot2 0x006e7d20
virtual void Validate(void) const; //slot3 0x006e72a0
virtual void SetDebugCameraType(long) const; //slot4 0x0088e0f0
virtual void DeactivateBoastUi(void) const; //slot5 0x008a0c60
virtual bool IsXbox(void) const; //slot6 0x0088e100
virtual void NewScriptFrame(void) const; //slot7 0x006e7410
virtual bool StartScriptingEntity(ScriptMethods const &, CScriptGameResourceObjectScriptedThingBase &, EScriptAIPriorityABI); //slot8 0x0089b5b0
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
virtual bool MsgOnHeroCastSpell(EHeroAbilityABI &) const; //slot52 0x00893f80
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
virtual ScriptMethods GetThingWithScriptName(ImplicitCharString const &) const; //slot72 0x008a7d60
};
struct NativeParent {unsigned char UnrecoveredPrefix[0x5a];bool ScriptFlag;};
struct CDrinkersReactMarker {void* VTable;GameMethods* Game;CScriptThing Me;NativeParent* Parent;void* Master;
 bool IsActiveThreadTerminating();void Main();
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDrinkersReactMarker,Parent)==0x14);
FABLE_STATIC_ASSERT(offsetof(NativeParent,ScriptFlag)==0x5a);
#endif
void CDrinkersReactMarker::Main(){
 while(!IsWithinDistance(Game->GetHero(),Me,4.0f)&&!IsWithinDistance(Game->GetThingWithScriptName("Q_HobbeToothWhisper"),Me,4.0f)){
  Game->NewScriptFrame();if(IsActiveThreadTerminating())return;
 }
 if(!IsActiveThreadTerminating())Parent->ScriptFlag=false;
}