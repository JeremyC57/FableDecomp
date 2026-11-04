// NScript::CV_HeroDuelScript::DuelLost() -- Thunder has lost the duel: flag him defeated, take Thunder and then the hero under scripted control, play CS_GIBBETHILL_HERODUEL_END (or _END_W when the hero killed Whisper) inside a paused, camera-fixed movie sequence, then reward the hero (script-def XP and morality, Thunder's trophy), record DefeatedThunder in the master data, stop Thunder following the hero and close the mission as succeeded
// Leaf: ?DuelLost@CV_HeroDuelScript@NScript@@QAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9B240, 1094 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): Ego_r.pdb name set ?DuelLost@CV_HeroDuelScript@NScript@@QAEXXZ; only caller the landed
//   CV_HeroDuelScript::CThunder::Main 0x00E9A5C0 (site 0x00E9A95A, ParentClass->DuelLost() once Thunder is beaten); literals
//   "CS_GIBBETHILL_HERODUEL_END_W" 0x012ED46C, "CS_GIBBETHILL_HERODUEL_END" 0x012ED450, "OBJECT_TROPHY_THUNDER" 0x012E8D2C, and
//   the script-def fields HeroDuelDefeatThunderXP (+0x680) / HeroDuelDefeatThunderMorality (+0x67C). (The PDB name says the
//   duel is lost from Thunder's side: the body sets ThunderDefeated, DefeatedThunder and MissionSucceeded.)
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, ~CScriptThing 0x004AA840, CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430 /
//   ~CBaseClass 0x0099A2E0 (the first exit's dtors expanded), IsValid 0x00CD23B9 / Release 0x00CD2770 / ~ScriptedThingBase
//   0x007E74D0 / operator= 0x008ABD10, map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, ~MovieBase 0x006E7B80,
//   RunCutsceneMacro 0x00CBFB7D, IsActiveThreadTerminating 0x00CB7940, operator delete 0x00BFE9BC; CGameScriptInterfaceBase slots
//   7, 8, 70, 72, 121 GiveHeroObject, 128 GiveHeroExperience, 156 GiveHeroMorality, 370, 371, 379, 480, 512.
// Data: pGlobalScriptDef 0x0143E90C (engine/fable_script_def_global.h).
// Layout: CV_HeroDuelScript (engine header) MissionSucceeded +0x48, MissionOver +0x4a, ThunderDefeated +0x4d;
//   CQ_SunnyvaleMasterData DefeatedThunder +0x61, WhisperKilledByHero +0x74.
// Shape: FirstFleeCutscene 0x00E9A970 (same TU): handles in an inner block, CWideScreenMagicPauseEntities for the pause pair.
//   Matched on the first grade.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/NScript_CV_HeroDuelScript.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"
inline CScriptThing::~CScriptThing() { }
void NScript::CV_HeroDuelScript::DuelLost()
{
    ThunderDefeated = true;
    CScriptThing thunder = Game->GetThingWithScriptName("Thunder");
    {
        CScriptGameResourceObjectScriptedThingBase thunderHandle;
        if (thunderHandle.IsValid())
            thunderHandle.Release();
        while (!Game->StartScriptingEntity(thunder, thunderHandle, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectScriptedThingBase heroHandle;
        if (heroHandle.IsValid())
            heroHandle.Release();
        while (!Game->StartScriptingEntity(Game->GetHero(), heroHandle, (EScriptAIPriority)4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
        cast["HERO"] = heroHandle;
        cast["THUNDER"] = thunderHandle;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities pause(*Game);
        Game->FixMovieSequenceCamera(true);
        Game->EntitySetCutsceneBehaviour(thunder, (ECutsceneBehaviour)2);
        if (MasterScriptData->WhisperKilledByHero) {
            if (IsActiveThreadTerminating())
                return;
            RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_END_W", cast, 0, 0, false, true);
        } else {
            if (IsActiveThreadTerminating())
                return;
            RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_END", cast, 0, 0, false, true);
        }
        Game->EntitySetCutsceneBehaviour(thunder, (ECutsceneBehaviour)1);
        Game->FixMovieSequenceCamera(false);
    }
    Game->GiveHeroExperience(pGlobalScriptDef->HeroDuelDefeatThunderXP);
    Game->GiveHeroMorality(pGlobalScriptDef->HeroDuelDefeatThunderMorality);
    Game->GiveHeroObject("OBJECT_TROPHY_THUNDER", -1, false);
    MasterScriptData->DefeatedThunder = true;
    Game->SetEntityAsRegionFollowing(Game->GetHero(), thunder, false);
    MissionSucceeded = true;
    MissionOver = true;
}