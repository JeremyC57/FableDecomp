// NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard2::Main() -- the second Arena entrance guard: take scripted control, join FACTION_HERO, unkillable, no combo multiplier, not pushable, face GuardStareMarker; every frame: when talked to say TEXT_QST_005_V2_ENTRANCE_GUARD2_RETURN_WELL_DONE once Q_Arena is complete, else GREETING the first time and GREETING_AGAIN after; when hit by the hero (except a heal-life spell) and no hit cut-scene is running say ATTACKED, reset the camera and heal 100
// Grade: DIFFER(2655v2755) -- 503 masked difflines: VC7.1's function-wide inline budget. Retail expands all eight implicit CScriptGameResourceObjectMovieBase dtor sites (`call 0x006E7AB0; GROBase vtable; call 0x0099A430`, the normal paths tail-merged into one `call 0x0099A430` and each scope's two return pads cross-jumped), hoists the movie vtable in ebp and spills the talked-to bool (frame 0xf0); ours calls the dtor COMDAT 0x006E7B80 at every site, ebp is the zero reg, frame 0xec. The same TU's fan Mains (0x00CF18D0 / 0x00CF2B70 / 0x00CF3D20, chunk 804) are parked on the same gap.
// Next idea: none without a per-TU inline knob; meters: /d2inlS20 (diagnostic only) matches the whole body and leaves only the exit-pad cross-jumping (md60); MOVIEBASE_FORCEINLINE expands every site but adds a MovieBase vtable reset per site (e4, md81); STB OUTOFLINE_DTOR frees budget for the movie dtors but calls ~seh_me where retail expands it (e6); TRetVoid returns (e3) and dropping the declared ~CCountedPointer<MovieBase> (e5) change nothing
// Leaf: ?Main@CEntranceGuard2@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF0D20, 2755 bytes, thiscall, plain `ret` (two epilogue copies), frame 0xf0.
// Identity (proven): Ego_r.pdb ?Main@CEntranceGuard2@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ; slot 1 of the CEntranceGuard2 entity
//   vtable 0x012C6174 stored by the landed Alloc 0x00CF0C40.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CQ_ARENAHOLDINGSCRIPTSCRIPT_CENTRANCEGUARD2
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_ArenaHoldingScriptScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/ETextGroupSelectionMethod.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };

void NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard2::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetInFaction(Me, "FACTION_HERO");
    Game->EntitySetAsKillable(Me, false, false);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->SetIsPushableByHero(Me, false);
    bool HaveChatted = false;
    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (Game->IsQuestCompleted("Q_Arena")) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD2_RETURN_WELL_DONE", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
            }
            else {
                if (IsActiveThreadTerminating())
                    return;
                if (HaveChatted) {
                    if (IsActiveThreadTerminating())
                        return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD2_GREETING_AGAIN", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
                }
                else {
                    if (IsActiveThreadTerminating())
                        return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD2_GREETING", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
                    HaveChatted = true;
                }
            }
        }
        else if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
                 (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            if (!ParentClass.InHitCutsceneAlready) {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass.InHitCutsceneAlready = true;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_ENTRANCE_GUARD2_ATTACKED", GROUP_SELECT_NONE, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetThingWithScriptName("GuardStareMarker"), false);
                Game->CameraDefault();
                Game->ModifyThingHealth(Me, 100.0f, false);
                ParentClass.InHitCutsceneAlready = false;
            }
        }
    }
}
