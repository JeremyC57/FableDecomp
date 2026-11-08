// NScript::CQ_ArenaHoldingScriptScript::CBowerstoneFan::Main() -- the Bowerstone arena fan: until the hero attacks a fan or the arena is over, greet the hero when talked to (INTRO_FIRST, then INTRO_SECOND; once Q_Arena is completed WELL_DONE, then WELL_DONE_REPEAT) and flag PlayerAttackedFans when hit; then wait for FansCanLeaveNow and run to the FanLeavingMarker (attacked), or walk there answering talk and hits (LEAVING_HIT), and fade out
// Inline-budget: flag /d2inlS15 -- the implicit CScriptGameResourceObjectMovieBase dtor at the second-loop and exit sites: VC7.1's caller IL-size cap (default S9) keeps them COMDAT calls 0x006E7B80 in this 4.3 KB Main, retail expands every site; RELOCATION_MATCH for S15..S20, S14 is the default shape
// Leaf: ?Main@CBowerstoneFan@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ
// Flags: /d2inlS15
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CF2B70, 4298 bytes, thiscall, plain `ret`, no EH frame (sub esp,0xb4; every exit runs its own destructor cascade).
// Identity (proven): slot 1 (Main) of the CBowerstoneFan entity vtable 0x012C6384 (the only reference to the row is .rdata 0x012C6388),
//   stored by the ctor inside CBowerstoneFan::Alloc 0x00CF2A90 (0x00CF2AC6); slot 0 sdd 0x00CF2B50, 2 Init 0x00CF2B40, 3 GetParentScript
//   0x00CF2B30 (both landed rows name the class); registration "BowerstoneFan" in CQ_ArenaHoldingScriptScript::Main 0x00CF0790 (landed);
//   Ego_r.pdb / FableWin.pdb name set ?Main@CBowerstoneFan@CQ_ArenaHoldingScriptScript@NScript@@EAEXXZ.
// Callees (retail census): CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30 (x39); ScriptedThingBase IsValid 0x00CD23B9 /
//   Release 0x00CD2770 / MoveToPosition 0x007E72F0 / ClearCommands 0x007E7360 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 /
//   GetScriptThing 0x007E7490 / ~ 0x007E74D0; CBaseObject ctor 0x0099A380 / dtor 0x0099A430 (the expanded movie ctor/dtor);
//   ~CCountedPointer<CScriptGameResourceObjectMovieBase> 0x006E7AB0 (x11: the declared-only specialization the implicit movie dtor
//   expands around, gf 0x00D8ABF0 precedent); ~CScriptThing 0x004AA840 (GetThingWithScriptName temporaries); CCharString 0x0099EBF0 /
//   ~ 0x0099EAE0; operator delete 0x00BFE9BC; FadeOutAndKillEntity 0x00CBE716 ?FadeOutAndKillEntity@@YIXABVCScriptThing@@M_N1@Z
//   (fastcall; see NAME_FIXES.tsv for the `_NM1` mis-spelling bound at the same address). Retail calls NO ??0/??1 of the movie base:
//   every movie ctor and dtor is expanded.
// Layout: engine/CQ_ArenaHoldingScriptScript.h (new, chunk 804, hdr/): parent PlayerAttackedFans +0x48, FansCanLeaveNow +0x49,
//   ArenaOver +0x4b, InHitCutsceneAlready +0x4c (ParentClass is the reference at +0x14 of the 0x1c-byte entity script).
//   FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR (implicit dtor; a user/FORCEINLINE dtor adds a MovieBase vtable
//   store per site) + FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR.
// Shape: take seh_me, first loop `while (!PlayerAttackedFans && !ArenaOver)` (hit -> flag; talk -> Q_Arena completed ? WELL_DONE
//   arms : intro arms, each line in its own movie + pause block); then PlayerAttackedFans ? wait FansCanLeaveNow, re-take seh_me,
//   run to FanLeavingMarker : walk there answering talk / hits. The final `if (T) return; FadeOutAndKillEntity(Me,1.0f,true,true);`
//   is written at the end of BOTH arms: retail tail-merges them (push 1; mov ecx,edi; mov dl,1; push 1.0f), one shared copy does not.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CQ_ArenaHoldingScriptScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
enum EScriptEntityMoveType { ENTITY_MOVE_WALK=0, ENTITY_MOVE_RUN=1, ENTITY_MOVE_SNEAK=2 };
void FABLE_FASTCALL FadeOutAndKillEntity(const CScriptThing&,float,bool,bool);        // 0x00CBE716

void NScript::CQ_ArenaHoldingScriptScript::CBowerstoneFan::Main()
{
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid()) seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    bool Spoken = false;
    while (!ParentClass.PlayerAttackedFans && !ParentClass.ArenaOver) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return;
            ParentClass.PlayerAttackedFans = true;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating()) return;
            if (Game->IsQuestCompleted("Q_Arena")) {
                if (IsActiveThreadTerminating()) return;
                if (Spoken) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_WELL_DONE_REPEAT", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_WELL_DONE", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                        Spoken = true;
                    }
                }
            } else {
                if (IsActiveThreadTerminating()) return;
                if (Spoken) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_INTRO_SECOND", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_INTRO_FIRST", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    Spoken = true;
                }
            }
        }
    }
    if (IsActiveThreadTerminating()) return;
    bool SpokenLeaving = false;
    if (ParentClass.PlayerAttackedFans) {
        if (IsActiveThreadTerminating()) return;
        if (seh_me.IsValid()) seh_me.Release();
        while (!ParentClass.FansCanLeaveNow) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (seh_me.IsValid()) seh_me.Release();
        while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        seh_me.MoveToPosition(Game->GetThingWithScriptName("FanLeavingMarker").GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
        while (seh_me.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        FadeOutAndKillEntity(Me, 1.0f, true, true);
    } else {
        if (IsActiveThreadTerminating()) return;
        seh_me.MoveToPosition(Game->GetThingWithScriptName("FanLeavingMarker").GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
        while (seh_me.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                seh_me.ClearCommands();
                if (SpokenLeaving) {
                    if (IsActiveThreadTerminating()) return;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_WELL_DONE_REPEAT", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                } else {
                    if (IsActiveThreadTerminating()) return;
                    {
                        CScriptGameResourceObjectMovieBase speech_movie;
                        Game->StartMovieSequence("", speech_movie);
                        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                        if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                            seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_WELL_DONE", GROUP_SELECT_NONE, false, true, false);
                            while (seh_me.IsPerformingScriptTask()) {
                                Game->NewScriptFrame();
                                if (IsActiveThreadTerminating()) return;
                            }
                            if (IsActiveThreadTerminating()) return;
                        }
                        SpokenLeaving = true;
                    }
                }
                seh_me.MoveToPosition(Game->GetThingWithScriptName("FanLeavingMarker").GetPos(), 1.0f, ENTITY_MOVE_WALK, false, true);
            }
            if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
                if (IsActiveThreadTerminating()) return;
                seh_me.ClearCommands();
                if (!ParentClass.InHitCutsceneAlready) {
                    if (IsActiveThreadTerminating()) return;
                    ParentClass.InHitCutsceneAlready = true;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_005_V2_FAN_BOWERSTONE_LEAVING_HIT", GROUP_SELECT_NONE, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) return;
                        }
                        if (IsActiveThreadTerminating()) return;
                    }
                    ParentClass.InHitCutsceneAlready = false;
                }
                seh_me.MoveToPosition(Game->GetThingWithScriptName("FanLeavingMarker").GetPos(), 1.0f, ENTITY_MOVE_RUN, false, true);
            }
        }
        if (IsActiveThreadTerminating()) return;
        FadeOutAndKillEntity(Me, 1.0f, true, true);
    }
}
