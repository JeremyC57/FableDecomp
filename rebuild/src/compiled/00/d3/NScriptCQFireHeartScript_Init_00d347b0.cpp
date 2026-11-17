// CQ_FireHeartScript::Init() -- register the demon-door lookout region and reset the quest state: flags, the pad FX names, the starting sun count by the hero's morality, the puzzle patterns and counts from the script def, the OPENCAGE cut-scene flag, the per-pad / per-prophet / per-villager bookkeeping vectors and the morality changes
// Leaf: ?Init@CQ_FireHeartScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D347B0, length 779 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / operator=(const CCharString&) 0x0099EFB0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, vector<CFireHeartPatternDef>::operator= 0x00D34D40,
//   map<CCharString, bool>::operator[] 0x008ADF10, vector<bool>::resize 0x00467686, CScriptThing() 0x006E7B40 /
//   ~CScriptThing 0x004AA840, vector<CScriptThing>::resize 0x00D34AC0, scripted-thing handle ctor 0x007E72A0 / dtor
//   0x007E74D0, vector<CScriptGameResourceObjectScriptedThingBase>::resize 0x00CD3CCB; through the interface (+0x40):
//   GetHeroMorality (157), AddQuestRegion (271), GetActiveQuestName (655).
// NF87: moved off the row-local CQ_FireHeartScript (CScriptBase-derived view), CCutsceneFlagMap / CCutsceneArgMap and
//   CScriptGameResourceObjectScriptedThingBase models onto engine/CQ_FireHeartScript.h (CLevelScriptBase, Game +0x40), so 0x008ADF10 binds
//   the STLport map<CCharString, bool>::operator[] spelling. The rest as NF48 (VECTOR_ASSIGN, VECTOR_RESIZE_OUTOFLINE).
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_CTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_map.h"
#include "engine/CQ_FireHeartScript.h"
#include "engine/fable_script_def_global.h"

void NScript::CQ_FireHeartScript::Init()
{
    Game->AddQuestRegion(Game->GetActiveQuestName(), "DemonDoor_LookoutPoint");
    QuestStartScreenShown = false;
    MissionFailed = false;
    MissionSucceeded = false;
    RoundEnded = false;
    RoundBeginning = false;
    PreventRoundEnd = false;
    ResetBlocks = false;
    HasHeroHitVillagersGroupOne = false;
    HasHeroHitVillagersGroupTwo = false;
    InfoDisplayed = false;
    PadSunFX = pGlobalScriptDef->FHSunPadFX;
    PadMoonFX = pGlobalScriptDef->FHMoonPadFX;
    if (Game->GetHeroMorality() < 0.5f) {
        if (IsActiveThreadTerminating())
            return;
        StartNumberOfSuns = pGlobalScriptDef->FHNumberOfSunsEvil;
    } else {
        if (IsActiveThreadTerminating())
            return;
        StartNumberOfSuns = pGlobalScriptDef->FHNumberOfSunsGood;
    }
    Pattern = pGlobalScriptDef->FHPatterns;
    PadRadius = pGlobalScriptDef->FHPadRadius;
    NumberOfRounds = pGlobalScriptDef->FHNumberOfRounds;
    NumberOfPatterns = pGlobalScriptDef->FHNumberOfPatterns;
    NumberOfPads = pGlobalScriptDef->FHNumberOfPads;
    NumberOfSuns = 0;
    PuzzleRound = 0;
    CutsceneFlags["OPENCAGE"] = false;
    FreedProphets = 0;
    KilledProphets = 0;
    NumberOfProphets = pGlobalScriptDef->FHNumberOfProphets;
    NumberOfVillagers = pGlobalScriptDef->FHNumberOfVillagers;
    StartingPattern.resize(NumberOfPads, false);
    ProphetsKilled.resize(NumberOfProphets, false);
    ProphetsReleased.resize(NumberOfProphets, false);
    ShouldProphetSpeak.resize(NumberOfProphets, false);
    ShouldVillagerSpeak.resize(NumberOfVillagers, false);
    Prophets.resize(NumberOfProphets);
    Villagers.resize(NumberOfVillagers);
    CommentGivenThisRound.resize(5, false);
    Guest = 0;
    ProphetsScripted = 0;
    seh_Prophet.resize(5);
    MissionHistory = (EMissionHistory)0;
    MoralityChangeOnKilled = pGlobalScriptDef->FHMoralityChangeOnKilled;
    MoralityChangeOnFreed = pGlobalScriptDef->FHMoralityChangeOnFreed;
    HasDemonDoorBeenEnabled = false;
    HasScytheCutscenePlayed = false;
    KilledAllPrisoners = true;
    FreedAllPrisoners = true;
    EvilRounds = 0;
    GoodRounds = 0;
}
