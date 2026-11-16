// NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::Main() -- the Arena entrance guard: greets the hero, and while the Arena quest is active asks whether to enter (no non-summoned followers allowed: fade out, the quest start screen, teleport into the Hall of Heroes), otherwise points at the quest state; when hit he complains, heals and stays friendly; after the first talk he switches to the return lines and clears his marker once the quest is done
// Inline-budget: flags /d2inlS15 /d2inlT69 -- /d2inlS15 is the landed same-TU fan Mains' flag (COakvaleFan / CBowerstoneFan / CHookCoastFan ::Main 0x00CF18D0 / 0x00CF2B70 / 0x00CF3D20: VC7.1's caller IL-size cap, which keeps the implicit MovieBase dtor out of line by default); /d2inlT69 is per row: with S15 alone the only byte difference is ONE inline decision, the seh_me dtor at the hit-2 `if (IsActiveThreadTerminating()) return;` (retail 0x00CF84D6 jumps to the shared out-of-line ~CScriptGameResourceObjectScriptedThingBase exit 0x00CF8832; ours expanded it and joined the expanded pad 0x00CF8740). Measured window with S15: T69..T73 RELOCATION_MATCH, T74..T80 that one site, T<=68 / T>=85 far off; T69 is the smallest. Without S15 no T matches (T66 / T70 / T72: 2,000+ byte differences).
// Shape (EVIDENCED by retail bytes): in both loops the hit test is the ELSE of the talk test -- the talk block's end jumps past it (intro loop 0x00CF75C0 `jmp 0x00CF786D` to the loop test; second loop 0x00CF83E2 `jmp 0x00CF8699` to the DoneArena test), so `if (talked) { ... } else if (hit) { ... }`. Every earlier draft tested the hit after the talk block, which cost 185 hunks of register allocation (the FE flags word left ebx, the talked / follower bools took bl).
// Leaf: ?Main@CEntranceGuard@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ
// Flags: /d2inlS15 /d2inlT69
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
#include "engine/ETextGroupSelectionMethod.h"
#include "engine/EHeroAbility.h"
template<> CCountedPointer<CScriptThing>::CCountedPointer(const CCountedPointer<CScriptThing>&);
inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(other), PImp(other.PImp) { }
void NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->SetThingHasInformation(Me, false, true, false);
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
    while (!IntroDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_MAIN_GREETING", GROUP_SELECT_NONE, false, true, false);
                while (seh_me.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                }
                if (IsActiveThreadTerminating()) return;
            }
            if (Game->IsQuestActive("Q_Arena")) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_MAIN_ENTRY", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game->GiveHeroYesNoQuestion("TEXT_QST_005_V2_ENTRANCE_GUARD_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                long answer = Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer < 0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                    answer = Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return;
                if (answer == 1) {
                    if (IsActiveThreadTerminating()) return;
                    bool has_followers = false;
                    std::vector<CScriptThing> followers;
                    Game->GetFollowingEntityList(Game->GetHero(), followers);
                    for (unsigned int i = 0; i < followers.size(); i++) {
                        if (IsActiveThreadTerminating()) return;
                        if (!followers[i].IsSummonedCreature()) {
                            if (IsActiveThreadTerminating()) return;
                            has_followers = true;
                        }
                    }
                    if (IsActiveThreadTerminating()) return;
                    if (has_followers) {
                        if (IsActiveThreadTerminating()) return;
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_NO_FOLLOWERS", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_COME_IN", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                        Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.5f);
                        Game->Pause(0.5f);
                        Game->KickOffQuestStartScreen("Q_Arena", true, false);
                        Game->Pause(1.0f);
                        movie.Release();
                        Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("ARENA_HallOfHeroesEntrance"), false);
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_RETURN", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
                }
            } else if (Game->IsQuestCompleted("Q_Arena")) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_WELL_DONE", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
            } else if (PMasterData->WhiteBalverineFinished) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_NEED_QUEST", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
            } else {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_NEED_MORE", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
            }
            IntroDone = true;
        } else if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
            if (IsActiveThreadTerminating()) return;
            if (!ParentClass.InHitCutsceneAlready) {
                if (IsActiveThreadTerminating()) return;
                ParentClass.InHitCutsceneAlready = true;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_ATTACKED", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game->ModifyThingHealth(Me, 10000.0f, false);
                Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
                ParentClass.InHitCutsceneAlready = false;
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            if (Game->IsQuestActive("Q_Arena")) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_RETURN_GREETING", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game->GiveHeroYesNoQuestion("TEXT_QST_005_V2_ENTRANCE_GUARD_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
                long answer = Game->MsgIsQuestionAnsweredYesOrNo();
                while (answer < 0) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return;
                    answer = Game->MsgIsQuestionAnsweredYesOrNo();
                }
                if (IsActiveThreadTerminating()) return;
                if (answer == 1) {
                    if (IsActiveThreadTerminating()) return;
                    bool has_followers = false;
                    std::vector<CScriptThing> followers;
                    Game->GetFollowingEntityList(Game->GetHero(), followers);
                    for (unsigned int i = 0; i < followers.size(); i++) {
                        if (IsActiveThreadTerminating()) return;
                        if (!followers[i].IsSummonedCreature()) {
                            if (IsActiveThreadTerminating()) return;
                            has_followers = true;
                        }
                    }
                    if (IsActiveThreadTerminating()) return;
                    if (has_followers) {
                        if (IsActiveThreadTerminating()) return;
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_NO_FOLLOWERS", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                        Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
                    } else {
                        if (IsActiveThreadTerminating()) return;
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_COME_IN", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                        Game->FadeScreenOutUntilNextCallToFadeScreenIn(0.5f, 0.5f);
                        Game->Pause(0.5f);
                        Game->KickOffQuestStartScreen("Q_Arena", true, false);
                        Game->Pause(1.0f);
                        movie.Release();
                        Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("ARENA_HallOfHeroesEntrance"), false);
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_RETURN", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
                }
            } else if (Game->IsQuestCompleted("Q_Arena")) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_RETURN_WELL_DONE", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
            } else if (PMasterData->WhiteBalverineFinished) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_RETURN_NEED_QUEST", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
            } else {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_RETURN_NEED_MORE", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
            }
        } else if ((Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO")))) {
            if (IsActiveThreadTerminating()) return;
            if (!ParentClass.InHitCutsceneAlready) {
                if (IsActiveThreadTerminating()) return;
                ParentClass.InHitCutsceneAlready = true;
                CScriptGameResourceObjectMovieBase movie;
                Game->StartMovieSequence("", movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD_ATTACKED", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) return;
                    }
                    if (IsActiveThreadTerminating()) return;
                }
                Game->CameraDefault();
                Game->ModifyThingHealth(Me, 100.0f, false);
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
                ParentClass.InHitCutsceneAlready = false;
            }
        }
        if (!DoneArena && Game->IsQuestCompleted("Q_Arena")) {
            if (IsActiveThreadTerminating()) return;
            Game->ClearThingHasInformation(Me);
            Game->MiniMapRemoveMarker(Me);
            DoneArena = true;
        }
    }
}