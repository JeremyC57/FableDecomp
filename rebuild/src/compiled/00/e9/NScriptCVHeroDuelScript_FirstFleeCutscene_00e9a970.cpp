// NScript::CV_HeroDuelScript::FirstFleeCutscene() -- Thunder flees the duel the first time: hide the health bar, hush the supporters, take Thunder and then the hero under scripted control, play CS_GIBBETHILL_HERODUEL_CLIFF and the _W / _N follow-up (Whisper killed by the hero or not) inside a paused, camera-fixed movie sequence with the hero undamageable, then turn teleporting off and make Thunder follow the hero across regions
// Leaf: ?FirstFleeCutscene@CV_HeroDuelScript@NScript@@QAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9A970, 1341 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): Ego_r.pdb name set ?FirstFleeCutscene@CV_HeroDuelScript@NScript@@QAEXXZ (QAEXXZ: public, no args); its
//   only caller is the landed CV_HeroDuelScript::CThunder::Main 0x00E9A5C0 (call site 0x00E9A6F8, ParentClass->FirstFleeCutscene(),
//   the first of its three cutscene calls, before SecondFleeCutscene 0x00E9AEB0 and DuelLost 0x00E9B240); literals
//   "CS_GIBBETHILL_HERODUEL_CLIFF" 0x012ED414, "_CLIFF_W" 0x012ED3F4, "_CLIFF_N" 0x012ED3D4.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, ~CScriptThing 0x004AA840, CBaseObject() 0x0099A380 / ~CBaseClass 0x0099A2E0 /
//   ~CBaseObject 0x0099A430 (the handle ctor and the first exit's dtors expanded), IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   ~ScriptedThingBase 0x007E74D0 / operator= 0x008ABD10, map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E,
//   ~MovieBase 0x006E7B80, RunCutsceneMacro 0x00CBFB7D, IsActiveThreadTerminating 0x00CB7940, operator delete 0x00BFE9BC;
//   CGameScriptInterfaceBase slots 7, 8, 70, 72, 235 EntitySetAsAllowedToFollowHero, 370, 371, 379 (CWideScreenMagicPauseEntities),
//   480 SetEntityAsRegionFollowing, 512 EntitySetCutsceneBehaviour, 516 EntitySetAsDamageable, 565 SetTeleportingAsActive.
// Layout: CV_HeroDuelScript (engine header, retail = Ego_r.pdb - 0x14) OkToShowHealthBar +0x4e, StopSupportersCheering +0x52;
//   CQ_SunnyvaleMasterData WhisperKilledByHero +0x74.
// Shape: the landed same-TU SecondFleeCutscene 0x00E9AEB0 (handles in an inner block, plain-inline implicit dtors: VC7.1
//   expands them only at the first wait loop's exit), with the TurnToBalv 0x00E0A820 cutscene block: CWideScreenMagicPauseEntities
//   (Game cached in edi; its dtor unpauses at every exit after the second CScriptThing local is destroyed, which an explicit
//   `game->PauseAllNonScriptedEntities(false)` before the return cannot order that way). Matched on the first grade.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/NScript_CV_HeroDuelScript.h"
#include "engine/fable_script_cutscene.h"                // RunCutsceneMacro 0x00CBFB7D (+ stlport/fable_map.h)
inline CScriptThing::~CScriptThing() { }
void NScript::CV_HeroDuelScript::FirstFleeCutscene()
{
    OkToShowHealthBar = false;
    StopSupportersCheering = true;
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
        Game->EntitySetAsDamageable(Game->GetHero(), false);
        CScriptThing thunderInScene = Game->GetThingWithScriptName("Thunder");
        Game->EntitySetCutsceneBehaviour(thunderInScene, (ECutsceneBehaviour)2);
        RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_CLIFF", cast, 0, 0, false, true);
        Game->EntitySetCutsceneBehaviour(thunderInScene, (ECutsceneBehaviour)1);
        cast["HERO"] = heroHandle;
        cast["THUNDER"] = thunderHandle;
        Game->EntitySetCutsceneBehaviour(thunderInScene, (ECutsceneBehaviour)2);
        if (MasterScriptData->WhisperKilledByHero) {
            if (IsActiveThreadTerminating())
                return;
            RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_CLIFF_W", cast, 0, 0, false, true);
        } else {
            if (IsActiveThreadTerminating())
                return;
            RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_CLIFF_N", cast, 0, 0, false, true);
        }
        Game->EntitySetCutsceneBehaviour(thunderInScene, (ECutsceneBehaviour)1);
        Game->EntitySetAsDamageable(Game->GetHero(), true);
        Game->FixMovieSequenceCamera(false);
    }
    Game->SetTeleportingAsActive(false);
    Game->EntitySetAsAllowedToFollowHero(thunder, true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), thunder, true);
    OkToShowHealthBar = true;
}