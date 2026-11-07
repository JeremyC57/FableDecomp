// NScript::CV_FisticuffsClubScript::CFisticuffsMember::Main() -- a punch-club crowd member: runs to his spot, cheers or boos the fight (rand), answers the hero, joins the fighter queue when challenged, fights while healthy, and walks off and fades out when the club closes
// Grade: DIFFER(8968v8836) -- 132 lines: VC7.1 expands the implicit ~seh_me (~CScriptGameResourceObjectScriptedThingBase) at the natural loop exit where retail calls 0x007E74D0 and jumps to the shared [~destination; ~ring; ~seh_me] tail 0x00E86DC6; the first thread exit then cross-jumps into that copy (retail expands it in place), the in-body exit pads follow the loop instead of case 1, and the post-switch exit expands DeregisterTimer (retail calls ~CTimer 0x00CD4470). 12 chunk 819 attempts: the CTyler 0x00E87030 define/include set, the declared-only MovieBase dtor spec and CDefPointer pGlobalScriptDef change nothing; /d2inlT60..100 (diagnostic) never matches, so it is not a pure budget offset
// Next idea: for (;; NSF) { T_RET; ... } (a7, 143 lines) is byte-exact to 0x00E86976 and then expands ~seh_me in the loop-top pad: find what makes retail's second ~seh_me site call the COMDAT (a cheaper inline before the loop, or a spelling of seh_me retail's macro used)
// Leaf: ?Main@CFisticuffsMember@CV_FisticuffsClubScript@NScript@@EAEXXZ
// Identity: Ego_r.pdb / FableWin.pdb ?Main@CFisticuffsMember@CV_FisticuffsClubScript@NScript@@EAEXXZ (private virtual); entity vtable
//   0x012EB0F8 slot 1 = 0x00E84B80 (its only .rdata reference 0x012EB0FC), stored by the ctor 0x00E83920 (manifest label
//   "CScriptThing_Constructor_Helper" was corrected at landing 2026-11-06n); slot 0 the landed scalar deleting dtor 0x00E839D0, slot 2 Init 0x00E83A20.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E84B80, 8836 bytes incl. the FC_Status jump table 0x00E86DEC (6 entries: case 0 0x00E84DA3,
//   1 0x00E8697B, 2 0x00E85E0A, 3 0x00E84ECE, 4 0x00E863C9, 5 0x00E86807); case order 0, 3, 2, 4, 5, 1 from the temporary flag bits.
// Layout: new engine/CV_FisticuffsClubScript.h over engine/NScript_CV_FisticuffsClubScript.h (FABLE_RETAIL_NSCRIPT).
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_CONCAT_STRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CV_FisticuffsClubScript.h"
#include "engine/CIsThingAlive.h"
#include "engine/CPredicate_And.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CRGBColour.h"
#include "engine/EOpinionDeedType.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_fade.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
extern "C" int __cdecl rand();                             // 0x00BFEB16 (MSVCR71)
extern CDefPointer<const CScriptDef> pGlobalScriptDef;     // 0x0143E90C
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8
namespace NGlobalConsole { extern bool GDefaultToUsingPhysicsInCutsceneAnimations; }   // 0x01375748
namespace NScript {
class CIsVariableFalse : public CPredicateBase {           // vtable 0x012EA43C
public:
    const bool* Variable;                                  // +0x04
    CIsVariableFalse(const bool& variable) : Variable(&variable) { }
    virtual bool operator()() const;                       // slot 0, 0x00E7DDD0
    virtual CPredicateBase* Clone() const;                 // slot 1, 0x00E7DDE0
    virtual ~CIsVariableFalse() { }                        // slot 2, 0x00E7DE10
};
}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing):Thing(thing) {}
template<class P1,class P2> inline NScript::CPredicate_And<P1,P2>::~CPredicate_And() {}
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
__forceinline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
bool FABLE_FASTCALL IsDistanceFromThingToPositionOver(const CScriptThing&, const C3DVector&, float);   // 0x00CBE45C

void NScript::CV_FisticuffsClubScript::CFisticuffsMember::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    SetPredicate(CIsThingAlive(Me) && CIsVariableFalse(ParentClass->EverybodyAttack));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    bool queued_to_fight = false;
    CScriptThing ring = Game->GetThingWithScriptName("FisticuffsClubRing");
    CScriptThing destination = Game->GetThingWithScriptName(Me.GetDataString());
    CTimer leave_timer;
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        switch (FC_Status) {
        case FC_STATUS_REACHING_FC:
            if (IsDistanceBetweenThingsOver(Me, destination, 1.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!seh_me.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToThing(destination, 0.0f, ENTITY_MOVE_RUN, false, true, false, true);
                }
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    {
                        long conversation = Game->AddNewConversation(Me, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, ParentClass->GetQuestText("PUNCH_CLUB_MEMBER_ON_WAY_TO_FC"), false, Me, Game->GetHero()); }
                }
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                FC_Status = FC_STATUS_CHEERING;
            }
            break;
        case FC_STATUS_CHEERING:
            Game->EntitySetFacingAngleTowardsThing(Me, ring, false);
            if (ParentClass->HeroBeatTyler || ParentClass->PlayerLost) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                FC_Status = FC_STATUS_PRE_FIGHT_END;
            }
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), ParentClass->GetQuestText("RND_FRIENDLY_GREETING"), GROUP_SELECT_RANDOM_NO_REPEAT, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    }
                }
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGH)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, ParentClass->GetQuestText("FISTICUFFSMEMBER_ON_HIT_10"), false, Me, Game->GetHero()); }
                ParentClass->BeenAttacked();
                ParentClass->BooNumber += 4.0f;
            }
            if (IsDistanceFromThingToPositionOver(Me, destination.GetPos(), 2.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                C3DVector pos = destination.GetPos();
                while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), pos, 2.0f)) {
                    Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    seh_me.MoveToPosition(pos, 0.0f, ENTITY_MOVE_WALK, false, true);
                    while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (!seh_me.IsPerformingScriptTask() && (!ParentClass->FightFinished || ParentClass->HeroInFisticuffsClub)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (ParentClass->CheerNumber > 1.0f) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (rand() % 5 == 0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        ParentClass->CheerNumber -= 0.3f;
                        switch (rand() % 3) {
                        case 0: seh_me.PlayAnimation("SCRIPT_CHEER_1", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                        case 1: seh_me.PlayAnimation("SCRIPT_CHEER_2", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                        case 2: seh_me.PlayAnimation("SCRIPT_CHEER_3", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                        }
                    } else if (ShoutTimer.GetTime() == 0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.PlayAnimation("ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                        {
                            long conversation = Game->AddNewConversation(Me, false, false);
                            Game->AddPersonToConversation(conversation, Game->GetHero());
                            Game->AddLineToConversation(conversation, CheerName, false, Me, Game->GetHero()); }
                        ShoutTimer = rand() % 25 + 25;
                    }
                } else if (ParentClass->BooNumber > 1.0f) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (rand() % 5 == 0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        ParentClass->BooNumber -= 0.3f;
                        switch (rand() % 3) {
                        case 0: seh_me.PlayAnimation("SCRIPT_BOO_1", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                        case 1: seh_me.PlayAnimation("SCRIPT_BOO_2", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                        case 2: seh_me.PlayAnimation("SCRIPT_BOO_3", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false); break;
                        }
                    } else if (ShoutTimer.GetTime() == 0) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.PlayAnimation("ST_OPINION_NEUTRAL_SHOUTING_WITH_HANDS_CUPPED", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                        {
                            long conversation = Game->AddNewConversation(Me, false, false);
                            Game->AddPersonToConversation(conversation, Game->GetHero());
                            Game->AddLineToConversation(conversation, BooName, false, Me, Game->GetHero()); }
                        ShoutTimer = rand() % 25 + 25;
                    }
                }
            }
            if (!ParentClass->FighterOneInRing && !ParentClass->HeroPaidEntryFee && ParentClass->Fighter1Timer.GetTime() <= 0 && rand() % 25 == 0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetOpinionReactionsEnabled(Me, false);
                Game->EntitySetDeedReactionsEnabled(Me, false);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (!ParentClass->FighterTwoInRing) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    queued_to_fight = true;
                    ParentClass->SFighterOne = Me;
                    ParentClass->FighterOneInRing = true;
                    ParentClass->Fighter2Timer = pGlobalScriptDef->FC_FighterDelay;
                    FC_Status = FC_STATUS_WAITING;
                } else {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass->FighterOneInRing = true;
                    ParentClass->SFighterOne = Me;
                    CScriptThing fighter_marker = Game->GetThingWithScriptName("FighterOne");
                    C3DVector pos = fighter_marker.GetPos();
                    seh_me.MoveToPosition(pos, 3.0f, ENTITY_MOVE_WALK, false, true);
                    while (IsDistanceFromThingToPositionOver(Me, pos, 3.0f)) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        if (!seh_me.IsPerformingScriptTask()) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            seh_me.ClearCommands();
                            seh_me.MoveToPosition(pos, 1.0f, ENTITY_MOVE_WALK, false, true);
                        }
                        if (ParentClass->HeroPaidEntryFee) {
                            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                            break;
                        }
                    }
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    Game->EntitySetFacingAngleTowardsThing(Me, ring, false);
                    ParentClass->ChallengeAccepted = true;
                    ParentClass->FightFinished = false;
                    if (seh_me.IsValid()) seh_me.Release();
                    FC_Status = FC_STATUS_FIGHTING;
                }
            }
            if (!ParentClass->FighterTwoInRing && ParentClass->FighterOneInRing && ParentClass->Fighter2Timer.GetTime() <= 0 && rand() % 25 == 0) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetOpinionReactionsEnabled(Me, false);
                Game->EntitySetDeedReactionsEnabled(Me, false);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ParentClass->FighterTwoInRing = true;
                ParentClass->SFighterTwo = Me;
                CScriptThing fighter_marker = Game->GetThingWithScriptName("FighterTwo");
                C3DVector pos = fighter_marker.GetPos();
                seh_me.MoveToPosition(pos, 3.0f, ENTITY_MOVE_WALK, false, true);
                while (IsDistanceFromThingToPositionOver(Me, pos, 3.0f)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    if (!seh_me.IsPerformingScriptTask()) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.ClearCommands();
                        seh_me.MoveToPosition(pos, 1.0f, ENTITY_MOVE_WALK, false, true);
                    }
                    if (ParentClass->HeroPaidEntryFee) {
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        break;
                    }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetFacingAngleTowardsThing(Me, ring, false);
                ParentClass->ChallengeAccepted = true;
                ParentClass->FightFinished = false;
                if (seh_me.IsValid()) seh_me.Release();
                FC_Status = FC_STATUS_FIGHTING;
            }
            break;
        case FC_STATUS_FIGHTING: {
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    long conversation = Game->AddNewConversation(Me, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, ParentClass->GetQuestText("FISTICUFFSMEMBER_ON_HIT_10"), false, Me, Game->GetHero()); }
                ParentClass->BeenAttacked();
            }
            if (ParentClass->HeroPaidEntryFee) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntityTeleportToPosition(Me, destination.GetPos(), 0.0f, false, false);
                FC_Status = FC_STATUS_CHEERING;
            }
            const CScriptDef* def = pGlobalScriptDef.Get();
            if (Game->GetHealth(Me) < def->FC_MinimumFighterHealth) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->ModifyThingHealth(Me, 15.0f, false);
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                {
                    C3DVector pos = destination.GetPos();
                    while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), pos, 2.1f)) {
                        Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.MoveToPosition(pos, 0.1f, ENTITY_MOVE_WALK, false, true);
                        while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetOpinionReactionsEnabled(Me, true);
                Game->EntitySetDeedReactionsEnabled(Me, true);
                FC_Status = FC_STATUS_CHEERING;
            } else if (ParentClass->FightFinished && (Me == ParentClass->SFighterOne || Me == ParentClass->SFighterTwo)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), pGlobalScriptDef->FC_ShoutOutDistance)) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    {
                        long conversation = Game->AddNewConversation(Me, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, ParentClass->GetQuestText("FIGHTER_AWAITING_NEXT_FIGHT_10"), false, Me, Game->GetHero()); }
                }
                queued_to_fight = true;
                ParentClass->SFighterOne = Me;
                ParentClass->FighterOneInRing = true;
                ParentClass->Fighter2Timer = pGlobalScriptDef->FC_FighterDelay;
                ParentClass->FighterTwoInRing = false;
                ParentClass->ChallengeAccepted = false;
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                FC_Status = FC_STATUS_WAITING;
            }
            break;
        }
        case FC_STATUS_WAITING:
            if (ParentClass->HeroPaidEntryFee) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntityTeleportToPosition(Me, destination.GetPos(), 0.0f, false, false);
                queued_to_fight = false;
                FC_Status = FC_STATUS_CHEERING;
            } else if (queued_to_fight) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (seh_me.IsValid()) seh_me.Release();
                while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                CScriptThing fighter_marker = Game->GetThingWithScriptName("FighterOne");
                {
                    C3DVector pos = fighter_marker.GetPos();
                    while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), pos, 2.1f)) {
                        Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.MoveToPosition(pos, 0.1f, ENTITY_MOVE_WALK, false, true);
                        while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetFacingAngleTowardsThing(Me, ring, false);
                while (!ParentClass->ChallengeAccepted && !ParentClass->HeroPaidEntryFee && ParentClass->FisticuffsActive) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetFacingAngleTowardsThing(Me, ring, false);
                if (!ParentClass->HeroPaidEntryFee && ParentClass->FisticuffsActive) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    ParentClass->FightFinished = false;
                    if (seh_me.IsValid()) seh_me.Release();
                    ParentClass->AddSpawnedFunction(new CSpawnedFunc<CV_FisticuffsClubScript>(CCharString("ParentClass.") + CCharString("FisticuffsChallenge"), 0, ParentClass, &CV_FisticuffsClubScript::FisticuffsChallenge), CCharString(""));
                    FC_Status = FC_STATUS_FIGHTING;
                }
            }
            break;
        case FC_STATUS_PRE_FIGHT_END:
            Me.SetFriendsWithEverythingFlag(true);
            Game->ClearThingBestEnemyTarget(Me);
            if (ParentClass->HeroBeatTyler) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetPersonalityOverride(Me, "OPINION_PERSONALITY_PUNCHCLUB_MEMBER_WON");
            } else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                Game->EntitySetPersonalityOverride(Me, "OPINION_PERSONALITY_PUNCHCLUB_MEMBER_LOST");
            }
            Game->EntitySetStategroupEnabled(Me, "SG_MINION_WANDER", true);
            seh_me.Release();
            FC_Status = FC_STATUS_LEAVING_FC;
            break;
        case FC_STATUS_LEAVING_FC:
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            seh_me.MoveToThing(destination, 0.0f, ENTITY_MOVE_RUN, false, true, false, true);
            leave_timer = 60;
            while (leave_timer.GetTime() > 0 && IsDistanceBetweenThingsOver(Me, destination, 1.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    {
                        long conversation = Game->AddNewConversation(Me, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, ParentClass->GetQuestText("PUNCH_CLUB_MEMBER_ON_WAY_HOME"), false, Me, Game->GetHero()); }
                }
                if (!seh_me.IsPerformingScriptTask()) {
                    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                    destination = Game->GetRandomThingWithScriptName("FisticuffsUseThis");
                    seh_me.MoveToThing(destination, 0.0f, ENTITY_MOVE_RUN, false, true, false, true);
                }
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            if (Game->IsCameraPosOnScreen(Me.GetPos()) || IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 15.0f)) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                destination = Game->GetRandomThingWithScriptName("FisticuffsUseThis");
                {
                    C3DVector pos = destination.GetPos();
                    while (IsDistanceFromThingToPositionOver(seh_me.GetScriptThing(), pos, 5.0f)) {
                        Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                        seh_me.MoveToPosition(pos, 3.0f, ENTITY_MOVE_RUN, false, true);
                        while (seh_me.IsPerformingScriptTask()) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                        if (IsActiveThreadTerminating()) return _Fail_Val.Return(); }
                }
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            FadeOutAndKillEntity(Me, 1.0f, true, true);
            return _Fail_Val.Return();
        }
        if (!ParentClass->FisticuffsActive && !ParentClass->HeroInFisticuffsClub) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            FC_Status = FC_STATUS_LEAVING_FC;
            destination = Game->GetRandomThingWithScriptName("FighterAppearsHere");
        }
    }
}
