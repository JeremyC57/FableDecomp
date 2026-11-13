// NScript::CV_TravellingHeroesScript::CTH_Whisper::Main() -- Whisper on her travelling-heroes visit: live while alive; past gameflow position 2100 drop the "TravHero Whisper" rumour and remove her; else give her information, keep her persistent and unpushable, sheathe her weapons, set her morph seed, take scripted control through the parent's seh_Rival; then each frame: when the hero hits her (not with a heal-life spell) or talks to her, drop any open conversation and the rumour, mark WhisperCutscenePlayed and play CS_TRAVHERO_WHISPERLEAVE / CS_TRAVHERO_WHISPERPUB; when the hero is between 1 and 5 units away face him and, every (rand()%30+30)*15 frames, say the next of the TEXT_QST_B14_WHISPERPUB_<n> lines (n cycles 1..3)
// Grade: DIFFER(1350v1366) -- Blocker 1 (CIsThingAlive copy-inline case): retail expands the by-value CScriptThing copy of SetIsPushableByHero(Me, false) inline while the CIsThingAlive predicate copy calls 0x004ABE90; ours calls both (115 lines; the rest follows from the 16-byte shift)
// Next idea: none in plain C++ (FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY expands both copies, md50; an explicit CScriptThing(Me) temp changes nothing); only an inline_depth(0) region reproduces it, which is not allowed
// Leaf: ?Main@CTH_Whisper@CV_TravellingHeroesScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE3140, 1366 bytes, thiscall, plain `ret` (two epilogue copies), frame 0x44.
// Identity (proven): Ego_r.pdb + FableWin.pdb ?Main@CTH_Whisper@CV_TravellingHeroesScript@NScript@@EAEXXZ; the single .rdata reference is
//   slot 1 of the CTH_Whisper entity vtable 0x012F4294 (slot 0 scalar dtor 0x00EE0220, slot 2 Init 0x00EE0200, slot 3 GetParentScript
//   0x00EE01F0, the class's landed labels).
// Callees: CIsThingAlive (vtable 0x012C32F0; ctor inline, dtor 0x00E241B0 called) + CScriptThing copy 0x004ABE90, SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, ScriptedThingBase IsValid 0x00CD23B9 / Release 0x00CD2770, CCharString 0x0099EBF0 / ~ 0x0099EAE0,
//   CV_TravellingHeroesScript::PlayCutscene 0x00EE2DB0, IsDistanceBetweenThingsUnder 0x00CBE2FF / IsDistanceBetweenThingsOver 0x00CBE3EA
//   (__fastcall), GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690, rand 0x00BFEB16 (MSVCR71);
//   CScriptThing slots 21 MsgIsHitBy, 27 MsgIsTalkedToBy, 41 MsgIsHitBySpecialAbilityFrom, 42 MsgIsHitByAnySpecialAbilityFrom;
//   CGameScriptInterfaceBase slots 7, 8, 70, 108, 360, 364-367, 475, 477, 479, 506, 719, 844.
// Strings: "TravHero Whisper" 0x012F4320, "SCRIPT_NAME_HERO" 0x0125D1C8, "CS_TRAVHERO_WHISPERLEAVE" 0x012F45D0,
//   "CS_TRAVHERO_WHISPERPUB" 0x012F45B8, "TEXT_QST_B14_WHISPERPUB_" 0x012F459C.
// Layout: engine/CV_TravellingHeroesScript.h CTH_Whisper (Ego_r.pdb, 0x1c): ParentClass +0x14, PMasterData +0x18 (PostSavePosition +4);
//   the parent's seh_Rival +0x48, WhisperCutscenePlayed +0x7c.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CV_TRAVELLINGHEROESSCRIPT_PLAYCUTSCENE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CV_TravellingHeroesScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_text.h"
#include <stdlib.h>
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum EHeroAbility { HERO_ABILITY_NULL=0, HERO_ABILITY_HEAL_LIFE_SPELL=14 };
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing& thing) : Thing(thing) { }

void NScript::CV_TravellingHeroesScript::CTH_Whisper::Main()
{
    SetPredicate(CIsThingAlive(Me));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    if (PMasterData->PostSavePosition > 2100) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveRumourCategory("TravHero Whisper");
        Game->RemoveThing(Me, false, true);
        return;
    }
    Game->SetThingHasInformation(Me, false, true, false);
    Game->SetThingPersistent(Me, true);
    Game->SetIsPushableByHero(Me, false);
    Game->EntitySheatheWeapons(Me, false);
    Game->EntitySetAppearanceMorphSeed(Me, 1);
    {
        CV_TravellingHeroesScript& parent = ParentClass;
        if (parent.seh_Rival.IsValid())
            parent.seh_Rival.Release();
    }
    while (!Game->StartScriptingEntity(Me, ParentClass.seh_Rival, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    long conversation = -1;
    long face_timer = 0;
    long line = 0;
    long talk_timer = 0;
    while (!IsActiveThreadTerminating()) {
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO") ||
            (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))) {
            if (IsActiveThreadTerminating())
                return;
            if (conversation != -1) {
                if (IsActiveThreadTerminating())
                    return;
                Game->RemoveConversation(conversation, false);
            }
            Game->RemoveRumourCategory("TravHero Whisper");
            ParentClass.WhisperCutscenePlayed = true;
            ParentClass.PlayCutscene("CS_TRAVHERO_WHISPERLEAVE");
        }
        else if (Me.MsgIsTalkedToBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            if (conversation != -1) {
                if (IsActiveThreadTerminating())
                    return;
                Game->RemoveConversation(conversation, false);
            }
            Game->RemoveRumourCategory("TravHero Whisper");
            ParentClass.WhisperCutscenePlayed = true;
            ParentClass.PlayCutscene("CS_TRAVHERO_WHISPERPUB");
        }
        else if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 5.0f) && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 1.0f)) {
            if (IsActiveThreadTerminating())
                return;
            if (talk_timer <= 0) {
                if (IsActiveThreadTerminating())
                    return;
                if (line > 3) {
                    if (IsActiveThreadTerminating())
                        return;
                    line = 1;
                }
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B14_WHISPERPUB_" + GFIntToCharString(line), false, Me, Game->GetHero());
                ++line;
                talk_timer = (rand() % 30 + 30) * 15;
                face_timer = 15;
            }
            else if (face_timer <= 0) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                face_timer = 15;
            }
        }
        --face_timer;
        --talk_timer;
        Game->NewScriptFrame();
    }
}
