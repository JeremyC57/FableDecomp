// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageWoman::Main() -- the graveyard gate woman: with the conscious predicate, wander near home until the gravekeeper is introduced or she has been talked to (aside line on talk; run to the escape marker, ATTACKED line and fade out when the hero hits her or uses heal-life on her; RUN_AWAY and fade once the visitors were attacked), then walk to the escape marker, answering talk with the LEAVING_FIRST / LEAVING_SECOND asides, and fade out when she arrives
// Grade: DIFFER(4259v4245) -- 36 difflines: retail expands the predicate temporary CIsThingConscious(Me) ctor/dtor inline but CALLS the nested CScriptThing copy 0x004ABE90 (after `mov ebx,0`) and dtor 0x004AA840 inside it, while VC7.1 here expands that copy (the same TU must expand it at the 4 by-value Set*Wander* pushes: GOTCHAS 1913/1933 open); knock-on: retail keeps ebp == 0 through the function (3 cmp ecx,ebp vs our test ecx,ecx; 2 extra xor ebp,ebp).
// Next idea: a CIsThingConscious ctor taking the thing through a non-inline path that VC7.1 still expands one level (e.g. the predicate ctor defined inline in the header with a member-init of a CScriptThing whose copy ctor is declared but defined after Main, plus FABLE_RETAIL_* opt-in for the by-value pushes) -- or wait for the GOTCHAS 1913/1933 recipe.
// Leaf: ?Main@CSecretPassageWoman@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC5F80, 4245 bytes, thiscall, EH frame (predicate, seh_me, FriendEscape, movie, pause locals).
// Identity (proven): slot 1 (Main) of the entity-script vtable 0x012DA7A4 (only reference to 0x00DC5F80 is .rdata 0x012DA7A8),
//   stored by the CSecretPassageWoman ctor at 0x00DC5E86 (Alloc 0x00DC5E50; Init 0x00DC5F00 slot 2, GetParentScript 0x00DC5EF0
//   slot 3); Ego_r.pdb / FableWin.pdb name set ?Main@CSecretPassageWoman@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ;
//   registration "SecretPassageWoman" at 0x00DC2B40. (The chunk oracle row spells the class `CSecretPass`: a truncated name.)
// Callees: CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30 (x38), CScriptThing copy 0x004ABE90 /
//   dtor 0x004AA840, CScriptGameResourceObjectScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770 / ClearCommands 0x007E7360 /
//   ~ 0x007E74D0 and its virtuals IsPerformingScriptTask 0x007E7450, MoveToPosition 0x007E72F0, Speak 0x007E7390, GetScriptThing
//   0x007E7490; CScriptGameResourceObjectMovieBase Construct 0x006E7B60 / DestroyOutOfLine 0x006E7B80, ~CCountedPointer<
//   CScriptGameResourceObjectMovieBase> 0x006E7AB0 (declared-only specialization), CBaseObject ctor 0x0099A380 / dtor 0x0099A430,
//   CBaseClass dtor 0x0099A2E0, FadeOutAndKillEntity 0x00CBE716 (fastcall, x3), CCharString 0x0099EBF0 / ~ 0x0099EAE0, operator
//   delete 0x00BFE9BC; Game slots 21 MsgIsHitBy, 27 MsgIsTalkedToBy, 264 GetHealth, 364 AddNewConversation and the wander /
//   brain / conversation / pause slots. 23 string literals, same order as retail (paired by instruction order: strchk's offset pairing
//   does not apply to a DIFFER row); float constants: 0.0f from memory, 1.0f / 3.0f / 4.0f as immediates.
// Layout: engine/CQ_OpeningGraveyardSecretPassageScript.h (unchanged; ParentClass is a reference): CSecretPassageWoman
//   AttackedByHero +0x1c; parent GravekeeperIntroduced +0x4d, VisitorsAttacked +0x70. engine/CWideScreenMagicPauseEntities.h with
//   FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR (the pause ctor expanded as retail).
// Shape: first loop `while (!GravekeeperIntroduced && !HaveTalked)` with talk / hit / else-if run-away arms; the second loop is
//   `if (T) return; while (true) { ...; Game->NewScriptFrame(); if (T) return; }` so the second movie ctor is the Construct call; the
//   movie + pause live in an inner block that closes before MoveToPosition (their dtor order then matches).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/EScriptingStateGroups.h"
#include "engine/CQ_OpeningGraveyardSecretPassageScript.h"
#include "engine/CIsThingConscious.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) {}
inline CScriptThing::~CScriptThing() {}
inline const C3DVector& CScriptThing::GetPos() const { if(PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&,float,bool,bool);

void NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageWoman::Main()
{
    SetPredicate(CIsThingConscious(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    Game->SetCreatureBrain(Me, "BRAIN_PASSIVE_OVERRIDE");
    Game->SetWanderCentrePoint(Me, Me.GetHomePos());
    Game->SetWanderMinDistance(Me, 0.0f);
    Game->SetWanderMaxDistance(Me, 4.0f);
    Game->SetScriptingStateGroup(Me, ESSG_WANDER_NEAR);
    CScriptThing FriendEscape = Game->GetThingWithScriptName("GraveFriendLeavingMarker");
    bool HaveTalked = false;
    while (!ParentClass.GravekeeperIntroduced && !HaveTalked) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_049_GATEWOMAN_ASIDE", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            HaveTalked = true;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            Game->SetCreatureBrain(Me, "BRAIN_GOOD_VILLAGER_BASE");
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            AttackedByHero = true;
            ParentClass.VisitorsAttacked = true;
            seh_me.MoveToPosition(FriendEscape.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
            long Conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(Conversation, Game->GetHero());
            Game->AddLineToConversation(Conversation, "TEXT_QST_049_GATEWOMAN_PRE_DOOR_ATTACKED", false, Me, Game->GetHero());
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            FadeOutAndKillEntity(Me, 1.0f, true, true);
        } else if (ParentClass.VisitorsAttacked) {
            if (IsActiveThreadTerminating()) return;
            Game->SetCreatureBrain(Me, "BRAIN_GOOD_VILLAGER_BASE");
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            AttackedByHero = true;
            seh_me.MoveToPosition(FriendEscape.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
            long Conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(Conversation, Game->GetHero());
            Game->AddLineToConversation(Conversation, "TEXT_QST_049_GATEWOMAN_PRE_DOOR_RUN_AWAY", false, Me, Game->GetHero());
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            FadeOutAndKillEntity(Me, 1.0f, true, true);
        }
    }
    if (IsActiveThreadTerminating()) return;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    seh_me.MoveToPosition(FriendEscape.GetPos(), 3.0f, ENTITY_MOVE_WALK, false, true);
    bool HaveSpoken = false;
    if (IsActiveThreadTerminating()) return;
    while (true) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            seh_me.ClearCommands();
            {
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (HaveSpoken) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_049_GATEWOMAN_ASIDE_LEAVING_SECOND", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            } else {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_049_GATEWOMAN_ASIDE_LEAVING_FIRST", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                HaveSpoken = true;
            }
            }
            seh_me.MoveToPosition(FriendEscape.GetPos(), 3.0f, ENTITY_MOVE_WALK, false, true);
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            Game->SetCreatureBrain(Me, "BRAIN_GOOD_VILLAGER_BASE");
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            seh_me.ClearCommands();
            AttackedByHero = true;
            ParentClass.VisitorsAttacked = true;
            seh_me.MoveToPosition(FriendEscape.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
            long Conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(Conversation, Game->GetHero());
            Game->AddLineToConversation(Conversation, "TEXT_QST_049_GATEWOMAN_PRE_DOOR_ATTACKED", false, Me, Game->GetHero());
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            FadeOutAndKillEntity(Me, 1.0f, true, true);
        } else if (ParentClass.VisitorsAttacked) {
            if (IsActiveThreadTerminating()) return;
            Game->SetCreatureBrain(Me, "BRAIN_GOOD_VILLAGER_BASE");
            if (seh_me.IsValid()) seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            AttackedByHero = true;
            seh_me.MoveToPosition(FriendEscape.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
            long Conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(Conversation, Game->GetHero());
            Game->AddLineToConversation(Conversation, "TEXT_QST_049_GATEWOMAN_PRE_DOOR_RUN_AWAY", false, Me, Game->GetHero());
            while (seh_me.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            FadeOutAndKillEntity(Me, 1.0f, true, true);
        }
        if (!seh_me.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return;
            FadeOutAndKillEntity(Me, 1.0f, true, true);
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
}
