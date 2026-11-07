// NScript::CV_KnotholeGladeGatesScript::WatchForCutscene1() -- spawned by DoMission: make the gate guard unpushable and unkillable, wait for BalvsKilled, then play CS_KNOTHOLEGATES_BALVSKILLED (HERO / CHIEF / LOOKOUT) inside a paused movie sequence, grant the gates experience and renown, and remove the guard if still alive
// Leaf: ?WatchForCutscene1@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ
// Inline-split: one single-site `do { } while (0);` round the three cast-map assignments makes VC7.1 expand the implicit
//   CScriptGameResourceObjectScriptedThingBase operator= there (CBaseObject::operator= 0x0099A3B0 + the PImp copy), as retail does;
//   without it the three assignments call the implicit operator= (1112v1334).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EA2720, 1334 bytes, plain `ret` (two epilogues).
// Identity (proven): the catalog placeholder "_global::KnotHoleGates_OnScript" is wrong. DoMission 0x00EA1090 builds
//   CSpawnedFunc<CV_KnotholeGladeGatesScript>("WatchForCutscene1", 0, this, &...) at 0x00EA1251..0x00EA127F (name literal
//   0x012EDBA4, member pointer 0x00EA2720 stored at +0x34) and hands it to AddSpawnedFunction with "KnotholeGlade"; FableWin.pdb and
//   Ego_r.pdb both hold ?WatchForCutscene1@CV_KnotholeGladeGatesScript@NScript@@QAEXXZ (public, void, no arguments).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, IsActiveThreadTerminating 0x00CB7940, CBaseObject()
//   0x0099A380 / operator= 0x0099A3B0 / ~CBaseObject 0x0099A430, the STB dtor COMDAT 0x007E74D0, ~CCountedPointer<MovieBase> 0x006E7AB0,
//   cast-map operator[] 0x00CD3D2E / ~map 0x00CDBFB0 (the map ctor is expanded: malloc 0x00BFEA0E of the 0x24 header node),
//   RunCutsceneMacro 0x00CBFB7D, ~CScriptThing 0x004AA840 (expanded at the first wait exit: ~CBaseClass 0x0099A2E0), __ftol2.
//   Interface (+0x40) slots: GetThingWithScriptName 72, SetIsPushableByHero 844 (CScriptThing by value: the copy is expanded),
//   SetIsThingForcePushable 745, EntitySetAsKillable 517, NewScriptFrame 7, GetHero 70, StartScriptingEntity 8, StartMovieSequence 370,
//   PauseAllNonScriptedEntities 379, FixMovieSequenceCamera 371, GiveHeroExperience 128, GiveHeroRenownPoints 161, RemoveThing 108;
//   CScriptThing slot 75 IsAlive through the inline IsAlive.
// Data: pGlobalScriptDef 0x0143E90C: EXPERIENCE_GRANT_LARGE +0x44, WBK_RenownAwardForGates +0xF98 (engine/CScriptDef.h).
// Layout: Ego_r.pdb CV_KnotholeGladeGatesScript (128 bytes) - 0x14, as the landed InitialiseVariables 0x00E9FEC0: BalvsKilled +0x51,
//   CutsceneStarting +0x52, SaidBefore +0x4E, GatesOpened +0x4D; base CLevelScriptBase<T> (Game +0x40), as the landed ctor 0x00E9FBD0.
//   No engine/CV_KnotholeGladeGatesScript.h exists yet: the class is modelled here as in the landed siblings (header debt).
// Strings: "KGG_HeroAtHole", "KG_GateGuard", "KG_Chief", "HERO", "CHIEF", "LOOKOUT", "", "CS_KNOTHOLEGATES_BALVSKILLED" (in that order).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();   // 0x006E7AB0 (out of line in retail)
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_map.h"
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0
inline CScriptThing::~CScriptThing() { }
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;   // 0x0143E90C

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_KnotholeGladeGatesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                         // +0x48
    bool MissionFailed;                            // +0x49
    bool MissionOver;                              // +0x4a
    bool Cutscene1Start;                           // +0x4b
    bool HeroAlertedToProblem;                     // +0x4c
    bool GatesOpened;                              // +0x4d
    bool SaidBefore;                               // +0x4e
    bool HeroKilledAVillager;                      // +0x4f
    bool IntroductionFinished;                     // +0x50
    bool BalvsKilled;                              // +0x51
    bool CutsceneStarting;                         // +0x52
    bool Mentioned;                                // +0x53
    bool ToldSuccess;                              // +0x54
    long BalvKillCount;                            // +0x58
    CScriptThing TheGate;                          // +0x5c
    bool FirstBalvDead;                            // +0x68
    void WatchForCutscene1();
};
}

void NScript::CV_KnotholeGladeGatesScript::WatchForCutscene1()
{
    CScriptThing hero_at_hole = Game->GetThingWithScriptName("KGG_HeroAtHole");
    CScriptThing gate_guard = Game->GetThingWithScriptName("KG_GateGuard");
    Game->SetIsPushableByHero(gate_guard, false);
    Game->SetIsThingForcePushable(gate_guard, false);
    Game->EntitySetAsKillable(gate_guard, false, true);
    while (!BalvsKilled) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CutsceneStarting = true;
    {
        CScriptThing chief = Game->GetThingWithScriptName("KG_Chief");
        CScriptGameResourceObjectScriptedThingBase seh_Chief;
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        CScriptGameResourceObjectScriptedThingBase seh_Lookout;
        Game->StartScriptingEntity(Game->GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(chief, seh_Chief, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(gate_guard, seh_Lookout, SCRIPT_AI_PRIORITY_HIGHEST);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        do {
            mymap["HERO"] = seh_Hero;
            mymap["CHIEF"] = seh_Chief;
            mymap["LOOKOUT"] = seh_Lookout;
        } while (0);
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CGameScriptInterfaceBase* game = Game;
        game->PauseAllNonScriptedEntities(true);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_KNOTHOLEGATES_BALVSKILLED", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        SaidBefore = true;
        GatesOpened = true;
        Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_LARGE);
        Game->GiveHeroRenownPoints((long)pGlobalScriptDef->WBK_RenownAwardForGates);
        game->PauseAllNonScriptedEntities(false);
    }
    if (gate_guard.IsAlive())
        Game->RemoveThing(gate_guard, false, true);
}