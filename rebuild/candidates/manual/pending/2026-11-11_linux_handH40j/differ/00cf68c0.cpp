// NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::Main() -- the Arena entrance guard: greets the hero, and while the Arena quest is active asks whether to enter (no non-summoned followers allowed: fade out, the quest start screen, teleport into the Hall of Heroes), otherwise points at the quest state; when hit he complains, heals and stays friendly; after the first talk he switches to the return lines and clears his marker once the quest is done
// Grade: DIFFER(8150v8070) -- 864 masked lines, 392 hunks (1472 before; chunk H40j, 5 grades). Levers: the second loop `for (;; NSF) { if (T) return;` keeping the pre-loop check; FABLE_RETAIL_..._MOVIEBASE_FORCEINLINE_CTOR (retail expands all 4 movie ctors), with the CCountedPointer<Movie> dtor specialization declared before the movie header (otherwise C2908). Left: the movie-dtor call / expand split (retail expands the implicit MovieBase dtor at all 6 sites; inline trace s71..s460 against T74) and the frame slots.
// Next idea: make the talk-block movies share retail's slot (+0x14, below seh_me) so the pads merge, then lift the remaining dtor sites over the threshold with `&& true` only where the trace says s is between 74 and 150.
// Leaf: ?Main@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF68C0, 8070 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): slot 1 (Main) of the entity vtable 0x012C6158 that the landed CEntranceGuard::Alloc 0x00CF0B60 stores (slot 0
//   ??_G 0x00CF0C20, 2 Init 0x00CF0C10, 3 GetParentScript 0x00CF0C00, 4 OnPersist 0x00CF6890); Ego_r.pdb / FableWin.pdb hold
//   ?Main@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ; IntroDone / DoneArena (+0x1c / +0x1d) are the bools the landed Init clears.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CQ_ARENAHOLDINGSCRIPTSCRIPT_ENTRANCEGUARD
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_ArenaHoldingScriptScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
template<> CCountedPointer<CScriptThing>::CCountedPointer(const CCountedPointer<CScriptThing>&);
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
#define T IsActiveThreadTerminating()
#define NSF Game->NewScriptFrame()
#define EG_SAY(TEXT) \
    if (Game->GetHealth(seh_me.GetScriptThing())>0.0f) { \
        seh_me.Speak(Game->GetHero(),TEXT,GROUP_SELECT_NONE,false,true,false); \
        while (seh_me.IsPerformingScriptTask()) { \
            NSF; \
            if (T) return; \
        } \
        if (T) return; \
    }
#define EG_MOVIE \
    CScriptGameResourceObjectMovieBase movie; \
    Game->StartMovieSequence("",movie); \
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
#define EG_FACE Game->EntitySetFacingAngleTowardsThing(Me,Game->GetThingWithScriptName("GuardStareMarker"),false);
#define EG_ANSWER \
    Game->GiveHeroYesNoQuestion("TEXT_QST_005_V2_ENTRANCE_GUARD_QUESTION","TEXT_OBJECT_HERO_ANSWER_YES","TEXT_OBJECT_HERO_ANSWER_NO","",true); \
    long answer=Game->MsgIsQuestionAnsweredYesOrNo(); \
    while (answer<0) { \
        NSF; \
        if (T) return; \
        answer=Game->MsgIsQuestionAnsweredYesOrNo(); \
    } \
    if (T) return;
#define EG_ENTER \
    if (T) return; \
    bool has_followers=false; \
    std::vector<CScriptThing> followers; \
    Game->GetFollowingEntityList(Game->GetHero(),followers); \
    for (unsigned int i=0;i<followers.size();i++) { \
        if (T) return; \
        if (!followers[i].IsSummonedCreature()) { \
            if (T) return; \
            has_followers=true; \
        } \
    } \
    if (T) return; \
    if (has_followers) { \
        if (T) return; \
        EG_SAY("TEXT_QST_005_V2_ENTRANCE_GUARD_NO_FOLLOWERS") \
        EG_FACE \
    } \
    else { \
        if (T) return; \
        EG_SAY("TEXT_QST_005_V2_ENTRANCE_GUARD_COME_IN") \
        Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f,0.5f); \
        Game->Pause(0.5f); \
        Game->KickOffQuestStartScreen("Q_Arena",true,false); \
        Game->Pause(1.0f); \
        movie.Release(); \
        Game->EntityTeleportToThing(Game->GetHero(),Game->GetThingWithScriptName("ARENA_HallOfHeroesEntrance"),false); \
    }
#define EG_LINE(TEXT) \
    if (T) return; \
    EG_SAY(TEXT) \
    EG_FACE
#define EG_HIT (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL,"SCRIPT_NAME_HERO")))
void NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me,seh_me,SCRIPT_AI_PRIORITY_HIGHEST)) {
        NSF;
        if (T) return;
    }
    if (T) return;
    Game->SetThingHasInformation(Me,false,true,false);
    Game->EntitySetAsKillable(Me,false,false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me,false);
    Game->SetIsPushableByHero(Me,false);
    EG_FACE
    while (!IntroDone) {
        NSF;
        if (T) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            EG_MOVIE
            EG_SAY("TEXT_QST_005_V2_ENTRANCE_GUARD_MAIN_GREETING")
            if (Game->IsQuestActive("Q_Arena")) {
                if (T) return;
                EG_SAY("TEXT_QST_005_V2_ENTRANCE_GUARD_MAIN_ENTRY")
                EG_ANSWER
                if (answer==1) {
                    EG_ENTER
                }
                else {
                    EG_LINE("TEXT_QST_005_V2_ENTRANCE_GUARD_RETURN")
                }
            }
            else if (Game->IsQuestCompleted("Q_Arena")) {
                EG_LINE("TEXT_QST_005_V2_ENTRANCE_GUARD_WELL_DONE")
            }
            else if (PMasterData->WhiteBalverineFinished) {
                EG_LINE("TEXT_QST_005_V2_ENTRANCE_GUARD_NEED_QUEST")
            }
            else {
                EG_LINE("TEXT_QST_005_V2_ENTRANCE_GUARD_NEED_MORE")
            }
            IntroDone=true;
        }
        if (EG_HIT) {
            if (T) return;
            if (!ParentClass.InHitCutsceneAlready) {
                if (T) return;
                ParentClass.InHitCutsceneAlready=true;
                EG_MOVIE
                EG_SAY("TEXT_QST_005_V2_ENTRANCE_GUARD_ATTACKED")
                Game->ModifyThingHealth(Me,10000.0f,false);
                Game->EntitySetThingAsAllyOfThing(Me,Game->GetHero());
                EG_FACE
                ParentClass.InHitCutsceneAlready=false;
            }
        }
    }
    if (T) return;
    for (;;NSF) {
        if (T) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (T) return;
            EG_MOVIE
            if (Game->IsQuestActive("Q_Arena")) {
                if (T) return;
                EG_SAY("TEXT_QST_005_V2_ENTRANCE_GUARD_RETURN_GREETING")
                EG_ANSWER
                if (answer==1) {
                    EG_ENTER
                }
                else {
                    EG_LINE("TEXT_QST_005_V2_ENTRANCE_GUARD_RETURN")
                }
            }
            else if (Game->IsQuestCompleted("Q_Arena")) {
                EG_LINE("TEXT_QST_005_V2_ENTRANCE_GUARD_RETURN_WELL_DONE")
            }
            else if (PMasterData->WhiteBalverineFinished) {
                EG_LINE("TEXT_QST_005_V2_ENTRANCE_GUARD_RETURN_NEED_QUEST")
            }
            else {
                EG_LINE("TEXT_QST_005_V2_ENTRANCE_GUARD_RETURN_NEED_MORE")
            }
        }
        if (EG_HIT) {
            if (T) return;
            if (!ParentClass.InHitCutsceneAlready) {
                if (T) return;
                ParentClass.InHitCutsceneAlready=true;
                EG_MOVIE
                EG_SAY("TEXT_QST_005_V2_ENTRANCE_GUARD_ATTACKED")
                Game->CameraDefault();
                Game->ModifyThingHealth(Me,100.0f,false);
                EG_FACE
                ParentClass.InHitCutsceneAlready=false;
            }
        }
        if (!DoneArena && Game->IsQuestCompleted("Q_Arena")) {
            if (T) return;
            Game->ClearThingHasInformation(Me);
            Game->MiniMapRemoveMarker(Me);
            DoneArena=true;
        }
    }
}
