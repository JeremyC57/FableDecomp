// 00d06130 behaviour test (AUTH30): NScript::CQ_BanditCampBossBattleScript::CAssassin3::Main, from Codex's 00d05a60 fixture (shared headers,
// link stubs only). Main is NOT copied here: vt.py links the row object (g1.obj), and entry.Main() reaches it through the vftable.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FABLE_RETAIL_ASSASSIN2_PARENT_DECL_PASS29
#define FABLE_RETAIL_COUNTED_DESTRUCTOR_INLINE_PASS29
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
// Inline-budget: inline the movie destructor at both early exits, sharing their cleanup tail.
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BANDIT_CAMP_ASSASSIN_TYPED_CTORS_PASS29
#define FABLE_RETAIL_BANDIT_CAMP_ASSASSINS_PASS29
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_BASE_OBJECT

#define FABLE_RETAIL_CISTHINGALIVE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "engine/EScriptAIPriority.h"
#include "engine/TRetVoid.h"
#include "engine/EHeroAbility.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CIsThingAlive.h"
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }
#ifdef _MSC_VER
#pragma inline_depth(2)
#endif
FABLE_FORCEINLINE NScript::CIsThingAlive::~CIsThingAlive() { }
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline CScriptThing::~CScriptThing() {}
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
namespace NScript {
inline CQ_BanditCampBossBattleScript::CAssassin3::CAssassin3(
    CQ_BanditCampBossBattleScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(parent.Game, me), ParentClass(parent)
{
    PMasterData = static_cast<CQ_SunnyvaleMasterData*>(data);
}
}
// Link stub: the row declares this specialisation out of line (landed elsewhere); the test supplies a body.
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer() { ReleaseData(); }
namespace NScript {
CScriptBase* CQ_BanditCampBossBattleScript::CAssassin3::GetParentScript() const { return &ParentClass; }
}

#define FABLE_RETAIL_CRGBCOLOUR
#include "engine/CRGBColour.h"
#include "engine/C3DVector.h"

static void check(bool value) { if (!value) { puts("assertion failed"); exit(1); } }
struct Scenario {
    int mode, stopAt, probe, frames, starts, startFailures, taskFrames, taskLeft;
    int predicates, releases, movieCount, speaks, pauses, resumes, clears, stage;
    bool alive, valid, paused, attacking, cutscene;
};
static Scenario state;

typedef NScript::CQ_BanditCampBossBattleScript Quest;
static Quest* activeQuest;
static CScriptThing* fixtureHero;
static const CCharString* strings[16];
static const char* contents[16];
static int liveStrings;
static const char* textOf(const CCharString& value) {
    for(int i=0;i<liveStrings;++i) if(strings[i]==&value) return contents[i];
    abort(); return "";
}
CCharString::CCharString(const char* text,long length):PStringData(0) {
    check(length==-1 && liveStrings<16); strings[liveStrings]=this; contents[liveStrings++]=text;
}
CCharString::~CCharString() {
    for(int i=0;i<liveStrings;++i) if(strings[i]==this) {
        --liveStrings; strings[i]=strings[liveStrings]; contents[i]=contents[liveStrings]; return;
    }
    check(PStringData==0);
}
static bool heroName(const CCharString& x) { check(strcmp(textOf(x),"SCRIPT_NAME_HERO")==0); return true; }
bool CScriptThing::MsgIsHitBy(const CCharString& x) const {return heroName(x) && state.mode==1;}
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString& x) const {return heroName(x) && (state.mode==2 || state.mode==3);}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility ability,const CCharString& x) const {
 check(ability==HERO_ABILITY_HEAL_LIFE_SPELL); return heroName(x) && state.mode==3;
}
bool CScriptThing::MsgIsTalkedToBy(const CCharString& x) const {return heroName(x) && state.mode==4;}
// Exercise the defensive entry branch as well as the ordinary empty-handle path.
bool CScriptGameResourceObjectScriptedThingBase::IsValid() const {
    return state.valid || !PImp.IsNull();
}
void CScriptGameResourceObjectScriptedThingBase::Release() { ++state.releases; PImp.Reset(0); }
CScriptThing CScriptGameResourceObjectScriptedThingBase::GetScriptThing() const {return *fixtureHero;}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& who,const char* line,ETextGroupSelectionMethod method,bool one,bool two,bool three) {
 check(&who==fixtureHero && !one && two && !three && method==GROUP_SELECT_NONE && state.paused);
 check(strcmp(line,state.speaks?"TEXT_QST_009_ASSASSIN3_REPEAT":"TEXT_QST_009_ASSASSIN3_INTRO")==0);++state.speaks;state.taskLeft=state.taskFrames;
}
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask() const {return state.taskLeft-->0;}
void CEntityScriptBase::SetPredicate(const NScript::CPredicateBase&) {++state.predicates;}
bool CEntityScriptBase::IsActiveThreadTerminating() const {return ++state.probe==state.stopAt;}
namespace NScript {
CQ_BanditCampBossBattleScript::CQ_BanditCampBossBattleScript(CScriptDataBase* data,CGameScriptInterfaceBase& game):CLevelScriptBase<CQ_SunnyvaleMasterData>(data,game) {}
}

static int movieOwnership;
static int destroyedMovies;
static CScriptGameResourceObjectMovieBase* retainedMovie;
CBaseObject& CBaseObject::operator=(const CBaseObject&) { return *this; }
class FixtureMovie : public CScriptGameResourceObjectMovieBase {
public:
    virtual ~FixtureMovie() { check(!state.paused); ++destroyedMovies; }
};
void CScriptGameResourceObjectMovieBase::SetImplementation(CScriptGameResourceObjectMovieBase* value) {
    PImp.Reset(value);
}

static int actorOwnership;
static int constructedActors;
static int destroyedActors;
static CScriptGameResourceObjectScriptedThingBase* retainedActor;
class FixtureActorResource : public CScriptGameResourceObjectScriptedThingBase {
public:
    FixtureActorResource() { ++constructedActors; }
    virtual ~FixtureActorResource() { ++destroyedActors; }
};
void CScriptGameResourceObjectScriptedThingBase::SetImplementation(CScriptGameResourceObjectScriptedThingBase* value) {
    PImp.reset(value);
}
class FixtureGame : public CGameScriptInterfaceBase {
public:
    virtual void Error(const CCharString&, const CCharString&, const CCharString&, unsigned long) const { abort(); }
    virtual void TraceMessage(const CCharString&) const { abort(); }
    virtual void Validate() const { abort(); }
    virtual void SetDebugCameraType(long) const { abort(); }
    virtual void DeactivateBoastUi() const { abort(); }
    virtual bool IsXbox() const { abort(); }
    virtual void NewScriptFrame() const { ++state.frames; check(state.frames<40); if(state.starts>state.startFailures && !state.paused){++state.stage; if(state.stage==3 || (state.mode!=4 && state.stage==2)) activeQuest->AssassinCutsceneTriggered=true;} }
    virtual bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase& handle, EScriptAIPriority priority) {
        check(priority==SCRIPT_AI_PRIORITY_HIGHEST);
        if (++state.starts<=state.startFailures) return false;
        if (actorOwnership) {
            handle.SetImplementation(new FixtureActorResource);
            if (actorOwnership==2) *retainedActor=handle;
        }
        return true;
    }
    virtual bool IsEntityUnderScriptedControl(const CScriptThing&) const { abort(); }
    virtual bool IsActiveThreadTerminating() const { abort(); }
    virtual bool IsLevelLoaded(const CCharString&) const { abort(); }
    virtual bool IsRegionLoaded(const CCharString&) const { abort(); }
    virtual bool IsRegionLoadedAndPreloaded(const CCharString&) const { abort(); }
    virtual bool IsRegionDefLoaded(const CCharString&) const { abort(); }
    virtual const CCharString& GetRegionName() const { abort(); }
    virtual bool MsgIsLevelLoaded(const CCharString&) const { abort(); }
    virtual bool MsgIsLevelUnloaded(const CCharString&) const { abort(); }
    virtual bool MsgOnLevelLoaded(std::list<CCharString,std::allocator<CCharString> >&) const { abort(); }
    virtual bool MsgOnLevelUnloaded(std::list<CCharString,std::allocator<CCharString> >&) const { abort(); }
    virtual bool MsgIsRegionLoaded(const CCharString&) const { abort(); }
    virtual bool MsgIsRegionUnloaded(const CCharString&) const { abort(); }
    virtual bool MsgOnRegionLoaded(CCharString&) const { abort(); }
    virtual bool MsgOnRegionUnloaded(CCharString&) const { abort(); }
    virtual bool MsgOnRegionPreunload(CCharString&) const { abort(); }
    virtual bool MsgOnQuestCompleted(const CCharString&) const { abort(); }
    virtual bool MsgOnAnyQuestCompleted(CCharString&) const { abort(); }
    virtual bool MsgOnQuestFailed(const CCharString&) const { abort(); }
    virtual bool MsgOnQuestCompletedBeforeScreenShown(const CCharString&) const { abort(); }
    virtual bool MsgOnQuestFailedBeforeScreenShown(const CCharString&) const { abort(); }
    virtual bool MsgOnQuestAccepted(CCharString&) const { abort(); }
    virtual bool MsgOnFeatAccepted(long&) const { abort(); }
    virtual bool MsgIsBoastMade(long) const { abort(); }
    virtual bool MsgOnBoastMade(long&, CCharString&) const { abort(); }
    virtual bool MsgOnBoastsMade(std::vector<std::pair<long,CCharString>,std::allocator<std::pair<long,CCharString> > >&) const { abort(); }
    virtual void RemoveBoastMessage() const { abort(); }
    virtual bool IsQuestStartScreenActive(CCharString&) const { abort(); }
    virtual bool MsgOnLeavingQuestStartScreen(CCharString&) const { abort(); }
    virtual bool MsgOnLeavingExperienceSpendingScreen() const { abort(); }
    virtual long MsgIsQuestionAnsweredYesOrNo() const { abort(); }
    virtual bool MsgIsGameInfoClickedPast() const { abort(); }
    virtual bool MsgIsTutorialClickedPast() const { abort(); }
    virtual bool MsgIsActionModeButtonPressed() const { abort(); }
    virtual bool MsgOnExpressionPerformed(CCharString&) const { abort(); }
    virtual bool MsgIsCutSceneSkipped() const { abort(); }
    virtual void RemoveAllCutSceneSkippedMessages() const { abort(); }
    virtual bool MsgOnHeroHairTypeChanged(EClothingCoversArea, CCharString&) const { abort(); }
    virtual bool MsgOnHeroUsedTeleporter(CCharString&) const { abort(); }
    virtual bool MsgOnHeroUsedGuildSeal() const { abort(); }
    virtual bool MsgOnGameSavedManually() const { abort(); }
    virtual bool MsgOnHeroSlept(bool&) const { abort(); }
    virtual bool MsgOnHeroFiredRangedWeapon() const { abort(); }
    virtual bool MsgOnHeroCastSpell(EHeroAbility&) const { abort(); }
    virtual bool MsgOnHeroPickedPocket(CScriptThing&) const { abort(); }
    virtual bool MsgOnHeroPickedLock(CScriptThing&) const { abort(); }
    virtual bool MsgOnFishingGameFinished(CScriptThing&) const { abort(); }
    virtual bool MsgOnTavernGameFinished(const CScriptThing&) const { abort(); }
    virtual bool MsgOnHeroRewardedWithItemsFrom(CCharString&) const { abort(); }
    virtual bool MsgOnChestOpeningCancelled() const { abort(); }
    virtual void DontPopulateNextLoadedRegion() const { abort(); }
    virtual void GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString&) const { abort(); }
    virtual void GetWanderingPopulationScriptDefNameInRegion(const CCharString&, CCharString&) const { abort(); }
    virtual bool IsHeroAllowedHenchmenInCurrentRegion() const { abort(); }
    virtual bool IsHeroAllowedHenchmenInRegion(const CCharString&) const { abort(); }
    virtual void PostAddScriptedEntities() const { abort(); }
    virtual bool IsPlayerHoldingUnsheatheRangedWeaponButton() const { abort(); }
    virtual bool IsPlayerHoldingLockTargetButton() const { abort(); }
    virtual bool IsPlayerHoldingFireRangedWeaponButton() const { abort(); }
    virtual bool IsPlayerHoldingFirstPersonTargetingButton() const { abort(); }
    virtual bool IsHeroInProjectileWeaponMode() const { abort(); }
    virtual CScriptThing& GetHero() { return *fixtureHero; }
    virtual CScriptThing GetHeroTargetedThing() const { abort(); }
    virtual CScriptThing GetThingWithScriptName(const CCharString&, const CCharString&) const { abort(); }
    virtual CScriptThing GetThingWithScriptName(const CCharString&) const { abort(); }
    virtual CScriptThing GetRandomThingWithScriptName(const CCharString&) const { abort(); }
    virtual long GetAllThingsWithScriptName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    virtual long GetAllCreaturesInAreaWithScriptName(const CCharString&, const C3DVector&, float, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing&, const CCharString&) const { abort(); }
    virtual CScriptThing GetFurthestWithScriptName(const CScriptThing&, const CCharString&) const { abort(); }
    virtual long GetAllThingsWithDefNameByDistanceFrom(const CScriptThing&, const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    virtual long GetAllThingsWithDefName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    virtual CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString&) const { abort(); }
    virtual CScriptThing GetFurthestWithDefName(const CScriptThing&, const CCharString&) const { abort(); }
    virtual CScriptThing GetThingWithUID(unsigned __int64) const { abort(); }
    virtual long GetAllCreaturesExcludingHero(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    virtual long GetAllThingsInLevel(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    virtual bool IsThingWithThisUIDAlive(unsigned __int64) const { abort(); }
    virtual long RegisterTimer() { abort(); }
    virtual void DeregisterTimer(long) { abort(); }
    virtual void SetTimer(long, long) { abort(); }
    virtual long GetTimer(long) const { abort(); }
    virtual CScriptThing CreateCreature(const CCharString&, const C3DVector&, const CCharString&, bool) const { abort(); }
    virtual CScriptThing CreateCreatureNearby(const CCharString&, const C3DVector&, float, const CCharString&, bool) const { abort(); }
    virtual CScriptThing CreateCreatureOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const { abort(); }
    virtual CScriptThing TurnCreatureInto(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void SetCreatureCreationDelayFrames(long) { abort(); }
    virtual void ResetCreatureCreationDelayFrames() { abort(); }
    virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, const CCharString&) const { abort(); }
    virtual CScriptThing CreateObject(const CCharString&, const C3DVector&, float, const CCharString&) const { abort(); }
    virtual CScriptThing CreateObjectOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const { abort(); }
    virtual CScriptThing CreateEffect(const CCharString&, const CScriptThing&, const CCharString&, const CCharString&, bool, bool) const { abort(); }
    virtual CScriptThing CreateEffect(const CCharString&, const C3DVector&, const CCharString&, float, bool, bool) const { abort(); }
    virtual CScriptThing CreateLight(const C3DVector&, const CRGBColour&, const CCharString&, float, float, bool) const { abort(); }
    virtual CScriptThing CreateExperienceOrb(const C3DVector&, long) const { abort(); }
    virtual CScriptThing CreateExplosion(const CCharString&, const C3DVector&, const C3DVector*, CCharString) const { abort(); }
    virtual CScriptThing CreatePhysicalBarrier(float, const C3DVector&, const C3DVector*, CCharString) const { abort(); }
    virtual CScriptThing CreateRumble(const C3DVector&, float, float, CCharString) const { abort(); }
    virtual void ClearAllRumbles() const { abort(); }
    virtual void RemoveThing(const CScriptThing&, bool, bool) const { abort(); }
    virtual void ShowOnScreenMessage(const CCharString&, float) const { abort(); }
    virtual void ShowOnScreenMessage(const CCharString&, const CCharString&, const CCharString&, float) const { abort(); }
    virtual void ShowOnScreenMessage(const C2DVector&, const CCharString&, const CRGBColour&, const CCharString&) const { abort(); }
    virtual void AddScreenMessage(const CCharString&, ETextGroupSelectionMethod) const { abort(); }
    virtual void AddScreenTitleMessage(const CCharString&, float, bool) const { abort(); }
    virtual void GiveHeroYesNoQuestion(const CCharString&, const CCharString&, const CCharString&, const CCharString&, bool) const { abort(); }
    virtual void DisplayGameInfo(const CCharString&) const { abort(); }
    virtual void DisplayGameInfoText(const CWideString&) const { abort(); }
    virtual bool IsSafeToDisplayGameInfo() const { abort(); }
    virtual bool DisplayTutorial(ETutorialCategory) const { abort(); }
    virtual bool IsTutorialSystemEnabled() const { abort(); }
    virtual void GiveHeroWeapon(const CCharString&, bool) const { abort(); }
    virtual void GiveHeroObject(const CCharString&, long, bool) const { abort(); }
    virtual void SetWeaponAsHerosActiveWeapon(const CCharString&) const { abort(); }
    virtual void GiveHeroItem(const CScriptThing&) const { abort(); }
    virtual bool GiveHeroItemsFromContainer(const CScriptThing&, bool) const { abort(); }
    virtual void TakeObjectFromHero(const CCharString&) const { abort(); }
    virtual void GiveHeroGold(long) const { abort(); }
    virtual long GetHeroGold() const { abort(); }
    virtual void GiveHeroExperience(long) const { abort(); }
    virtual void SetHeroAbleToGainExperience(bool) const { abort(); }
    virtual void SheatheHeroWeapons() const { abort(); }
    virtual void SetHeroWillAsUsable(bool) const { abort(); }
    virtual void SetHeroWeaponsAsUsable(bool) const { abort(); }
    virtual void SetWeaponOutCrimeEnabled(bool) const { abort(); }
    virtual void SetGuardsIgnoreCrimes(bool) const { abort(); }
    virtual void RemoveAllHeroWeapons() const { abort(); }
    virtual bool IsReportedOrUnreportedCrimeKnown(const CScriptThing&) const { abort(); }
    virtual void ConfiscateAllHeroItems() const { abort(); }
    virtual void ConfiscateAllHeroWeapons() const { abort(); }
    virtual void ConfiscateItemsOfTypeFromHero(const CCharString&) const { abort(); }
    virtual void ReturnAllConfiscatedItemsToHero() const { abort(); }
    virtual bool GiveHeroTutorial(ETutorialCategory) const { abort(); }
    virtual void MakeHeroCarryItemInHand(const CCharString&) const { abort(); }
    virtual void MakeHeroCarryItemInHand(const CScriptThing&, bool, bool) const { abort(); }
    virtual void AddTattooToHero(const CCharString&) const { abort(); }
    virtual void GiveHeroAbility(EHeroAbility, bool) const { abort(); }
    virtual bool IsPlayerZTargetingThing(const CScriptThing&) const { abort(); }
    virtual bool IsPlayerCreatureBlocking() const { abort(); }
    virtual bool IsPlayerCreatureReadyToFireProjectileWeapon(float&) const { abort(); }
    virtual long GetPlayerCreatureCombatMultiplier() const { abort(); }
    virtual long GetPlayerCreatureCombatMultiplierRunningNumHits() const { abort(); }
    virtual void ResetPlayerCreatureCombatMultiplier() const { abort(); }
    virtual bool IsPlayerCreatureFlourishEnabled() const { abort(); }
    virtual void SetPlayerCreatureOnlyTarget(const CScriptThing&) const { abort(); }
    virtual void ResetPlayerCreatureOnlyTarget() const { abort(); }
    virtual void RespawnHero(bool) const { abort(); }
    virtual void GiveHeroMorality(float) const { abort(); }
    virtual float GetHeroMorality() const { abort(); }
    virtual EMorality GetHeroMoralityCategory() const { abort(); }
    virtual void ChangeHeroMoralityDueToTheft() const { abort(); }
    virtual void ChangeHeroMoralityDueToPicklock() const { abort(); }
    virtual void GiveHeroRenownPoints(long) const { abort(); }
    virtual long GetHeroRenownLevel() const { abort(); }
    virtual bool IsHeroRenownLevelFull() const { abort(); }
    virtual void IncreaseHeroRenownLevel() const { abort(); }
    virtual long GetHeroStrengthLevel() const { abort(); }
    virtual long GetHeroSkillLevel() const { abort(); }
    virtual long GetHeroWillLevel() const { abort(); }
    virtual long GetHeroStatLevel(EHeroTrainableStatType) const { abort(); }
    virtual long GetHeroStatMax(EHeroTrainableStatType) const { abort(); }
    virtual void SetHeroAge(float) const { abort(); }
    virtual float GetHeroAge() const { abort(); }
    virtual void SetHeroAsTeenager(bool) const { abort(); }
    virtual void SetHeroAsApprentice(bool) const { abort(); }
    virtual float GetDistanceHeroCanBeHeardFrom() const { abort(); }
    virtual long GetHeroRoughExperienceLevel() const { abort(); }
    virtual long GetHeroExperienceAvailableToSpend() const { abort(); }
    virtual float GetHeroFatness() const { abort(); }
    virtual float GetHeroScariness() const { abort(); }
    virtual float GetHeroAttractiveness() const { abort(); }
    virtual float GetHeroWillEnergyLevel() const { abort(); }
    virtual void SetHeroWillEnergyLevel(float) const { abort(); }
    virtual void SetHeroWillEnergyAsAbleToRefill(bool) const { abort(); }
    virtual long GetNumberOfItemsOfTypeInInventory(const CCharString&) const { abort(); }
    virtual bool IsObjectInThingsPossession(const CCharString&, const CScriptThing&) const { abort(); }
    virtual bool IsHeroHandLampLit() const { abort(); }
    virtual void SetHeroHandLampAsLit(bool) const { abort(); }
    virtual bool IsWearingClothingItem(const CScriptThing&, const CCharString&) const { abort(); }
    virtual bool IsHeroNaked() const { abort(); }
    virtual void RemoveHeroClothing() const { abort(); }
    virtual void SetHeroAsWearing(const CCharString&) const { abort(); }
    virtual void ChangeHeroHairstyle(const CCharString&) const { abort(); }
    virtual void RemoveHeroHairstyle() const { abort(); }
    virtual bool IsWearingHairstyle(CScriptThing&, const CCharString&) const { abort(); }
    virtual bool IsPlayerCarryingItemOfType(const CCharString&) const { abort(); }
    virtual bool IsPlayerWieldingWeapon() const { abort(); }
    virtual bool IsEntityWieldingWeapon(const CScriptThing&) const { abort(); }
    virtual bool IsEntityWieldingMeleeWeapon(const CScriptThing&) const { abort(); }
    virtual bool IsEntityWieldingRangedWeapon(const CScriptThing&) const { abort(); }
    virtual bool GetPreviouslyWieldedMeleeWeaponName(CCharString&) const { abort(); }
    virtual bool GetPreviouslyWieldedRangedWeaponName(CCharString&) const { abort(); }
    virtual bool IsEntityAbleToAttack(const CScriptThing&) const { abort(); }
    virtual CScriptThing EntityGetThingInPrimarySlot(const CScriptThing&) const { abort(); }
    virtual void ApplyHeroPenaltyForDeath() const { abort(); }
    virtual void GiveHeroTitle(const CCharString&) const { abort(); }
    virtual EHeroTitle GetHeroTitle() const { abort(); }
    virtual void EntitySetAsMarryable(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing&, bool) const { abort(); }
    virtual void EntityForceMarriageToHero(const CScriptThing&, bool) const { abort(); }
    virtual bool IsEntityMarriedToHero(const CScriptThing&) const { abort(); }
    virtual bool IsEntityMarriable(const CScriptThing&) const { abort(); }
    virtual bool GetHeroHasMarried() const { abort(); }
    virtual bool GetHeroHasCurrentMarriage() const { abort(); }
    virtual bool GetHeroHasDivorcedMarriage() const { abort(); }
    virtual bool GetHeroHasChildren() const { abort(); }
    virtual bool GetHeroHasMurderedWife() const { abort(); }
    virtual bool IsHeroChild() const { abort(); }
    virtual void CancelHeroTeleportEffects() const { abort(); }
    virtual long GetNumberOfTimesHeroHasHadSex() const { abort(); }
    virtual void SetNumberOfTimesHeroHasHadSex(long) const { abort(); }
    virtual void SetHeroAsHavingHadSex(bool) const { abort(); }
    virtual void SetHeroAsHavingHadGaySex(bool) const { abort(); }
    virtual void GiveThingHeroRewardItem(const CScriptThing&, const CCharString&, const CCharString&) const { abort(); }
    virtual void GiveThingItemInHand(const CScriptThing&, const CCharString&, bool) const { abort(); }
    virtual void GiveThingItemInSlot(const CScriptThing&, const CCharString&, const CCharString&) const { abort(); }
    virtual void GiveHeroExpression(const CCharString&, long, bool) const { abort(); }
    virtual bool HeroHasExpression(const CCharString&) const { abort(); }
    virtual bool IsHeroPerformingExpression(const CCharString&) const { abort(); }
    virtual bool IsDPadButtonHeldForExpression(const CCharString&) const { abort(); }
    virtual void EntityFollowThing(const CScriptThing&, const CScriptThing&, float, bool) const { abort(); }
    virtual void EntityStopFollowing(const CScriptThing&) const { abort(); }
    virtual bool GetFollowingEntityList(const CScriptThing&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    virtual bool GetHeroSummonedCreaturesList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    virtual bool GetPerceivingHeroEntityList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    virtual bool IsEntityFollowingHero(const CScriptThing&) const { abort(); }
    virtual void EntitySetAsAllowedToFollowHero(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsAllowedToChangeRegionFollowingState(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(const CScriptThing&, bool) const { abort(); }
    virtual void TeleportAllFollowersToHeroPosition() const { abort(); }
    virtual void EntityTeleportToHeroPosition(const CScriptThing&) const { abort(); }
    virtual void SendEntityEvent(NEntityEvents::EEventType, const CScriptThing&, const CScriptThing&, CThing*) const { abort(); }
    virtual float GetWaterHeightAtPosition(const C3DVector&) const { abort(); }
    virtual bool IsFishingSpotEnabled(const CScriptThing&) const { abort(); }
    virtual void DisableFishingSpot(const CScriptThing&) const { abort(); }
    virtual void UpdateFishWeight(const CScriptThing&) const { abort(); }
    virtual float GetBestFishWeight(const CCharString&) const { abort(); }
    virtual void HeroGoFishing(bool) const { abort(); }
    virtual long GetHeroFishingLevel() const { abort(); }
    virtual void HeroGoDigging() const { abort(); }
    virtual void HeroStopDigging() const { abort(); }
    virtual void HeroPlayOracleMinigame() const { abort(); }
    virtual bool IsHeroPlayingOracleMinigame() const { abort(); }
    virtual bool HasHeroWonOracleMinigame() const { abort(); }
    virtual void HeroPlayFireheartMinigame() const { abort(); }
    virtual void HeroQuitFireheartMinigame() const { abort(); }
    virtual bool HasHeroForceQuitFireheartMinigame() const { abort(); }
    virtual float GetHeroHealth() const { abort(); }
    virtual float GetHeroHealthMax() const { abort(); }
    virtual float GetHeroHealthPercentage() const { abort(); }
    virtual long GetHeroWillEnergy() const { abort(); }
    virtual long GetHeroWillEnergyMax() const { abort(); }
    virtual long GetHeroWillEnergyPercentage() const { abort(); }
    virtual void ChangeHeroHealthBy(float, bool, bool) const { abort(); }
    virtual float GetHealth(const CScriptThing&) const { return state.alive?1.0f:0.0f; }
    virtual void ModifyThingHealth(const CScriptThing&, float, bool) const { abort(); }
    virtual void EntitySetMaxHealth(const CScriptThing&, float, bool) const { abort(); }
    virtual void SetThingAsKilled(const CScriptThing&) const { abort(); }
    virtual void GiveHeroNewQuestObjective(const CCharString&, unsigned long) const { abort(); }
    virtual void TellHeroQuestObjectiveCompleted(unsigned long) const { abort(); }
    virtual void TellHeroQuestObjectiveFailed(unsigned long) const { abort(); }
    virtual void AddQuestRegion(const CCharString&, const CCharString&) const { abort(); }
    virtual void SetQuestWorldMapOffset(const CCharString&, const C2DCoordI&) const { abort(); }
    virtual bool IsHeroOnQuest() const { abort(); }
    virtual void HeroReceiveMessageFromGuildMaster(const CCharString&, const CCharString&, bool, bool) const { abort(); }
    virtual void SetGuildMasterMessages(bool) const { abort(); }
    virtual void ActivateQuest(const CCharString&) const { abort(); }
    virtual void ActivateMultipleQuests(const CArray<CCharString>&) const { abort(); }
    virtual void ActivateQuestWithoutLoadingResources(const CCharString&) const { abort(); }
    virtual void ActivateMultipleQuestsWithoutLoadingResources(const CArray<CCharString>&) const { abort(); }
    virtual void DeactivateQuest(const CCharString&, unsigned long) const { abort(); }
    virtual void DeactivateQuestLater(const CCharString&, unsigned long) const { abort(); }
    virtual void PrepareQuestsWhenFinalQuestIsActivated(const CCharString&) const { abort(); }
    virtual void PrepareQuestsWhenFinalQuestIsCompleted() const { abort(); }
    virtual bool IsQuestActive(const CCharString&) const { abort(); }
    virtual bool IsQuestRegistered(const CCharString&) const { abort(); }
    virtual bool IsQuestCompleted(const CCharString&) const { abort(); }
    virtual bool IsQuestFailed(const CCharString&) const { abort(); }
    virtual void SetQuestAsCompleted(const CCharString&, bool, bool, bool) const { abort(); }
    virtual void SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) const { abort(); }
    virtual void SetQuestAsPersistent(const CCharString&, bool) const { abort(); }
    virtual const CCharString& GetExclusiveQuestScriptName() const { abort(); }
    virtual void AddQuestCard(const CCharString&, const CCharString&, bool, bool) const { abort(); }
    virtual void RemoveQuestCardFromGuild(const CCharString&) const { abort(); }
    virtual void RemoveQuestCardFromHero(const CCharString&) const { abort(); }
    virtual void GiveHeroQuestCardDirectly(const CCharString&, const CCharString&, bool) const { abort(); }
    virtual void SetQuestCardObjective(const CCharString&, const CCharString&, const CCharString&, const CCharString&) const { abort(); }
    virtual void SetQuestCardGoldReward(const CCharString&, long) const { abort(); }
    virtual void SetQuestCardRenownReward(const CCharString&, long) const { abort(); }
    virtual void RemoveAllAvailableQuestCardsFromGuild() const { abort(); }
    virtual void FailAllActiveQuests() const { abort(); }
    virtual void GetAllActiveQuestInfo(std::vector<CCharString,std::allocator<CCharString> >&, std::vector<CCharString,std::allocator<CCharString> >&) const { abort(); }
    virtual void AddFeatCard(long, const CCharString&, const CCharString&) const { abort(); }
    virtual void AddBoast(const CCharString&, long, long, long, bool, const CCharString&, long) const { abort(); }
    virtual void RemoveBoast(long, const CCharString&) const { abort(); }
    virtual void SetBoastAsFailed(long, const CCharString&) const { abort(); }
    virtual void SetBoastAsCompleted(long, const CCharString&) const { abort(); }
    virtual bool IsBoastTaken(long, const CCharString&) const { abort(); }
    virtual void AddLogBookEntry(const CWideString&, const CWideString&, const CWideString&, long) const { abort(); }
    virtual void KickOffQuestStartScreen(const CCharString&, bool, bool) const { abort(); }
    virtual void KickOffDeathScreen() const { abort(); }
    virtual void KickOffCreditsScreen(CCharString&) const { abort(); }
    virtual void SetPreferredQuickAccessItem(const CCharString&, long, long) const { abort(); }
    virtual CCharString GetDeathRecoveryMarkerName() const { abort(); }
    virtual void SetDeathRecoveryMarkerName(const CCharString&) const { abort(); }
    virtual void ResetDeathRecoveryMarkerNameToDefault() const { abort(); }
    virtual bool IsToFailQuestOnDeath() const { abort(); }
    virtual void SetWhetherToFailQuestOnDeath(bool) const { abort(); }
    virtual void ResetWhetherToFailQuestOnDeathToDefault() const { abort(); }
    virtual CScriptThing GetMostRecentValidUsedTarget() const { abort(); }
    virtual CCharString GetMostRecentValidUsedTargetName() const { abort(); }
    virtual void DisplayQuestInfo(bool) const { abort(); }
    virtual void SetQuestInfoName(const char*) const { abort(); }
    virtual void SetQuestInfoText(const char*) const { abort(); }
    virtual long AddQuestInfoBar(float, float, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float) const { abort(); }
    virtual long AddQuestInfoBarHealth(const CScriptThing&, const CRGBColour&, const CCharString&, float) const { abort(); }
    virtual long AddQuestInfoTimer(const CTimer&, const CCharString&, float) const { abort(); }
    virtual long AddQuestInfoCounter(const CCounter&, const CCharString&, float) const { abort(); }
    virtual long AddQuestInfoCounter(const CCharString&, long, float) const { abort(); }
    virtual long AddQuestInfoCounterList(const CCharString&, long, float) const { abort(); }
    virtual long AddQuestInfoTick(const CCharString&, bool, float) const { abort(); }
    virtual long AddQuestInfoTick(EGameAction, bool, float) const { abort(); }
    virtual void UpdateQuestInfoBar(long, float, float, float) const { abort(); }
    virtual void ChangeQuestInfoBarColour(long, const CRGBColour&, const CRGBColour&) const { abort(); }
    virtual void UpdateQuestInfoTimer(long, float) const { abort(); }
    virtual void UpdateQuestInfoCounter(long, long, long) const { abort(); }
    virtual void UpdateQuestInfoCounterList(long, long, long) const { abort(); }
    virtual void UpdateQuestInfoTick(long, bool) const { abort(); }
    virtual void RemoveQuestInfoElement(long) const { abort(); }
    virtual void RemoveAllQuestInfoElements() const { abort(); }
    virtual void DisplayTime(bool) const { abort(); }
    virtual void DisplayMoneyBag(bool) const { abort(); }
    virtual void DisplayMiniGameInfo(bool, EMiniGameType) const { abort(); }
    virtual void UpdateMiniGameInfoBar(float) const { abort(); }
    virtual bool IsEntityPickPocketable(const CScriptThing&) const { abort(); }
    virtual bool IsEntityPickLockable(const CScriptThing&) const { abort(); }
    virtual bool IsEntityStealable(const CScriptThing&) const { abort(); }
    virtual void EntitySetAsPickPocketed(const CScriptThing&) const { abort(); }
    virtual void EntitySetAsPickLocked(const CScriptThing&) const { abort(); }
    virtual void EntitySetAsStolen(const CScriptThing&) const { abort(); }
    virtual void MiniMapAddMarker(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void MiniMapSetMarkerGraphic(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void MiniMapRemoveMarker(const CScriptThing&) const { abort(); }
    virtual void MiniMapRemoveAllMarkers() const { abort(); }
    virtual void MiniMapAllowRouteBetweenRegions(const CCharString&, const CCharString&, bool) const { abort(); }
    virtual void MiniMapSetAsEnabled(bool) const { abort(); }
    virtual void EntitySetAsHiddenOnMiniMap(const CScriptThing&, bool) const { abort(); }
    virtual void SetHUDEnabled(bool) const { abort(); }
    virtual bool TextEntryExists(const CCharString&) const { abort(); }
    virtual CCharString GetValidTextEntryNameWithAttitude(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void SetThingHasInformation(const CScriptThing&, bool, bool, bool) const { abort(); }
    virtual void ClearThingHasInformation(const CScriptThing&) const { ++state.clears; }
    virtual void EntitySetWillBeUsingNarrator(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void EntityResetAsPureAINarrator(const CScriptThing&) const { abort(); }
    virtual long AddNewConversation(const CScriptThing&, bool, bool) const { abort(); }
    virtual void AddPersonToConversation(long, const CScriptThing&) const { abort(); }
    virtual void AddLineToConversation(long, const CCharString&, bool, const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void RemoveConversation(long, bool) const { abort(); }
    virtual bool IsConversationActive(long) const { abort(); }
    virtual void PlayAVIMovie(const CCharString&) const { abort(); }
    virtual void StartMovieSequence(const CCharString& title, CScriptGameResourceObjectMovieBase& movie) {
        check(!*textOf(title)); ++state.movieCount;
        if (movieOwnership) {
            movie.SetImplementation(new FixtureMovie);
            if (movieOwnership==2) *retainedMovie=movie;
        }
    }
    virtual void FixMovieSequenceCamera(bool) const { abort(); }
    virtual void FadeScreenOutUntilNextCallToFadeScreenIn(float, float) { abort(); }
    virtual bool FadeScreenOut(float, float, CRGBColour) { abort(); }
    virtual void FadeScreenIn() { abort(); }
    virtual bool IsScreenFadingOut() const { abort(); }
    virtual void Pause(float) const { abort(); }
    virtual void EndCutFade() { abort(); }
    virtual void EndLetterBox() { abort(); }
    virtual void PauseAllNonScriptedEntities(bool on) { check(on!=state.paused);state.paused=on;if(on)++state.pauses;else ++state.resumes; }
    virtual void PauseAllEntities(bool) { abort(); }
    virtual void SetAllowScreenFadingOnNextRegionChange(bool) { abort(); }
    virtual void SetAllowScreenFadingIfAlreadyFaded(bool) { abort(); }
    virtual void SetToKeepHeroAbilitiesDuringCutscenes(bool) const { abort(); }
    virtual void SetToDisplayTutorialsDuringCutscenes(bool) const { abort(); }
    virtual void SetCutsceneMode(bool, bool) const { abort(); }
    virtual bool IsInCutscene() const { abort(); }
    virtual void SetCutsceneSkippable(bool) const { abort(); }
    virtual void SetCutsceneSkippableWhilePaused(bool) const { abort(); }
    virtual void SetCutsceneActionMode(bool, const CCharString&) const { abort(); }
    virtual void PreloadNewScene(float) const { abort(); }
    virtual void StartProgressDisplay() const { abort(); }
    virtual void StopProgressDisplay() const { abort(); }
    virtual void SetScreenMessagesEnabled(bool) const { abort(); }
    virtual bool IsHeroControlledByPlayer() const { abort(); }
    virtual bool IsInMovieSequence() const { abort(); }
    virtual void CancelAbilitiesForCutscene() const { abort(); }
    virtual void ResumeAbilitiesForCutscene() const { abort(); }
    virtual void CancelUsingAbility(EHeroAbility) const { abort(); }
    virtual void SetAbilityAvailability(EHeroAbility, bool) const { abort(); }
    virtual void SetEnvironmentalEffectsAlwaysUpdate(bool) const { abort(); }
    virtual void SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool) const { abort(); }
    virtual void RemoveDeadCreature(const CCharString&) const { abort(); }
    virtual void CameraSetCameraPreloadFlag(bool) const { abort(); }
    virtual void CameraCircleAroundThing(const CScriptThing&, const C3DVector&, float) const { abort(); }
    virtual void CameraCircleAroundPos(const C3DVector&, const C3DVector&, float) const { abort(); }
    virtual void CameraMoveToPosAndLookAtPos(const C3DVector&, const C3DVector&, float) const { abort(); }
    virtual void CameraMoveToPosAndLookAtThing(const C3DVector&, const CScriptThing&, float) const { abort(); }
    virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const C3DVector&, float, float) const { abort(); }
    virtual void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const CScriptThing&, float, float) const { abort(); }
    virtual void CameraMoveBetweenLookFromAndLookTo(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&, float) const { abort(); }
    virtual void CameraUseCameraPoint(const CCharString&, const CScriptThing&, float, long, long) const { abort(); }
    virtual void CameraUseCameraPoint(const CScriptThing&, const CScriptThing&, float, long, long) const { abort(); }
    virtual void CameraUseCameraPoint(const CCharString&, const C3DVector*, const CRightHandedSet*, float, long, long) const { abort(); }
    virtual void CameraUseCameraPoint(const CScriptThing&, const C3DVector*, const CRightHandedSet*, float, long, long) const { abort(); }
    virtual void CameraDoConversation(const CScriptThing&, const CScriptThing&, ECameraOp, bool) const { abort(); }
    virtual void CameraDefault() { abort(); }
    virtual void CameraResetToViewBehindHero(float) { abort(); }
    virtual bool IsCameraInScriptedMode() const { abort(); }
    virtual void CameraUseScreenEffect(float, float, float) const { abort(); }
    virtual void CameraCancelScreenEffect() const { abort(); }
    virtual bool IsCameraPosOnScreen(const C3DVector&) const { abort(); }
    virtual float GetGameAngleXY() const { abort(); }
    virtual void CameraEarthquakeIntensityAtPos(const C3DVector&, float, float) const { abort(); }
    virtual void CameraShake(float, float) const { abort(); }
    virtual void OpenDoor(const CScriptThing&) const { abort(); }
    virtual void CloseDoor(const CScriptThing&) const { abort(); }
    virtual void OpenHouseDoors(const CScriptThing&) const { abort(); }
    virtual void CloseHouseDoors(const CScriptThing&) const { abort(); }
    virtual void JamDoor(const CScriptThing&) const { abort(); }
    virtual void SetDoorTriggerType(const CScriptThing&, EDoorTriggerType) const { abort(); }
    virtual void OverrideAutomaticHouseLocking(const CScriptThing&, bool) const { abort(); }
    virtual void SetHouseOwnedByPlayer(const CScriptThing&, bool, bool) const { abort(); }
    virtual void SetBuyableHouseAsScripted(const CScriptThing&, bool) const { abort(); }
    virtual bool IsChestOpen(const CScriptThing&) const { abort(); }
    virtual bool OpenChest(const CScriptThing&, bool) const { abort(); }
    virtual void CloseChest(const CScriptThing&) const { abort(); }
    virtual long GetNumberOfKeysNeededToUnlockChest(const CScriptThing&, CCharString&) const { abort(); }
    virtual void DisplayLockedChestMessage(const CScriptThing&) const { abort(); }
    virtual void SetTrophyAsMountable(const CScriptThing&, bool) const { abort(); }
    virtual void SetVillageLimbo(const CScriptThing&, bool) const { abort(); }
    virtual void SetCreatureNotReload(const CScriptThing&) const { abort(); }
    virtual bool IsSleepingTime(const CScriptThing&) const { abort(); }
    virtual void EnableGuards(const CScriptThing&, bool) const { abort(); }
    virtual void EnableVillagerDefTypes(const CScriptThing&, bool, const CCharString&) const { abort(); }
    virtual CScriptThing TryToRespawnDefNamed(const CScriptThing&, const CCharString&, const C3DVector&) const { abort(); }
    virtual void ClearHeroEnemyOfGuards(const CScriptThing&) const { abort(); }
    virtual void SetThingAsUsable(const CScriptThing&, bool) const { abort(); }
    virtual void SetThingHomeBuilding(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void SetVillageAttitude(const CScriptThing&, EScriptVillageAttitude) const { abort(); }
    virtual void AddCrimeCommitted(const CScriptThing&, ECrime, bool, const CScriptThing*, const CScriptThing*, EOpinionPostDeedType) const { abort(); }
    virtual void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void ClearThingBestEnemyTarget(const CScriptThing&) const { abort(); }
    virtual void EntitySetInLimbo(const CScriptThing&, bool, bool) const { abort(); }
    virtual bool IsEntityInLimbo(const CScriptThing&) const { abort(); }
    virtual bool EntityGetShotStrikePos(const CScriptThing&, C3DVector&) const { abort(); }
    virtual void EntitySetNegateAllHits(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetEvadeAllHits(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAbleToBeEngagedInCombat(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAlwaysBlockAttacksFromThing(const CScriptThing&, const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAttackThingImmediately(const CScriptThing&, const CScriptThing&, bool, bool) const { abort(); }
    virtual void EntitySetCombatType(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void EntityResetCombatTypeToDefault(const CScriptThing&) const { abort(); }
    virtual void EntitySetMaxNumberOfAttackers(const CScriptThing&, long) const { abort(); }
    virtual void EntityClearMaxNumberOfAttackers(const CScriptThing&) const { abort(); }
    virtual void EntityAttachToScript(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void EntitySetCombatAbility(const CScriptThing&, float) const { abort(); }
    virtual void EntitySetRangedTarget(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void EntityClearRangedTarget(const CScriptThing&) const { abort(); }
    virtual void EntitySetTargetable(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetTargetingType(const CScriptThing&, ETargetingType) const { abort(); }
    virtual void EntitySetTargetingValidTargetWithoutLOS(const CScriptThing&, bool) const { abort(); }
    virtual void EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool) const { abort(); }
    virtual void EntityTeleportToPosition(const CScriptThing&, const C3DVector&, float, bool, bool) const { abort(); }
    virtual void EntitySetFacingAngle(const CScriptThing&, float, bool) const { abort(); }
    virtual void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetPerceptionVariables(const CScriptThing&, float, float, float) const { abort(); }
    virtual void SetThingPersistent(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetThingAsWantingMoney(const CScriptThing&, long) const { abort(); }
    virtual void EntitySetAppearanceMorphSeed(const CScriptThing&, long) const { abort(); }
    virtual void SetEntityAsRegionFollowing(const CScriptThing&, const CScriptThing&, bool) const { abort(); }
    virtual void SetEntityAsFollowingHeroThroughTeleporters(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAppearanceSeed(const CScriptThing&, unsigned long) const { abort(); }
    virtual void EntityGetAppearanceSeed(const CScriptThing&, unsigned long&) const { abort(); }
    virtual void EntitySetAsForSale(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetStockItemPrice(const CScriptThing&, long) const { abort(); }
    virtual long EntityGetStockItemPrice(const CScriptThing&) const { abort(); }
    virtual void EntityPlayObjectAnimation(const CScriptThing&, const CCharString&, bool) const { abort(); }
    virtual void EntitySetMaxRunningSpeed(const CScriptThing&, float) const { abort(); }
    virtual void EntitySetMaxWalkingSpeed(const CScriptThing&, float) const { abort(); }
    virtual void EntityResetMaxRunningSpeed(const CScriptThing&) const { abort(); }
    virtual void EntityResetMaxWalkingSpeed(const CScriptThing&) const { abort(); }
    virtual void EntityAttachToVillage(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void EntitySetAsSittingOnFloor(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsHavingBoundHands(CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsRemoveAllMovementBlockingModes(CScriptThing&) const { abort(); }
    virtual void EntitySetAsScared(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsDrunk(CScriptThing&, bool) const { abort(); }
    virtual void EntityForceToLookAtThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void EntityForceToLookAtCamera(const CScriptThing&, bool) const { abort(); }
    virtual void EntityForceToLookAtNothing(const CScriptThing&) const { abort(); }
    virtual void EntityResetForceToLookAt(const CScriptThing&) const { abort(); }
    virtual void EntitySetShotAccuracyPercentage(const CScriptThing&, long) const { abort(); }
    virtual CScriptThing EntityGetStandingOnThing(const CScriptThing&) const { abort(); }
    virtual CScriptThing EntityGetStandingInsideBuilding(const CScriptThing&) const { abort(); }
    virtual void EntityDropGenericBox(const CScriptThing&) const { abort(); }
    virtual void EntitySheatheWeapons(const CScriptThing&, bool) const { abort(); }
    virtual void EntityUnsheatheWeapons(const CScriptThing&, bool) const { abort(); }
    virtual void EntityUnsheatheMeleeWeapon(const CScriptThing&, bool) const { abort(); }
    virtual void EntityUnsheatheRangedWeapon(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAlpha(const CScriptThing&, float, bool) const { abort(); }
    virtual void EntitySetAsDrawable(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetCutsceneBehaviour(const CScriptThing&, ECutsceneBehaviour) const { abort(); }
    virtual ESex EntityGetSex(const CScriptThing&) const { abort(); }
    virtual void EntitySetAsAbleToWalkThroughSolidObjects(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsRespondToHit(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsDamageable(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsKillable(const CScriptThing&, bool, bool) const { abort(); }
    virtual void EntitySetAsLocked(const CScriptThing&, bool) const { abort(); }
    virtual void EntityDecapitate(const CScriptThing&) const { abort(); }
    virtual void EntityGiveGold(const CScriptThing&, long) const { abort(); }
    virtual void EntitySetAllowBossPhaseChanges(const CScriptThing&, bool) const { abort(); }
    virtual long EntityGetBossPhase(const CScriptThing&) const { abort(); }
    virtual void EntitySetBossPhase(const CScriptThing&, long) const { abort(); }
    virtual void EntityResetCreatureMode(const CScriptThing&) const { abort(); }
    virtual bool EntitySetAsReceivingEvents(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsToAddToStatChangesWhenHit(const CScriptThing&, bool) const { abort(); }
    virtual void EntityLeaveCombatStance(const CScriptThing&) const { abort(); }
    virtual void EntitySetAsUseMovementInActions(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsDisplayingEmoteIcon(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetAsCollidableToThings(const CScriptThing&, bool) const { abort(); }
    virtual void EntityEnableGravity(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetLightAsOn(const CScriptThing&, bool) const { abort(); }
    virtual void EntityFadeOut(const CScriptThing&, float) const { abort(); }
    virtual void EntityFadeIn(const CScriptThing&, float) const { abort(); }
    virtual void EntityBeginLoadingAnimation(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void EntityBeginLoadingBasicAnimations(const CScriptThing&) const { abort(); }
    virtual bool EntityCastForcePush(const CScriptThing&, bool) const { abort(); }
    virtual void EntityCastLightningAtTarget(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void BeginLoadingMesh(const CCharString&) const { abort(); }
    virtual bool EntityWillTeleportToArea(const CScriptThing&, C3DVector, float, float) const { abort(); }
    virtual void EntityStartScreamerSuperAttackThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void EntityEndScreamerSuperAttackThing(const CScriptThing&) const { abort(); }
    virtual void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool) { abort(); }
    virtual void SetLightColour(const CScriptThing&, const CRGBColour&) const { abort(); }
    virtual void CreatureGeneratorSetFamily(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void CreatureGeneratorTrigger(const CScriptThing&) const { abort(); }
    virtual void CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(const CScriptThing&, bool) const { abort(); }
    virtual bool CreatureGeneratorIsDepleted(const CScriptThing&) const { abort(); }
    virtual bool CreatureGeneratorIsDestroyed(const CScriptThing&) const { abort(); }
    virtual void CreatureGeneratorSetGeneratedCreatureScriptName(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void CreatureGeneratorSetNumTriggers(const CScriptThing&, long) const { abort(); }
    virtual long CreatureGeneratorGetNumGeneratedCreatures(const CScriptThing&) const { abort(); }
    virtual bool CreatureGeneratorAreAllCreaturesAlive(const CScriptThing&) const { abort(); }
    virtual void CreatureGeneratorAddTriggerer(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void CreatureGeneratorRemoveTriggerer(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void SetCreatureGeneratorEnabled(const CScriptThing&, bool) const { abort(); }
    virtual void SetCreatureGeneratorsEnabled(const CCharString&, bool) const { abort(); }
    virtual void SetCreatureGeneratorsEnabledDuringScript(const CCharString&, bool) const { abort(); }
    virtual void SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup, bool) const { abort(); }
    virtual bool IsCreatureGenerationEnabledForRegion(const CCharString&) const { abort(); }
    virtual bool IsCreatureFlying(const CScriptThing&) const { abort(); }
    virtual void SetTeleporterAsActive(const CScriptThing&, bool) const { abort(); }
    virtual bool IsTeleporterActive(const CScriptThing&) const { abort(); }
    virtual void SetTeleportingAsActive(bool) const { abort(); }
    virtual bool IsTeleportingActive() const { abort(); }
    virtual void SetRegionExitAsActive(const CScriptThing&, bool) const { abort(); }
    virtual void SetRegionEntranceAsActive(const CScriptThing&, bool) const { abort(); }
    virtual void SetRegionTextDisplayAsActive(bool) const { abort(); }
    virtual void SetHeroSleepingAsEnabled(bool) const { abort(); }
    virtual bool IsHeroSleepingEnabled() const { abort(); }
    virtual void SetExperienceSpendingAsEnabled(bool) const { abort(); }
    virtual void SetMoralityChangingAsEnabled(bool) const { abort(); }
    virtual void SetSummonerDeathExplosionAffectsHero(bool) const { abort(); }
    virtual CScriptThing GetNearestEnabledDiggingSpot(const CScriptThing&) const { abort(); }
    virtual bool IsDiggingSpotEnabled(const CScriptThing&) const { abort(); }
    virtual bool IsDiggingSpotHidden(const CScriptThing&) const { abort(); }
    virtual void SetDiggingSpotAsHidden(const CScriptThing&, bool) const { abort(); }
    virtual bool CheckForCameraMessage(const CCharString&) const { abort(); }
    virtual void WaitForCameraMessage(const CCharString&) const { abort(); }
    virtual void SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const { abort(); }
    virtual void SetFireToThing(const CScriptThing&) const { abort(); }
    virtual void ExtinguishFiresOnThing(const CScriptThing&) const { abort(); }
    virtual bool IsThingOnFire(const CScriptThing&) const { abort(); }
    virtual void AddItemToContainer(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void RemoveItemFromContainer(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void EntitySetDeathContainerAsEnabled(const CScriptThing&, bool) const { abort(); }
    virtual void GetItemDefNamesFromContainer(const CScriptThing&, std::vector<CCharString,std::allocator<CCharString> >&) const { abort(); }
    virtual void SetCreatureBrain(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void EntitySetStategroupEnabled(const CScriptThing&, const CCharString&, bool) const { abort(); }
    virtual void EntitySetAllStategroupsEnabled(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetCombatEnabled(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetSleepEnabled(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetOpinionReactionsEnabled(const CScriptThing&, bool) const { abort(); }
    virtual void EntitySetDeedReactionsEnabled(const CScriptThing&, bool) const { abort(); }
    virtual void DebugGetAllTextEntriesForTargetedThing(std::set<unsigned long,std::less<unsigned long>,std::allocator<unsigned long> >&) const { abort(); }
    virtual void EntitySetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void EntityUnsetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void EntitySetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void EntityUnsetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void EntitySetInFaction(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void SetFactionAsAlliedToFaction(const CCharString&, const CCharString&) const { abort(); }
    virtual void SetFactionAsNeutralToFaction(const CCharString&, const CCharString&) const { abort(); }
    virtual void SetFactionAsEnemyToFaction(const CCharString&, const CCharString&) const { abort(); }
    virtual bool AreEntitiesEnemies(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType) const { abort(); }
    virtual void GetOpinionAttitudeAsString(EOpinionAttitudeType, CCharString&) const { abort(); }
    virtual EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(const CScriptThing&) const { abort(); }
    virtual void EntityGetOpinionAttitudeToPlayerAsString(const CScriptThing&, CCharString&) const { abort(); }
    virtual float EntityGetOpinionOfPlayer(const CScriptThing&, EOpinion) const { abort(); }
    virtual void EntitySetOpinionReactionMask(const CScriptThing&, long) const { abort(); }
    virtual void EntitySetOpinionReactionMask(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void EntitySetOpinionDeedMask(const CScriptThing&, long) const { abort(); }
    virtual void EntitySetOpinionDeedMask(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void EntitySetOpinionDeedTypeEnabled(const CScriptThing&, EOpinionDeedType, bool) const { abort(); }
    virtual void EntitySetOpinionAttitudeEnabled(const CScriptThing&, EOpinionAttitudeType, bool) const { abort(); }
    virtual void EntitySetOpinionReactionEnabled(const CScriptThing&, EOpinionReactionType, bool) const { abort(); }
    virtual void EntitySetPersonalityOverride(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void EntitySetPersonalityOverride(const CScriptThing&, long) const { abort(); }
    virtual void EntityClearPersonalityOverride(const CScriptThing&) const { abort(); }
    virtual void EntitySetAsOpinionSource(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void EntitySetAsOpinionSource(const CScriptThing&, long) const { abort(); }
    virtual void EntityUnsetAsOpinionSource(const CScriptThing&, bool) const { abort(); }
    virtual void OpinionSourceSetAsExclusive(const CScriptThing&, bool) const { abort(); }
    virtual void OpinionSourceSetAsAttentionGrabbing(const CScriptThing&, bool) const { abort(); }
    virtual void EntityPostOpinionDeedToAll(const CScriptThing&, EOpinionDeedType) const { abort(); }
    virtual void EntityPostOpinionDeedToRecipient(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { abort(); }
    virtual void EntityPostOpinionDeedToRecipientVillage(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { abort(); }
    virtual long EntityPostOpinionDeedKeepSearchingForWitnesses(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { abort(); }
    virtual void RemoveOpinionDeedStillSearchingForWitnesses(const CScriptThing&, long) const { abort(); }
    virtual bool IsDeedWitnessed(long) const { abort(); }
    virtual bool CanThingBe_Seen_ByOtherThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual bool CanThingBe_NearlySeen_ByOtherThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual bool CanThingBe_Smelled_ByOtherThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual bool CanThingBe_Heard_ByOtherThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void EntitySetAsAwareOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    virtual void EntitySetSoundRadius(const CScriptThing&, float) const { abort(); }
    virtual void EntitySetSmellRadius(const CScriptThing&, float) const { abort(); }
    virtual void EntitySetSightRadius(const CScriptThing&, float) const { abort(); }
    virtual void EntitySetExtendedSightRadius(const CScriptThing&, float) const { abort(); }
    virtual void EntitySetGiveUpChaseRadius(const CScriptThing&, float) const { abort(); }
    virtual float EntityGetHearingRadius(const CScriptThing&) const { abort(); }
    virtual bool ManuallyTriggerTrap(const CScriptThing&) const { abort(); }
    virtual bool ManuallyResetTrap(const CScriptThing&) const { abort(); }
    virtual void SetTimeOfDay(float) const { abort(); }
    virtual long GetTimeOfDay() const { abort(); }
    virtual void SetTimeAsStopped(bool, long&) const { abort(); }
    virtual void FastForwardTimeTo(float, float) const { abort(); }
    virtual bool IsTimeOfDayBetween(long, long) const { abort(); }
    virtual long GetDayOfWeek() const { abort(); }
    virtual long GetDayCount() const { abort(); }
    virtual long GetWorldFrame() const { abort(); }
    virtual long GetConstantFPS() const { abort(); }
    virtual CCharString GetActiveQuestName() const { abort(); }
    virtual void TransitionToTheme(const CCharString&, float) const { abort(); }
    virtual void ResetToDefaultTheme(float) const { abort(); }
    virtual void TransitionToThemeAllInternals(const CCharString&, float) const { abort(); }
    virtual void ResetToDefaultThemeAllInternals(float) const { abort(); }
    virtual void TransitionToThemeExternals(const CCharString&, float) const { abort(); }
    virtual void ResetToDefaultThemeExternals(float) const { abort(); }
    virtual void SetEnvironmentThemeWeightAllChannels(const CCharString&, float) const { abort(); }
    virtual void SetEnvironmentThemeWeightAllInternals(const CCharString&, float) const { abort(); }
    virtual void SetEnvironmentThemeWeightExternals(const CCharString&, float) const { abort(); }
    virtual void SetSoundThemesAsEnabledForRegion(const CCharString&, bool) const { abort(); }
    virtual void SetAllSoundsAsMuted(bool) const { abort(); }
    virtual void* RadialBlurFadeTo(float, C3DVector, float, C3DVector, float, float, float) const { abort(); }
    virtual void* RadialBlurFadeTo(float, float, float, float, float, float, float) const { abort(); }
    virtual void RadialBlurFadeOut(float, void*) const { abort(); }
    virtual bool IsRadialBlurFadeActive() const { abort(); }
    virtual void CancelRadialBlurFade() const { abort(); }
    virtual void RadialBlurSetCenterWorldPos(void*, const C3DVector&) const { abort(); }
    virtual void* DisplacementMonochromeEffectColourFadeTo(float, const CRGBFloatColour&) const { abort(); }
    virtual void DisplacementMonochromeEffectColourFadeOut(float, void*) const { abort(); }
    virtual void* ScreenFilterFadeTo(float, float, float, float, float, const CRGBFloatColour&, const std::vector<CScreenFilterSThingByPass,std::allocator<CScreenFilterSThingByPass> >&) const { abort(); }
    virtual void ScreenFilterFadeOut(float, void*) const { abort(); }
    virtual void SetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&, void*) const { abort(); }
    virtual void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&) const { abort(); }
    virtual bool IsGiftRomantic(const CCharString&) const { abort(); }
    virtual bool IsGiftFriendly(const CCharString&) const { abort(); }
    virtual bool IsGiftOffensive(const CCharString&) const { abort(); }
    virtual bool IsThingABed(const CScriptThing&) const { abort(); }
    virtual bool IsThingAChest(const CScriptThing&) const { abort(); }
    virtual bool IsThingADoor(const CScriptThing&) const { abort(); }
    virtual bool IsThingSmashable(const CScriptThing&) const { abort(); }
    virtual bool IsThingSearchable(const CScriptThing&) const { abort(); }
    virtual void ApplyScriptBrush(const CCharString&) const { abort(); }
    virtual void EnableDecals(bool) const { abort(); }
    virtual unsigned long PlayCriteriaSoundOnThing(const CScriptThing&, const CCharString&) const { abort(); }
    virtual unsigned long PlaySoundOnThing(const CScriptThing&, const CCharString&) const { abort(); }
    virtual unsigned long PlaySoundAtPos(const C3DVector&, const CCharString&) const { abort(); }
    virtual unsigned long Play2DSound(const CCharString&) const { abort(); }
    virtual bool IsSoundPlaying(unsigned long) const { abort(); }
    virtual void EnableSounds(bool) const { abort(); }
    virtual void StopSound(unsigned long) const { abort(); }
    virtual void OverrideMusic(EMusicSetType, bool, bool) const { abort(); }
    virtual void StopOverrideMusic(bool) const { abort(); }
    virtual void CacheMusicSet(EMusicSetType) const { abort(); }
    virtual void EnableDangerMusic(bool) const { abort(); }
    virtual bool IsDangerMusicEnabled() const { abort(); }
    virtual void StartCountdownTimer(float) const { abort(); }
    virtual float GetCountdownTimer() const { abort(); }
    virtual void AutoSaveCheckPoint() const { abort(); }
    virtual void AutoSaveQuestStart() const { abort(); }
    virtual void AutoSave() const { abort(); }
    virtual void SetSavingAsEnabled(bool) const { abort(); }
    virtual bool IsSavingEnabled() const { abort(); }
    virtual void SetSaveGameMarkerPos(const C3DVector&) const { abort(); }
    virtual void ResetToFrontEnd() const { abort(); }
    virtual void SetGuildSealRecallLocation(const C3DVector&, float) const { abort(); }
    virtual C3DVector GetGuildSealRecallPos() const { abort(); }
    virtual float GetGuildSealRecallAngleXY() const { abort(); }
    virtual void SetReadableObjectText(const CScriptThing&, const CWideString&) const { abort(); }
    virtual void SetReadableObjectTextTag(const CScriptThing&, const CCharString&) const { abort(); }
    virtual CWideString GetFormattedString(const CCharString&, const std::vector<CWideString,std::allocator<CWideString> >&) const { abort(); }
    virtual CWideString GetTextString(const CCharString&) const { abort(); }
    virtual void AddRumourCategory(const CCharString&) const { abort(); }
    virtual void AddNewRumourToCategory(const CCharString&, const CCharString&) const { abort(); }
    virtual void RemoveRumourCategory(const CCharString&) const { abort(); }
    virtual void SetCategoryActivity(const CCharString&, bool) const { abort(); }
    virtual void AddGossipVillage(const CCharString&, const CCharString&) const { abort(); }
    virtual void AddGossipFactionToCategory(const CCharString&, const CCharString&) const { abort(); }
    virtual void SetIsGossipForPlayer(const CCharString&, bool) const { abort(); }
    virtual void SetIsGossipForPlayer(CCharString, bool) { abort(); }
    virtual void UpdateOnlineScore_Archery(float) const { abort(); }
    virtual void UpdateOnlineScore_ChickenKick(float) const { abort(); }
    virtual void UpdateOnlineScore_ChapelOrTemple(float) const { abort(); }
    virtual void UpdateOnlineScore_FishingCompo(float) const { abort(); }
    virtual void UpdateScore_FishingCompetition(float) const { abort(); }
    virtual float GetBestTimePairs() const { abort(); }
    virtual float GetBestTimeSorting() const { abort(); }
    virtual long GetBestScoreBlackjack() const { abort(); }
    virtual long GetBestScoreCoinGolfOakVale() const { abort(); }
    virtual long GetBestScoreCoinGolfSnowSpire() const { abort(); }
    virtual long GetBestScoreShoveHaPenny() const { abort(); }
    virtual float GetBestTimeGuessTheAddition() const { abort(); }
    virtual bool IsHeroInTavernGame() const { abort(); }
    virtual long GetNumHousesOwned() const { abort(); }
    virtual void StartSneaking() const { abort(); }
    virtual long GetStealDuration(const CScriptThing&) const { abort(); }
    virtual void SetUseableByHero(const CScriptThing&, bool) const { abort(); }
    virtual void SetOwnedByHero(const CScriptThing&, bool) const { abort(); }
    virtual void SetTavernTableAvailableForUse(const CScriptThing&, bool) const { abort(); }
    virtual void SetIsThingTurncoatable(const CScriptThing&, bool) const { abort(); }
    virtual void SetIsThingForcePushable(const CScriptThing&, bool) const { abort(); }
    virtual void SetIsThingLightningable(const CScriptThing&, bool) const { abort(); }
    virtual void SetIsThingEpicSpellable(const CScriptThing&, bool) const { abort(); }
    virtual bool IsThingTurncoated(const CScriptThing&) const { abort(); }
    virtual void AddCreatureScriptedMode(const CScriptThing&, const CCharString&) const { abort(); }
    virtual void RemoveCreatureScriptedMode(const CScriptThing&) const { abort(); }
    virtual void ForceShipsVisible() const { abort(); }
    virtual bool GetSleepingPositionAndOrientationFromBed(const CScriptThing&, const CScriptThing&, C3DVector&, C3DVector&) { abort(); }
    virtual void SetBedAvailability(const CScriptThing&, bool) { abort(); }
    virtual void RepopulateVillage(const CScriptThing&) const { abort(); }
    virtual void SmashAllWindowsWithinRadiusOfPoint(const C3DVector&, float) { abort(); }
    virtual CScriptThing SetResidency(const CScriptThing&, bool) { abort(); }
    virtual void SetThankingPhrase(CScriptThing, unsigned long) { abort(); }
    virtual unsigned long GetThankingPhrase(CScriptThing) { abort(); }
    virtual void ResetThankingPhrase(CScriptThing) { abort(); }
    virtual void SetIgnoringPhrase(CScriptThing, unsigned long) { abort(); }
    virtual unsigned long GetIgnoringPhrase(CScriptThing) { abort(); }
    virtual void ResetIgnoringPhrase(CScriptThing) { abort(); }
    virtual void SetWanderCentrePoint(CScriptThing, C3DVector) { abort(); }
    virtual C3DVector GetWanderCentrePoint(CScriptThing) { abort(); }
    virtual void ResetWanderCentrePoint(CScriptThing) { abort(); }
    virtual void SetWanderMinDistance(CScriptThing, float) { abort(); }
    virtual float GetWanderMinDistance(CScriptThing) { abort(); }
    virtual void ResetWanderMinDistance(CScriptThing) { abort(); }
    virtual void SetWanderMaxDistance(CScriptThing, float) { abort(); }
    virtual float GetWanderMaxDistance(CScriptThing) { abort(); }
    virtual void ResetWanderMaxDistance(CScriptThing) { abort(); }
    virtual void SetGossipCounter(CScriptThing, long) { abort(); }
    virtual long GetGossipCounter(CScriptThing) { abort(); }
    virtual void ResetGossipCounter(CScriptThing) { abort(); }
    virtual void SetMaxGossipPhrase(CScriptThing, long) { abort(); }
    virtual long GetMaxGossipPhrase(CScriptThing) { abort(); }
    virtual void ResetMaxGossipPhrase(CScriptThing) { abort(); }
    virtual void SetWarningPhrase(CScriptThing, unsigned long) { abort(); }
    virtual unsigned long GetWarningPhrase(CScriptThing) { abort(); }
    virtual void ResetWarningPhrase(CScriptThing) { abort(); }
    virtual void SetBeerRequestPhrase(CScriptThing, unsigned long) { abort(); }
    virtual unsigned long GetBeerRequestPhrase(CScriptThing) { abort(); }
    virtual void ResetBeerRequestPhrase(CScriptThing) { abort(); }
    virtual void SetScriptingStateGroup(CScriptThing, EScriptingStateGroups) { abort(); }
    virtual EScriptingStateGroups GetScriptingStateGroup(CScriptThing) { abort(); }
    virtual void ResetScriptingStateGroup(CScriptThing) { abort(); }
    virtual void SetMaxHeroReactionDistance(CScriptThing, float) { abort(); }
    virtual float GetMaxHeroReactionDistance(CScriptThing) { abort(); }
    virtual void ResetMaxHeroReactionDistance(CScriptThing) { abort(); }
    virtual void SetActionFrequency(CScriptThing, long) { abort(); }
    virtual long GetActionFrequency(CScriptThing) { abort(); }
    virtual void ResetActionFrequency(CScriptThing) { abort(); }
    virtual void SetActionFrequencyVariation(CScriptThing, float) { abort(); }
    virtual float GetActionFrequencyVariation(CScriptThing) { abort(); }
    virtual void ResetActionFrequencyVariation(CScriptThing) { abort(); }
    virtual void SetAction(CScriptThing, CCharString) { abort(); }
    virtual CCharString GetAction(CScriptThing) { abort(); }
    virtual void ResetAction(CScriptThing) { abort(); }
    virtual void SetFaceHeroForAction(CScriptThing, bool) { abort(); }
    virtual bool GetFaceHeroForAction(CScriptThing) { abort(); }
    virtual void ResetFaceHeroForAction(CScriptThing) { abort(); }
    virtual void SetTargetName(CScriptThing, CCharString) { abort(); }
    virtual CCharString GetTargetName(CScriptThing) { abort(); }
    virtual void ResetTargetName(CScriptThing) { abort(); }
    virtual void SetFollowDistance(CScriptThing, float) { abort(); }
    virtual float GetFollowDistance(CScriptThing) { abort(); }
    virtual void ResetFollowDistance(CScriptThing) { abort(); }
    virtual void SetAttackHeroOnSight(CScriptThing, bool) { abort(); }
    virtual bool GetAttackHeroOnSight(CScriptThing) { abort(); }
    virtual void ResetAttackHeroOnSight(CScriptThing) { abort(); }
    virtual void SetTimeToSpendHarassingHero(CScriptThing, long) { abort(); }
    virtual long GetTimeToSpendHarassingHero(CScriptThing) { abort(); }
    virtual void ResetTimeToSpendHarassingHero(CScriptThing) { abort(); }
    virtual void SetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing, float) { abort(); }
    virtual float GetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) { abort(); }
    virtual void ResetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) { abort(); }
    virtual void SetCombatNearbyBreakOffRange(CScriptThing, float) { abort(); }
    virtual float GetCombatNearbyBreakOffRange(CScriptThing) { abort(); }
    virtual void ResetCombatNearbyBreakOffRange(CScriptThing) { abort(); }
    virtual void SetStealStealableItems(CScriptThing, bool) { abort(); }
    virtual bool GetStealStealableItems(CScriptThing) { abort(); }
    virtual void ResetStealStealableItems(CScriptThing) { abort(); }
    virtual void SetRecoverStealableItems(CScriptThing, bool) { abort(); }
    virtual bool GetRecoverStealableItems(CScriptThing) { abort(); }
    virtual void ResetRecoverStealableItems(CScriptThing) { abort(); }
    virtual void SetTakeStealableItemToRandomDestination(CScriptThing, bool) { abort(); }
    virtual bool GetTakeStealableItemToRandomDestination(CScriptThing) { abort(); }
    virtual void ResetTakeStealableItemToRandomDestination(CScriptThing) { abort(); }
    virtual void SetKillSelfAndStealableItemAfterReachingDestination(CScriptThing, bool) { abort(); }
    virtual bool GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) { abort(); }
    virtual void ResetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) { abort(); }
    virtual void SetAllowedToFollow(CScriptThing, bool) { abort(); }
    virtual bool GetAllowedToFollow(CScriptThing) { abort(); }
    virtual void ResetAllowedToFollow(CScriptThing) { abort(); }
    virtual void SetTableName(CScriptThing, CCharString) { abort(); }
    virtual CCharString GetTableName(CScriptThing) { abort(); }
    virtual void ResetTableName(CScriptThing) { abort(); }
    virtual void SetSeatName(CScriptThing, CCharString) { abort(); }
    virtual CCharString GetSeatName(CScriptThing) { abort(); }
    virtual void ResetSeatName(CScriptThing) { abort(); }
    virtual void SetDisableHeadLooking(CScriptThing, bool) { abort(); }
    virtual bool GetDisableHeadLooking(CScriptThing) { abort(); }
    virtual void ResetDisableHeadLooking(CScriptThing) { abort(); }
    virtual void SetIsPushableByHero(CScriptThing, bool) { abort(); }
    virtual bool GetIsPushableByHero(CScriptThing) { abort(); }
    virtual void ResetIsPushableByHero(CScriptThing) { abort(); }
    virtual void SetLookForFiniteTime(CScriptThing, bool) { abort(); }
    virtual bool GetLookForFiniteTime(CScriptThing) { abort(); }
    virtual void ResetLookForFiniteTime(CScriptThing) { abort(); }
    virtual void SetAvoidRegionExits(CScriptThing, bool) { abort(); }
    virtual bool GetAvoidRegionExits(CScriptThing) { abort(); }
    virtual void ResetAvoidRegionExits(CScriptThing) { abort(); }
    virtual void SetTargetingDistanceOffset(CScriptThing, float) { abort(); }
    virtual float GetTargetingDistanceOffset(CScriptThing) { abort(); }
    virtual void ResetTargetingDistanceOffset(CScriptThing) { abort(); }
    virtual void SetPlayerUsingMeleeDummies(bool) { abort(); }
    virtual bool GetPlayerUsingMeleeDummies() { abort(); }
    virtual void SetPlayerUsingRangedDummies(bool) { abort(); }
    virtual bool GetPlayerUsingRangedDummies() { abort(); }
    virtual void SetPlayerUsingWillDummies(bool) { abort(); }
    virtual bool GetPlayerUsingWillDummies() { abort(); }
    virtual void SetCheapHeadLooking(bool) { abort(); }
    virtual bool GetCheapHeadLooking() { abort(); }
    virtual void SetQuitTavernGame(bool) { abort(); }
    virtual bool GetQuitTavernGame() { abort(); }
    virtual void SetPrizeTavernTable(bool) { abort(); }
    virtual bool GetPrizeTavernTable() { abort(); }
    virtual void SetBettingActive(bool) { abort(); }
    virtual bool GetBettingActive() { abort(); }
    virtual void SetBettingAccept(bool) { abort(); }
    virtual bool GetBettingAccept() { abort(); }
    virtual void SetBettingAmount(long) { abort(); }
    virtual long GetBettingAmount() { abort(); }
    virtual void SetCountBetMoneyDown(bool) { abort(); }
    virtual bool GetCountBetMoneyDown() { abort(); }
    virtual void SetSpotTheAdditionBeaten(bool) { abort(); }
    virtual bool GetSpotTheAdditionBeaten() { abort(); }
    virtual void SetGlobalTargetingDistanceOffset(float) { abort(); }
    virtual float GetGlobalTargetingDistanceOffset() { abort(); }
    virtual void SetTradingPriceMult(float) { abort(); }
    virtual float GetTradingPriceMult() { abort(); }
    virtual void SetBoastingEnabled(bool) { abort(); }
    virtual bool GetBoastingEnabled() { abort(); }
    virtual void SetActiveGossipCategories(CCharString, bool) { abort(); }
    virtual bool* GetActiveGossipCategories(CCharString) { abort(); }
    virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetActiveGossipCategories() { abort(); }
    virtual int GetActiveGossipCategoriesSize() { abort(); }
    virtual void ClearActiveGossipCategories() { abort(); }
    virtual bool* GetIsGossipForPlayer(CCharString) { abort(); }
    virtual const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetIsGossipForPlayer() { abort(); }
    virtual int GetIsGossipForPlayerSize() { abort(); }
    virtual void ClearIsGossipForPlayer() { abort(); }
    virtual void SetGossip(CCharString, CCharString, long) { abort(); }
    virtual CCharString GetGossip(CCharString, long) { abort(); }
    virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossip(CCharString) { abort(); }
    virtual int GetGossipSize(CCharString) { abort(); }
    virtual void ClearGossip(CCharString) { abort(); }
    virtual void RemoveGossip(CCharString) { abort(); }
    virtual void AddGossip(CCharString, CCharString) { abort(); }
    virtual void AddGossip(CCharString) { abort(); }
    virtual void SetGossipVillages(CCharString, CCharString, long) { abort(); }
    virtual CCharString GetGossipVillages(CCharString, long) { abort(); }
    virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipVillages(CCharString) { abort(); }
    virtual int GetGossipVillagesSize(CCharString) { abort(); }
    virtual void ClearGossipVillages(CCharString) { abort(); }
    virtual void RemoveGossipVillages(CCharString) { abort(); }
    virtual void AddGossipVillages(CCharString, CCharString) { abort(); }
    virtual void AddGossipVillages(CCharString) { abort(); }
    virtual void SetGossipFactions(CCharString, CCharString, long) { abort(); }
    virtual CCharString GetGossipFactions(CCharString, long) { abort(); }
    virtual const std::vector<CCharString,std::allocator<CCharString> >* GetGossipFactions(CCharString) { abort(); }
    virtual int GetGossipFactionsSize(CCharString) { abort(); }
    virtual void ClearGossipFactions(CCharString) { abort(); }
    virtual void RemoveGossipFactions(CCharString) { abort(); }
    virtual void AddGossipFactions(CCharString, CCharString) { abort(); }
    virtual void AddGossipFactions(CCharString) { abort(); }
};



CCharString::CCharString() : PStringData(0) {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CBaseObject::CBaseObject() {}
CBaseObject::~CBaseObject() {}
CScriptBase::CScriptBase() {}
CScriptBase::~CScriptBase() {}
CEntityScriptBase::~CEntityScriptBase() {}
CGameScriptInterfaceBase::~CGameScriptInterfaceBase() {}
template<class T> CLevelScriptBase<T>::~CLevelScriptBase() {}
CScriptThing::CScriptThing() {}
CScriptThing::CScriptThing(const CScriptThing& source) : CBaseClass(source), PImp(source.PImp) {}

bool NScript::CIsThingAlive::operator()() const { abort(); }
NScript::CPredicateBase* NScript::CIsThingAlive::Clone() const { abort(); }

void CScriptGameResourceObjectScriptedThingBase::ClearAllActions() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::ClearAllActionsIncludingLoopingAnimations() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::ClearCommands() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Converse(const CScriptGameResourceObjectScriptedThingBase&, const char*) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::DropGenericBox() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::FireProjectileWeaponAtTarget(const CScriptThing&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::FollowPreCalculatedRoute(const CScriptThing&, EScriptEntityMoveType, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::FollowThing(const CScriptThing&, float, bool) { abort(); }
bool CScriptGameResourceObjectScriptedThingBase::IsFollowActionRunning(const CScriptThing&) const { abort(); }
bool CScriptGameResourceObjectScriptedThingBase::IsFollowingThing() const { abort(); }
void CScriptGameResourceObjectScriptedThingBase::MoveToAndPickUpGenericBox(CScriptThing&, EScriptEntityMoveType, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::MoveToPosition(const C3DVector&, float, EScriptEntityMoveType, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::MoveToThing(const CScriptThing&, float, EScriptEntityMoveType, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PerformExpression(const CScriptThing&, const CCharString&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PlayAnimation(const CCharString&, bool, bool, bool, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PlayCombatAnimation(const CCharString&, bool, bool, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PlayLoopingAnimation(const CCharString&, long, bool, bool, bool, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&, unsigned long, ETextGroupSelectionMethod, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::StopFollowingThing(const CScriptThing&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::SummonerLightningOrbAttackTarget(const CScriptThing&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::UnsheatheWeapons() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Validate() const { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Wait(float) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::WaitForEntityToFinishPerformingTasks(const CScriptGameResourceObjectScriptedThingBase&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::WaitWhilePerformingTasks() { abort(); }
void CScriptThing::DecrementScriptCounter() { abort(); }
bool CScriptThing::GetActivationTriggerStatus() const { abort(); }
float CScriptThing::GetAngleXY() const { abort(); }
CCharString CScriptThing::GetCurrentMapName() const { abort(); }
EScriptAIPriority CScriptThing::GetCurrentScriptPriority() const { abort(); }
EAIStateGroupType CScriptThing::GetCurrentStateGroupType() const { abort(); }
CCharString CScriptThing::GetDataString() const { abort(); }
CCharString CScriptThing::GetDefName() const { abort(); }
C3DVector CScriptThing::GetFocalPos() const { abort(); }
CCharString CScriptThing::GetHomeMapName() const { abort(); }
C3DVector CScriptThing::GetHomePos() const { abort(); }
const CCharString& CScriptThing::GetName() const { abort(); }
CThing* CScriptThing::GetPThing() const { abort(); }
FableNScriptUInt64 CScriptThing::GetPThingUniqueID() const { abort(); }
const C3DVector& CScriptThing::GetPos() const { abort(); }
long CScriptThing::GetScriptCounter() { abort(); }
void CScriptThing::IncrementScriptCounter() { abort(); }
bool CScriptThing::IsAlive() const { abort(); }
bool CScriptThing::IsAwareOfHero() const { abort(); }
bool CScriptThing::IsBeingCarriedBy(const CCharString&) const { abort(); }
bool CScriptThing::IsDead() const { abort(); }
bool CScriptThing::IsNull() const { abort(); }
bool CScriptThing::IsOpenDoor() const { abort(); }
bool CScriptThing::IsSneaking() const { abort(); }
bool CScriptThing::IsSummonedCreature() const { abort(); }
bool CScriptThing::IsUnconscious() const { abort(); }
bool CScriptThing::IsUsable() const { abort(); }
bool CScriptThing::MsgAttackedWithBareHandsWithoutHittingAnything() const { abort(); }
bool CScriptThing::MsgAttackedWithMeleeWeaponWithoutHittingAnything() const { abort(); }
bool CScriptThing::MsgExpressionPerformedTo(CCharString&) const { abort(); }
bool CScriptThing::MsgGetThingsKilled(std::vector<unsigned long,std::allocator<unsigned long> >&) const { abort(); }
bool CScriptThing::MsgHitEnemyWithBareHands() const { abort(); }
bool CScriptThing::MsgHitEnemyWithMeleeWeapon() const { abort(); }
bool CScriptThing::MsgHitEnemyWithRangedWeapon() const { abort(); }
bool CScriptThing::MsgHitFriendWithBareHands() const { abort(); }
bool CScriptThing::MsgHitFriendWithMeleeWeapon() const { abort(); }
bool CScriptThing::MsgHitFriendWithRangedWeapon() const { abort(); }
long CScriptThing::MsgHowLongWasExpressionPerformed() const { abort(); }
bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByAnyNonAggressiveSpecialAbilityFrom(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithDecapitate(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithFlourish(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithProjectileWeapon(const CCharString&, float&) const { abort(); }
bool CScriptThing::MsgIsHitByWithWeapon(const CCharString&, const CCharString&) const { abort(); }
bool CScriptThing::MsgIsKicked() const { abort(); }
bool CScriptThing::MsgIsKilledBy(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsKnockedOutBy(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsPresentedWithItem(CCharString&) const { abort(); }
bool CScriptThing::MsgIsTriggeredBy(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsUsedBy(const CCharString&) const { abort(); }
bool CScriptThing::MsgOnCutSceneAnimEvent(CCharString&) const { abort(); }
bool CScriptThing::MsgOnMoralityChanged(long&) const { abort(); }
bool CScriptThing::MsgOpenedChest() const { abort(); }
bool CScriptThing::MsgPerformedAnyAggressiveSpecialAbility() const { abort(); }
bool CScriptThing::MsgPerformedAnyNonAggressiveSpecialAbility() const { abort(); }
bool CScriptThing::MsgPerformedAnySpecialAbility() const { abort(); }
bool CScriptThing::MsgPerformedFlourish() const { abort(); }
bool CScriptThing::MsgPerformedSpecialAbility(EHeroAbility) const { abort(); }
bool CScriptThing::MsgPerformedSuccessfulFlourish() const { abort(); }
bool CScriptThing::MsgReceivedInventoryItem() const { abort(); }
bool CScriptThing::MsgReceivedMoney(long&) const { abort(); }
bool CScriptThing::MsgTalkedToAnyone() const { abort(); }
CScriptThing CScriptThing::MsgWhoExpressedToMe() const { abort(); }
CScriptThing CScriptThing::MsgWhoHitMe() const { abort(); }
CScriptThing CScriptThing::MsgWhoKilledMe() const { abort(); }
CScriptThing CScriptThing::MsgWhoKnockedMeOut() const { abort(); }
CScriptThing CScriptThing::MsgWhoTalkedToMe() const { abort(); }
void CScriptThing::SetActivationTriggerStatus(bool) { abort(); }
void CScriptThing::SetAsUsable(bool) { abort(); }
void CScriptThing::SetDataString(const CCharString&) { abort(); }
void CScriptThing::SetFriendsWithEverythingFlag(bool) { abort(); }
void CScriptThing::SetToKillOnLevelUnload(bool) { abort(); }
void CScriptThing::UpdateThingAttachment() { abort(); }
void NScript::CQ_BanditCampBossBattleScript::CAssassin3::Init() { abort(); }
void CEntityScriptBase::Init() { abort(); }
void CEntityScriptBase::OnInterrupted() { abort(); }
void CEntityScriptBase::OnPersist(CPersistContext&) { abort(); }
void CEntityScriptBase::OnPredicateFail() { abort(); }
void NScript::CQ_BanditCampBossBattleScript::Init() { abort(); }
void NScript::CQ_BanditCampBossBattleScript::Main() { abort(); }
void NScript::CQ_BanditCampBossBattleScript::RegisterMain() { abort(); }
void NScript::CQ_SunnyvaleMasterData::Init() { abort(); }
void NScript::CQ_SunnyvaleMasterData::OnPersist(CPersistContext&) { abort(); }
void CScriptBase::Init() { abort(); }
void CScriptBase::Main() { abort(); }
void CScriptBase::OnPersist(CPersistContext&) { abort(); }
void CScriptBase::RegisterMain() { abort(); }
void CBaseObject::Validate() const { abort(); }
CBaseClass::CBaseClass() {}
bool CScriptThing::IsEqualTo(const CScriptThing&) const { abort(); }
void CScriptGameResourceObjectScriptedThingBase::AskHeroQuestion(const CScriptThing&, const char*, const CCharString&, const CCharString&, const CCharString&, bool, bool) { abort(); }

static unsigned int scenarioCount;
static int run(int mode,int stop,bool alive,bool valid,int failures,int taskFrames,bool alreadyFinished)
{
    CScriptGameResourceObjectMovieBase retained; retainedMovie=&retained; destroyedMovies=0;
    CScriptGameResourceObjectScriptedThingBase otherActor;
    retainedActor=&otherActor;
    constructedActors=0; destroyedActors=0;
    ++scenarioCount;
    memset(&state,0,sizeof(state)); state.mode=mode; state.stopAt=stop; state.alive=alive;
    state.valid=valid; state.startFailures=failures; state.taskFrames=taskFrames; state.cutscene=alreadyFinished;
    FixtureGame game; NScript::CQ_SunnyvaleMasterData data; CScriptThing me; fixtureHero=&me;
    Quest quest(&data,game); activeQuest=&quest; quest.AssassinsUnderAttack=false; quest.AssassinCutsceneTriggered=alreadyFinished;
    Quest::CAssassin3 actor(quest,&data,me);
    CEntityScriptBase& entry=actor;entry.Main();
    const int created=(actorOwnership && state.starts>state.startFailures) ? 1 : 0;
    check(constructedActors==created);
    check(destroyedActors==(actorOwnership==2 ? 0 : created));
    otherActor.SetImplementation(0);
    check(destroyedActors==created);
    retainedActor=0;
    const int made=movieOwnership ? state.movieCount : 0;
    check(destroyedMovies==made-((movieOwnership==2 && made>0) ? 1 : 0));
    retained.SetImplementation(0);
    check(destroyedMovies==made);
    retainedMovie=0;
    state.attacking=quest.AssassinsUnderAttack;state.cutscene=quest.AssassinCutsceneTriggered;check(liveStrings==0);
    check(state.predicates==1 && state.pauses==state.resumes && !state.paused);
    if (stop) { check(state.probe==stop && state.clears==0); }
    else {
        check(state.clears==1 && state.starts==failures+1);
        check(state.attacking==(!alreadyFinished && (mode==1 || mode==2)));
        check(state.releases==int(valid)+int(state.cutscene));
        check(state.speaks==((mode==4 && alive && !alreadyFinished)?3:0));
        check(state.movieCount==((mode==4 && !alreadyFinished)?3:0));
    }
    return state.probe;
}
int main()
{
    for(actorOwnership=0;actorOwnership<3;++actorOwnership)
    for(movieOwnership=0;movieOwnership<3;++movieOwnership)
    for (int mode=0;mode<5;++mode) for(int alive=0;alive<2;++alive)
        for(int valid=0;valid<2;++valid) for(int failures=0;failures<2;++failures)
        for(int task=0;task<2;++task) for(int ended=0;ended<2;++ended) {
            int count=run(mode,0,alive!=0,valid!=0,failures,task,ended!=0);
            for(int stop=1;stop<=count;++stop) run(mode,stop,alive!=0,valid!=0,failures,task,ended!=0);
        }
    check(scenarioCount==10404);
    puts("00d06130_TEST PASS"); return 0;
}
