// NScript::CV_HeroDuelScript::Duel() -- start the hero duel on Gibbet Hill: create Thunder (CREATURE_RIVAL_HERO_THUNDER) at ThunderWaitPos, take Thunder and then the hero under scripted control, play CS_GIBBETHILL_HERODUEL_HILLTOP_A (Whisper killed by the hero) or _HILLTOP_B inside a paused, camera-fixed movie sequence, then pause the script for a second
// Leaf: ?Duel@CV_HeroDuelScript@NScript@@QAEXXZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9BE60, 1171 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven): Ego_r.pdb name set ?Duel@CV_HeroDuelScript@NScript@@QAEXXZ; only caller the landed
//   CV_HeroDuelScript::DoMission 0x00E9B690 (site 0x00E9BBA8: `GotToExecutionTree = true; Duel(); DuelStarted = true;`);
//   literals "CREATURE_RIVAL_HERO_THUNDER" 0x012D0818, "ThunderWaitPos" 0x012ED548, "Thunder" 0x012ED164,
//   "CS_GIBBETHILL_HERODUEL_HILLTOP_A" 0x012ED524, "CS_GIBBETHILL_HERODUEL_HILLTOP_B" 0x012ED500.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, ~CScriptThing 0x004AA840 (the temporaries and the first exit expand it),
//   CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430 / ~CBaseClass 0x0099A2E0, IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   ~ScriptedThingBase 0x007E74D0 / operator= 0x008ABD10, map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E,
//   ~MovieBase 0x006E7B80, RunCutsceneMacro 0x00CBFB7D, IsActiveThreadTerminating 0x00CB7940, operator delete 0x00BFE9BC;
//   CScriptThing slot 6 GetPos (virtual call on the GetThingWithScriptName temporary); CGameScriptInterfaceBase slots 7, 8, 70,
//   72, 91 CreateCreature, 370, 371, 376 Pause, 379 (CWideScreenMagicPauseEntities), 512 EntitySetCutsceneBehaviour.
// Layout: CQ_SunnyvaleMasterData WhisperKilledByHero +0x74 (engine header).
// Shape: FirstFleeCutscene 0x00E9A970 (same TU), except that Thunder's handle lives for the whole function (the exits
//   after its wait loop and the tail after Pause(1.0f) share one ~ScriptedThingBase / ~CScriptThing epilogue) and only the
//   hero's handle and the cutscene objects sit in the inner block. Matched on the first grade.
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
#include "engine/fable_script_cutscene.h"
inline CScriptThing::~CScriptThing() { }
void NScript::CV_HeroDuelScript::Duel()
{
    CScriptThing thunder = Game->CreateCreature("CREATURE_RIVAL_HERO_THUNDER", Game->GetThingWithScriptName("ThunderWaitPos").GetPos(), "Thunder", false);
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
    {
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
        CScriptThing thunderInScene = Game->GetThingWithScriptName("Thunder");
        Game->EntitySetCutsceneBehaviour(thunderInScene, (ECutsceneBehaviour)2);
        if (MasterScriptData->WhisperKilledByHero) {
            if (IsActiveThreadTerminating())
                return;
            RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_HILLTOP_A", cast, 0, 0, false, true);
        } else {
            if (IsActiveThreadTerminating())
                return;
            RunCutsceneMacro("CS_GIBBETHILL_HERODUEL_HILLTOP_B", cast, 0, 0, false, true);
        }
        Game->EntitySetCutsceneBehaviour(thunderInScene, (ECutsceneBehaviour)1);
        Game->FixMovieSequenceCamera(false);
    }
    Game->Pause(1.0f);
}