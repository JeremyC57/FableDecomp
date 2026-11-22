// Twelve original-script scenarios plus the real predicate Clone path.
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define private public
// 00cfce90 NScript::CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets::Main
// Leaf: ?Main@CScytheBeforeGettingTablets@CQ_AwakeningTheOracleScript@NScript@@EAEXXZ
// Flags:
// Inline-budget: the native predicate keeps its CScriptThing copy out of line.
// Native weak destructor selection is documented in LINK_REVIEW.json.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_CLEVELSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_SCYTHE_BEFORE_TABLETS_PASS27
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectScriptedThingBase;
template<> FABLE_FORCEINLINE CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::~CCountedPointer();
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CQ_AwakeningTheOracleScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EHeroAbility.h"
#include "engine/TRetVoid.h"
#include "engine/fable_script_text.h"

inline CScriptThing::~CScriptThing() {}
#undef private
namespace NScript {
#ifdef _MSC_VER
#pragma inline_depth(2)
#endif
inline CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline CIsThingAlive::~CIsThingAlive() {}

CCountedPointer<CEntityScriptBase> FABLE_FASTCALL CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets::Alloc(
        CQ_AwakeningTheOracleScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CScytheBeforeGettingTablets(parent, data, me));
}

inline CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets::CScytheBeforeGettingTablets(
        CQ_AwakeningTheOracleScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(parent.Game, me), ParentClass(&parent)
{
    PMasterData = (CQ_SunnyvaleMasterData*)data;
}
CScriptBase* CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets::GetParentScript() const
{
    return ParentClass;
}
inline void CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets::Main()
{
    Game.EntitySetAsKillable(Me, false, false);
    Game.EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game.SetIsPushableByHero(Me, false);
    Game.EntityAttachToScript(Me, Game.GetActiveQuestName());
    SetPredicate(CIsThingAlive(Me));
    Game.NewScriptFrame();
    TRetVoid _Fail_Val;
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.SetThingPersistent(Me, true);
    while (!ParentClass->PlayedOpeningCS) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.SetThingHasInformation(Me, false, false, false);
    Game.EntitySetFacingAngleTowardsThing(Me, Game.GetThingWithScriptName("MK_OW_SCYTHE3"), false);
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, (EScriptAIPriority)4)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    long NextLine = 10;
    while (ParentClass->TabletsCollected < 4) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (!Game.TextEntryExists("TEXT_QST_B03_SCYTHE_ON_SPEAK_" + GFIntToCharString(NextLine))) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                NextLine = 10;
            }
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(), "TEXT_QST_B03_SCYTHE_ON_SPEAK_" + GFIntToCharString(NextLine), GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    do { if (IsActiveThreadTerminating()) return _Fail_Val.Return(); } while (false);
                }
                if (IsActiveThreadTerminating()) {
                    return _Fail_Val.Return();
                }
            }
            NextLine += 10;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CScriptGameResourceObjectMovieBase speech_movie;
            Game.StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(Game);
            if (Game.GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game.GetHero(), "TEXT_QST_B03_SCYTHE_ON_HIT_NEW", GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game.NewScriptFrame();
                    do { if (IsActiveThreadTerminating()) return _Fail_Val.Return(); } while (false);
                }
                if (IsActiveThreadTerminating()) {
                    return _Fail_Val.Return();
                }
            }
            Game.EntitySetThingAsAllyOfThing(Me, Game.GetHero());
            Game.EntitySetThingAsAllyOfThing(Game.GetHero(), Me);
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game.RemoveThing(Me, false, true);
}
}

// Inline-budget: preserve the out-of-line CScriptThing copy in the native factory.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline CEntityScriptBase::CEntityScriptBase(CGameScriptInterfaceBase& game, const CScriptThing& me)
    : Game(game), Me(me)
{}
#ifdef _MSC_VER
#pragma inline_depth()
#endif

// Inline-budget: expand the original counted release inside the native scripted-resource scalar.
template<> FABLE_FORCEINLINE CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::~CCountedPointer()
{
    if (Info) {
        --Info->RefCount;
        if (Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            delete Info;
        }
    }
    Data=0;
    Info=0;
}

inline bool NScript::CIsThingAlive::operator()() const { return Thing.IsAlive(); }
inline NScript::CPredicateBase* NScript::CIsThingAlive::Clone() const
{
    return new CIsThingAlive(*this);
}

#include "engine/CQ_SunnyvaleMasterData.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CRGBColour.h"
#include "engine/C3DVector.h"
#include "engine/EScriptAIPriority.h"

using NScript::CQ_AwakeningTheOracleScript;

struct Conditions {
    int frame, finish, open, script_ready, stop;
    int setup, scripts, invalidated, looked_up, attached, info_set, predicates;
    int talks_left, queries, missing_after, text_id[8], speaks, spoken_id[8];
    int movies, pauses, resumes, tasks_left, waits, health_checks, allies, removes;
    float health;
    bool hit, any_spell, healing, terminate_on_speech, started_paused, initial_script_valid;
};
static Conditions state;
static CScriptThing hero, script_actor;
static CQ_AwakeningTheOracleScript* quest;
static char number_output[16];
static char text_output[68];

static void FailUnless(bool ok, int line)
{
    if (!ok) {
        printf("check failed on line %d\n", line);
        fflush(stdout);
        abort();
    }
}
#define Check(ok) FailUnless((ok), __LINE__)

CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CBaseObject::CBaseObject() : IntelligentPointer(0) { }
CBaseObject::~CBaseObject() { }
void CBaseObject::Validate() const { }
CGameScriptInterfaceBase::~CGameScriptInterfaceBase() { }

CCharString::CCharString() : PStringData(0) { }
CCharString::CCharString(const char* text, long length) : PStringData((CCharStringData*)text)
{
    Check(length == -1);
}
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { }
CCharString::~CCharString() { }
CCharString::operator const char*() const { return (const char*)PStringData; }

CCharString FABLE_FASTCALL GFIntToCharString(long value)
{
    sprintf(number_output, "%ld", value);
    return CCharString(number_output);
}
CCharString FABLE_FASTCALL operator+(const char* prefix, const CCharString& number)
{
    Check(strlen(prefix)+strlen(number) < sizeof(text_output));
    strcpy(text_output, prefix);
    strcat(text_output, number);
    return CCharString(text_output);
}
static int SpeechLine(const char* text)
{
    const char* prefix = "TEXT_QST_B03_SCYTHE_ON_SPEAK_";
    Check(strncmp(text,prefix,strlen(prefix)) == 0);
    return atoi(text + strlen(prefix));
}

CScriptBase::CScriptBase() { }
CScriptBase::~CScriptBase() { }
void NScript::CQ_SunnyvaleMasterData::OnPersist(CPersistContext&) { abort(); }
void NScript::CQ_SunnyvaleMasterData::Init() { abort(); }
void CScriptBase::RegisterMain() { }
void CScriptBase::Main() { }
void CScriptBase::Init() { }
void CScriptBase::OnPersist(CPersistContext&) { }

CQ_AwakeningTheOracleScript::CQ_AwakeningTheOracleScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game), PlayedOpeningCS(false), TabletsCollected(0) { }
void CQ_AwakeningTheOracleScript::RegisterMain() { }
void CQ_AwakeningTheOracleScript::Main() { }
void CQ_AwakeningTheOracleScript::Init() { }

CEntityScriptBase::~CEntityScriptBase() { }
void CEntityScriptBase::Init() { }
void CEntityScriptBase::OnPersist(CPersistContext&) { }
void CEntityScriptBase::OnPredicateFail() { }
void CEntityScriptBase::OnInterrupted() { }
bool CEntityScriptBase::IsActiveThreadTerminating() const
{
    return state.frame >= state.stop || (state.terminate_on_speech && state.speaks != 0 && state.tasks_left == 0);
}
void CEntityScriptBase::SetPredicate(const NScript::CPredicateBase& pred)
{
    Check(pred());
    NScript::CPredicateBase* copy = pred.Clone();
    Check(copy && (*copy)());
    delete copy;
    ++state.predicates;
}
CScriptThing::CScriptThing() { }
bool CScriptThing::MsgIsTalkedToBy(const CCharString& who) const
{
    Check(strcmp(who, "SCRIPT_NAME_HERO") == 0);
    if (!state.talks_left)
        return false;
    --state.talks_left;
    return true;
}
bool CScriptThing::MsgIsHitBy(const CCharString& who) const
{
    Check(strcmp(who, "SCRIPT_NAME_HERO") == 0);
    return state.hit;
}
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString& who) const
{
    Check(strcmp(who, "SCRIPT_NAME_HERO") == 0);
    return state.any_spell;
}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility spell, const CCharString& who) const
{
    Check(spell == HERO_ABILITY_HEAL_LIFE_SPELL && strcmp(who, "SCRIPT_NAME_HERO") == 0);
    return state.healing;
}

bool CScriptGameResourceObjectScriptedThingBase::IsValid() const { return state.initial_script_valid; }
void CScriptGameResourceObjectScriptedThingBase::Release()
{
    Check(state.initial_script_valid);
    state.initial_script_valid=false;
    ++state.invalidated;
}
CScriptThing CScriptGameResourceObjectScriptedThingBase::GetScriptThing() const { return script_actor; }
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask() const
{
    ++state.waits;
    if (state.tasks_left == 0)
        return false;
    --state.tasks_left;
    return true;
}
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing& listener, const char* text,
    ETextGroupSelectionMethod group, bool a, bool b, bool c)
{
    Check(&listener == &hero && !a && b && !c && state.started_paused);
    Check(state.speaks < int(sizeof(state.spoken_id)/sizeof(state.spoken_id[0])));
    if (group == GROUP_SELECT_RANDOM_NO_REPEAT) {
        Check(strcmp(text, "TEXT_QST_B03_SCYTHE_ON_HIT_NEW") == 0);
        state.spoken_id[state.speaks++] = 0;
    } else {
        Check(group == GROUP_SELECT_NONE);
        state.spoken_id[state.speaks++] = SpeechLine(text);
    }
}

void CScriptGameResourceObjectScriptedThingBase::AskHeroQuestion(const CScriptThing&, const char*, const CCharString&,
    const CCharString&, const CCharString&, bool, bool) { abort(); }
const CCharString& CScriptThing::GetName() const { abort(); }
CCharString CScriptThing::GetDefName() const { abort(); }
CCharString CScriptThing::GetDataString() const { abort(); }
void CScriptThing::SetDataString(const CCharString&) { abort(); }
C3DVector CScriptThing::GetFocalPos() const { abort(); }
const C3DVector& CScriptThing::GetPos() const { abort(); }
C3DVector CScriptThing::GetHomePos() const { abort(); }
CCharString CScriptThing::GetCurrentMapName() const { abort(); }
CCharString CScriptThing::GetHomeMapName() const { abort(); }
float CScriptThing::GetAngleXY() const { abort(); }
CThing* CScriptThing::GetPThing() const { abort(); }
FableNScriptUInt64 CScriptThing::GetPThingUniqueID() const { abort(); }
EAIStateGroupType CScriptThing::GetCurrentStateGroupType() const { abort(); }
EScriptAIPriority CScriptThing::GetCurrentScriptPriority() const { abort(); }
bool CScriptThing::IsSneaking() const { abort(); }
bool CScriptThing::IsAwareOfHero() const { abort(); }
bool CScriptThing::MsgIsTriggeredBy(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsKilledBy(const CCharString&) const { abort(); }
CScriptThing CScriptThing::MsgWhoKilledMe() const { abort(); }
bool CScriptThing::MsgIsUsedBy(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithFlourish(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithDecapitate(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithWeapon(const CCharString&, const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithProjectileWeapon(const CCharString&, float&) const { abort(); }
CScriptThing CScriptThing::MsgWhoHitMe() const { abort(); }
CScriptThing CScriptThing::MsgWhoTalkedToMe() const { abort(); }
bool CScriptThing::MsgExpressionPerformedTo(CCharString&) const { abort(); }
CScriptThing CScriptThing::MsgWhoExpressedToMe() const { abort(); }
long CScriptThing::MsgHowLongWasExpressionPerformed() const { abort(); }
bool CScriptThing::MsgIsKnockedOutBy(const CCharString&) const { abort(); }
CScriptThing CScriptThing::MsgWhoKnockedMeOut() const { abort(); }
bool CScriptThing::MsgReceivedMoney(long&) const { abort(); }
bool CScriptThing::MsgIsPresentedWithItem(CCharString&) const { abort(); }
bool CScriptThing::MsgReceivedInventoryItem() const { abort(); }
bool CScriptThing::MsgPerformedSpecialAbility(EHeroAbility) const { abort(); }
bool CScriptThing::MsgPerformedAnySpecialAbility() const { abort(); }
bool CScriptThing::MsgPerformedAnyAggressiveSpecialAbility() const { abort(); }
bool CScriptThing::MsgPerformedAnyNonAggressiveSpecialAbility() const { abort(); }
bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByAnyNonAggressiveSpecialAbilityFrom(const CCharString&) const { abort(); }
bool CScriptThing::MsgOpenedChest() const { abort(); }
bool CScriptThing::MsgHitEnemyWithMeleeWeapon() const { abort(); }
bool CScriptThing::MsgHitFriendWithMeleeWeapon() const { abort(); }
bool CScriptThing::MsgHitEnemyWithRangedWeapon() const { abort(); }
bool CScriptThing::MsgHitFriendWithRangedWeapon() const { abort(); }
bool CScriptThing::MsgHitEnemyWithBareHands() const { abort(); }
bool CScriptThing::MsgHitFriendWithBareHands() const { abort(); }
bool CScriptThing::MsgAttackedWithMeleeWeaponWithoutHittingAnything() const { abort(); }
bool CScriptThing::MsgAttackedWithBareHandsWithoutHittingAnything() const { abort(); }
bool CScriptThing::MsgTalkedToAnyone() const { abort(); }
bool CScriptThing::MsgGetThingsKilled(std::vector<unsigned long,std::allocator<unsigned long> >&) const { abort(); }
bool CScriptThing::MsgPerformedFlourish() const { abort(); }
bool CScriptThing::MsgPerformedSuccessfulFlourish() const { abort(); }
bool CScriptThing::MsgOnMoralityChanged(long&) const { abort(); }
bool CScriptThing::MsgOnCutSceneAnimEvent(CCharString&) const { abort(); }
bool CScriptThing::MsgIsKicked() const { abort(); }
bool CScriptThing::IsUnconscious() const { abort(); }
bool CScriptThing::IsUsable() const { abort(); }
bool CScriptThing::IsBeingCarriedBy(const CCharString&) const { abort(); }
bool CScriptThing::IsOpenDoor() const { abort(); }
bool CScriptThing::IsSummonedCreature() const { abort(); }
void CScriptThing::SetAsUsable(bool) { abort(); }
void CScriptThing::SetFriendsWithEverythingFlag(bool) { abort(); }
bool CScriptThing::GetActivationTriggerStatus() const { abort(); }
void CScriptThing::SetActivationTriggerStatus(bool) { abort(); }
void CScriptThing::SetToKillOnLevelUnload(bool) { abort(); }
void CScriptThing::UpdateThingAttachment() { abort(); }
void CScriptThing::IncrementScriptCounter() { abort(); }
void CScriptThing::DecrementScriptCounter() { abort(); }
long CScriptThing::GetScriptCounter() { abort(); }
bool CScriptThing::IsAlive() const { return true; }
bool CScriptThing::IsDead() const { abort(); }
bool CScriptThing::IsNull() const { abort(); }
bool CScriptThing::IsEqualTo(const CScriptThing&) const { abort(); }
void CScriptGameResourceObjectScriptedThingBase::SummonerLightningOrbAttackTarget(const CScriptThing&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::FireProjectileWeaponAtTarget(const CScriptThing&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::MoveToPosition(const C3DVector&, float, EScriptEntityMoveType, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::MoveToThing(const CScriptThing&, float, EScriptEntityMoveType, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::FollowPreCalculatedRoute(const CScriptThing&, EScriptEntityMoveType, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::FollowThing(const CScriptThing&, float, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::StopFollowingThing(const CScriptThing&) { abort(); }
bool CScriptGameResourceObjectScriptedThingBase::IsFollowActionRunning(const CScriptThing&) const { abort(); }
void CScriptGameResourceObjectScriptedThingBase::ClearCommands() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::WaitWhilePerformingTasks() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&, unsigned long, ETextGroupSelectionMethod, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Converse(const CScriptGameResourceObjectScriptedThingBase&, const char*) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PerformExpression(const CScriptThing&, const CCharString&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PlayAnimation(const CCharString&, bool, bool, bool, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PlayCombatAnimation(const CCharString&, bool, bool, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PlayLoopingAnimation(const CCharString&, long, bool, bool, bool, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::ClearAllActions() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::ClearAllActionsIncludingLoopingAnimations() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::MoveToAndPickUpGenericBox(CScriptThing&, EScriptEntityMoveType, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::DropGenericBox() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::UnsheatheWeapons() { abort(); }
bool CScriptGameResourceObjectScriptedThingBase::IsFollowingThing() const { abort(); }
void CScriptGameResourceObjectScriptedThingBase::WaitForEntityToFinishPerformingTasks(const CScriptGameResourceObjectScriptedThingBase&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Wait(float) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Validate() const { abort(); }
class TestGame : public CGameScriptInterfaceBase {
public:
    void NewScriptFrame() const
    {
        ++state.frame;
        quest->PlayedOpeningCS = state.frame >= state.open;
        if (state.frame >= state.finish)
            quest->TabletsCollected = 4;
    }
    void EntitySetAsKillable(const CScriptThing&, bool killable, bool affects_object) const
    {
        Check(!killable && !affects_object && state.setup == 0);
        ++state.setup;
    }
    void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool enabled) const
    {
        Check(!enabled && state.setup == 1);
        ++state.setup;
    }
    void SetIsPushableByHero(CScriptThing, bool enabled)
    {
        Check(!enabled && state.setup == 2);
        ++state.setup;
    }
    CCharString GetActiveQuestName() const { return CCharString("TestQuest"); }
    void EntityAttachToScript(const CScriptThing&, const CCharString& name) const
    {
        Check(strcmp(name, "TestQuest") == 0 && state.setup == 3);
        ++state.attached;
    }
    void SetThingPersistent(const CScriptThing&, bool value) const
    {
        Check(value && state.attached == 1 && state.predicates == 1);
        ++state.setup;
    }
    void SetThingHasInformation(const CScriptThing&, bool a, bool b, bool c) const
    {
        Check(quest->PlayedOpeningCS && !a && !b && !c);
        ++state.info_set;
    }
    CScriptThing GetThingWithScriptName(const CCharString& name) const
    {
        Check(strcmp(name, "MK_OW_SCYTHE3") == 0 && state.info_set == 1);
        ++state.looked_up;
        return CScriptThing();
    }
    void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool value) const
    {
        Check(!value && state.looked_up == 1);
    }
    bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&,
        EScriptAIPriority priority)
    {
        Check((int)priority == 4 && state.looked_up == 1 && !state.initial_script_valid);
        ++state.scripts;
        return state.frame >= state.script_ready;
    }
    bool TextEntryExists(const CCharString& tag) const
    {
        Check(state.queries < int(sizeof(state.text_id)/sizeof(state.text_id[0])));
        state.text_id[state.queries++] = SpeechLine(tag);
        return state.queries < state.missing_after;
    }
    void StartMovieSequence(const CCharString& tag, CScriptGameResourceObjectMovieBase&)
    {
        Check(strcmp(tag,"") == 0 && !state.started_paused);
        ++state.movies;
    }
    void PauseAllNonScriptedEntities(bool value)
    {
        Check(value != state.started_paused);
        state.started_paused = value;
        if (value)
            ++state.pauses;
        else
            ++state.resumes;
    }
    float GetHealth(const CScriptThing&) const
    {
        Check(state.started_paused && state.movies > 0);
        ++state.health_checks;
        return state.health;
    }
    CScriptThing& GetHero() { return hero; }
    void EntitySetThingAsAllyOfThing(const CScriptThing& first, const CScriptThing& second) const
    {
        Check(state.started_paused && (&first == &hero) != (&second == &hero));
        ++state.allies;
        Check((&first == &hero) == (state.allies == 2));
    }
    void RemoveThing(const CScriptThing&, bool a, bool b) const
    {
        Check(!a && b && !state.started_paused && quest->TabletsCollected == 4);
        ++state.removes;
    }
private:
    void Error(const CCharString&, const CCharString&, const CCharString&, unsigned long) const { abort(); }
    void TraceMessage(const CCharString&) const { abort(); }
    void Validate() const { abort(); }
    void SetDebugCameraType(long) const { abort(); }
    void DeactivateBoastUi() const { abort(); }
    bool IsXbox() const { abort(); }
    bool IsEntityUnderScriptedControl(const CScriptThing&) const { abort(); }
    bool IsActiveThreadTerminating() const { abort(); }
    bool IsLevelLoaded(const CCharString&) const { abort(); }
    bool IsRegionLoaded(const CCharString&) const { abort(); }
    bool IsRegionLoadedAndPreloaded(const CCharString&) const { abort(); }
    bool IsRegionDefLoaded(const CCharString&) const { abort(); }
    const CCharString& GetRegionName() const { abort(); }
    bool MsgIsLevelLoaded(const CCharString&) const { abort(); }
    bool MsgIsLevelUnloaded(const CCharString&) const { abort(); }
    bool MsgOnLevelLoaded(std::list<CCharString,std::allocator<CCharString> >&) const { abort(); }
    bool MsgOnLevelUnloaded(std::list<CCharString,std::allocator<CCharString> >&) const { abort(); }
    bool MsgIsRegionLoaded(const CCharString&) const { abort(); }
    bool MsgIsRegionUnloaded(const CCharString&) const { abort(); }
    bool MsgOnRegionLoaded(CCharString&) const { abort(); }
    bool MsgOnRegionUnloaded(CCharString&) const { abort(); }
    bool MsgOnRegionPreunload(CCharString&) const { abort(); }
    bool MsgOnQuestCompleted(const CCharString&) const { abort(); }
    bool MsgOnAnyQuestCompleted(CCharString&) const { abort(); }
    bool MsgOnQuestFailed(const CCharString&) const { abort(); }
    bool MsgOnQuestCompletedBeforeScreenShown(const CCharString&) const { abort(); }
    bool MsgOnQuestFailedBeforeScreenShown(const CCharString&) const { abort(); }
    bool MsgOnQuestAccepted(CCharString&) const { abort(); }
    bool MsgOnFeatAccepted(long&) const { abort(); }
    bool MsgIsBoastMade(long) const { abort(); }
    bool MsgOnBoastMade(long&, CCharString&) const { abort(); }
    bool MsgOnBoastsMade(std::vector<std::pair<long,CCharString>,std::allocator<std::pair<long,CCharString> > >&) const { abort(); }
    void RemoveBoastMessage() const { abort(); }
    bool IsQuestStartScreenActive(CCharString&) const { abort(); }
    bool MsgOnLeavingQuestStartScreen(CCharString&) const { abort(); }
    bool MsgOnLeavingExperienceSpendingScreen() const { abort(); }
    long MsgIsQuestionAnsweredYesOrNo() const { abort(); }
    bool MsgIsGameInfoClickedPast() const { abort(); }
    bool MsgIsTutorialClickedPast() const { abort(); }
    bool MsgIsActionModeButtonPressed() const { abort(); }
    bool MsgOnExpressionPerformed(CCharString&) const { abort(); }
    bool MsgIsCutSceneSkipped() const { abort(); }
    void RemoveAllCutSceneSkippedMessages() const { abort(); }
    bool MsgOnHeroHairTypeChanged(EClothingCoversArea, CCharString&) const { abort(); }
    bool MsgOnHeroUsedTeleporter(CCharString&) const { abort(); }
    bool MsgOnHeroUsedGuildSeal() const { abort(); }
    bool MsgOnGameSavedManually() const { abort(); }
    bool MsgOnHeroSlept(bool&) const { abort(); }
    bool MsgOnHeroFiredRangedWeapon() const { abort(); }
    bool MsgOnHeroCastSpell(EHeroAbility&) const { abort(); }
    bool MsgOnHeroPickedPocket(CScriptThing&) const { abort(); }
    bool MsgOnHeroPickedLock(CScriptThing&) const { abort(); }
    bool MsgOnFishingGameFinished(CScriptThing&) const { abort(); }
    bool MsgOnTavernGameFinished(const CScriptThing&) const { abort(); }
    bool MsgOnHeroRewardedWithItemsFrom(CCharString&) const { abort(); }
    bool MsgOnChestOpeningCancelled() const { abort(); }
    void DontPopulateNextLoadedRegion() const { abort(); }
    void GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString&) const { abort(); }
    void GetWanderingPopulationScriptDefNameInRegion(const CCharString&, CCharString&) const { abort(); }
    bool IsHeroAllowedHenchmenInCurrentRegion() const { abort(); }
    bool IsHeroAllowedHenchmenInRegion(const CCharString&) const { abort(); }
    void PostAddScriptedEntities() const { abort(); }
    bool IsPlayerHoldingUnsheatheRangedWeaponButton() const { abort(); }
    bool IsPlayerHoldingLockTargetButton() const { abort(); }
    bool IsPlayerHoldingFireRangedWeaponButton() const { abort(); }
    bool IsPlayerHoldingFirstPersonTargetingButton() const { abort(); }
    bool IsHeroInProjectileWeaponMode() const { abort(); }
    CScriptThing GetHeroTargetedThing() const { abort(); }
    CScriptThing GetThingWithScriptName(const CCharString&, const CCharString&) const { abort(); }
    CScriptThing GetRandomThingWithScriptName(const CCharString&) const { abort(); }
    long GetAllThingsWithScriptName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    long GetAllCreaturesInAreaWithScriptName(const CCharString&, const C3DVector&, float, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    CScriptThing GetNearestWithScriptName(const CScriptThing&, const CCharString&) const { abort(); }
    CScriptThing GetFurthestWithScriptName(const CScriptThing&, const CCharString&) const { abort(); }
    long GetAllThingsWithDefNameByDistanceFrom(const CScriptThing&, const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    long GetAllThingsWithDefName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString&) const { abort(); }
    CScriptThing GetFurthestWithDefName(const CScriptThing&, const CCharString&) const { abort(); }
    CScriptThing GetThingWithUID(unsigned __int64) const { abort(); }
    long GetAllCreaturesExcludingHero(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    long GetAllThingsInLevel(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    bool IsThingWithThisUIDAlive(unsigned __int64) const { abort(); }
    long RegisterTimer() { abort(); }
    void DeregisterTimer(long) { abort(); }
    void SetTimer(long, long) { abort(); }
    long GetTimer(long) const { abort(); }
    CScriptThing CreateCreature(const CCharString&, const C3DVector&, const CCharString&, bool) const { abort(); }
    CScriptThing CreateCreatureNearby(const CCharString&, const C3DVector&, float, const CCharString&, bool) const { abort(); }
    CScriptThing CreateCreatureOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const { abort(); }
    CScriptThing TurnCreatureInto(const CScriptThing&, const CCharString&) const { abort(); }
    void SetCreatureCreationDelayFrames(long) { abort(); }
    void ResetCreatureCreationDelayFrames() { abort(); }
    CScriptThing CreateObject(const CCharString&, const C3DVector&, const CCharString&) const { abort(); }
    CScriptThing CreateObject(const CCharString&, const C3DVector&, float, const CCharString&) const { abort(); }
    CScriptThing CreateObjectOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const { abort(); }
    CScriptThing CreateEffect(const CCharString&, const CScriptThing&, const CCharString&, const CCharString&, bool, bool) const { abort(); }
    CScriptThing CreateEffect(const CCharString&, const C3DVector&, const CCharString&, float, bool, bool) const { abort(); }
    CScriptThing CreateLight(const C3DVector&, const CRGBColour&, const CCharString&, float, float, bool) const { abort(); }
    CScriptThing CreateExperienceOrb(const C3DVector&, long) const { abort(); }
    CScriptThing CreateExplosion(const CCharString&, const C3DVector&, const C3DVector*, CCharString) const { abort(); }
    CScriptThing CreatePhysicalBarrier(float, const C3DVector&, const C3DVector*, CCharString) const { abort(); }
    CScriptThing CreateRumble(const C3DVector&, float, float, CCharString) const { abort(); }
    void ClearAllRumbles() const { abort(); }
    void ShowOnScreenMessage(const CCharString&, float) const { abort(); }
    void ShowOnScreenMessage(const CCharString&, const CCharString&, const CCharString&, float) const { abort(); }
    void ShowOnScreenMessage(const C2DVector&, const CCharString&, const CRGBColour&, const CCharString&) const { abort(); }
    void AddScreenMessage(const CCharString&, ETextGroupSelectionMethod) const { abort(); }
    void AddScreenTitleMessage(const CCharString&, float, bool) const { abort(); }
    void GiveHeroYesNoQuestion(const CCharString&, const CCharString&, const CCharString&, const CCharString&, bool) const { abort(); }
    void DisplayGameInfo(const CCharString&) const { abort(); }
    void DisplayGameInfoText(const CWideString&) const { abort(); }
    bool IsSafeToDisplayGameInfo() const { abort(); }
    bool DisplayTutorial(ETutorialCategory) const { abort(); }
    bool IsTutorialSystemEnabled() const { abort(); }
    void GiveHeroWeapon(const CCharString&, bool) const { abort(); }
    void GiveHeroObject(const CCharString&, long, bool) const { abort(); }
    void SetWeaponAsHerosActiveWeapon(const CCharString&) const { abort(); }
    void GiveHeroItem(const CScriptThing&) const { abort(); }
    bool GiveHeroItemsFromContainer(const CScriptThing&, bool) const { abort(); }
    void TakeObjectFromHero(const CCharString&) const { abort(); }
    void GiveHeroGold(long) const { abort(); }
    long GetHeroGold() const { abort(); }
    void GiveHeroExperience(long) const { abort(); }
    void SetHeroAbleToGainExperience(bool) const { abort(); }
    void SheatheHeroWeapons() const { abort(); }
    void SetHeroWillAsUsable(bool) const { abort(); }
    void SetHeroWeaponsAsUsable(bool) const { abort(); }
    void SetWeaponOutCrimeEnabled(bool) const { abort(); }
    void SetGuardsIgnoreCrimes(bool) const { abort(); }
    void RemoveAllHeroWeapons() const { abort(); }
    bool IsReportedOrUnreportedCrimeKnown(const CScriptThing&) const { abort(); }
    void ConfiscateAllHeroItems() const { abort(); }
    void ConfiscateAllHeroWeapons() const { abort(); }
    void ConfiscateItemsOfTypeFromHero(const CCharString&) const { abort(); }
    void ReturnAllConfiscatedItemsToHero() const { abort(); }
    bool GiveHeroTutorial(ETutorialCategory) const { abort(); }
    void MakeHeroCarryItemInHand(const CCharString&) const { abort(); }
    void MakeHeroCarryItemInHand(const CScriptThing&, bool, bool) const { abort(); }
    void AddTattooToHero(const CCharString&) const { abort(); }
    void GiveHeroAbility(EHeroAbility, bool) const { abort(); }
    bool IsPlayerZTargetingThing(const CScriptThing&) const { abort(); }
    bool IsPlayerCreatureBlocking() const { abort(); }
    bool IsPlayerCreatureReadyToFireProjectileWeapon(float&) const { abort(); }
    long GetPlayerCreatureCombatMultiplier() const { abort(); }
    long GetPlayerCreatureCombatMultiplierRunningNumHits() const { abort(); }
    void ResetPlayerCreatureCombatMultiplier() const { abort(); }
    bool IsPlayerCreatureFlourishEnabled() const { abort(); }
    void SetPlayerCreatureOnlyTarget(const CScriptThing&) const { abort(); }
    void ResetPlayerCreatureOnlyTarget() const { abort(); }
    void RespawnHero(bool) const { abort(); }
    void GiveHeroMorality(float) const { abort(); }
    float GetHeroMorality() const { abort(); }
    EMorality GetHeroMoralityCategory() const { abort(); }
    void ChangeHeroMoralityDueToTheft() const { abort(); }
    void ChangeHeroMoralityDueToPicklock() const { abort(); }
    void GiveHeroRenownPoints(long) const { abort(); }
    long GetHeroRenownLevel() const { abort(); }
    bool IsHeroRenownLevelFull() const { abort(); }
    void IncreaseHeroRenownLevel() const { abort(); }
    long GetHeroStrengthLevel() const { abort(); }
    long GetHeroSkillLevel() const { abort(); }
    long GetHeroWillLevel() const { abort(); }
    long GetHeroStatLevel(EHeroTrainableStatType) const { abort(); }
    long GetHeroStatMax(EHeroTrainableStatType) const { abort(); }
    void SetHeroAge(float) const { abort(); }
    float GetHeroAge() const { abort(); }
    void SetHeroAsTeenager(bool) const { abort(); }
    void SetHeroAsApprentice(bool) const { abort(); }
    float GetDistanceHeroCanBeHeardFrom() const { abort(); }
    long GetHeroRoughExperienceLevel() const { abort(); }
    long GetHeroExperienceAvailableToSpend() const { abort(); }
    float GetHeroFatness() const { abort(); }
    float GetHeroScariness() const { abort(); }
    float GetHeroAttractiveness() const { abort(); }
    float GetHeroWillEnergyLevel() const { abort(); }
    void SetHeroWillEnergyLevel(float) const { abort(); }
    void SetHeroWillEnergyAsAbleToRefill(bool) const { abort(); }
    long GetNumberOfItemsOfTypeInInventory(const CCharString&) const { abort(); }
    bool IsObjectInThingsPossession(const CCharString&, const CScriptThing&) const { abort(); }
    bool IsHeroHandLampLit() const { abort(); }
    void SetHeroHandLampAsLit(bool) const { abort(); }
    bool IsWearingClothingItem(const CScriptThing&, const CCharString&) const { abort(); }
    bool IsHeroNaked() const { abort(); }
    void RemoveHeroClothing() const { abort(); }
    void SetHeroAsWearing(const CCharString&) const { abort(); }
    void ChangeHeroHairstyle(const CCharString&) const { abort(); }
    void RemoveHeroHairstyle() const { abort(); }
    bool IsWearingHairstyle(CScriptThing&, const CCharString&) const { abort(); }
    bool IsPlayerCarryingItemOfType(const CCharString&) const { abort(); }
    bool IsPlayerWieldingWeapon() const { abort(); }
    bool IsEntityWieldingWeapon(const CScriptThing&) const { abort(); }
    bool IsEntityWieldingMeleeWeapon(const CScriptThing&) const { abort(); }
    bool IsEntityWieldingRangedWeapon(const CScriptThing&) const { abort(); }
    bool GetPreviouslyWieldedMeleeWeaponName(CCharString&) const { abort(); }
    bool GetPreviouslyWieldedRangedWeaponName(CCharString&) const { abort(); }
    bool IsEntityAbleToAttack(const CScriptThing&) const { abort(); }
    CScriptThing EntityGetThingInPrimarySlot(const CScriptThing&) const { abort(); }
    void ApplyHeroPenaltyForDeath() const { abort(); }
    void GiveHeroTitle(const CCharString&) const { abort(); }
    EHeroTitle GetHeroTitle() const { abort(); }
    void EntitySetAsMarryable(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing&, bool) const { abort(); }
    void EntityForceMarriageToHero(const CScriptThing&, bool) const { abort(); }
    bool IsEntityMarriedToHero(const CScriptThing&) const { abort(); }
    bool IsEntityMarriable(const CScriptThing&) const { abort(); }
    bool GetHeroHasMarried() const { abort(); }
    bool GetHeroHasCurrentMarriage() const { abort(); }
    bool GetHeroHasDivorcedMarriage() const { abort(); }
    bool GetHeroHasChildren() const { abort(); }
    bool GetHeroHasMurderedWife() const { abort(); }
    bool IsHeroChild() const { abort(); }
    void CancelHeroTeleportEffects() const { abort(); }
    long GetNumberOfTimesHeroHasHadSex() const { abort(); }
    void SetNumberOfTimesHeroHasHadSex(long) const { abort(); }
    void SetHeroAsHavingHadSex(bool) const { abort(); }
    void SetHeroAsHavingHadGaySex(bool) const { abort(); }
    void GiveThingHeroRewardItem(const CScriptThing&, const CCharString&, const CCharString&) const { abort(); }
    void GiveThingItemInHand(const CScriptThing&, const CCharString&, bool) const { abort(); }
    void GiveThingItemInSlot(const CScriptThing&, const CCharString&, const CCharString&) const { abort(); }
    void GiveHeroExpression(const CCharString&, long, bool) const { abort(); }
    bool HeroHasExpression(const CCharString&) const { abort(); }
    bool IsHeroPerformingExpression(const CCharString&) const { abort(); }
    bool IsDPadButtonHeldForExpression(const CCharString&) const { abort(); }
    void EntityFollowThing(const CScriptThing&, const CScriptThing&, float, bool) const { abort(); }
    void EntityStopFollowing(const CScriptThing&) const { abort(); }
    bool GetFollowingEntityList(const CScriptThing&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    bool GetHeroSummonedCreaturesList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    bool GetPerceivingHeroEntityList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    bool IsEntityFollowingHero(const CScriptThing&) const { abort(); }
    void EntitySetAsAllowedToFollowHero(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsAllowedToChangeRegionFollowingState(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(const CScriptThing&, bool) const { abort(); }
    void TeleportAllFollowersToHeroPosition() const { abort(); }
    void EntityTeleportToHeroPosition(const CScriptThing&) const { abort(); }
    void SendEntityEvent(NEntityEvents::EEventType, const CScriptThing&, const CScriptThing&, CThing*) const { abort(); }
    float GetWaterHeightAtPosition(const C3DVector&) const { abort(); }
    bool IsFishingSpotEnabled(const CScriptThing&) const { abort(); }
    void DisableFishingSpot(const CScriptThing&) const { abort(); }
    void UpdateFishWeight(const CScriptThing&) const { abort(); }
    float GetBestFishWeight(const CCharString&) const { abort(); }
    void HeroGoFishing(bool) const { abort(); }
    long GetHeroFishingLevel() const { abort(); }
    void HeroGoDigging() const { abort(); }
    void HeroStopDigging() const { abort(); }
    void HeroPlayOracleMinigame() const { abort(); }
    bool IsHeroPlayingOracleMinigame() const { abort(); }
    bool HasHeroWonOracleMinigame() const { abort(); }
    void HeroPlayFireheartMinigame() const { abort(); }
    void HeroQuitFireheartMinigame() const { abort(); }
    bool HasHeroForceQuitFireheartMinigame() const { abort(); }
    float GetHeroHealth() const { abort(); }
    float GetHeroHealthMax() const { abort(); }
    float GetHeroHealthPercentage() const { abort(); }
    long GetHeroWillEnergy() const { abort(); }
    long GetHeroWillEnergyMax() const { abort(); }
    long GetHeroWillEnergyPercentage() const { abort(); }
    void ChangeHeroHealthBy(float, bool, bool) const { abort(); }
    void ModifyThingHealth(const CScriptThing&, float, bool) const { abort(); }
    void EntitySetMaxHealth(const CScriptThing&, float, bool) const { abort(); }
    void SetThingAsKilled(const CScriptThing&) const { abort(); }
    void GiveHeroNewQuestObjective(const CCharString&, unsigned long) const { abort(); }
    void TellHeroQuestObjectiveCompleted(unsigned long) const { abort(); }
    void TellHeroQuestObjectiveFailed(unsigned long) const { abort(); }
    void AddQuestRegion(const CCharString&, const CCharString&) const { abort(); }
    void SetQuestWorldMapOffset(const CCharString&, const C2DCoordI&) const { abort(); }
    bool IsHeroOnQuest() const { abort(); }
    void HeroReceiveMessageFromGuildMaster(const CCharString&, const CCharString&, bool, bool) const { abort(); }
    void SetGuildMasterMessages(bool) const { abort(); }
    void ActivateQuest(const CCharString&) const { abort(); }
    void ActivateMultipleQuests(const CArray<CCharString>&) const { abort(); }
    void ActivateQuestWithoutLoadingResources(const CCharString&) const { abort(); }
    void ActivateMultipleQuestsWithoutLoadingResources(const CArray<CCharString>&) const { abort(); }
    void DeactivateQuest(const CCharString&, unsigned long) const { abort(); }
    void DeactivateQuestLater(const CCharString&, unsigned long) const { abort(); }
    void PrepareQuestsWhenFinalQuestIsActivated(const CCharString&) const { abort(); }
    void PrepareQuestsWhenFinalQuestIsCompleted() const { abort(); }
    bool IsQuestActive(const CCharString&) const { abort(); }
    bool IsQuestRegistered(const CCharString&) const { abort(); }
    bool IsQuestCompleted(const CCharString&) const { abort(); }
    bool IsQuestFailed(const CCharString&) const { abort(); }
    void SetQuestAsCompleted(const CCharString&, bool, bool, bool) const { abort(); }
    void SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) const { abort(); }
    void SetQuestAsPersistent(const CCharString&, bool) const { abort(); }
    const CCharString& GetExclusiveQuestScriptName() const { abort(); }
    void AddQuestCard(const CCharString&, const CCharString&, bool, bool) const { abort(); }
    void RemoveQuestCardFromGuild(const CCharString&) const { abort(); }
    void RemoveQuestCardFromHero(const CCharString&) const { abort(); }
    void GiveHeroQuestCardDirectly(const CCharString&, const CCharString&, bool) const { abort(); }
    void SetQuestCardObjective(const CCharString&, const CCharString&, const CCharString&, const CCharString&) const { abort(); }
    void SetQuestCardGoldReward(const CCharString&, long) const { abort(); }
    void SetQuestCardRenownReward(const CCharString&, long) const { abort(); }
    void RemoveAllAvailableQuestCardsFromGuild() const { abort(); }
    void FailAllActiveQuests() const { abort(); }
    void GetAllActiveQuestInfo(std::vector<CCharString,std::allocator<CCharString> >&, std::vector<CCharString,std::allocator<CCharString> >&) const { abort(); }
    void AddFeatCard(long, const CCharString&, const CCharString&) const { abort(); }
    void AddBoast(const CCharString&, long, long, long, bool, const CCharString&, long) const { abort(); }
    void RemoveBoast(long, const CCharString&) const { abort(); }
    void SetBoastAsFailed(long, const CCharString&) const { abort(); }
    void SetBoastAsCompleted(long, const CCharString&) const { abort(); }
    bool IsBoastTaken(long, const CCharString&) const { abort(); }
    void AddLogBookEntry(const CWideString&, const CWideString&, const CWideString&, long) const { abort(); }
    void KickOffQuestStartScreen(const CCharString&, bool, bool) const { abort(); }
    void KickOffDeathScreen() const { abort(); }
    void KickOffCreditsScreen(CCharString&) const { abort(); }
    void SetPreferredQuickAccessItem(const CCharString&, long, long) const { abort(); }
    CCharString GetDeathRecoveryMarkerName() const { abort(); }
    void SetDeathRecoveryMarkerName(const CCharString&) const { abort(); }
    void ResetDeathRecoveryMarkerNameToDefault() const { abort(); }
    bool IsToFailQuestOnDeath() const { abort(); }
    void SetWhetherToFailQuestOnDeath(bool) const { abort(); }
    void ResetWhetherToFailQuestOnDeathToDefault() const { abort(); }
    CScriptThing GetMostRecentValidUsedTarget() const { abort(); }
    CCharString GetMostRecentValidUsedTargetName() const { abort(); }
    void DisplayQuestInfo(bool) const { abort(); }
    void SetQuestInfoName(const char*) const { abort(); }
    void SetQuestInfoText(const char*) const { abort(); }
    long AddQuestInfoBar(float, float, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float) const { abort(); }
    long AddQuestInfoBarHealth(const CScriptThing&, const CRGBColour&, const CCharString&, float) const { abort(); }
    long AddQuestInfoTimer(const CTimer&, const CCharString&, float) const { abort(); }
    long AddQuestInfoCounter(const CCounter&, const CCharString&, float) const { abort(); }
    long AddQuestInfoCounter(const CCharString&, long, float) const { abort(); }
    long AddQuestInfoCounterList(const CCharString&, long, float) const { abort(); }
    long AddQuestInfoTick(const CCharString&, bool, float) const { abort(); }
    long AddQuestInfoTick(EGameAction, bool, float) const { abort(); }
    void UpdateQuestInfoBar(long, float, float, float) const { abort(); }
    void ChangeQuestInfoBarColour(long, const CRGBColour&, const CRGBColour&) const { abort(); }
    void UpdateQuestInfoTimer(long, float) const { abort(); }
    void UpdateQuestInfoCounter(long, long, long) const { abort(); }
    void UpdateQuestInfoCounterList(long, long, long) const { abort(); }
    void UpdateQuestInfoTick(long, bool) const { abort(); }
    void RemoveQuestInfoElement(long) const { abort(); }
    void RemoveAllQuestInfoElements() const { abort(); }
    void DisplayTime(bool) const { abort(); }
    void DisplayMoneyBag(bool) const { abort(); }
    void DisplayMiniGameInfo(bool, EMiniGameType) const { abort(); }
    void UpdateMiniGameInfoBar(float) const { abort(); }
    bool IsEntityPickPocketable(const CScriptThing&) const { abort(); }
    bool IsEntityPickLockable(const CScriptThing&) const { abort(); }
    bool IsEntityStealable(const CScriptThing&) const { abort(); }
    void EntitySetAsPickPocketed(const CScriptThing&) const { abort(); }
    void EntitySetAsPickLocked(const CScriptThing&) const { abort(); }
    void EntitySetAsStolen(const CScriptThing&) const { abort(); }
    void MiniMapAddMarker(const CScriptThing&, const CCharString&) const { abort(); }
    void MiniMapSetMarkerGraphic(const CScriptThing&, const CCharString&) const { abort(); }
    void MiniMapRemoveMarker(const CScriptThing&) const { abort(); }
    void MiniMapRemoveAllMarkers() const { abort(); }
    void MiniMapAllowRouteBetweenRegions(const CCharString&, const CCharString&, bool) const { abort(); }
    void MiniMapSetAsEnabled(bool) const { abort(); }
    void EntitySetAsHiddenOnMiniMap(const CScriptThing&, bool) const { abort(); }
    void SetHUDEnabled(bool) const { abort(); }
    CCharString GetValidTextEntryNameWithAttitude(const CScriptThing&, const CCharString&) const { abort(); }
    void ClearThingHasInformation(const CScriptThing&) const { abort(); }
    void EntitySetWillBeUsingNarrator(const CScriptThing&, const CCharString&) const { abort(); }
    void EntityResetAsPureAINarrator(const CScriptThing&) const { abort(); }
    long AddNewConversation(const CScriptThing&, bool, bool) const { abort(); }
    void AddPersonToConversation(long, const CScriptThing&) const { abort(); }
    void AddLineToConversation(long, const CCharString&, bool, const CScriptThing&, const CScriptThing&) const { abort(); }
    void RemoveConversation(long, bool) const { abort(); }
    bool IsConversationActive(long) const { abort(); }
    void PlayAVIMovie(const CCharString&) const { abort(); }
    void FixMovieSequenceCamera(bool) const { abort(); }
    void FadeScreenOutUntilNextCallToFadeScreenIn(float, float) { abort(); }
    bool FadeScreenOut(float, float, CRGBColour) { abort(); }
    void FadeScreenIn() { abort(); }
    bool IsScreenFadingOut() const { abort(); }
    void Pause(float) const { abort(); }
    void EndCutFade() { abort(); }
    void EndLetterBox() { abort(); }
    void PauseAllEntities(bool) { abort(); }
    void SetAllowScreenFadingOnNextRegionChange(bool) { abort(); }
    void SetAllowScreenFadingIfAlreadyFaded(bool) { abort(); }
    void SetToKeepHeroAbilitiesDuringCutscenes(bool) const { abort(); }
    void SetToDisplayTutorialsDuringCutscenes(bool) const { abort(); }
    void SetCutsceneMode(bool, bool) const { abort(); }
    bool IsInCutscene() const { abort(); }
    void SetCutsceneSkippable(bool) const { abort(); }
    void SetCutsceneSkippableWhilePaused(bool) const { abort(); }
    void SetCutsceneActionMode(bool, const CCharString&) const { abort(); }
    void PreloadNewScene(float) const { abort(); }
    void StartProgressDisplay() const { abort(); }
    void StopProgressDisplay() const { abort(); }
    void SetScreenMessagesEnabled(bool) const { abort(); }
    bool IsHeroControlledByPlayer() const { abort(); }
    bool IsInMovieSequence() const { abort(); }
    void CancelAbilitiesForCutscene() const { abort(); }
    void ResumeAbilitiesForCutscene() const { abort(); }
    void CancelUsingAbility(EHeroAbility) const { abort(); }
    void SetAbilityAvailability(EHeroAbility, bool) const { abort(); }
    void SetEnvironmentalEffectsAlwaysUpdate(bool) const { abort(); }
    void SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool) const { abort(); }
    void RemoveDeadCreature(const CCharString&) const { abort(); }
    void CameraSetCameraPreloadFlag(bool) const { abort(); }
    void CameraCircleAroundThing(const CScriptThing&, const C3DVector&, float) const { abort(); }
    void CameraCircleAroundPos(const C3DVector&, const C3DVector&, float) const { abort(); }
    void CameraMoveToPosAndLookAtPos(const C3DVector&, const C3DVector&, float) const { abort(); }
    void CameraMoveToPosAndLookAtThing(const C3DVector&, const CScriptThing&, float) const { abort(); }
    void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const C3DVector&, float, float) const { abort(); }
    void CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const CScriptThing&, float, float) const { abort(); }
    void CameraMoveBetweenLookFromAndLookTo(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&, float) const { abort(); }
    void CameraUseCameraPoint(const CCharString&, const CScriptThing&, float, long, long) const { abort(); }
    void CameraUseCameraPoint(const CScriptThing&, const CScriptThing&, float, long, long) const { abort(); }
    void CameraUseCameraPoint(const CCharString&, const C3DVector*, const CRightHandedSet*, float, long, long) const { abort(); }
    void CameraUseCameraPoint(const CScriptThing&, const C3DVector*, const CRightHandedSet*, float, long, long) const { abort(); }
    void CameraDoConversation(const CScriptThing&, const CScriptThing&, ECameraOp, bool) const { abort(); }
    void CameraDefault() { abort(); }
    void CameraResetToViewBehindHero(float) { abort(); }
    bool IsCameraInScriptedMode() const { abort(); }
    void CameraUseScreenEffect(float, float, float) const { abort(); }
    void CameraCancelScreenEffect() const { abort(); }
    bool IsCameraPosOnScreen(const C3DVector&) const { abort(); }
    float GetGameAngleXY() const { abort(); }
    void CameraEarthquakeIntensityAtPos(const C3DVector&, float, float) const { abort(); }
    void CameraShake(float, float) const { abort(); }
    void OpenDoor(const CScriptThing&) const { abort(); }
    void CloseDoor(const CScriptThing&) const { abort(); }
    void OpenHouseDoors(const CScriptThing&) const { abort(); }
    void CloseHouseDoors(const CScriptThing&) const { abort(); }
    void JamDoor(const CScriptThing&) const { abort(); }
    void SetDoorTriggerType(const CScriptThing&, EDoorTriggerType) const { abort(); }
    void OverrideAutomaticHouseLocking(const CScriptThing&, bool) const { abort(); }
    void SetHouseOwnedByPlayer(const CScriptThing&, bool, bool) const { abort(); }
    void SetBuyableHouseAsScripted(const CScriptThing&, bool) const { abort(); }
    bool IsChestOpen(const CScriptThing&) const { abort(); }
    bool OpenChest(const CScriptThing&, bool) const { abort(); }
    void CloseChest(const CScriptThing&) const { abort(); }
    long GetNumberOfKeysNeededToUnlockChest(const CScriptThing&, CCharString&) const { abort(); }
    void DisplayLockedChestMessage(const CScriptThing&) const { abort(); }
    void SetTrophyAsMountable(const CScriptThing&, bool) const { abort(); }
    void SetVillageLimbo(const CScriptThing&, bool) const { abort(); }
    void SetCreatureNotReload(const CScriptThing&) const { abort(); }
    bool IsSleepingTime(const CScriptThing&) const { abort(); }
    void EnableGuards(const CScriptThing&, bool) const { abort(); }
    void EnableVillagerDefTypes(const CScriptThing&, bool, const CCharString&) const { abort(); }
    CScriptThing TryToRespawnDefNamed(const CScriptThing&, const CCharString&, const C3DVector&) const { abort(); }
    void ClearHeroEnemyOfGuards(const CScriptThing&) const { abort(); }
    void SetThingAsUsable(const CScriptThing&, bool) const { abort(); }
    void SetThingHomeBuilding(const CScriptThing&, const CScriptThing&) const { abort(); }
    void SetVillageAttitude(const CScriptThing&, EScriptVillageAttitude) const { abort(); }
    void AddCrimeCommitted(const CScriptThing&, ECrime, bool, const CScriptThing*, const CScriptThing*, EOpinionPostDeedType) const { abort(); }
    void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) const { abort(); }
    void ClearThingBestEnemyTarget(const CScriptThing&) const { abort(); }
    void EntitySetInLimbo(const CScriptThing&, bool, bool) const { abort(); }
    bool IsEntityInLimbo(const CScriptThing&) const { abort(); }
    bool EntityGetShotStrikePos(const CScriptThing&, C3DVector&) const { abort(); }
    void EntitySetNegateAllHits(const CScriptThing&, bool) const { abort(); }
    void EntitySetEvadeAllHits(const CScriptThing&, bool) const { abort(); }
    void EntitySetAbleToBeEngagedInCombat(const CScriptThing&, bool) const { abort(); }
    void EntitySetAlwaysBlockAttacksFromThing(const CScriptThing&, const CScriptThing&, bool) const { abort(); }
    void EntitySetAttackThingImmediately(const CScriptThing&, const CScriptThing&, bool, bool) const { abort(); }
    void EntitySetCombatType(const CScriptThing&, const CCharString&) const { abort(); }
    void EntityResetCombatTypeToDefault(const CScriptThing&) const { abort(); }
    void EntitySetMaxNumberOfAttackers(const CScriptThing&, long) const { abort(); }
    void EntityClearMaxNumberOfAttackers(const CScriptThing&) const { abort(); }
    void EntitySetCombatAbility(const CScriptThing&, float) const { abort(); }
    void EntitySetRangedTarget(const CScriptThing&, const CScriptThing&) const { abort(); }
    void EntityClearRangedTarget(const CScriptThing&) const { abort(); }
    void EntitySetTargetable(const CScriptThing&, bool) const { abort(); }
    void EntitySetTargetingType(const CScriptThing&, ETargetingType) const { abort(); }
    void EntitySetTargetingValidTargetWithoutLOS(const CScriptThing&, bool) const { abort(); }
    void EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool) const { abort(); }
    void EntityTeleportToPosition(const CScriptThing&, const C3DVector&, float, bool, bool) const { abort(); }
    void EntitySetFacingAngle(const CScriptThing&, float, bool) const { abort(); }
    void EntitySetPerceptionVariables(const CScriptThing&, float, float, float) const { abort(); }
    void EntitySetThingAsWantingMoney(const CScriptThing&, long) const { abort(); }
    void EntitySetAppearanceMorphSeed(const CScriptThing&, long) const { abort(); }
    void SetEntityAsRegionFollowing(const CScriptThing&, const CScriptThing&, bool) const { abort(); }
    void SetEntityAsFollowingHeroThroughTeleporters(const CScriptThing&, bool) const { abort(); }
    void EntitySetAppearanceSeed(const CScriptThing&, unsigned long) const { abort(); }
    void EntityGetAppearanceSeed(const CScriptThing&, unsigned long&) const { abort(); }
    void EntitySetAsForSale(const CScriptThing&, bool) const { abort(); }
    void EntitySetStockItemPrice(const CScriptThing&, long) const { abort(); }
    long EntityGetStockItemPrice(const CScriptThing&) const { abort(); }
    void EntityPlayObjectAnimation(const CScriptThing&, const CCharString&, bool) const { abort(); }
    void EntitySetMaxRunningSpeed(const CScriptThing&, float) const { abort(); }
    void EntitySetMaxWalkingSpeed(const CScriptThing&, float) const { abort(); }
    void EntityResetMaxRunningSpeed(const CScriptThing&) const { abort(); }
    void EntityResetMaxWalkingSpeed(const CScriptThing&) const { abort(); }
    void EntityAttachToVillage(const CScriptThing&, const CScriptThing&) const { abort(); }
    void EntitySetAsSittingOnFloor(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsHavingBoundHands(CScriptThing&, bool) const { abort(); }
    void EntitySetAsRemoveAllMovementBlockingModes(CScriptThing&) const { abort(); }
    void EntitySetAsScared(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsDrunk(CScriptThing&, bool) const { abort(); }
    void EntityForceToLookAtThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    void EntityForceToLookAtCamera(const CScriptThing&, bool) const { abort(); }
    void EntityForceToLookAtNothing(const CScriptThing&) const { abort(); }
    void EntityResetForceToLookAt(const CScriptThing&) const { abort(); }
    void EntitySetShotAccuracyPercentage(const CScriptThing&, long) const { abort(); }
    CScriptThing EntityGetStandingOnThing(const CScriptThing&) const { abort(); }
    CScriptThing EntityGetStandingInsideBuilding(const CScriptThing&) const { abort(); }
    void EntityDropGenericBox(const CScriptThing&) const { abort(); }
    void EntitySheatheWeapons(const CScriptThing&, bool) const { abort(); }
    void EntityUnsheatheWeapons(const CScriptThing&, bool) const { abort(); }
    void EntityUnsheatheMeleeWeapon(const CScriptThing&, bool) const { abort(); }
    void EntityUnsheatheRangedWeapon(const CScriptThing&, bool) const { abort(); }
    void EntitySetAlpha(const CScriptThing&, float, bool) const { abort(); }
    void EntitySetAsDrawable(const CScriptThing&, bool) const { abort(); }
    void EntitySetCutsceneBehaviour(const CScriptThing&, ECutsceneBehaviour) const { abort(); }
    ESex EntityGetSex(const CScriptThing&) const { abort(); }
    void EntitySetAsAbleToWalkThroughSolidObjects(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsRespondToHit(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsDamageable(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsLocked(const CScriptThing&, bool) const { abort(); }
    void EntityDecapitate(const CScriptThing&) const { abort(); }
    void EntityGiveGold(const CScriptThing&, long) const { abort(); }
    void EntitySetAllowBossPhaseChanges(const CScriptThing&, bool) const { abort(); }
    long EntityGetBossPhase(const CScriptThing&) const { abort(); }
    void EntitySetBossPhase(const CScriptThing&, long) const { abort(); }
    void EntityResetCreatureMode(const CScriptThing&) const { abort(); }
    bool EntitySetAsReceivingEvents(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsToAddToStatChangesWhenHit(const CScriptThing&, bool) const { abort(); }
    void EntityLeaveCombatStance(const CScriptThing&) const { abort(); }
    void EntitySetAsUseMovementInActions(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsDisplayingEmoteIcon(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsCollidableToThings(const CScriptThing&, bool) const { abort(); }
    void EntityEnableGravity(const CScriptThing&, bool) const { abort(); }
    void EntitySetLightAsOn(const CScriptThing&, bool) const { abort(); }
    void EntityFadeOut(const CScriptThing&, float) const { abort(); }
    void EntityFadeIn(const CScriptThing&, float) const { abort(); }
    void EntityBeginLoadingAnimation(const CScriptThing&, const CCharString&) const { abort(); }
    void EntityBeginLoadingBasicAnimations(const CScriptThing&) const { abort(); }
    bool EntityCastForcePush(const CScriptThing&, bool) const { abort(); }
    void EntityCastLightningAtTarget(const CScriptThing&, const CScriptThing&) const { abort(); }
    void BeginLoadingMesh(const CCharString&) const { abort(); }
    bool EntityWillTeleportToArea(const CScriptThing&, C3DVector, float, float) const { abort(); }
    void EntityStartScreamerSuperAttackThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    void EntityEndScreamerSuperAttackThing(const CScriptThing&) const { abort(); }
    void SetHeroGuideToShowQuestCardsWhenSpokenTo(bool) { abort(); }
    void SetLightColour(const CScriptThing&, const CRGBColour&) const { abort(); }
    void CreatureGeneratorSetFamily(const CScriptThing&, const CCharString&) const { abort(); }
    void CreatureGeneratorTrigger(const CScriptThing&) const { abort(); }
    void CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(const CScriptThing&, bool) const { abort(); }
    bool CreatureGeneratorIsDepleted(const CScriptThing&) const { abort(); }
    bool CreatureGeneratorIsDestroyed(const CScriptThing&) const { abort(); }
    void CreatureGeneratorSetGeneratedCreatureScriptName(const CScriptThing&, const CCharString&) const { abort(); }
    void CreatureGeneratorSetNumTriggers(const CScriptThing&, long) const { abort(); }
    long CreatureGeneratorGetNumGeneratedCreatures(const CScriptThing&) const { abort(); }
    bool CreatureGeneratorAreAllCreaturesAlive(const CScriptThing&) const { abort(); }
    void CreatureGeneratorAddTriggerer(const CScriptThing&, const CScriptThing&) const { abort(); }
    void CreatureGeneratorRemoveTriggerer(const CScriptThing&, const CScriptThing&) const { abort(); }
    void SetCreatureGeneratorEnabled(const CScriptThing&, bool) const { abort(); }
    void SetCreatureGeneratorsEnabled(const CCharString&, bool) const { abort(); }
    void SetCreatureGeneratorsEnabledDuringScript(const CCharString&, bool) const { abort(); }
    void SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup, bool) const { abort(); }
    bool IsCreatureGenerationEnabledForRegion(const CCharString&) const { abort(); }
    bool IsCreatureFlying(const CScriptThing&) const { abort(); }
    void SetTeleporterAsActive(const CScriptThing&, bool) const { abort(); }
    bool IsTeleporterActive(const CScriptThing&) const { abort(); }
    void SetTeleportingAsActive(bool) const { abort(); }
    bool IsTeleportingActive() const { abort(); }
    void SetRegionExitAsActive(const CScriptThing&, bool) const { abort(); }
    void SetRegionEntranceAsActive(const CScriptThing&, bool) const { abort(); }
    void SetRegionTextDisplayAsActive(bool) const { abort(); }
    void SetHeroSleepingAsEnabled(bool) const { abort(); }
    bool IsHeroSleepingEnabled() const { abort(); }
    void SetExperienceSpendingAsEnabled(bool) const { abort(); }
    void SetMoralityChangingAsEnabled(bool) const { abort(); }
    void SetSummonerDeathExplosionAffectsHero(bool) const { abort(); }
    CScriptThing GetNearestEnabledDiggingSpot(const CScriptThing&) const { abort(); }
    bool IsDiggingSpotEnabled(const CScriptThing&) const { abort(); }
    bool IsDiggingSpotHidden(const CScriptThing&) const { abort(); }
    void SetDiggingSpotAsHidden(const CScriptThing&, bool) const { abort(); }
    bool CheckForCameraMessage(const CCharString&) const { abort(); }
    void WaitForCameraMessage(const CCharString&) const { abort(); }
    void SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const { abort(); }
    void SetFireToThing(const CScriptThing&) const { abort(); }
    void ExtinguishFiresOnThing(const CScriptThing&) const { abort(); }
    bool IsThingOnFire(const CScriptThing&) const { abort(); }
    void AddItemToContainer(const CScriptThing&, const CCharString&) const { abort(); }
    void RemoveItemFromContainer(const CScriptThing&, const CCharString&) const { abort(); }
    void EntitySetDeathContainerAsEnabled(const CScriptThing&, bool) const { abort(); }
    void GetItemDefNamesFromContainer(const CScriptThing&, std::vector<CCharString,std::allocator<CCharString> >&) const { abort(); }
    void SetCreatureBrain(const CScriptThing&, const CCharString&) const { abort(); }
    void EntitySetStategroupEnabled(const CScriptThing&, const CCharString&, bool) const { abort(); }
    void EntitySetAllStategroupsEnabled(const CScriptThing&, bool) const { abort(); }
    void EntitySetCombatEnabled(const CScriptThing&, bool) const { abort(); }
    void EntitySetSleepEnabled(const CScriptThing&, bool) const { abort(); }
    void EntitySetOpinionReactionsEnabled(const CScriptThing&, bool) const { abort(); }
    void EntitySetDeedReactionsEnabled(const CScriptThing&, bool) const { abort(); }
    void DebugGetAllTextEntriesForTargetedThing(std::set<unsigned long,std::less<unsigned long>,std::allocator<unsigned long> >&) const { abort(); }
    void EntitySetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    void EntityUnsetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    void EntityUnsetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    void EntitySetInFaction(const CScriptThing&, const CCharString&) const { abort(); }
    void SetFactionAsAlliedToFaction(const CCharString&, const CCharString&) const { abort(); }
    void SetFactionAsNeutralToFaction(const CCharString&, const CCharString&) const { abort(); }
    void SetFactionAsEnemyToFaction(const CCharString&, const CCharString&) const { abort(); }
    bool AreEntitiesEnemies(const CScriptThing&, const CScriptThing&) const { abort(); }
    EOpinionAttitudeType GetNextInOpinionAttitudeGraph(EOpinionAttitudeType) const { abort(); }
    void GetOpinionAttitudeAsString(EOpinionAttitudeType, CCharString&) const { abort(); }
    EOpinionAttitudeType EntityGetOpinionAttitudeToPlayer(const CScriptThing&) const { abort(); }
    void EntityGetOpinionAttitudeToPlayerAsString(const CScriptThing&, CCharString&) const { abort(); }
    float EntityGetOpinionOfPlayer(const CScriptThing&, EOpinion) const { abort(); }
    void EntitySetOpinionReactionMask(const CScriptThing&, long) const { abort(); }
    void EntitySetOpinionReactionMask(const CScriptThing&, const CCharString&) const { abort(); }
    void EntitySetOpinionDeedMask(const CScriptThing&, long) const { abort(); }
    void EntitySetOpinionDeedMask(const CScriptThing&, const CCharString&) const { abort(); }
    void EntitySetOpinionDeedTypeEnabled(const CScriptThing&, EOpinionDeedType, bool) const { abort(); }
    void EntitySetOpinionAttitudeEnabled(const CScriptThing&, EOpinionAttitudeType, bool) const { abort(); }
    void EntitySetOpinionReactionEnabled(const CScriptThing&, EOpinionReactionType, bool) const { abort(); }
    void EntitySetPersonalityOverride(const CScriptThing&, const CCharString&) const { abort(); }
    void EntitySetPersonalityOverride(const CScriptThing&, long) const { abort(); }
    void EntityClearPersonalityOverride(const CScriptThing&) const { abort(); }
    void EntitySetAsOpinionSource(const CScriptThing&, const CCharString&) const { abort(); }
    void EntitySetAsOpinionSource(const CScriptThing&, long) const { abort(); }
    void EntityUnsetAsOpinionSource(const CScriptThing&, bool) const { abort(); }
    void OpinionSourceSetAsExclusive(const CScriptThing&, bool) const { abort(); }
    void OpinionSourceSetAsAttentionGrabbing(const CScriptThing&, bool) const { abort(); }
    void EntityPostOpinionDeedToAll(const CScriptThing&, EOpinionDeedType) const { abort(); }
    void EntityPostOpinionDeedToRecipient(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { abort(); }
    void EntityPostOpinionDeedToRecipientVillage(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { abort(); }
    long EntityPostOpinionDeedKeepSearchingForWitnesses(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { abort(); }
    void RemoveOpinionDeedStillSearchingForWitnesses(const CScriptThing&, long) const { abort(); }
    bool IsDeedWitnessed(long) const { abort(); }
    bool CanThingBe_Seen_ByOtherThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    bool CanThingBe_NearlySeen_ByOtherThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    bool CanThingBe_Smelled_ByOtherThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    bool CanThingBe_Heard_ByOtherThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    bool IsThingAwareOfOtherThingInAnyWay(const CScriptThing&, const CScriptThing&) const { abort(); }
    void EntitySetAsAwareOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
    void EntitySetSoundRadius(const CScriptThing&, float) const { abort(); }
    void EntitySetSmellRadius(const CScriptThing&, float) const { abort(); }
    void EntitySetSightRadius(const CScriptThing&, float) const { abort(); }
    void EntitySetExtendedSightRadius(const CScriptThing&, float) const { abort(); }
    void EntitySetGiveUpChaseRadius(const CScriptThing&, float) const { abort(); }
    float EntityGetHearingRadius(const CScriptThing&) const { abort(); }
    bool ManuallyTriggerTrap(const CScriptThing&) const { abort(); }
    bool ManuallyResetTrap(const CScriptThing&) const { abort(); }
    void SetTimeOfDay(float) const { abort(); }
    long GetTimeOfDay() const { abort(); }
    void SetTimeAsStopped(bool, long&) const { abort(); }
    void FastForwardTimeTo(float, float) const { abort(); }
    bool IsTimeOfDayBetween(long, long) const { abort(); }
    long GetDayOfWeek() const { abort(); }
    long GetDayCount() const { abort(); }
    long GetWorldFrame() const { abort(); }
    long GetConstantFPS() const { abort(); }
    void TransitionToTheme(const CCharString&, float) const { abort(); }
    void ResetToDefaultTheme(float) const { abort(); }
    void TransitionToThemeAllInternals(const CCharString&, float) const { abort(); }
    void ResetToDefaultThemeAllInternals(float) const { abort(); }
    void TransitionToThemeExternals(const CCharString&, float) const { abort(); }
    void ResetToDefaultThemeExternals(float) const { abort(); }
    void SetEnvironmentThemeWeightAllChannels(const CCharString&, float) const { abort(); }
    void SetEnvironmentThemeWeightAllInternals(const CCharString&, float) const { abort(); }
    void SetEnvironmentThemeWeightExternals(const CCharString&, float) const { abort(); }
    void SetSoundThemesAsEnabledForRegion(const CCharString&, bool) const { abort(); }
    void SetAllSoundsAsMuted(bool) const { abort(); }
    void* RadialBlurFadeTo(float, C3DVector, float, C3DVector, float, float, float) const { abort(); }
    void* RadialBlurFadeTo(float, float, float, float, float, float, float) const { abort(); }
    void RadialBlurFadeOut(float, void*) const { abort(); }
    bool IsRadialBlurFadeActive() const { abort(); }
    void CancelRadialBlurFade() const { abort(); }
    void RadialBlurSetCenterWorldPos(void*, const C3DVector&) const { abort(); }
    void* DisplacementMonochromeEffectColourFadeTo(float, const CRGBFloatColour&) const { abort(); }
    void DisplacementMonochromeEffectColourFadeOut(float, void*) const { abort(); }
    void* ScreenFilterFadeTo(float, float, float, float, float, const CRGBFloatColour&, const std::vector<CScreenFilterSThingByPass,std::allocator<CScreenFilterSThingByPass> >&) const { abort(); }
    void ScreenFilterFadeOut(float, void*) const { abort(); }
    void SetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&, void*) const { abort(); }
    void UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&) const { abort(); }
    bool IsGiftRomantic(const CCharString&) const { abort(); }
    bool IsGiftFriendly(const CCharString&) const { abort(); }
    bool IsGiftOffensive(const CCharString&) const { abort(); }
    bool IsThingABed(const CScriptThing&) const { abort(); }
    bool IsThingAChest(const CScriptThing&) const { abort(); }
    bool IsThingADoor(const CScriptThing&) const { abort(); }
    bool IsThingSmashable(const CScriptThing&) const { abort(); }
    bool IsThingSearchable(const CScriptThing&) const { abort(); }
    void ApplyScriptBrush(const CCharString&) const { abort(); }
    void EnableDecals(bool) const { abort(); }
    unsigned long PlayCriteriaSoundOnThing(const CScriptThing&, const CCharString&) const { abort(); }
    unsigned long PlaySoundOnThing(const CScriptThing&, const CCharString&) const { abort(); }
    unsigned long PlaySoundAtPos(const C3DVector&, const CCharString&) const { abort(); }
    unsigned long Play2DSound(const CCharString&) const { abort(); }
    bool IsSoundPlaying(unsigned long) const { abort(); }
    void EnableSounds(bool) const { abort(); }
    void StopSound(unsigned long) const { abort(); }
    void OverrideMusic(EMusicSetType, bool, bool) const { abort(); }
    void StopOverrideMusic(bool) const { abort(); }
    void CacheMusicSet(EMusicSetType) const { abort(); }
    void EnableDangerMusic(bool) const { abort(); }
    bool IsDangerMusicEnabled() const { abort(); }
    void StartCountdownTimer(float) const { abort(); }
    float GetCountdownTimer() const { abort(); }
    void AutoSaveCheckPoint() const { abort(); }
    void AutoSaveQuestStart() const { abort(); }
    void AutoSave() const { abort(); }
    void SetSavingAsEnabled(bool) const { abort(); }
    bool IsSavingEnabled() const { abort(); }
    void SetSaveGameMarkerPos(const C3DVector&) const { abort(); }
    void ResetToFrontEnd() const { abort(); }
    void SetGuildSealRecallLocation(const C3DVector&, float) const { abort(); }
    C3DVector GetGuildSealRecallPos() const { abort(); }
    float GetGuildSealRecallAngleXY() const { abort(); }
    void SetReadableObjectText(const CScriptThing&, const CWideString&) const { abort(); }
    void SetReadableObjectTextTag(const CScriptThing&, const CCharString&) const { abort(); }
    CWideString GetFormattedString(const CCharString&, const std::vector<CWideString,std::allocator<CWideString> >&) const { abort(); }
    CWideString GetTextString(const CCharString&) const { abort(); }
    void AddRumourCategory(const CCharString&) const { abort(); }
    void AddNewRumourToCategory(const CCharString&, const CCharString&) const { abort(); }
    void RemoveRumourCategory(const CCharString&) const { abort(); }
    void SetCategoryActivity(const CCharString&, bool) const { abort(); }
    void AddGossipVillage(const CCharString&, const CCharString&) const { abort(); }
    void AddGossipFactionToCategory(const CCharString&, const CCharString&) const { abort(); }
    void SetIsGossipForPlayer(const CCharString&, bool) const { abort(); }
    void SetIsGossipForPlayer(CCharString, bool) { abort(); }
    void UpdateOnlineScore_Archery(float) const { abort(); }
    void UpdateOnlineScore_ChickenKick(float) const { abort(); }
    void UpdateOnlineScore_ChapelOrTemple(float) const { abort(); }
    void UpdateOnlineScore_FishingCompo(float) const { abort(); }
    void UpdateScore_FishingCompetition(float) const { abort(); }
    float GetBestTimePairs() const { abort(); }
    float GetBestTimeSorting() const { abort(); }
    long GetBestScoreBlackjack() const { abort(); }
    long GetBestScoreCoinGolfOakVale() const { abort(); }
    long GetBestScoreCoinGolfSnowSpire() const { abort(); }
    long GetBestScoreShoveHaPenny() const { abort(); }
    float GetBestTimeGuessTheAddition() const { abort(); }
    bool IsHeroInTavernGame() const { abort(); }
    long GetNumHousesOwned() const { abort(); }
    void StartSneaking() const { abort(); }
    long GetStealDuration(const CScriptThing&) const { abort(); }
    void SetUseableByHero(const CScriptThing&, bool) const { abort(); }
    void SetOwnedByHero(const CScriptThing&, bool) const { abort(); }
    void SetTavernTableAvailableForUse(const CScriptThing&, bool) const { abort(); }
    void SetIsThingTurncoatable(const CScriptThing&, bool) const { abort(); }
    void SetIsThingForcePushable(const CScriptThing&, bool) const { abort(); }
    void SetIsThingLightningable(const CScriptThing&, bool) const { abort(); }
    void SetIsThingEpicSpellable(const CScriptThing&, bool) const { abort(); }
    bool IsThingTurncoated(const CScriptThing&) const { abort(); }
    void AddCreatureScriptedMode(const CScriptThing&, const CCharString&) const { abort(); }
    void RemoveCreatureScriptedMode(const CScriptThing&) const { abort(); }
    void ForceShipsVisible() const { abort(); }
    bool GetSleepingPositionAndOrientationFromBed(const CScriptThing&, const CScriptThing&, C3DVector&, C3DVector&) { abort(); }
    void SetBedAvailability(const CScriptThing&, bool) { abort(); }
    void RepopulateVillage(const CScriptThing&) const { abort(); }
    void SmashAllWindowsWithinRadiusOfPoint(const C3DVector&, float) { abort(); }
    CScriptThing SetResidency(const CScriptThing&, bool) { abort(); }
    void SetThankingPhrase(CScriptThing, unsigned long) { abort(); }
    unsigned long GetThankingPhrase(CScriptThing) { abort(); }
    void ResetThankingPhrase(CScriptThing) { abort(); }
    void SetIgnoringPhrase(CScriptThing, unsigned long) { abort(); }
    unsigned long GetIgnoringPhrase(CScriptThing) { abort(); }
    void ResetIgnoringPhrase(CScriptThing) { abort(); }
    void SetWanderCentrePoint(CScriptThing, C3DVector) { abort(); }
    C3DVector GetWanderCentrePoint(CScriptThing) { abort(); }
    void ResetWanderCentrePoint(CScriptThing) { abort(); }
    void SetWanderMinDistance(CScriptThing, float) { abort(); }
    float GetWanderMinDistance(CScriptThing) { abort(); }
    void ResetWanderMinDistance(CScriptThing) { abort(); }
    void SetWanderMaxDistance(CScriptThing, float) { abort(); }
    float GetWanderMaxDistance(CScriptThing) { abort(); }
    void ResetWanderMaxDistance(CScriptThing) { abort(); }
    void SetGossipCounter(CScriptThing, long) { abort(); }
    long GetGossipCounter(CScriptThing) { abort(); }
    void ResetGossipCounter(CScriptThing) { abort(); }
    void SetMaxGossipPhrase(CScriptThing, long) { abort(); }
    long GetMaxGossipPhrase(CScriptThing) { abort(); }
    void ResetMaxGossipPhrase(CScriptThing) { abort(); }
    void SetWarningPhrase(CScriptThing, unsigned long) { abort(); }
    unsigned long GetWarningPhrase(CScriptThing) { abort(); }
    void ResetWarningPhrase(CScriptThing) { abort(); }
    void SetBeerRequestPhrase(CScriptThing, unsigned long) { abort(); }
    unsigned long GetBeerRequestPhrase(CScriptThing) { abort(); }
    void ResetBeerRequestPhrase(CScriptThing) { abort(); }
    void SetScriptingStateGroup(CScriptThing, EScriptingStateGroups) { abort(); }
    EScriptingStateGroups GetScriptingStateGroup(CScriptThing) { abort(); }
    void ResetScriptingStateGroup(CScriptThing) { abort(); }
    void SetMaxHeroReactionDistance(CScriptThing, float) { abort(); }
    float GetMaxHeroReactionDistance(CScriptThing) { abort(); }
    void ResetMaxHeroReactionDistance(CScriptThing) { abort(); }
    void SetActionFrequency(CScriptThing, long) { abort(); }
    long GetActionFrequency(CScriptThing) { abort(); }
    void ResetActionFrequency(CScriptThing) { abort(); }
    void SetActionFrequencyVariation(CScriptThing, float) { abort(); }
    float GetActionFrequencyVariation(CScriptThing) { abort(); }
    void ResetActionFrequencyVariation(CScriptThing) { abort(); }
    void SetAction(CScriptThing, CCharString) { abort(); }
    CCharString GetAction(CScriptThing) { abort(); }
    void ResetAction(CScriptThing) { abort(); }
    void SetFaceHeroForAction(CScriptThing, bool) { abort(); }
    bool GetFaceHeroForAction(CScriptThing) { abort(); }
    void ResetFaceHeroForAction(CScriptThing) { abort(); }
    void SetTargetName(CScriptThing, CCharString) { abort(); }
    CCharString GetTargetName(CScriptThing) { abort(); }
    void ResetTargetName(CScriptThing) { abort(); }
    void SetFollowDistance(CScriptThing, float) { abort(); }
    float GetFollowDistance(CScriptThing) { abort(); }
    void ResetFollowDistance(CScriptThing) { abort(); }
    void SetAttackHeroOnSight(CScriptThing, bool) { abort(); }
    bool GetAttackHeroOnSight(CScriptThing) { abort(); }
    void ResetAttackHeroOnSight(CScriptThing) { abort(); }
    void SetTimeToSpendHarassingHero(CScriptThing, long) { abort(); }
    long GetTimeToSpendHarassingHero(CScriptThing) { abort(); }
    void ResetTimeToSpendHarassingHero(CScriptThing) { abort(); }
    void SetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing, float) { abort(); }
    float GetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) { abort(); }
    void ResetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) { abort(); }
    void SetCombatNearbyBreakOffRange(CScriptThing, float) { abort(); }
    float GetCombatNearbyBreakOffRange(CScriptThing) { abort(); }
    void ResetCombatNearbyBreakOffRange(CScriptThing) { abort(); }
    void SetStealStealableItems(CScriptThing, bool) { abort(); }
    bool GetStealStealableItems(CScriptThing) { abort(); }
    void ResetStealStealableItems(CScriptThing) { abort(); }
    void SetRecoverStealableItems(CScriptThing, bool) { abort(); }
    bool GetRecoverStealableItems(CScriptThing) { abort(); }
    void ResetRecoverStealableItems(CScriptThing) { abort(); }
    void SetTakeStealableItemToRandomDestination(CScriptThing, bool) { abort(); }
    bool GetTakeStealableItemToRandomDestination(CScriptThing) { abort(); }
    void ResetTakeStealableItemToRandomDestination(CScriptThing) { abort(); }
    void SetKillSelfAndStealableItemAfterReachingDestination(CScriptThing, bool) { abort(); }
    bool GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) { abort(); }
    void ResetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) { abort(); }
    void SetAllowedToFollow(CScriptThing, bool) { abort(); }
    bool GetAllowedToFollow(CScriptThing) { abort(); }
    void ResetAllowedToFollow(CScriptThing) { abort(); }
    void SetTableName(CScriptThing, CCharString) { abort(); }
    CCharString GetTableName(CScriptThing) { abort(); }
    void ResetTableName(CScriptThing) { abort(); }
    void SetSeatName(CScriptThing, CCharString) { abort(); }
    CCharString GetSeatName(CScriptThing) { abort(); }
    void ResetSeatName(CScriptThing) { abort(); }
    void SetDisableHeadLooking(CScriptThing, bool) { abort(); }
    bool GetDisableHeadLooking(CScriptThing) { abort(); }
    void ResetDisableHeadLooking(CScriptThing) { abort(); }
    bool GetIsPushableByHero(CScriptThing) { abort(); }
    void ResetIsPushableByHero(CScriptThing) { abort(); }
    void SetLookForFiniteTime(CScriptThing, bool) { abort(); }
    bool GetLookForFiniteTime(CScriptThing) { abort(); }
    void ResetLookForFiniteTime(CScriptThing) { abort(); }
    void SetAvoidRegionExits(CScriptThing, bool) { abort(); }
    bool GetAvoidRegionExits(CScriptThing) { abort(); }
    void ResetAvoidRegionExits(CScriptThing) { abort(); }
    void SetTargetingDistanceOffset(CScriptThing, float) { abort(); }
    float GetTargetingDistanceOffset(CScriptThing) { abort(); }
    void ResetTargetingDistanceOffset(CScriptThing) { abort(); }
    void SetPlayerUsingMeleeDummies(bool) { abort(); }
    bool GetPlayerUsingMeleeDummies() { abort(); }
    void SetPlayerUsingRangedDummies(bool) { abort(); }
    bool GetPlayerUsingRangedDummies() { abort(); }
    void SetPlayerUsingWillDummies(bool) { abort(); }
    bool GetPlayerUsingWillDummies() { abort(); }
    void SetCheapHeadLooking(bool) { abort(); }
    bool GetCheapHeadLooking() { abort(); }
    void SetQuitTavernGame(bool) { abort(); }
    bool GetQuitTavernGame() { abort(); }
    void SetPrizeTavernTable(bool) { abort(); }
    bool GetPrizeTavernTable() { abort(); }
    void SetBettingActive(bool) { abort(); }
    bool GetBettingActive() { abort(); }
    void SetBettingAccept(bool) { abort(); }
    bool GetBettingAccept() { abort(); }
    void SetBettingAmount(long) { abort(); }
    long GetBettingAmount() { abort(); }
    void SetCountBetMoneyDown(bool) { abort(); }
    bool GetCountBetMoneyDown() { abort(); }
    void SetSpotTheAdditionBeaten(bool) { abort(); }
    bool GetSpotTheAdditionBeaten() { abort(); }
    void SetGlobalTargetingDistanceOffset(float) { abort(); }
    float GetGlobalTargetingDistanceOffset() { abort(); }
    void SetTradingPriceMult(float) { abort(); }
    float GetTradingPriceMult() { abort(); }
    void SetBoastingEnabled(bool) { abort(); }
    bool GetBoastingEnabled() { abort(); }
    void SetActiveGossipCategories(CCharString, bool) { abort(); }
    bool* GetActiveGossipCategories(CCharString) { abort(); }
    const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetActiveGossipCategories() { abort(); }
    int GetActiveGossipCategoriesSize() { abort(); }
    void ClearActiveGossipCategories() { abort(); }
    bool* GetIsGossipForPlayer(CCharString) { abort(); }
    const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& GetIsGossipForPlayer() { abort(); }
    int GetIsGossipForPlayerSize() { abort(); }
    void ClearIsGossipForPlayer() { abort(); }
    void SetGossip(CCharString, CCharString, long) { abort(); }
    CCharString GetGossip(CCharString, long) { abort(); }
    const std::vector<CCharString,std::allocator<CCharString> >* GetGossip(CCharString) { abort(); }
    int GetGossipSize(CCharString) { abort(); }
    void ClearGossip(CCharString) { abort(); }
    void RemoveGossip(CCharString) { abort(); }
    void AddGossip(CCharString, CCharString) { abort(); }
    void AddGossip(CCharString) { abort(); }
    void SetGossipVillages(CCharString, CCharString, long) { abort(); }
    CCharString GetGossipVillages(CCharString, long) { abort(); }
    const std::vector<CCharString,std::allocator<CCharString> >* GetGossipVillages(CCharString) { abort(); }
    int GetGossipVillagesSize(CCharString) { abort(); }
    void ClearGossipVillages(CCharString) { abort(); }
    void RemoveGossipVillages(CCharString) { abort(); }
    void AddGossipVillages(CCharString, CCharString) { abort(); }
    void AddGossipVillages(CCharString) { abort(); }
    void SetGossipFactions(CCharString, CCharString, long) { abort(); }
    CCharString GetGossipFactions(CCharString, long) { abort(); }
    const std::vector<CCharString,std::allocator<CCharString> >* GetGossipFactions(CCharString) { abort(); }
    int GetGossipFactionsSize(CCharString) { abort(); }
    void ClearGossipFactions(CCharString) { abort(); }
    void RemoveGossipFactions(CCharString) { abort(); }
    void AddGossipFactions(CCharString, CCharString) { abort(); }
    void AddGossipFactions(CCharString) { abort(); }
};
static void Reset()
{
    Conditions start = {0};
    state = start;
    state.open = 1;
    state.script_ready = 1;
    state.finish = 2;
    state.stop = 100;
    state.missing_after = 100;
    state.health = 5.0f;
    quest->PlayedOpeningCS = false;
    quest->TabletsCollected = 0;
}

static void Run(CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets& node)
{
    node.Main();
    Check(state.attached == 1 && state.predicates == 1);
    Check(!state.started_paused && state.pauses == state.resumes && state.movies == state.pauses);
}

int main()
{
    TestGame game;
    NScript::CQ_SunnyvaleMasterData master_data;
    CQ_AwakeningTheOracleScript parent(&master_data, game);
    quest = &parent;
    CScriptThing me;
    CCountedPointer<CEntityScriptBase> allocated = CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets::Alloc(parent, &master_data, me);
    Check(allocated.Get() && allocated->GetParentScript() == &parent);
    CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets& node =
        *static_cast<CQ_AwakeningTheOracleScript::CScytheBeforeGettingTablets*>(allocated.Get());

    Reset();
    state.stop = 1;
    Run(node);
    Check(state.setup == 3 && state.scripts == 0 && state.removes == 0);

    Reset();
    state.open = 4;
    state.script_ready = 6;
    state.finish = 7;
    state.initial_script_valid = true;
    Run(node);
    Check(state.frame == 7 && state.scripts == 3 && state.invalidated == 1 && state.removes == 1);

    Reset();
    state.open = 4;
    state.stop = 2;
    Run(node);
    Check(state.frame == 2 && state.scripts == 0 && state.removes == 0);

    Reset();
    state.script_ready = 6;
    state.stop = 3;
    Run(node);
    Check(state.frame == 3 && state.scripts == 2 && state.removes == 0);

    Reset();
    state.talks_left = 2;
    state.tasks_left = 2;
    state.finish = 5;
    state.missing_after = 2;
    Run(node);
    Check(state.speaks == 2 && state.queries == 2 && state.waits == 4 && state.removes == 1);
    Check(state.text_id[0] == 10 && state.text_id[1] == 20);
    Check(state.spoken_id[0] == 10 && state.spoken_id[1] == 10);

    Reset();
    state.talks_left = 1;
    state.health = 0.0f;
    Run(node);
    Check(state.speaks == 0 && state.movies == 1 && state.removes == 1);

    Reset();
    state.any_spell = state.healing = true;
    Run(node);
    Check(!state.speaks && !state.movies && !state.allies && state.removes == 1);

    Reset();
    state.hit = true;
    Run(node);
    Check(state.speaks == 1 && state.spoken_id[0] == 0 && state.allies == 2 && state.removes == 1);

    Reset();
    state.any_spell = true;
    state.health = 0.0f;
    Run(node);
    Check(!state.speaks && state.movies == 1 && state.allies == 2 && state.removes == 1);

    Reset();
    state.talks_left = 1;
    state.stop = 3;
    state.tasks_left = 2;
    Run(node);
    Check(state.speaks == 1 && state.waits == 1 && !state.allies && !state.removes);

    Reset();
    state.hit = true;
    state.stop = 3;
    state.tasks_left = 2;
    Run(node);
    Check(state.speaks == 1 && state.waits == 1 && !state.allies && !state.removes);

    Reset();
    state.hit = true;
    state.terminate_on_speech = true;
    Run(node);
    Check(state.speaks == 1 && state.waits == 1 && !state.allies && !state.removes);

    puts("00cfce90_TEST PASS");
    return 0;
}