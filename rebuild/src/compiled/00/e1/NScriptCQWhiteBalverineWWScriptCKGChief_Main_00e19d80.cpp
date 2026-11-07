// NScript::CQ_WhiteBalverineWWScript::CKG_Chief::Main() -- the chief during the white balverine war: live while conscious; unkillable, not combo / stat counted, friends with everything; take scripted control and attach to the active quest; until MissionSucceeded: when talked to, if the white balverine is dead play CS_WBK_CHIEF4 with HERO / CHIEF and set MissionSucceeded, else say BALVERINE_GONE_FIRST / _SECOND under a paused speech movie; when hit by the hero (except a heal-life spell) say TEXT_QST_074_CHIEF_BEEN_ATTACKED and stay friends with everything; finally clear his information marker
// Leaf: ?Main@CKG_Chief@CQ_WhiteBalverineWWScript@NScript@@EAEXXZ
// Inline-split: one `do { } while (0);` round the two cast-map assignments heats that single block so VC7.1 expands both implicit
//   CScriptGameResourceObjectScriptedThingBase::operator= copies (CBaseObject::operator= 0x0099A3B0 + the inline CCountedPointer
//   share) as retail does at 0x00E19FD1 / 0x00E1A051; without it both stay calls to the COMDAT 0x008ABD10 (GOTCHAS 2026-11-03 c2 rule).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E19D80, 2539 bytes, thiscall, plain `ret` (four epilogue copies).
// Identity (proven): slot 1 (Main) of the CKG_Chief entity vtable 0x012E0E68 (read from retail .rdata) stored by the landed Alloc
//   0x00E192E0; Ego_r.pdb ?Main@CKG_Chief@CQ_WhiteBalverineWWScript@NScript@@EAEXXZ; "KG_Chief" is the binding name Main 0x00E18630 registers.
// Callees: CIsThingConscious (vtable 0x012C2FE8, inline) + CScriptThing copy 0x004ABE90 / ~ 0x004AA840, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseObject 0x0099A380 / operator= 0x0099A3B0 / ~ 0x0099A430, ScriptedThingBase dtor 0x007E74D0 /
//   IsValid 0x00CD23B9 / Release 0x00CD2770 / GetScriptThing 0x007E7490 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450, MovieBase
//   dtor 0x006E7B80 (called at three return pads, expanded with the PImp dtor 0x006E7AB0 at the others), CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   cast map operator[] 0x00CD3D2E / dtor 0x00CDBFB0 (ctor expanded: STLport node malloc 0x00BFEA0E), RunCutsceneMacro 0x00CBFB7D (__fastcall);
//   CGameScriptInterfaceBase slots 7, 8 StartScriptingEntity, 70 GetHero, 264 GetHealth, 361 ClearThingHasInformation, 370 StartMovieSequence,
//   371 FixMovieSequenceCamera, 379 PauseAllNonScriptedEntities, 465 EntityAttachToScript, 517 EntitySetAsKillable, 526 / 527
//   EntitySetAsToAddToComboMultiplierWhenHit / ...StatChangesWhenHit, 655 GetActiveQuestName; CScriptThing slots 21 MsgIsHitBy, 27
//   MsgIsTalkedToBy, 41 / 42 MsgIsHitBySpecialAbilityFrom / MsgIsHitByAnySpecialAbilityFrom, 67 SetFriendsWithEverythingFlag.
// Strings (read from retail): "HERO" 0x01255174, "CHIEF" 0x012CB38C, "CS_WBK_CHIEF4" 0x012E0F44, TEXT_QST_074_CHIEF_BALVERINE_GONE_FIRST
//   0x012E0F1C / _SECOND 0x012E0EF0, TEXT_QST_074_CHIEF_BEEN_ATTACKED 0x012E0BB4, "SCRIPT_NAME_HERO" 0x0125D1C8.
// Layout: engine/CQ_WhiteBalverineWWScript.h (opt-in FABLE_RETAIL_CQ_WHITEBALVERINEWWSCRIPT_CKG_CHIEF, chunk 833): InitialChatted +0x1c;
//   ParentClass MissionSucceeded +0x49, WhiteBalverineAlive +0x4a. Retail stores 0 into InitialChatted on the first chat (`mov [esi+1Ch],bl`
//   at 0x00E1A208, ebx = 0): the source sets it false, as the parked Codex pass-13 model also read it.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CQ_WHITEBALVERINEWWSCRIPT_CKG_CHIEF
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_WhiteBalverineWWScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CIsThingConscious.h"
#include "engine/fable_script_cutscene.h"
#include "stlport/fable_map.h"
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_FORCE_PUSH=1, HERO_ABILITY_TIME_SPELL=2, HERO_ABILITY_ENFLAME_SPELL=3, HERO_ABILITY_PHYSICAL_SHIELD_SPELL=4, HERO_ABILITY_TURNCOAT_SPELL=5, HERO_ABILITY_DRAIN_LIFE_SPELL=6, HERO_ABILITY_RAISE_DEAD_SPELL=7, HERO_ABILITY_BERSERK=8, HERO_ABILITY_DOUBLE_STRIKE=9, HERO_ABILITY_SUMMON_SPELL=10, HERO_ABILITY_LIGHTNING_SPELL=11, HERO_ABILITY_BATTLE_CHARGE=12, HERO_ABILITY_ASSASSIN_RUSH=13, HERO_ABILITY_HEAL_LIFE_SPELL=14, HERO_ABILITY_GHOST_SWORD_SPELL=15, HERO_ABILITY_FIREBALL_SPELL=16, HERO_ABILITY_MULTI_ARROW=17, HERO_ABILITY_DIVINE_WRATH_SPELL=18, HERO_ABILITY_UNHOLY_POWER_SPELL=19, MAX_NUMBER_OF_HERO_ABILITIES=20 };
inline NScript::CIsThingConscious::CIsThingConscious(const CScriptThing& thing) : Thing(thing) { }
inline NScript::CIsThingConscious::~CIsThingConscious() { }

void NScript::CQ_WhiteBalverineWWScript::CKG_Chief::Main()
{
    {
        CIsThingConscious conscious(Me);
        SetPredicate(conscious);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->EntitySetAsKillable(Me, false, true);
    Game->EntitySetAsToAddToComboMultiplierWhenHit(Me, false);
    Game->EntitySetAsToAddToStatChangesWhenHit(Me, false);
    Me.SetFriendsWithEverythingFlag(true);
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
    Game->EntityAttachToScript(Me, Game->GetActiveQuestName());
    while (!ParentClass.MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (!ParentClass.WhiteBalverineAlive) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectScriptedThingBase seh_hero;
                Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                do {
                    mymap["HERO"] = seh_hero;
                    mymap["CHIEF"] = seh_me;
                } while (0);
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                Game->FixMovieSequenceCamera(true);
                RunCutsceneMacro("CS_WBK_CHIEF4", mymap, 0, 0, false, true);
                Game->FixMovieSequenceCamera(false);
                ParentClass.MissionSucceeded = true;
            }
            else {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (!InitialChatted) {
                    if (IsActiveThreadTerminating())
                        return;
                    InitialChatted = false;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BALVERINE_GONE_FIRST", (ETextGroupSelectionMethod)0, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
                else {
                    if (IsActiveThreadTerminating())
                        return;
                    if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                        seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BALVERINE_GONE_SECOND", (ETextGroupSelectionMethod)0, false, true, false);
                        while (seh_me.IsPerformingScriptTask()) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating())
                                return;
                        }
                        if (IsActiveThreadTerminating())
                            return;
                    }
                }
            }
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (seh_me.IsValid())
                seh_me.Release();
            while (!Game->StartScriptingEntity(Me, seh_me, SCRIPT_AI_PRIORITY_HIGHEST)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            {
                CScriptGameResourceObjectMovieBase speech_movie;
                Game->StartMovieSequence("", speech_movie);
                CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                if (Game->GetHealth(seh_me.GetScriptThing()) > 0.0f) {
                    seh_me.Speak(Game->GetHero(), "TEXT_QST_074_CHIEF_BEEN_ATTACKED", (ETextGroupSelectionMethod)0, false, true, false);
                    while (seh_me.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                }
            }
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            Me.SetFriendsWithEverythingFlag(true);
        }
    }
    if (IsActiveThreadTerminating())
        return;
    Game->ClearThingHasInformation(Me);
}