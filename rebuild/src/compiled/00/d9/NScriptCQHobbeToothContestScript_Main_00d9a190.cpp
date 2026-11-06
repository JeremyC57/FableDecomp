// NScript::CQ_HobbeToothContestScript::Main() -- the hobbe tooth contest quest main: set objective 1, wait for Orchard Farm East, bind the seven entity scripts, spawn WatchForTermination, create Whisper at WhisperStart, take the farmer and Whisper under scripted control, wait for the hero at MK_HTC_HEROSTART (the farmer's on-hit line if Whisper or the farmer was hit), set objective 2, play CS_HOBBESTOOTH_INTRO, kick off the quest start screen, run DisplayTimer, then clear the hobbes and play the won / draw / lost outro with its renown award
// Leaf: ?Main@CQ_HobbeToothContestScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D9A190, 4441 bytes, thiscall, plain `ret`, no EH frame; every exit runs one destructor cascade.
// Identity (proven): slot 2 (Main) of the quest-script vtable (RegisterMain 0x00D9A0A0 spawns it as "Main"); Ego_r.pdb name set
//   ?Main@CQ_HobbeToothContestScript@NScript@@UAEXXZ. Every bound Alloc is a landed CQ_HobbeToothContestScript::C*::Alloc row
//   (binding-name literal = class name less its C); the thread is the landed WatchForTermination 0x00D9B2F0 ("WatchForTermination");
//   DisplayTimer 0x00D9B420 is the landed row (Ego_r.pdb ?DisplayTimer@CQ_HobbeToothContestScript@NScript@@QAEXXZ).
// Callees: operator new 0x00BFEA1A, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0, CScriptBase::AddEntityScriptBinding 0x00CB8230 /
//   PostAddScriptedEntities 0x00CB8930 / AddSpawnedFunction 0x00CB7E50 / IsActiveThreadTerminating 0x00CB7940, CSpawnedFuncBase 0x00CDD450,
//   ~CScriptThing 0x004AA840, CBaseObject 0x0099A380, scripted-thing handle ctor 0x007E72A0 / dtor 0x007E74D0 / operator= 0x008ABD10 /
//   IsValid 0x00CD23B9 / Release 0x00CD2770 / Speak 0x007E7390 / IsPerformingScriptTask 0x007E7450 / GetScriptThing 0x007E7490, movie
//   ctor 0x006E7B60 / dtor 0x006E7B80, cast map ctor 0x00CDBF70 / dtor 0x00CDBFB0 / operator[] 0x00CD3D2E, RunCutsceneMacro 0x00CBFB7D,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF, KillAllThingsInVector 0x00CBED82, ~vector<CScriptThing> 0x008AC970, __ftol2 0x00BFEA70;
//   pGlobalScriptDef 0x0143E90C (HOB_BoastHobbeMany +0xF24, HOB_RenownAwardSuccess / Draw / Lost +0xF28 / +0xF2C / +0xF30); the shared
//   0.0f 0x0122DEDC (open naming decision, HANDOFF).
// Layout: engine/CQ_HobbeToothContestScript.h (new, chunk 799; Ego_r.pdb offsets - 0x14); CQ_SunnyvaleMasterData::HobbeContestKillMoreHobbes
//   +0x11B through MasterScriptData +0x44.
// Shape: the landed quest-Main binding model (CQ_RansomVictimScript::Main 0x00DE0850) with the cast-map cutscenes of the HeroSouls
//   PlayCutscene rows. The CreateCreature result is the named local `whisper` (destroyed last, at every exit); the GetThingWithScriptName
//   temporary under it dies at once. FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR: with a plain inline ctor VC7.1 calls it
//   at the farmer's on-hit cutscene, which also moves the frame slots. The three outro branches end in the same GiveHeroRenownPoints call
//   and map destructor; VC7.1 tail-merges them as retail does.
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HobbeToothContestScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/fable_script_cutscene.h"
#include "engine/CScriptDef.h"
#include "engine/CDefPointer.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
enum ETextGroupSelectionMethod { GROUP_SELECT_NONE=0, GROUP_SELECT_RANDOM=1, GROUP_SELECT_RANDOM_NO_REPEAT=2, GROUP_SELECT_SEQUENTIAL=3, GROUP_SELECT_FIRST_ONLY=4 };
extern CDefPointer<const CScriptDef> pGlobalScriptDef;
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
void __fastcall KillAllThingsInVector(CGameScriptInterfaceBase& game, const std::vector<CScriptThing>& things, bool fade);   // 0x00CBED82
template <> std::vector<CScriptThing, std::allocator<CScriptThing> >::~vector();   // 0x008AC970 (out of line in retail)
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::map();
template<> std::map<CCharString,CScriptGameResourceObjectScriptedThingBase>::~map();
namespace NScript {
#define FABLE_HOBBE_ENTITY(Name) \
    class CQ_HobbeToothContestScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_HobbeToothContestScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_HOBBE_ENTITY(CQ_HobbeToothHobbe)        // Alloc 0x00D9B7C0
FABLE_HOBBE_ENTITY(CQ_HobbeToothWhisper)      // Alloc 0x00D9CF60
FABLE_HOBBE_ENTITY(COrchardFarmFarmer)        // Alloc 0x00D9C560
FABLE_HOBBE_ENTITY(CDrinkingHobbe)            // Alloc 0x00D9BE30
FABLE_HOBBE_ENTITY(CDrinkersReactMarker)      // Alloc 0x00D9C1D0
FABLE_HOBBE_ENTITY(CSpawnNewHobbe)            // Alloc 0x00D9CE80
FABLE_HOBBE_ENTITY(CHobbeBarrel)              // Alloc 0x00D9C3C0
#undef FABLE_HOBBE_ENTITY
}

void NScript::CQ_HobbeToothContestScript::Main()
{
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HOBBE_CONTEST_OBJECTIVE_01", "", "Greatwood");
    while (!Game->IsLevelLoaded("OrchardFarmEast")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeToothContestScript>("Q_HobbeToothHobbe", this, &CQ_HobbeToothHobbe::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeToothContestScript>("Q_HobbeToothWhisper", this, &CQ_HobbeToothWhisper::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeToothContestScript>("OrchardFarmFarmer", this, &COrchardFarmFarmer::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeToothContestScript>("DrinkingHobbe", this, &CDrinkingHobbe::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeToothContestScript>("DrinkersReactMarker", this, &CDrinkersReactMarker::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeToothContestScript>("SpawnNewHobbe", this, &CSpawnNewHobbe::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_HobbeToothContestScript>("HobbeBarrel", this, &CHobbeBarrel::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_HobbeToothContestScript>("WatchForTermination", 0, this, &CQ_HobbeToothContestScript::WatchForTermination), CCharString(""));
    CScriptThing whisper = Game->CreateCreature("CREATURE_RIVAL_HERO_WHISPER_HOBBE_TEETH_CONTEST", Game->GetThingWithScriptName("WhisperStart").GetPos(), "Q_HobbeToothWhisper", false);
    CScriptThing farmer = Game->GetThingWithScriptName("OrchardFarmFarmer");
    CScriptGameResourceObjectScriptedThingBase seh_farmer;
    if (seh_farmer.IsValid()) seh_farmer.Release();
    while (!Game->StartScriptingEntity(farmer, seh_farmer, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_whisper;
    Game->StartScriptingEntity(whisper, seh_whisper, SCRIPT_AI_PRIORITY_HIGHEST);
    while (!IsDistanceBetweenThingsUnder(Game->GetThingWithScriptName("MK_HTC_HEROSTART"), Game->GetHero(), 8.0f) && !WhisperOrFarmerHit) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HOBBE_CONTEST_OBJECTIVE_02", "", "Greatwood");
    if (WhisperOrFarmerHit) {
        if (IsActiveThreadTerminating())
            return;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        if (Game->GetHealth(seh_farmer.GetScriptThing()) > 0.0f) {
            seh_farmer.Speak(Game->GetHero(), "TEXT_QST_035_FARMER_ON_HIT", GROUP_SELECT_NONE, false, true, false);
            while (seh_farmer.IsPerformingScriptTask()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    {
        CScriptGameResourceObjectScriptedThingBase seh_hero;
        while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["WHISPER"] = seh_whisper;
        mymap["FARMER"] = seh_farmer;
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_HOBBESTOOTH_INTRO", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
    }
    seh_farmer.Release();
    seh_whisper.Release();
    if (!QuestStartScreenShown) {
        if (IsActiveThreadTerminating())
            return;
        Game->KickOffQuestStartScreen("Q_HobbeToothContest", true, false);
        QuestStartScreenShown = true;
    }
    CS_IntroDone = true;
    Game->Pause(3.0f);
    DisplayTimer();
    CS_OutroBegin = true;
    CScriptGameResourceObjectScriptedThingBase seh_hero;
    while (!Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (seh_farmer.IsValid()) seh_farmer.Release();
    while (!Game->StartScriptingEntity(farmer, seh_farmer, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (seh_whisper.IsValid()) seh_whisper.Release();
    while (!Game->StartScriptingEntity(whisper, seh_whisper, SCRIPT_AI_PRIORITY_HIGHEST)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectMovieBase speech_movie;
    Game->StartMovieSequence("", speech_movie);
    CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
    Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0));
    Game->Pause(1.0f);
    std::vector<CScriptThing> hobbes;
    Game->GetAllThingsWithScriptName("Q_HobbeToothHobbe", hobbes);
    KillAllThingsInVector(*Game, hobbes, false);
    if (NumberOfHobbeTeethCollectedByHero > NumberOfHobbeTeethCollectedByWhisper + pGlobalScriptDef->HOB_BoastHobbeMany) {
        if (IsActiveThreadTerminating())
            return;
        MasterScriptData->HobbeContestKillMoreHobbes = true;
    }
    if (NumberOfHobbeTeethCollectedByHero > NumberOfHobbeTeethCollectedByWhisper) {
        if (IsActiveThreadTerminating())
            return;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["FARMER"] = seh_farmer;
        mymap["WHISPER"] = seh_whisper;
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_HOBBESTEETH_OUTRO_WON", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        seh_whisper.Release();
        Game->RemoveThing(whisper, false, true);
        seh_farmer.Release();
        seh_hero.Release();
        MissionSucceeded = true;
        Game->GiveHeroRenownPoints((long)pGlobalScriptDef->HOB_RenownAwardSuccess);
    } else if (NumberOfHobbeTeethCollectedByHero == NumberOfHobbeTeethCollectedByWhisper) {
        if (IsActiveThreadTerminating())
            return;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["FARMER"] = seh_farmer;
        mymap["WHISPER"] = seh_whisper;
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_HOBBESTEETH_OUTRO_DRAW", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        seh_whisper.Release();
        Game->RemoveThing(whisper, false, true);
        seh_farmer.Release();
        seh_hero.Release();
        MissionFailed = true;
        Game->GiveHeroRenownPoints((long)pGlobalScriptDef->HOB_RenownAwardDraw);
    } else {
        if (IsActiveThreadTerminating())
            return;
        std::map<CCharString,CScriptGameResourceObjectScriptedThingBase> mymap;
        mymap["HERO"] = seh_hero;
        mymap["FARMER"] = seh_farmer;
        mymap["WHISPER"] = seh_whisper;
        Game->FixMovieSequenceCamera(true);
        RunCutsceneMacro("CS_HOBBESTEETH_OUTRO_LOST", mymap, 0, 0, false, true);
        Game->FixMovieSequenceCamera(false);
        seh_whisper.Release();
        Game->RemoveThing(whisper, false, true);
        seh_farmer.Release();
        seh_hero.Release();
        MissionFailed = true;
        Game->GiveHeroRenownPoints((long)pGlobalScriptDef->HOB_RenownAwardLost);
    }
}