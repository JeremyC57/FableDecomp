// NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Main
// Leaf: ?Main@CPrisonRaceGuard@CQ_PrisonRaceScript@NScript@@EAEXXZ
// Flags:
#define FABLE_RETAIL_CQ_PRISONRACESCRIPT_GUARD_PASS27
#define FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS
#define FABLE_RETAIL_CSCRIPTRESOURCE_ORIGINAL_ACCESS
#define FABLE_RETAIL_CENTITYSCRIPTBASE_ORIGINAL_MEMBERS
#define FABLE_RETAIL_SCRIPT_MOVE_ENUM
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CSCRIPTTHING_NULLCOORD
#include "engine/CQ_PrisonRaceScript.h"
#include "engine/TRetVoid.h"
#include "engine/C3DVector.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/EHeroAbility.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "engine/EScriptAIPriority.h"
#include "engine/fable_script_animation_defaults.h"
extern CGameScriptInterfaceBase* pDSTGame;                   // 0x0143E8F8
inline CScriptThing::~CScriptThing() { }
inline float CScriptThing::GetAngleXY() const
{
    if (PImp.IsNull())
        return 0.0f;
    return (PImp->GetAngleXY());
}
FABLE_FORCEINLINE const C3DVector& CScriptThing::GetPos() const
{
    if (PImp.IsNull())
        return NullCoord;
    return PImp->GetPos();
}
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline bool CTimer::operator==(long time) const { return GetTime() == time; }
inline bool CTimer::operator>(long time) const { return GetTime() > time; }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_PrisonRaceScript::CPrisonRaceGuard::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    }
    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
    Game.Pause(1.0f);
    CScriptThing HomePosition = Game.GetNearestWithScriptName(Me, "GuardPosition");
    Game.EntitySheatheWeapons(Me, false);
    Game.EntityTeleportToThing(Me, HomePosition, false);
    Game.EntitySetFacingAngle(Me, HomePosition.GetAngleXY(), true);
    Game.EntitySetFacingAngle(Me, HomePosition.GetAngleXY(), true);
    CTimer WaveTimer;
    for (; true; Game.NewScriptFrame()) {
        do { if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; } } while (0);
        if (IsDistanceBetweenThingsUnder(Me, Game.GetHero(), (float)DistanceToPlayAnim) && WaveTimer == 0) {
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            long HLStyleConversationIndex = Game.AddNewConversation(Me, false, false);
            Game.AddPersonToConversation(HLStyleConversationIndex, Game.GetHero());
            Game.AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_055_RACE_TAUNT", false, Me, Game.GetHero());
            seh_me.PlayAnimation(Me.GetDataString(), false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            while (seh_me.IsPerformingScriptTask()) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            }
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            WaveTimer = 10;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            if (seh_me.IsValid())
                seh_me.Release();
            Game.GiveThingBestEnemyTarget(Me, Game.GetHero());
            CTimer attack_timer;
            attack_timer = 20;
            while (attack_timer > 0) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
                    || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                        && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    attack_timer = 20;
                }
            }
            Game.ClearThingBestEnemyTarget(Me);
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game.StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game.NewScriptFrame();
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            }
            if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            {
                float local_range = 0.0f;
                C3DVector local_position = HomePosition.GetPos();
                EScriptEntityMoveType local_movetype = ENTITY_MOVE_RUN;
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), local_position, local_range + 2.0)) {
                    Game.NewScriptFrame();
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    seh_me.MoveToPosition(local_position, local_range, local_movetype, false, true);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game.NewScriptFrame();
                        if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                    }
                    if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
                }
                if (IsActiveThreadTerminating()) { _Fail_Val.Return(); return; }
            }
            Game.EntitySetFacingAngle(Me, HomePosition.GetAngleXY(), false);
            Game.Pause(2.0f);
        }
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "engine/CRGBColour.h"

using NScript::CQ_PrisonRaceScript;

struct World
{
    long frame, terminate_at, reject_start, direct_hit, second_hit, ability_hit, heal_hit;
    long starts, pauses, face_changes, sheathed, teleports, taunts, animations, fights, fight_end, moves, releases;
    long timer_count, timer_value[2], timer_releases, wave_sets, attack_sets;
    long busy_until, distance_steps, resume_rejects;
    bool near_hero, valid_resource, no_home, ok;
};

static World State;
static const char* HeroName = "SCRIPT_NAME_HERO";

CGameScriptInterfaceBase* pDSTGame;
bool NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations = true;
C3DVector CScriptThing::NullCoord;

CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CBaseClassNonCopyable::CBaseClassNonCopyable() { }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { }
CGameScriptInterfaceBase::~CGameScriptInterfaceBase() { }
CBaseObject::CBaseObject() : IntelligentPointer(0) { }
CBaseObject::~CBaseObject() { }
void CBaseObject::Validate() const { }
CScriptBase::CScriptBase() { }
CScriptBase::~CScriptBase() { }
void CScriptBase::RegisterMain() { }
void CScriptBase::Main() { }
void CScriptBase::Init() { }
void CScriptBase::OnPersist(CPersistContext&) { }

CQ_PrisonRaceScript::CQ_PrisonRaceScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>(data, game) { }
void CQ_PrisonRaceScript::RegisterMain() { }
void CQ_PrisonRaceScript::Main() { }
void CQ_PrisonRaceScript::Init() { }
void CQ_PrisonRaceScript::OnPersist(CPersistContext&) { }

CEntityScriptBase::~CEntityScriptBase() { }
void CEntityScriptBase::Init() { }
void CEntityScriptBase::OnPersist(CPersistContext&) { }
void CEntityScriptBase::OnPredicateFail() { }
void CEntityScriptBase::OnInterrupted() { }
bool CEntityScriptBase::IsActiveThreadTerminating() const { return State.frame >= State.terminate_at; }

CQ_PrisonRaceScript::CPrisonRaceGuard::CPrisonRaceGuard(CQ_PrisonRaceScript& parent, CScriptDataBase* data, const CScriptThing& me)
    : CEntityScriptBase(*parent.Game, me), ParentClass(parent),
      PMasterData((NScript::CQ_SunnyvaleMasterData*)data), DistanceToPlayAnim(5) { }
void CQ_PrisonRaceScript::CPrisonRaceGuard::Init() { }
CScriptBase* CQ_PrisonRaceScript::CPrisonRaceGuard::GetParentScript() const { return &ParentClass; }

CCharString::CCharString() : PStringData(0) { }
CCharString::CCharString(const char* text, long length) : PStringData((CCharStringData*)text)
{ State.ok &= length == -1; }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { }
CCharString::~CCharString() { }
CCharString::operator const char*() const { return (const char*)PStringData; }
CScriptThing::CScriptThing() { }
CScriptThing::CScriptThing(const CScriptThing& other) : PImp(other.PImp) { }
void CScriptThing::SetImplementation(CCountedPointer<CScriptThing> value) { PImp = value; }
CCharString CScriptThing::GetDataString() const { return CCharString("TauntAnimation"); }

bool CScriptThing::MsgIsHitBy(const CCharString& name) const
{
    State.ok &= !strcmp(name, HeroName);
    return State.frame == State.direct_hit || State.frame == State.second_hit;
}
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const
{
    State.ok &= !strcmp(name, HeroName);
    return State.frame == State.ability_hit || State.frame == State.heal_hit;
}
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const
{
    State.ok &= !strcmp(name, HeroName) && ability == HERO_ABILITY_HEAL_LIFE_SPELL;
    return State.frame == State.heal_hit;
}

class HomeMarker : public CScriptThing
{
public:
    HomeMarker() : Position(2.0f, 3.0f, 4.0f) { }
    const C3DVector& GetPos() const { return Position; }
    float GetAngleXY() const { return 1.25f; }
private:
    C3DVector Position;
};

bool CScriptGameResourceObjectScriptedThingBase::IsValid() const { return State.valid_resource; }
void CScriptGameResourceObjectScriptedThingBase::Release()
{
    ++State.releases;
    State.valid_resource = false;
}
CScriptThing CScriptGameResourceObjectScriptedThingBase::GetScriptThing() const { return CScriptThing(); }
bool CScriptGameResourceObjectScriptedThingBase::IsPerformingScriptTask() const
{
    return State.frame < State.busy_until;
}
void CScriptGameResourceObjectScriptedThingBase::PlayAnimation(const CCharString& name, bool a, bool b, bool c,
                                                               bool d, bool e, bool f, bool g)
{
    State.ok &= !strcmp(name, "TauntAnimation") && !a && !b && !c && d && e && !f && !g;
    ++State.animations;
    State.busy_until = State.frame + 1;
}
void CScriptGameResourceObjectScriptedThingBase::MoveToPosition(const C3DVector& destination, float range,
                                                               EScriptEntityMoveType move, bool a, bool b)
{
    State.ok &= destination.X == (State.no_home ? 0.0f : 2.0f) &&
               destination.Y == (State.no_home ? 0.0f : 3.0f) && destination.Z == (State.no_home ? 0.0f : 4.0f) &&
               range == 0.0f && move == ENTITY_MOVE_RUN && !a && b;
    --State.distance_steps;
    ++State.moves;
    State.busy_until = State.frame + 1;
}
bool FABLE_FASTCALL IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float range)
{
    State.ok &= range == 5.0f;
    return State.near_hero;
}
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector& point, float range)
{
    State.ok &= point.X == (State.no_home ? 0.0f : 2.0f) &&
               point.Y == (State.no_home ? 0.0f : 3.0f) &&
               point.Z == (State.no_home ? 0.0f : 4.0f) && range == 2.0f;
    return State.distance_steps > 0;
}

void CScriptGameResourceObjectScriptedThingBase::AskHeroQuestion(const CScriptThing&, const char*,
                                                                const CCharString&, const CCharString&,
                                                                const CCharString&, bool, bool)
{
    abort();
}
const CCharString& CScriptThing::GetName() const { abort(); }
CCharString CScriptThing::GetDefName() const { abort(); }
void CScriptThing::SetDataString(const CCharString&) { abort(); }
C3DVector CScriptThing::GetFocalPos() const { abort(); }
C3DVector CScriptThing::GetHomePos() const { abort(); }
CCharString CScriptThing::GetCurrentMapName() const { abort(); }
CCharString CScriptThing::GetHomeMapName() const { abort(); }
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
bool CScriptThing::MsgIsTalkedToBy(const CCharString&) const { abort(); }
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
bool CScriptThing::IsAlive() const { abort(); }
bool CScriptThing::IsDead() const { abort(); }
bool CScriptThing::IsNull() const { abort(); }
bool CScriptThing::IsEqualTo(const CScriptThing&) const { abort(); }
void CScriptGameResourceObjectScriptedThingBase::SummonerLightningOrbAttackTarget(const CScriptThing&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::FireProjectileWeaponAtTarget(const CScriptThing&) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::MoveToThing(const CScriptThing&, float, EScriptEntityMoveType, bool, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::FollowPreCalculatedRoute(const CScriptThing&, EScriptEntityMoveType, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::FollowThing(const CScriptThing&, float, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::StopFollowingThing(const CScriptThing&) { abort(); }
bool CScriptGameResourceObjectScriptedThingBase::IsFollowActionRunning(const CScriptThing&) const { abort(); }
void CScriptGameResourceObjectScriptedThingBase::ClearCommands() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::WaitWhilePerformingTasks() { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&, unsigned long, ETextGroupSelectionMethod, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Speak(const CScriptThing&, const char*, ETextGroupSelectionMethod, bool, bool, bool) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::Converse(const CScriptGameResourceObjectScriptedThingBase&, const char*) { abort(); }
void CScriptGameResourceObjectScriptedThingBase::PerformExpression(const CScriptThing&, const CCharString&) { abort(); }
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

class TestGame : public CGameScriptInterfaceBase
{
public:
    void NewScriptFrame() const
    {
        ++State.frame;
        for (long i = 0; i < State.timer_count; ++i)
            if (State.timer_value[i] > 0)
                --State.timer_value[i];
    }
    bool StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority priority)
    {
        State.ok &= priority == SCRIPT_AI_PRIORITY_HIGHEST;
        if (++State.starts <= State.reject_start)
            return false;
        if (State.fight_end && State.resume_rejects > 0) {
            --State.resume_rejects;
            return false;
        }
        State.valid_resource = true;
        return true;
    }
    CScriptThing& GetHero() { return Hero; }
    CScriptThing GetNearestWithScriptName(const CScriptThing&, const CCharString& name) const
    {
        State.ok &= !strcmp(name, "GuardPosition");
        CScriptThing out;
        if (!State.no_home)
            out.SetImplementation(CCountedPointer<CScriptThing>(new HomeMarker));
        return out;
    }
    long RegisterTimer()
    {
        if (State.timer_count == 2)
            abort();
        return State.timer_count++;
    }
    void DeregisterTimer(long id)
    {
        State.ok &= id >= 0 && id < State.timer_count;
        ++State.timer_releases;
    }
    void SetTimer(long id, long value)
    {
        State.ok &= (id == 0 && value == 10) || (id == 1 && value == 20);
        State.timer_value[id] = value;
        if (id == 0)
            ++State.wave_sets;
        else
            ++State.attack_sets;
    }
    long GetTimer(long id) const { return State.timer_value[id]; }
    long AddNewConversation(const CScriptThing&, bool a, bool b) const
    {
        State.ok &= !a && !b;
        return 37;
    }
    void AddPersonToConversation(long id, const CScriptThing& actor) const { State.ok &= id == 37 && &actor == &Hero; }
    void AddLineToConversation(long id, const CCharString& key, bool flag, const CScriptThing&, const CScriptThing& actor) const
    {
        State.ok &= id == 37 && !strcmp(key, "TEXT_QST_055_RACE_TAUNT") && !flag && &actor == &Hero;
        ++State.taunts;
    }
    void Pause(float time) const
    {
        State.ok &= time == (State.pauses == 0 ? 1.0f : 2.0f);
        ++State.pauses;
    }
    void GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing& who) const
    {
        State.ok &= &who == &Hero;
        ++State.fights;
    }
    void ClearThingBestEnemyTarget(const CScriptThing&) const { State.fight_end = State.frame; }
    void EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool flag) const
    {
        State.ok &= !flag;
        ++State.teleports;
    }
    void EntitySetFacingAngle(const CScriptThing&, float angle, bool immediate) const
    {
        State.ok &= angle == (State.no_home ? 0.0f : 1.25f) && immediate == (State.face_changes < 2);
        ++State.face_changes;
    }
    void EntitySheatheWeapons(const CScriptThing&, bool flag) const
    {
        State.ok &= !flag;
        ++State.sheathed;
    }
private:
    CScriptThing Hero;

    // Overrides below reject virtual calls outside the dependencies of this row.
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
    CScriptThing GetThingWithScriptName(const CCharString&) const { abort(); }
    CScriptThing GetRandomThingWithScriptName(const CCharString&) const { abort(); }
    long GetAllThingsWithScriptName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    long GetAllCreaturesInAreaWithScriptName(const CCharString&, const C3DVector&, float, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    CScriptThing GetFurthestWithScriptName(const CScriptThing&, const CCharString&) const { abort(); }
    long GetAllThingsWithDefNameByDistanceFrom(const CScriptThing&, const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    long GetAllThingsWithDefName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    CScriptThing GetNearestWithDefName(const CScriptThing&, const CCharString&) const { abort(); }
    CScriptThing GetFurthestWithDefName(const CScriptThing&, const CCharString&) const { abort(); }
    CScriptThing GetThingWithUID(unsigned __int64) const { abort(); }
    long GetAllCreaturesExcludingHero(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    long GetAllThingsInLevel(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
    bool IsThingWithThisUIDAlive(unsigned __int64) const { abort(); }
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
    void RemoveThing(const CScriptThing&, bool, bool) const { abort(); }
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
    float GetHealth(const CScriptThing&) const { abort(); }
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
    bool TextEntryExists(const CCharString&) const { abort(); }
    CCharString GetValidTextEntryNameWithAttitude(const CScriptThing&, const CCharString&) const { abort(); }
    void SetThingHasInformation(const CScriptThing&, bool, bool, bool) const { abort(); }
    void ClearThingHasInformation(const CScriptThing&) const { abort(); }
    void EntitySetWillBeUsingNarrator(const CScriptThing&, const CCharString&) const { abort(); }
    void EntityResetAsPureAINarrator(const CScriptThing&) const { abort(); }
    void RemoveConversation(long, bool) const { abort(); }
    bool IsConversationActive(long) const { abort(); }
    void PlayAVIMovie(const CCharString&) const { abort(); }
    void StartMovieSequence(const CCharString&, CScriptGameResourceObjectMovieBase&) { abort(); }
    void FixMovieSequenceCamera(bool) const { abort(); }
    void FadeScreenOutUntilNextCallToFadeScreenIn(float, float) { abort(); }
    bool FadeScreenOut(float, float, CRGBColour) { abort(); }
    void FadeScreenIn() { abort(); }
    bool IsScreenFadingOut() const { abort(); }
    void EndCutFade() { abort(); }
    void EndLetterBox() { abort(); }
    void PauseAllNonScriptedEntities(bool) { abort(); }
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
    void EntityAttachToScript(const CScriptThing&, const CCharString&) const { abort(); }
    void EntitySetCombatAbility(const CScriptThing&, float) const { abort(); }
    void EntitySetRangedTarget(const CScriptThing&, const CScriptThing&) const { abort(); }
    void EntityClearRangedTarget(const CScriptThing&) const { abort(); }
    void EntitySetTargetable(const CScriptThing&, bool) const { abort(); }
    void EntitySetTargetingType(const CScriptThing&, ETargetingType) const { abort(); }
    void EntitySetTargetingValidTargetWithoutLOS(const CScriptThing&, bool) const { abort(); }
    void EntityTeleportToPosition(const CScriptThing&, const C3DVector&, float, bool, bool) const { abort(); }
    void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) const { abort(); }
    void EntitySetPerceptionVariables(const CScriptThing&, float, float, float) const { abort(); }
    void SetThingPersistent(const CScriptThing&, bool) const { abort(); }
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
    void EntitySetAsKillable(const CScriptThing&, bool, bool) const { abort(); }
    void EntitySetAsLocked(const CScriptThing&, bool) const { abort(); }
    void EntityDecapitate(const CScriptThing&) const { abort(); }
    void EntityGiveGold(const CScriptThing&, long) const { abort(); }
    void EntitySetAllowBossPhaseChanges(const CScriptThing&, bool) const { abort(); }
    long EntityGetBossPhase(const CScriptThing&) const { abort(); }
    void EntitySetBossPhase(const CScriptThing&, long) const { abort(); }
    void EntityResetCreatureMode(const CScriptThing&) const { abort(); }
    bool EntitySetAsReceivingEvents(const CScriptThing&, bool) const { abort(); }
    void EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool) const { abort(); }
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
    void EntitySetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
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
    CCharString GetActiveQuestName() const { abort(); }
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
    void SetIsPushableByHero(CScriptThing, bool) { abort(); }
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

static World Run(long stop, bool near, long rejects, long direct_hit, long second_hit,
                 long ability, long heal, long distance_steps, bool initially_valid,
                 long resume_rejects = 0, bool no_home = false)
{
    memset(&State, 0, sizeof(State));
    State.ok = true;
    State.near_hero = near;
    State.terminate_at = stop;
    State.reject_start = rejects;
    State.direct_hit = direct_hit;
    State.second_hit = second_hit;
    State.ability_hit = ability;
    State.heal_hit = heal;
    State.distance_steps = distance_steps;
    State.valid_resource = initially_valid;
    State.resume_rejects = resume_rejects;
    State.no_home = no_home;
    TestGame game;
    pDSTGame = &game;
    CQ_PrisonRaceScript quest(0, game);
    CScriptThing me;
    CQ_PrisonRaceScript::CPrisonRaceGuard guard(quest, 0, me);
    static_cast<CEntityScriptBase&>(guard).Main();
    pDSTGame = 0;
    return State;
}

int main()
{
    World start = Run(1, false, 5, -1, -1, -1, -1, 0, true);
    World setup = Run(0, false, 0, -1, -1, -1, -1, 0, false);
    World wave = Run(7, true, 1, -1, -1, -1, 0, 0, false);
    World combat = Run(45, false, 0, 2, 12, -1, -1, 2, false);
    World ability = Run(4, false, 0, -1, -1, 1, 0, 1, false);
    World heal = Run(5, false, 0, -1, -1, -1, 0, 0, false);
    World wave_stop = Run(1, true, 0, -1, -1, -1, -1, 0, false);
    World reacquire_stop = Run(23, false, 0, 2, -1, -1, -1, 1, false, 1);
    World movement_stop = Run(24, false, 0, 2, -1, -1, -1, 2, false);
    World no_home = Run(36, false, 0, 2, -1, -1, -1, 1, false, 1, true);
    bool ok = start.ok && start.starts == 1 && start.releases == 1 && !start.pauses && !start.timer_count &&
              setup.ok && setup.starts == 1 && !setup.pauses && !setup.face_changes &&
              wave.ok && wave.starts == 2 && wave.taunts == 1 && wave.animations == 1 && wave.wave_sets == 1 &&
              wave.pauses == 1 && wave.face_changes == 2 && wave.teleports == 1 && wave.sheathed == 1 && wave.timer_releases == 1 &&
              combat.ok && combat.fights == 1 && combat.fight_end == 32 && combat.attack_sets == 2 &&
              combat.moves == 2 && combat.face_changes == 3 && combat.pauses == 2 && combat.timer_releases == 2 &&
              ability.ok && ability.fights == 1 && ability.attack_sets == 1 && ability.fight_end == 0 &&
              ability.moves == 0 && ability.timer_releases == 2 &&
              heal.ok && heal.pauses == 1 && heal.fights == 0 && heal.attack_sets == 0 &&
              wave_stop.ok && wave_stop.animations == 1 && wave_stop.wave_sets == 0 &&
              reacquire_stop.ok && reacquire_stop.fight_end == 22 && reacquire_stop.starts == 2 &&
              reacquire_stop.moves == 0 && reacquire_stop.timer_releases == 2 &&
              movement_stop.ok && movement_stop.fight_end == 22 && movement_stop.moves == 1 &&
              movement_stop.face_changes == 2 && movement_stop.timer_releases == 2 &&
              no_home.ok && no_home.starts == 3 && no_home.moves == 1 && no_home.face_changes == 3;
    puts(ok ? "00ddb720_TEST PASS" : "00ddb720_TEST FAIL");
    return ok ? 0 : 1;
}