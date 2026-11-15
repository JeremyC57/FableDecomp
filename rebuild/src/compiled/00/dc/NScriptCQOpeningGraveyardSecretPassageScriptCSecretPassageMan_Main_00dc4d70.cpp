// NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageMan::Main() -- the graveyard gate man: with the conscious predicate, wander near home until the gravekeeper is introduced or he has been talked to (GATEMAN_ASIDE on talk; run to GraveFriendLeavingMarker, PRE_DOOR_ATTACKED line and fade out when the hero hits him (heal-life excepted); PRE_DOOR_RUN_AWAY and fade once the visitors were attacked), then walk to the marker answering talk with the ASIDE_LEAVING_FIRST / _SECOND lines, and fade out when he arrives
// Inline-budget: scoped `#pragma inline_depth(0)` round the row-local CIsThingConscious ctor / dtor with FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
//   (the landed predicate idiom; the matched twin CSecretPassageWoman::Main 0x00DC5F80 carries the same lines): retail expands the
//   predicate ctor / dtor (vtable store 0x012C2FE8 only) but CALLS the nested CScriptThing copy 0x004ABE90 / ~CScriptThing 0x004AA840
//   while it expands the by-value CScriptThing copies in the body (FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY, existing repo opt-in).
// Leaf: ?Main@CSecretPassageMan@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DC4D70, 4319 bytes, thiscall, plain `ret`, no EH frame (sub esp,0x84; every exit runs its own
//   destructor cascade).
// Identity (proven): slot 1 (Main) of the CSecretPassageMan entity vtable 0x012DA6A8 (the only reference to the row is .rdata 0x012DA6AC),
//   stored by the ctor inside CSecretPassageMan::Alloc 0x00DC4C40 (0x00DC4C76); slot 0 sdd 0x00DC4D50, 2 Init 0x00DC4CF0, 3 GetParentScript
//   0x00DC4CE0; Ego_r.pdb / FableWin.pdb name set ?Main@CSecretPassageMan@CQ_OpeningGraveyardSecretPassageScript@NScript@@EAEXXZ; the
//   GATEMAN text family (the sibling CSecretPassageWoman 0x00DC5F80, parked in hand790, speaks the GATEWOMAN lines with the same body).
// Callees (retail census): CEntityScriptBase::SetPredicate 0x00F35B10 / IsActiveThreadTerminating 0x00F35B30 (x39); CScriptThing copy
//   0x004ABE90 / ~ 0x004AA840; CBaseClass ~ 0x0099A2E0; CScriptGameResourceObjectMovieBase ctor 0x006E7B60 / ~ 0x006E7B80,
//   ~CCountedPointer<CScriptGameResourceObjectMovieBase> 0x006E7AB0 (declared-only specialization); ScriptedThingBase IsValid 0x00CD23B9 /
//   Release 0x00CD2770 / MoveToPosition 0x007E72F0 / ClearCommands 0x007E7360 (x4) / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 /
//   GetScriptThing 0x007E7490 / ~ 0x007E74D0; CBaseObject 0x0099A380 / ~ 0x0099A430; CCharString 0x0099EBF0 / ~ 0x0099EAE0; operator
//   delete 0x00BFE9BC; FadeOutAndKillEntity 0x00CBE716 (x4, ?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z).
// Layout: engine/CQ_OpeningGraveyardSecretPassageScript.h (repo, unchanged; ParentClass is a reference): CSecretPassageMan AttackedByHero
//   +0x1c; parent GravekeeperIntroduced, VisitorsAttacked. FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR.
// Shape: first loop `while (!GravekeeperIntroduced && !HaveTalked)` with talk / hit / else-if VisitorsAttacked arms (the run-away arms each
//   fetch FriendEscape and call ClearCommands); after it FriendEscape is fetched once more and the second loop is
//   `for (;; Game->NewScriptFrame()) { if (T) return; ... }` (EVIDENCED: the generated script-row idiom, as the matched twin 0x00DC5F80;
//   its frequency model keeps the const-0 piece out of ebp across the first loop's hit / run-away arms: retail `test ecx,ecx` at
//   0x00DC52CD / 0x00DC549F, the arms' exit pads zero esi locally at 0x00DC56F5 / 0x00DC5722, `xor ebp,ebp` at 0x00DC5582; the
//   `if (T) return; while (true) { ...; NewScriptFrame(); if (T) return; }` spelling gives 4255v4319), with the leaving asides in an
//   inner block that closes before MoveToPosition.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CPREDICATEBASE_NOVTABLE
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
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
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EHeroAbility.h"
#include "engine/EScriptEntityMoveType.h"
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing):Thing(thing) {}
inline NScript::CIsThingConscious::~CIsThingConscious() {}
#ifdef _MSC_VER
#pragma inline_depth()
#endif
inline CScriptThing::~CScriptThing() {}
inline const C3DVector& CScriptThing::GetPos() const { if(PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/fable_script_fade.h"   // FadeOutAndKillEntity 0x00CBE716 (__fastcall)

void NScript::CQ_OpeningGraveyardSecretPassageScript::CSecretPassageMan::Main()
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
                seh_me.Speak(Game->GetHero(), "TEXT_QST_049_GATEMAN_ASIDE", GROUP_SELECT_NONE, false, true, false);
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
            CScriptThing FriendEscape = Game->GetThingWithScriptName("GraveFriendLeavingMarker");
            seh_me.MoveToPosition(FriendEscape.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
            long Conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(Conversation, Game->GetHero());
            Game->AddLineToConversation(Conversation, "TEXT_QST_049_GATEMAN_PRE_DOOR_ATTACKED", false, Me, Game->GetHero());
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
            seh_me.ClearCommands();
            AttackedByHero = true;
            CScriptThing FriendEscape = Game->GetThingWithScriptName("GraveFriendLeavingMarker");
            seh_me.MoveToPosition(FriendEscape.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
            long Conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(Conversation, Game->GetHero());
            Game->AddLineToConversation(Conversation, "TEXT_QST_049_GATEMAN_PRE_DOOR_RUN_AWAY", false, Me, Game->GetHero());
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
    CScriptThing FriendEscape = Game->GetThingWithScriptName("GraveFriendLeavingMarker");
    seh_me.MoveToPosition(FriendEscape.GetPos(), 3.0f, ENTITY_MOVE_WALK, false, true);
    bool HaveSpoken = false;
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return;
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
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_049_GATEMAN_ASIDE_LEAVING_SECOND", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
            } else {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_049_GATEMAN_ASIDE_LEAVING_FIRST", GROUP_SELECT_NONE, false, true, false);
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
            Game->AddLineToConversation(Conversation, "TEXT_QST_049_GATEMAN_PRE_DOOR_ATTACKED", false, Me, Game->GetHero());
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
            seh_me.ClearCommands();
            AttackedByHero = true;
            seh_me.MoveToPosition(FriendEscape.GetPos(), 3.0f, ENTITY_MOVE_RUN, false, true);
            long Conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(Conversation, Game->GetHero());
            Game->AddLineToConversation(Conversation, "TEXT_QST_049_GATEMAN_PRE_DOOR_RUN_AWAY", false, Me, Game->GetHero());
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
    }
}