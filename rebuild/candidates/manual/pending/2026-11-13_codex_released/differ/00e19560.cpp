// NScript::CQ_WhiteBalverineWWScript::SpawnBalverines() -- the white balverine's three health rounds: make it 250 strong, then each time its health drops another quarter spawn 1, 2, 3 soldier balverines at MK_WBW_FIRSTSPAWN (or MK_WBW_FIRSTSPAWNB when the hero stands within 10 units of the first marker) and set them on the hero; the white balverine howls in a short cut-scene, and on the first round a soldier plays CS_WBW_FIRSTSPAWN / CS_WBW_FIRSTSPAWNB
// Grade: DIFFER(2159v2078) -- inline-budget split: VC7.1 expands the seh_soldier ScriptedThingBase ctor (retail calls 0x007E72A0) and ~CScriptThing(spawn_marker) at the in-while terminate exit (retail calls 0x004AA840 and tail-merges that exit, the post-while exit and the soldier exit into one ~seh / ~CScriptThing pad); everything else, frame 0xe4 and all slots agree
// Next idea: a source shape that makes the cut-scene block's while-exit colder than the for-loop exits (diagnostic /d2inlT60 drops the while-exit expansion but not the ctor one: not a pure budget gap)
// Leaf: ?SpawnBalverines@CQ_WhiteBalverineWWScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E19560, 2078 bytes, thiscall, plain `ret`, frame 0xe4. Ego_r.pdb ?SpawnBalverines@CQ_WhiteBalverineWWScript@NScript@@QAEXXZ.
// Shape: `for (long round = 1; round <= 3; )` with ++round before threshold -= quarter (retail's slot order: bools 0x12/0x13, spawn_marker
//   0x14, threshold 0x20, round 0x24; the do-while / threshold-first forms swap them); use_marker_b declared before first_spawn_done (retail
//   stores 0x13 first); a row-local `inline CScriptThing::~CScriptThing() { }` (the documented CScriptThing.h form) expands the in-for
//   balverine dtor and the two for-loop exits as retail does (without it every site is a call: 1929 bytes, md464).
// Callees (bindings checked: none bound elsewhere): IsActiveThreadTerminating 0x00CB7940, IsDistanceBetweenThingsUnder 0x00CBE2FF,
//   CScriptThing operator= 0x008AB980 / ~ 0x004AA840, CBaseObject 0x0099A380 / ~ 0x0099A430, CBaseClass::~CBaseClass 0x0099A2E0, ScriptedThingBase
//   ctor 0x007E72A0 / dtor 0x007E74D0 / operator= 0x008ABD10 / PlayAnimation 0x007E73D0 / IsPerformingScriptTask 0x007E7450 / IsValid 0x00CD23B9 /
//   Release 0x00CD2770, MovieBase ctor 0x006E7B60 / dtor 0x006E7B80 / PImp dtor 0x006E7AB0, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 /
//   operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D, CCharString 0x0099EBF0 / ~ 0x0099EAE0, __ftol2 0x00BFEA70, operator delete 0x00BFE9BC;
//   NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748 (established spelling; the probe's g_ScriptAnimFlag dropped).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_STLP_MAP
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
#include "engine/C3DVector.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_map.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#include "engine/fable_script_animation_defaults.h"   // NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations 0x01375748
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::map();                       // 0x00CDBF70
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0
inline CScriptThing::~CScriptThing() { }
__forceinline const C3DVector& CScriptThing::GetPos() const { return !PImp.Data ? NullCoord : PImp.Data->GetPos(); }

void NScript::CQ_WhiteBalverineWWScript::SpawnBalverines()
{
    bool use_marker_b = false;
    bool first_spawn_done = false;
    Game->ModifyThingHealth(WhiteBalverine, 250.0f, false);
    long quarter = (long)(Game->GetHealth(WhiteBalverine) * 0.25f);
    long threshold = (long)(Game->GetHealth(WhiteBalverine) - quarter);
    for (long round = 1; round <= 3; ) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Game->GetHealth(WhiteBalverine) < threshold) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing spawn_marker = Game->GetThingWithScriptName("MK_WBW_FIRSTSPAWN");
            if (IsDistanceBetweenThingsUnder(spawn_marker, Game->GetHero(), 10.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                spawn_marker = Game->GetThingWithScriptName("MK_WBW_FIRSTSPAWNB");
                use_marker_b = true;
            }
            for (long i = 0; i < round; ++i) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptThing balverine = Game->CreateCreature("CREATURE_BALVERINE_01", spawn_marker.GetPos(), "WBWW_SoldierBalverine", false);
                Game->GiveThingBestEnemyTarget(balverine, Game->GetHero());
                Game->Pause(0.2f);
            }
            if (IsActiveThreadTerminating())
                return;
            Game->EntitySetAsDamageable(WhiteBalverine, false);
            {
                CScriptGameResourceObjectScriptedThingBase seh_balverine;
                Game->StartScriptingEntity(WhiteBalverine, seh_balverine, SCRIPT_AI_PRIORITY_HIGHEST);
                {
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    Game->Pause(0.2f);
                    Game->EntitySetAttackThingImmediately(WhiteBalverine, Game->GetHero(), true, true);
                    Game->Pause(0.2f);
                    Game->PlaySoundOnThing(WhiteBalverine, "SND_LONGWOLFHOWL_01");
                    seh_balverine.PlayAnimation("HOWL", false, false, false, true, NGlobalConsole::GDefaultToUsingPhysicsInCutsceneAnimations, false, false);
                    while (seh_balverine.IsPerformingScriptTask()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating())
                            return;
                    }
                    if (IsActiveThreadTerminating())
                        return;
                    Game->CameraDefault();
                }
                if (seh_balverine.IsValid())
                    seh_balverine.Release();
            }
            Game->EntitySetAsDamageable(WhiteBalverine, true);
            if (!first_spawn_done) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptGameResourceObjectScriptedThingBase seh_soldier;
                CScriptThing soldier = Game->GetRandomThingWithScriptName("WBWW_SoldierBalverine");
                Game->StartScriptingEntity(soldier, seh_soldier, SCRIPT_AI_PRIORITY_HIGHEST);
                if (use_marker_b) {
                    if (IsActiveThreadTerminating())
                        return;
                    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["BALV"] = seh_soldier;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    RunCutsceneMacro("CS_WBW_FIRSTSPAWNB", mymap, 0, 0, false, true);
                    Game->Pause(0.1f);
                    Game->CameraDefault();
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
                    mymap["BALV"] = seh_soldier;
                    CScriptGameResourceObjectMovieBase speech_movie;
                    Game->StartMovieSequence("", speech_movie);
                    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
                    RunCutsceneMacro("CS_WBW_FIRSTSPAWN", mymap, 0, 0, false, true);
                    Game->Pause(0.1f);
                    Game->CameraDefault();
                }
                first_spawn_done = true;
            }
            ++round;
            threshold -= quarter;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
