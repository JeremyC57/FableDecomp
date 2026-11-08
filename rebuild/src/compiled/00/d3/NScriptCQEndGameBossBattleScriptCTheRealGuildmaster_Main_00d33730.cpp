// NScript::CQ_EndGameBossBattleScript::CTheRealGuildmaster::Main() -- the mortally wounded Guildmaster at the end game: take control, put him in MORTALLY_WOUNDED mode (friendly, unkillable, informative, not pushable, not a follower) at MK_ENDGAME_GMOUTOFWAY in the dying cycle; when the hero first reaches MK_ENDGAME_CUTSCENE_TRIGGER play CS_GM_DEATH (Hero / GM), kick off the quest start screen, then each frame say HELP_ASIDE every 15 s while the hero is within 5.5, and on a hero talk (FIRST_CHAT, then SECOND_CHAT) or hero hit (HIT, healing spells excepted) play CS_GM_DYING with that $LINE and resume the dying cycle
// Inline-budget: #pragma inline_depth(0) ... #pragma inline_depth() round the talk branch's first thread-check return (line 120) -- VC7.1 expanded the implicit ~CTimer (AsideTimer) cleanup of that one return and cross-jumped it into the loop-exit copy (jne 0x00D33D4A -> 0x00D343B9) where retail jumps to the shared pad that CALLS ~CTimer 0x00CD4470 (0x00D343E6); the aside and hit returns call it in both. Plain C++ (chunk 856: 103 grades of do{}while(0), `== true`, ~CTimer / GetTime / operator= / operator<= spellings, loop, wait-loop and break forms) and the per-row flags /d2inlS0-S20, /d2inlT50-T100 (chunk 856 diagnostics) never reproduce the per-site choice, so the owner's 2026-11-08 device (3) is used at the one site; RELOCATION_MATCH (chunk 864, 2 grades). The ~CTimer body is the dtor the row models (DeregisterTimer through pDSTGame); inline_depth(0) at that return makes c2 call it as retail does
// Grade: RELOCATION_MATCH -- chunk 864 (owner 2026-11-08 inline control, preference 3: no plain-C++ or per-row-flag spelling reproduces the single-site call); was DIFFER(3292v3292) one reloc-masked byte, md 0
// Leaf: ?Main@CTheRealGuildmaster@CQ_EndGameBossBattleScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D33730, 3292 bytes, thiscall, no EH frame, plain `ret`, frame 0xe0.
// Identity (proven): Ego_r.pdb ?Main@CTheRealGuildmaster@CQ_EndGameBossBattleScript@NScript@@EAEXXZ; slot 1 of the CTheRealGuildmaster
//   entity vtable 0x012CC708 (stored by Alloc 0x00D2FDC0; slot 0 scalar deleting dtor 0x00D2FE80, 2 Init 0x00D2FE70, 3 GetParentScript
//   0x00D2FE60); labels_anniversary_2026-11-04_waves23.tsv agrees. No Codex source (pass 13 left it NOT_COMPILED).
// Callees: ScriptedThingBase (ctor 0x007E72A0 called / expanded for seh_me, IsValid 0x00CD23B9, Release 0x00CD2770, PlayLoopingAnimation
//   0x007E73F0, operator= 0x008ABD10, ~ 0x007E74D0), MovieBase ctor expanded / dtor 0x006E7B80, map<CCharString, STB> ctor 0x00CDBF70 /
//   ~ 0x00CDBFB0 / operator[] 0x00CD3D2E, map<CCharString, CCharString> ctor 0x009AC2D0 / ~ 0x009AC310 / operator[] 0x009AC700,
//   CCharString operator=(const char*) 0x0099EFE0, RunCutsceneMacro 0x00CBFB7D, IsDistanceBetweenThingsUnder 0x00CBE2FF, CTimer ctor
//   0x00CD4450 / ~ 0x00CD4470, ~CScriptThing 0x004AA840, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748;
//   interface slots 7, 8, 70, 72, 309 KickOffQuestStartScreen, 360, 364, 365, 366, 370, 379, 472, 517, 526, 655, 745, 749, 844 and
//   235 EntitySetAsAllowedToFollowHero; CScriptThing slots 21, 27, 41, 42, 67 SetFriendsWithEverythingFlag.
// Data: engine/CQ_EndGameBossBattleScript.h (opt-in CTHEREALGUILDMASTER): ParentClass GuildmasterChat +0x4c.
// Shape: generated entity script (TRetVoid returns, T-check at each nested block head). Levers: MOVIEBASE_FORCEINLINE_CTOR +
//   OUTOFLINE_DTOR (retail expands every movie ctor and calls every movie dtor); the CTimer and its loop in a block of their own so the
//   timer overlays the prologue CCharString temporary at 0x10 (chunk 816 / 818); the loop as for (;; NSF) { if (T) return; ... } (the
//   loop-entry check then serves retail's expanded natural-exit ~CTimer and the seh_hero ctors stay calls).
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CQ_ENDGAMEBOSSBATTLESCRIPT_CTHEREALGUILDMASTER
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_EndGameBossBattleScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_animation_defaults.h"
#include "engine/TRetVoid.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A
void FABLE_FASTCALL RunCutsceneMacro(const CCharString&, std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>&, std::map<CCharString,bool>*, std::map<CCharString,CCharString>*, bool, bool);   // 0x00CBFB7D
inline CScriptThing::~CScriptThing() { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

void NScript::CQ_EndGameBossBattleScript::CTheRealGuildmaster::Main()
{
    TRetVoid _Fail_Val;
    CScriptGameResourceObjectScriptedThingBase seh_me;
    if (seh_me.IsValid())
        seh_me.Release();
    while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->AddCreatureScriptedMode(Me, "MORTALLY_WOUNDED");
    Me.SetFriendsWithEverythingFlag(true);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetAsKillable(Me, false, false);
    Game->SetThingHasInformation(Me, false, true, false);
    Game->SetIsPushableByHero(Me, false);
    Game->SetIsThingForcePushable(Me, false);
    Game->EntitySetAsAllowedToFollowHero(Me, false);
    Game->EntityTeleportToThing(Me, Game->GetThingWithScriptName("MK_ENDGAME_GMOUTOFWAY"), false);
    seh_me.PlayLoopingAnimation("CS_GUILDMASTER_DYING_CYCLE", -1, false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
    CScriptThing CutsceneTrigger = Game->GetThingWithScriptName("MK_ENDGAME_CUTSCENE_TRIGGER");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), CutsceneTrigger, 2.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (!ParentClass.GuildmasterChat) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        ParentClass.GuildmasterChat = true;
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        if (seh_hero.IsValid())
            seh_hero.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
        cast["Hero"] = seh_hero;
        cast["GM"] = seh_me;
        CScriptGameResourceObjectMovieBase cutscene_movie;
        Game->StartMovieSequence("", cutscene_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        RunCutsceneMacro("CS_GM_DEATH", cast, 0, 0, false, true);
    }
    seh_me.PlayLoopingAnimation("CS_GUILDMASTER_DYING_CYCLE", -1, false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    {
    bool ChattedBefore = false;
    CTimer AsideTimer;
    AsideTimer = 15;
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        if (IsDistanceBetweenThingsUnder(Game->GetHero(), Me, 5.5f) && AsideTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_039_GUILDMASTER_HELP_ASIDE", false, Me, Game->GetHero());
            AsideTimer = 15;
        }
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
#pragma inline_depth()
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            if (seh_hero.IsValid())
                seh_hero.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            std::map<CCharString, CCharString> text;
            if (ChattedBefore) {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                text["$LINE"] = "TEXT_QST_039_GUILDMASTER_SECOND_CHAT";
            }
            else {
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
                ChattedBefore = true;
                text["$LINE"] = "TEXT_QST_039_GUILDMASTER_FIRST_CHAT";
            }
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
            cast["Hero"] = seh_hero;
            cast["GM"] = seh_me;
            CScriptGameResourceObjectMovieBase cutscene_movie;
            Game->StartMovieSequence("", cutscene_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            RunCutsceneMacro("CS_GM_DYING", cast, 0, &text, false, true);
            seh_me.PlayLoopingAnimation("CS_GUILDMASTER_DYING_CYCLE", -1, false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            CScriptGameResourceObjectScriptedThingBase seh_hero;
            if (seh_hero.IsValid())
                seh_hero.Release();
            while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating()) return _Fail_Val.Return();
            std::map<CCharString, CCharString> text;
            text["$LINE"] = "TEXT_QST_039_GUILDMASTER_HIT";
            std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
            cast["Hero"] = seh_hero;
            cast["GM"] = seh_me;
            CScriptGameResourceObjectMovieBase cutscene_movie;
            Game->StartMovieSequence("", cutscene_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            RunCutsceneMacro("CS_GM_DYING", cast, 0, &text, false, true);
            seh_me.PlayLoopingAnimation("CS_GUILDMASTER_DYING_CYCLE", -1, false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
            Me.SetFriendsWithEverythingFlag(true);
        }
    }
    }
}