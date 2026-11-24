// NScript::CQ_TentacleKrakenBossFightScript::PhaseOne() -- the prison-escape kraken fight: wait for the hero at the main chamber entrance, set objective 05, turn every DoorBlock into an invisible activation-receptor door behind a magic barrier, play CS_KRAKEN_INTRO, mark and health-bar the kraken, keep the mother dodging and following the hero until the kraken dies, then lift the barriers, set objective 06 and send the hero to the graveyard
// Grade: DIFFER(3007v3071) -- call sequence = retail except vector<CScriptThing>::push_back: retail expands it (calls _M_insert_overflow 0x008AD5F0), ours calls it (c2 c1 61 at f 0.3125, sc 80 v 74); frame 0x90 v 0x94 follows from it. Forcing it (diagnostic probe p2, probe_hdr) = fg.py RELOCATION_MATCH, not raw-compared (B24S2)
// Next idea: an evidenced text that makes push_back (and CScriptThing::operator=, forced here as in 26 landed rows) cost below ~CScriptThing (c1 60) in c2 -- see ESCALATE.md
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
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_FORCEINLINE_ASSIGN
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
#define FABLE_RETAIL_PHASE1_ARCHER_PASS26
#include "engine/EScriptAIPriority.h"
#include "engine/EMusicSetType.h"
#include "engine/TRetVoid.h"
template <> std::vector<CScriptThing>::~vector();                                                           // 0x008AC970
#include "engine/fable_script_def_global.h"
inline CScriptThing::CScriptThing() { }
inline CScriptThing::~CScriptThing() { }
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
void NScript::CQ_TentacleKrakenBossFightScript::PhaseOne()
{
    TRetVoid _Fail_Val;
    CScriptThing StartMissionMarker = Game->GetThingWithScriptName("TKBF_MainChamberEntranceMarker");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), StartMissionMarker, 10.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_05", "KrakenChamber", "");
    std::vector<CScriptThing> DoorBlockVec;
    CScriptThing TempParticle;
    CScriptThing TempDoor;
    std::vector<CScriptThing> EffectVec;
    Game->GetAllThingsWithScriptName("DoorBlock", DoorBlockVec);
    if (false) {} else for (long i = 0; i < DoorBlockVec.size(); ++i) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        TempDoor = Game->CreateObject("OBJECT_ACTIVATION_RECEPTOR_DOOR_TEST", DoorBlockVec[i].GetPos(), "InvisoDoor");
        TempParticle = Game->CreateEffect("HK_MAGIC_BARRIER", DoorBlockVec[i].GetPos(), "DoorBlock", 0.0f, false, false);
        Game->EntitySetFacingAngle(TempDoor, DoorBlockVec[i].GetAngleXY(), true);
        Game->EntitySetFacingAngle(TempParticle, DoorBlockVec[i].GetAngleXY(), true);
        Game->EntitySetAlpha(TempDoor, 0.0f, true);
        DoorBlockVec[i] = TempDoor;
        EffectVec.push_back(TempParticle);
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    TheKraken = Game->GetThingWithScriptName("TheKraken");
    {
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> mymap;
        CScriptGameResourceObjectScriptedThingBase seh_Hero;
        Game->StartScriptingEntity(Game->GetHero(), seh_Hero, SCRIPT_AI_PRIORITY_HIGHEST);
        mymap["Hero"] = seh_Hero;
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
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    if (false) {} else for (long i = 0; i < DoorBlockVec.size(); ++i) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        Game->RemoveThing(DoorBlockVec[i], true, true);
        Game->CreateEffect("HK_MAGIC_BARRIER_OFF", EffectVec[i].GetPos(), "DoorBlockOff", 0.0f, false, false);
        Game->RemoveThing(EffectVec[i], true, true);
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    DoorBlockVec.clear();
    EffectVec.clear();
    MasterScriptData->MotherDodgingKraken = false;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_06", "Graveyard", "");
    CScriptThing exit_trigger = Game->GetThingWithScriptName("S_ExitTrigger");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), exit_trigger, 6.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->AddQuestRegion("Q_PrisonEscapeRescueMother", "Graveyard");
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return _Fail_Val.Return();
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("GraveyardFromPrisonRegionEntrance"), false);
}
