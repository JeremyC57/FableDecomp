// NScript::CQ_OrchardFarmRaidScript::DoCutsceneIfRequired() -- the Orchard Farm intro: find the hero's team spawn point (bandit or guard), then each frame wait for the Orchard Farm region; if the hero came in away from his team's spawn, close and lock the main gates, show the wrong-entrance info and wait until he leaves the region; once he arrives at the right entrance play the evil (bandits and the nearest guard) or good intro cut scene, kick off the matching quest start screen, open the main gates, deactivate the barricade quest and lock the farmhouse door
// Grade: DIFFER(2451v2412) -- md=41: (A) VC7.1 expands the inline CScriptThing release at three exits (loop exit, region-wait return, post-wait return 0x00DCFC14) where retail expands two and calls 0x004AA840 at the third, so the shared operator-delete tail sits in the other copy; (B) at DeactivateQuest("Q_OrchardFarm_Barricade") retail schedules `lea ecx` between the two ctor pushes and `lea eax` before `push ebp`
// Next idea: A: an inline device for the 0x00DCFC14 return only (inline_depth(0) there also merges the wait-loop copy into the loop-exit copy and flips the Data/Info zero order); B: find what puts this block in retail's interleaved schedule (same pattern at 0x00DCC613 / 0x00E58B8B, landed rows, natural there)
// Leaf: ?DoCutsceneIfRequired@CQ_OrchardFarmRaidScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCFA60, 2412 bytes, thiscall, no EH frame.
// Identity (proven, two sources): CQ_OrchardFarmRaidScript::Main 0x00DCC770 spawns it as a thread (0x00DCCE69 `mov [edi+34h],
//   0DCFA60h`) under the name literal "DoCutsceneIfRequired" (0x012DB1B8); Ego_r.pdb and FableWin.pdb both hold
//   ?DoCutsceneIfRequired@CQ_OrchardFarmRaidScript@NScript@@QAEXXZ.
// Callees: CBaseClass ctor / dtor 0x0099A2D0 / 0x0099A2E0, ~CScriptThing 0x004AA840, CCharString 0x0099EBF0 / 0x0099EAE0,
//   IsActiveThreadTerminating 0x00CB7940, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), scripted-thing ctor / dtor /
//   operator= 0x007E72A0 / 0x007E74D0 / 0x008ABD10, cast map ctor / dtor / operator[] 0x00CDBF70 / 0x00CDBFB0 / 0x00CD3D2E, movie
//   ctor / dtor 0x006E7B60 / 0x006E7B80, RunCutsceneMacro 0x00CBFB7D (__fastcall), ~vector<CScriptThing> 0x008AC970, operator
//   delete 0x00BFE9BC (the expanded CScriptThing release).
// Data: engine/CQ_OrchardFarmRaidScript.h (chunk 863 overlay): DoneIntroduction +0x50, HeroAtWrongEntrance +0x51, HeroTeam +0x78.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_OrchardFarmRaidScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_distance.h"
#include "engine/fable_script_cutscene.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline CScriptThing::CScriptThing() {}
inline CScriptThing::~CScriptThing() {}
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
template<> std::vector<CScriptThing>::~vector();              // 0x008AC970

void NScript::CQ_OrchardFarmRaidScript::DoCutsceneIfRequired()
{
    CScriptThing spawn;
    if (HeroTeam == 1) {
        if (IsActiveThreadTerminating()) return;
        spawn = Game->GetThingWithScriptName("BanditTeamSpawn");
    } else {
        if (IsActiveThreadTerminating()) return;
        spawn = Game->GetThingWithScriptName("GuardTeamSpawn");
    }
    for (; !IsActiveThreadTerminating(); Game->NewScriptFrame()) {
        while (!Game->IsRegionLoaded("OrchardFarm")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        if (IsDistanceBetweenThingsUnder(spawn, Game->GetHero(), 10.0f)) {
            if (IsActiveThreadTerminating()) return;
            if (HeroTeam == 1) {
                if (IsActiveThreadTerminating()) return;
                {
                    std::vector<CScriptThing> bandits;
                    Game->GetAllThingsWithScriptName("BanditTeamMember", bandits);
                    CScriptThing guard = Game->GetNearestWithScriptName(spawn, "GuardTeamMember");
                    CScriptGameResourceObjectScriptedThingBase seh_ban1;
                    CScriptGameResourceObjectScriptedThingBase seh_ban2;
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    CScriptGameResourceObjectScriptedThingBase seh_guard;
                    Game->StartScriptingEntity(guard, seh_guard, SCRIPT_AI_PRIORITY_HIGHEST);
                    Game->StartScriptingEntity(bandits[0], seh_ban1, SCRIPT_AI_PRIORITY_HIGHEST);
                    Game->StartScriptingEntity(bandits[1], seh_ban2, SCRIPT_AI_PRIORITY_HIGHEST);
                    Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                    Game->SheatheHeroWeapons();
                    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
                    cast["HERO"] = seh_hero;
                    cast["BAN1"] = seh_ban1;
                    cast["BAN2"] = seh_ban2;
                    cast["GUARD"] = seh_guard;
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("", movie);
                    CWideScreenMagicPauseEntities pause(*Game);
                    Game->FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_ORCHARD_EVIL_INTRO", cast, 0, 0, false, true);
                    Game->FixMovieSequenceCamera(false);
                }
                Game->KickOffQuestStartScreen("Q_OrchardFarmRaidEvil", true, false);
                Game->OpenDoor(Game->GetThingWithScriptName("OF_MainGates"));
            } else {
                if (IsActiveThreadTerminating()) return;
                Game->OpenDoor(Game->GetThingWithScriptName("OF_MainGates"));
                {
                    CScriptGameResourceObjectScriptedThingBase seh_hero;
                    Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
                    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
                    cast["HERO"] = seh_hero;
                    CScriptGameResourceObjectMovieBase movie;
                    Game->StartMovieSequence("", movie);
                    CWideScreenMagicPauseEntities pause(*Game);
                    Game->FixMovieSequenceCamera(true);
                    RunCutsceneMacro("CS_ORCHARD_GOOD_INTRO", cast, 0, 0, false, true);
                    Game->FixMovieSequenceCamera(false);
                }
                Game->KickOffQuestStartScreen("Q_OrchardFarmRaidGood", true, false);
            }
            Game->DeactivateQuest("Q_OrchardFarm_Barricade", 0);
            Game->Pause(1.0f);
            DoneIntroduction = true;
            Game->SetThingAsUsable(Game->GetThingWithScriptName("OFFarmhouseDoor"), false);
            return;
        }
        if (IsActiveThreadTerminating()) return;
        HeroAtWrongEntrance = true;
        Game->CloseDoor(Game->GetThingWithScriptName("OF_MainGates"));
        Game->SetThingAsUsable(Game->GetThingWithScriptName("OF_MainGates"), false);
        Game->Pause(1.0f);
        Game->DisplayGameInfo("TEXT_QST_051_WRONG_ENTRANCE");
        while (!Game->MsgIsGameInfoClickedPast()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        while (Game->IsRegionLoaded("OrchardFarm") != false) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        HeroAtWrongEntrance = false;
    }
}
