// NScript::CQ_TentacleKrakenBossFightScript::PhaseOne() -- the prison-escape kraken fight: wait for the hero at the main chamber entrance, set objective 05, turn every DoorBlock into an invisible activation-receptor door behind a magic barrier, play CS_KRAKEN_INTRO, mark and health-bar the kraken, keep the mother dodging and following the hero until the kraken dies, then lift the barriers, set objective 06 and send the hero to the graveyard
// Grade: DIFFER(3039v3071) -- inline-budget residue: retail expands ~CScriptThing(marker) at the first in-loop terminate exit only (calls 0x004AA840 elsewhere) and calls the cast map dtor 0x00CDBFB0 where VC7.1 expands it (the declared-only explicit specialisation is not honoured here); the marker / cast-map frame slots then swap
// Next idea: a source shape that keeps ~map out of line in this TU (the specialisation works in other rows); every inline ~CScriptThing form (row-local out-of-class, in-class) expands every site (4025 bytes), /d2inlS15 unchanged
// Leaf: ?PhaseOne@CQ_TentacleKrakenBossFightScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DF5000, 3071 bytes, thiscall, plain `ret`. Ego_r.pdb ?PhaseOne@CQ_TentacleKrakenBossFightScript@NScript@@QAEXXZ;
//   the body (TKBF_MainChamberEntranceMarker, TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_05 / _06, CS_KRAKEN_INTRO, HUD_QUEST_ICON_KRAKEN) is the
//   prison-escape kraken phase, so the never_attempted ANALYSIS label DoMission_PrisonEscape is stale (LABELS.tsv).
// Layout: new header engine/CQ_TentacleKrakenBossFightScript.h (FABLE_RETAIL_NSCRIPT, chunk 853): CLevelScriptBase<CQ_SunnyvaleMasterData>,
//   MissionSucceeded +0x48 / MissionFailed +0x49 / KrakenAutoSaveCheckPoint +0x4a, CScriptThing TheKraken +0x4c, size 0x58.
// Spellings: __forceinline CScriptThing::operator= and ScriptedThingBase::operator= (CBaseObject::operator= 0x0099A3B0 + the PImp share)
//   reproduce retail's expanded assignments; FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY expands the push_back copy; declared-only
//   ~vector<CScriptThing> gives the 0x008AC970 calls; the kraken bar colour is CRGBColour(0, 0, 255, 255) (B,G,R,A: red, opaque).
// Callees (bindings checked, none bound elsewhere): IsActiveThreadTerminating 0x00CB7940, IsDistanceBetweenThingsUnder 0x00CBE2FF, CScriptThing
//   ~ 0x004AA840, CBaseClass 0x0099A2D0, CBaseObject 0x0099A380 / ~ 0x0099A430 / operator= 0x0099A3B0, ScriptedThingBase dtor 0x007E74D0, MovieBase
//   PImp dtor 0x006E7AB0, cast map operator[] 0x00CD3D2E / _M_erase 0x00CD344A, RunCutsceneMacro 0x00CBFB7D, vector<CScriptThing> ~ 0x008AC970 /
//   erase 0x008AC9D0 / _M_insert_overflow 0x008AD5F0, pGlobalScriptDef 0x0143E90C, CCharString 0x0099EBF0 / ~ 0x0099EAE0, malloc / free / delete.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_COPY
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "stlport/fable_map.h"
template <> std::map<CCharString, CScriptGameResourceObjectScriptedThingBase>::~map();                      // 0x00CDBFB0
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_TentacleKrakenBossFightScript.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_distance.h"
#include "stlport/fable_map.h"
#include "stlport/fable_containers.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
template <> std::vector<CScriptThing>::~vector();                                                           // 0x008AC970
#include "engine/fable_script_def_global.h"
inline CScriptThing::CScriptThing() { }
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other)
{
    CScriptGameResourceObjectBase::operator=(other);
    PImp = other.PImp;
    return *this;
}

void NScript::CQ_TentacleKrakenBossFightScript::PhaseOne()
{
    CScriptThing marker = Game->GetThingWithScriptName("TKBF_MainChamberEntranceMarker");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), marker, 10.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_05", "KrakenChamber", "");
    std::vector<CScriptThing> door_blocks;
    CScriptThing barrier;
    CScriptThing inviso_door;
    std::vector<CScriptThing> barriers;
    Game->GetAllThingsWithScriptName("DoorBlock", door_blocks);
    for (unsigned long i = 0; i < door_blocks.size(); ++i) {
        if (IsActiveThreadTerminating())
            return;
        inviso_door = Game->CreateObject("OBJECT_ACTIVATION_RECEPTOR_DOOR_TEST", door_blocks[i].GetPos(), "InvisoDoor");
        barrier = Game->CreateEffect("HK_MAGIC_BARRIER", door_blocks[i].GetPos(), "DoorBlock", 0.0f, false, false);
        Game->EntitySetFacingAngle(inviso_door, door_blocks[i].GetAngleXY(), true);
        Game->EntitySetFacingAngle(barrier, door_blocks[i].GetAngleXY(), true);
        Game->EntitySetAlpha(inviso_door, 0.0f, true);
        door_blocks[i] = inviso_door;
        barriers.push_back(barrier);
    }
    if (IsActiveThreadTerminating())
        return;
    TheKraken = Game->GetThingWithScriptName("TheKraken");
    {
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        CScriptGameResourceObjectScriptedThingBase hero;
        Game->StartScriptingEntity(Game->GetHero(), hero, SCRIPT_AI_PRIORITY_HIGHEST);
        mymap["Hero"] = hero;
        RunCutsceneMacro("CS_KRAKEN_INTRO", mymap, 0, 0, false, true);
        Game->OverrideMusic((EMusicSetType)0x17, false, false);
    }
    Game->EntityTeleportToThing(Game->GetThingWithScriptName("MotherTorture"), Game->GetThingWithScriptName("MK_MOTHER_KRAKEN"), false);
    Game->MiniMapAddMarker(TheKraken, "HUD_ORB_QUEST_CORE");
    Game->AddQuestInfoBarHealth(TheKraken, CRGBColour(0, 0, 255, 255), "HUD_QUEST_ICON_KRAKEN", 1.0f);
    MasterScriptData->MotherDodgingKraken = true;
    Game->EntityFollowThing(Game->GetThingWithScriptName("MotherTorture"), Game->GetHero(), (float)pGlobalScriptDef->MotherFollowingHeroKrakenSafetyDistance, true);
    while (TheKraken.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    for (unsigned long j = 0; j < door_blocks.size(); ++j) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(door_blocks[j], true, true);
        Game->CreateEffect("HK_MAGIC_BARRIER_OFF", barriers[j].GetPos(), "DoorBlockOff", 0.0f, false, false);
        Game->RemoveThing(barriers[j], true, true);
    }
    if (IsActiveThreadTerminating())
        return;
    door_blocks.clear();
    barriers.clear();
    MasterScriptData->MotherDodgingKraken = false;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_06", "Graveyard", "");
    CScriptThing exit_trigger = Game->GetThingWithScriptName("S_ExitTrigger");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), exit_trigger, 6.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "Graveyard");
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("GraveyardFromPrisonRegionEntrance"), false);
}
