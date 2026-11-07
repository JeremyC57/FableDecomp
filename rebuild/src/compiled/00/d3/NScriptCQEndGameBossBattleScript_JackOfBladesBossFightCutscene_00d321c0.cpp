// NScript::CQ_EndGameBossBattleScript::JackOfBladesBossFightCutscene() -- the Jack of Blades boss-fight intro: take the hero and the sister under scripted control, hold the sister at MK_JACK1_HOLDJACK out of sight, play CS_JACK_BOSS_INTRO with the hero cast as "Hero" inside a paused movie sequence, then move the sister to MK_END1_SISTER and show her again
// Leaf: ?JackOfBladesBossFightCutscene@CQ_EndGameBossBattleScript@NScript@@QAEXXZ
// Inline-split: one single-site `do { } while (0);` round `mymap["Hero"] = seh_Hero;` makes VC7.1 expand the implicit
//   CScriptGameResourceObjectScriptedThingBase operator= there (CBaseObject::operator= 0x0099A3B0, then the declared-only
//   CCountedPointer<STB>::operator= 0x008AB1E0 called), as retail does; without it the assignment calls the implicit operator=.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D321C0, 1136 bytes, plain `ret` at 0x00D3262F (two epilogues).
// Identity (proven): the catalog placeholder "_global::CScriptGameResourceObject_LoadEndGameSister" is wrong. The only caller is
//   the direct call at 0x00D31358 inside CQ_EndGameBossBattleScript::JackOfBladesBossBattle 0x00D30030 (bound by the landed Main
//   0x00D2F7F0); FableWin.pdb's CQ_EndGameBossBattleScript source order is Main, JackOfBladesBossBattle, JackOfBladesBossFightCutscene,
//   JackOfBladesPhaseWatcher, and this body sits right after JackOfBladesBossBattle; it plays "CS_JACK_BOSS_INTRO" (the boss-fight
//   intro cut-scene) and is called directly (PhaseWatcher is a spawned function, referenced only by address); size 1136 against Ego_r's
//   ?JackOfBladesBossFightCutscene@CQ_EndGameBossBattleScript@NScript@@QAEXXZ 0x540 (both PDB name sets hold it; public, void, no args).
// Model: the landed CQ_EndGameBossBattleScript::CEndGameSister::Main 0x00D328C0 (defines, includes, declared-only PImp dtor and
//   cast-map ctor / dtor specialisations) on engine/CQ_EndGameBossBattleScript.h (CLevelScriptBase<CQ_SunnyvaleMasterData>, Game +0x40).
// Callees: CBaseObject() 0x0099A380 / operator= 0x0099A3B0 / ~CBaseObject 0x0099A430, STB IsValid 0x00CD23B9 / Release 0x00CD2770 /
//   dtor COMDAT 0x007E74D0 (expanded at the first wait exit), CCountedPointer<STB>::operator= 0x008AB1E0, CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840 (the sister-loop temporary's is expanded: ~CBaseClass 0x0099A2E0),
//   IsActiveThreadTerminating 0x00CB7940, cast map ctor 0x00CDBF70 / operator[] 0x00CD3D2E / dtor 0x00CDBFB0, RunCutsceneMacro
//   0x00CBFB7D, ~CCountedPointer<MovieBase> 0x006E7AB0. Interface (+0x40) slots: GetHero 70, StartScriptingEntity 8, NewScriptFrame 7,
//   GetThingWithScriptName 72, EntityTeleportToThing 472, EntitySetAsDrawable 511, StartMovieSequence 370, PauseAllNonScriptedEntities 379.
// Shape: the sister wait loop stays top-tested (`!StartScriptingEntity` kept in bl by neg / sbb / inc while the temporaries die), which
//   the plain while gives once the row-local inline ~CScriptThing expands its temporary; the hero wait loop is rotated.
// Strings: "EndGameSister", "MK_JACK1_HOLDJACK", "Hero", "", "CS_JACK_BOSS_INTRO", "MK_END1_SISTER" (each relocation paired with retail).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)
#include "engine/CQ_EndGameBossBattleScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
template <> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& other);   // 0x008AB1E0 (out of line in retail)
#include "engine/fable_script_cutscene.h"
#include "stlport/fable_map.h"
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::map();                       // 0x00CDBF70
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0
inline CScriptThing::~CScriptThing() { }
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };

void NScript::CQ_EndGameBossBattleScript::JackOfBladesBossFightCutscene()
{
    CScriptGameResourceObjectScriptedThingBase seh_Hero;
    if (seh_Hero.IsValid())
        seh_Hero.Release();
    while (!Game->StartScriptingEntity(Game->GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_Sister;
    if (seh_Sister.IsValid())
        seh_Sister.Release();
    while (!Game->StartScriptingEntity(Game->GetThingWithScriptName("EndGameSister"), seh_Sister, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->EntityTeleportToThing(Game->GetThingWithScriptName("EndGameSister"), Game->GetThingWithScriptName("MK_JACK1_HOLDJACK"), false);
    Game->EntitySetAsDrawable(Game->GetThingWithScriptName("EndGameSister"), false);
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
    do {
        mymap["Hero"] = seh_Hero;
    } while (0);
    {
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CGameScriptInterfaceBase* game = Game;
        game->PauseAllNonScriptedEntities(true);
        RunCutsceneMacro("CS_JACK_BOSS_INTRO", mymap, 0, 0, false, true);
        Game->EntityTeleportToThing(Game->GetThingWithScriptName("EndGameSister"), Game->GetThingWithScriptName("MK_END1_SISTER"), false);
        Game->EntitySetAsDrawable(Game->GetThingWithScriptName("EndGameSister"), true);
        game->PauseAllNonScriptedEntities(false);
    }
}