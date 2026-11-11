// NScript::CQ_BountyHuntScript::Main() -- the bounty hunt quest main: bind the seven entity scripts, start the boast and Fisher Creek hut-door threads, deactivate the fisherman quest when its region is not loaded, objective 01, wait for the lookout point and the hero at P_BHHERO2, the intro cutscene, wait to enter Greatwood from the lookout point, the failure / success watch threads, the second intro cutscene (leader, deputies, hostages), the first grunt spawn and objective 02; then until the quest succeeds or fails re-create each teleported gang member in the region it was sent to, and finally fail (intro / hostage 1 / hostage 2 text) or complete the quest, wait for the region to clear and deactivate it
// Grade: DIFFER(6831v6917) -- 280 masked lines (chunk H40d: compiles again on HEAD with the re-based `FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_MAIN` block in hdr/engine/CQ_BountyHuntScript.h; 1 grade). Independent causes read from the diff: (1) retail expands the movie dtor at both cutscene ends (0x00D13D94, 0x00D1460E), ours calls 0x006E7B80; (2) the three exit-wait branches: retail keeps each branch's closing thread check (`call T; je tail; jmp exit` at 0x00D14E43 and 0x00D14EE4, `jne exit` + fall-through at 0x00D14F84), ours merges the three into one and puts the shared tail after the first branch -- retail's `je tail ; jmp exit` form means each of those two `return`s still had its own cleanup copy when blocks were laid out; (3) the `GetDataString() == "FromLookoutPoint"` compare block is out of line in retail (0x00D13F92, after a `ret`), inline in ours; (4) cast2 / seh_hero2 / seh_hostage slots and the order of the last cast2 assignment against the movie ctor (0x00D1449D..0x00D14541); (5) the QuestSucceeded / QuestFailed loop test (0x00D14B5A); (6) string-compare registers
// Next idea: (2) first -- find what makes the two QuestFailed branches' closing `return`s distinct blocks (a statement after the wait loop that both arms own and c2 cross-jumps away, the 0x00E64FB0 lever, would also keep the thread checks apart; tools/xj.sh shows the late cross-jump's inputs without a grade); (1) is an inline-budget split: read it with tools/inl.sh before any device. The four nested entity classes now at HEAD (CBH_GangDeputy1 / 2, CBH_Hostage1 / 2) should come from their header opt-ins instead of the row-local FABLE_BH_ENTITY declarations before this row can land
// Leaf: ?Main@CQ_BountyHuntScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D13580, 6917 bytes, thiscall, plain `ret`, no EH frame.
// Identity (proven, two sources): Ego_r.pdb and FableWin.pdb name sets hold ?Main@CQ_BountyHuntScript@NScript@@UAEXXZ; the address has one
//   .rdata reference, slot 2 of the level-script vtable 0x012C9E68 (slot 1 the landed RegisterMain 0x00D13070, 3 Init 0x00D12D00,
//   4 OnPersist 0x00D13130).
// Callees: TeleportData ctor 0x00D15140 / operator= 0x00D15170 / implicit dtor COMDAT 0x00CD4550 (the early exit expands it); the
//   seven bound Allocs (CBH_Hostage1 0x00D160B0, CBH_Hostage2 0x00D16290, CBH_GangLeader 0x00D16380, CBH_GangDeputy1 0x00D15FC0,
//   CBH_GangDeputy2 0x00D161A0, CBH_GangGrunt 0x00D15940, CBH_GangGruntSpawn 0x00D153F0); threads WatchForBoasts 0x00D17A80,
//   WatchForEnableFisherCreekHutDoor 0x00D151D0, WatchForQuestFailure 0x00D15090, WatchForQuestSuccess 0x00D150F0,
//   WatchForAllGruntsKilled 0x00D17340; SpawnBanditGangGrunts 0x00D17470; RunCutsceneMacro 0x00CBFB7D; IsDistanceBetweenThingsUnder
//   0x00CBE2FF (GetHero first); CCharString compare 0x00411570; pGlobalScriptDef->BountyHuntCompletionMorality (+0xB5C).
// Layout: engine/CQ_BountyHuntScript.h with the chunk 812 MAIN opt-in (hdr/README.md).
#define FABLE_RETAIL_CWIDESCREENMAGICPAUSEENTITIES_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_INLINE
#define FABLE_RETAIL_CSCRIPTTHING_NULLCOORD
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_MAIN
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR
#include <string.h>
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class CScriptGameResourceObjectMovieBase;
template <> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();                        // 0x006E7AB0
#include "engine/CScriptThing.h"
#include "engine/C3DVector.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_BountyHuntScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CScriptGameResourceObjectMovieBase.h"
#include "engine/CWideScreenMagicPauseEntities.h"
#include "engine/CTimer.h"
#include "engine/fable_script_cutscene.h"
#include "engine/fable_script_def_global.h"
#include "engine/fable_script_distance.h"
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
inline CScriptThing::~CScriptThing() { }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
template <> CCountedPointer<CScriptGameResourceObjectScriptedThingBase>& CCountedPointer<CScriptGameResourceObjectScriptedThingBase>::operator=(const CCountedPointer<CScriptGameResourceObjectScriptedThingBase>&);   // 0x008AB1E0
__forceinline CScriptGameResourceObjectScriptedThingBase& CScriptGameResourceObjectScriptedThingBase::operator=(const CScriptGameResourceObjectScriptedThingBase& other) { CScriptGameResourceObjectBase::operator=(other); PImp = other.PImp; return *this; }
extern CGameScriptInterfaceBase* pDSTGame;                              // 0x0143E8F8
__forceinline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }

namespace NScript {
#define FABLE_BH_ENTITY(Name) \
    class CQ_BountyHuntScript::Name : public CEntityScriptBase { \
    public: \
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_BountyHuntScript&, CScriptDataBase*, const CScriptThing&); \
    };
FABLE_BH_ENTITY(CBH_Hostage1)               // Alloc 0x00D160B0
FABLE_BH_ENTITY(CBH_Hostage2)               // Alloc 0x00D16290
FABLE_BH_ENTITY(CBH_GangLeader)             // Alloc 0x00D16380
FABLE_BH_ENTITY(CBH_GangDeputy2)            // Alloc 0x00D161A0
FABLE_BH_ENTITY(CBH_GangDeputy1)            // Alloc 0x00D15FC0
FABLE_BH_ENTITY(CBH_GangGrunt)              // Alloc 0x00D15940
FABLE_BH_ENTITY(CBH_GangGruntSpawn)         // Alloc 0x00D153F0
#undef FABLE_BH_ENTITY
}

void NScript::CQ_BountyHuntScript::Main()
{
    TeleportData teleport;
    CCharString defName;
    CCharString scriptName;
    CCharString teleportTo;
    CCharString teleportMkr;
    CCharString regionName;
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BountyHuntScript>("BH_Hostage1", this, &CBH_Hostage1::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BountyHuntScript>("BH_Hostage2", this, &CBH_Hostage2::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BountyHuntScript>("BH_GangLeader", this, &CBH_GangLeader::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BountyHuntScript>("BH_GangDeputy1", this, &CBH_GangDeputy1::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BountyHuntScript>("BH_GangDeputy2", this, &CBH_GangDeputy2::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BountyHuntScript>("BH_GangGrunt", this, &CBH_GangGrunt::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_BountyHuntScript>("BH_GangGruntSpawn", this, &CBH_GangGruntSpawn::Alloc, 1));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>("WatchForBoasts", 0, this, &CQ_BountyHuntScript::WatchForBoasts), CCharString(""));
    if (Game->IsQuestActive("V_Fisherman") && !Game->IsRegionLoaded("Fisherman")) {
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuest("V_Fisherman", 0);
        FishermanDeactivated = true;
    }
    AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>("WatchForEnableFisherCreekHutDoor", 0, this, &CQ_BountyHuntScript::WatchForEnableFisherCreekHutDoor), CCharString(""));
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BOUNTY_HUNT_OBJECTIVE_01", "GreatwoodEntrance", "HeroGuildComplexInside");
    Game->SetCreatureGeneratorsCreatureGroupAsEnabled((ECreatureGroup)4, false);
    Game->SetCreatureGeneratorsCreatureGroupAsEnabled((ECreatureGroup)0x8000, false);
    while (!Game->IsRegionLoaded("LookoutPoint")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), Game->GetThingWithScriptName("P_BHHERO2"), 10.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase seh_hero;
    std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast;
    Game->StartScriptingEntity(Game->GetHero(), seh_hero, SCRIPT_AI_PRIORITY_HIGHEST);
    {
        CScriptGameResourceObjectMovieBase speech_movie;
        Game->StartMovieSequence("", speech_movie);
        CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
        cast["Hero"] = seh_hero;
        RunCutsceneMacro("CS_BOUNTYHUNT_INTRO", cast, 0, 0, false, true);
        if (seh_hero.IsValid()) seh_hero.Release();
    }
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        while (!Game->MsgOnRegionLoaded(regionName)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (regionName == "GreatwoodEntrance") {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing entrance = Game->GetNearestWithDefName(Game->GetHero(), "REGION_ENTRANCE_POINT");
            if (entrance.IsAlive() && entrance.GetDataString() == "FromLookoutPoint") {
                if (IsActiveThreadTerminating())
                    return;
                BountyHuntIntroComplete = true;
                break;
            }
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->IsRegionLoaded("GreatwoodEntrance")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>("WatchForQuestFailure", 0, this, &CQ_BountyHuntScript::WatchForQuestFailure), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>("WatchForQuestSuccess", 0, this, &CQ_BountyHuntScript::WatchForQuestSuccess), CCharString(""));
    CScriptGameResourceObjectScriptedThingBase seh_leader;
    CScriptGameResourceObjectScriptedThingBase seh_deputy1;
    CScriptGameResourceObjectScriptedThingBase seh_deputy2;
    CScriptThing leader = Game->GetThingWithScriptName("BH_GangLeader");
    CScriptThing deputy1 = Game->GetThingWithScriptName("BH_GangDeputy1");
    CScriptThing deputy2 = Game->GetThingWithScriptName("BH_GangDeputy2");
    CScriptThing hostage1 = Game->GetThingWithScriptName("BH_Hostage1");
    CScriptThing hostage2 = Game->GetThingWithScriptName("BH_Hostage2");
    {
        std::map<CCharString, CScriptGameResourceObjectScriptedThingBase> cast2;
        CScriptGameResourceObjectScriptedThingBase seh_hero2;
        CScriptGameResourceObjectScriptedThingBase seh_hostage2;
        CScriptGameResourceObjectScriptedThingBase seh_hostage1;
        Game->StartScriptingEntity(leader, seh_leader, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(deputy1, seh_deputy1, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(deputy2, seh_deputy2, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(hostage1, seh_hostage1, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(hostage2, seh_hostage2, SCRIPT_AI_PRIORITY_HIGHEST);
        Game->StartScriptingEntity(Game->GetHero(), seh_hero2, SCRIPT_AI_PRIORITY_HIGHEST);
        cast2["LEADER"] = seh_leader;
        cast2["DEPUTY"] = seh_deputy1;
        cast2["DEPUTY2"] = seh_deputy2;
        cast2["HERO"] = seh_hero2;
        cast2["FEMALE"] = seh_hostage2;
        cast2["MALE"] = seh_hostage1;
        {
            CScriptGameResourceObjectMovieBase speech_movie;
            Game->StartMovieSequence("", speech_movie);
            CWideScreenMagicPauseEntities PauseMyEntitiesPlease(*Game);
            RunCutsceneMacro("CS_BOUNTYHUNT_INTRO_TWO", cast2, 0, 0, false, true);
            if (seh_hero2.IsValid()) seh_hero2.Release();
            if (seh_hostage2.IsValid()) seh_hostage2.Release();
            if (seh_hostage1.IsValid()) seh_hostage1.Release();
        }
    }
    if (seh_leader.IsValid()) seh_leader.Release();
    if (seh_deputy1.IsValid()) seh_deputy1.Release();
    if (seh_deputy2.IsValid()) seh_deputy2.Release();
    BanditCutsceneDone = true;
    SpawnBanditGangGrunts("M_BH_GruntSpawnEntrance1");
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_BOUNTY_HUNT_OBJECTIVE_02", "Fisherman", "HeroGuildComplexInside");
    Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
    QuestStarted = true;
    if (IsActiveThreadTerminating())
        return;
    unsigned long appearanceSeed;
    float health;
    while (!QuestSucceeded && !QuestFailed) {
        if (Game->MsgOnRegionLoaded(regionName)) {
            if (IsActiveThreadTerminating())
                return;
            int count = TeleportInfo.size();
            for (int i = 0; i < count; ++i) {
                teleport = TeleportInfo[i];
                if ((regionName == "GreatwoodEntrance" && teleport.teleportTo == "GreatwoodEntrance" && FishComplete) ||
                    (regionName == "GreatwoodLake" && teleport.teleportTo == "GreatwoodLake" && FishComplete && Deputy1Killed) ||
                    (regionName == "Fisherman" && teleport.teleportTo == "Fisherman")) {
                    if (IsActiveThreadTerminating())
                        return;
                    defName = teleport.defName;
                    appearanceSeed = teleport.appearanceSeed;
                    health = teleport.health;
                    scriptName = teleport.scriptName;
                    teleportMkr = teleport.teleportMkr;
                    CScriptThing marker = Game->GetThingWithScriptName(teleportMkr);
                    C3DVector pos = marker.GetPos();
                    CScriptThing creature = Game->CreateCreature(defName, pos, scriptName, false);
                    Game->EntitySetAppearanceSeed(creature, appearanceSeed);
                    Game->ModifyThingHealth(creature, health - Game->GetHealth(creature), false);
                    Game->SetThingHasInformation(creature, false, false, false);
                    Game->EntityAttachToScript(creature, "Q_BountyHunt");
                    TeleportInfo[i].defName = "";
                    TeleportInfo[i].appearanceSeed = 0;
                    TeleportInfo[i].health = 0;
                    TeleportInfo[i].scriptName = "";
                    TeleportInfo[i].teleportMkr = "";
                    TeleportInfo[i].teleportTo = "";
                }
            }
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsActiveThreadTerminating())
        return;
    if (FishermanDeactivated) {
        if (IsActiveThreadTerminating())
            return;
        Game->ActivateQuest("V_Fisherman");
    }
    if (QuestFailed) {
        if (IsActiveThreadTerminating())
            return;
        if (VillagerKilled && !VillagerIntroDone) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_017_QUEST_FAILED_INTRO", true);
        }
        else if (Hostage1Killed) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_017_QUEST_FAILED_HOSTAGE1", true);
        }
        else if (Hostage2Killed) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_017_QUEST_FAILED_HOSTAGE2", true);
        }
        else {
            if (IsActiveThreadTerminating())
                return;
        }
    }
    else if (QuestSucceeded) {
        if (IsActiveThreadTerminating())
            return;
        if (BoastActiveTimeLimit && BoastTime.GetTime() > 0) {
            if (IsActiveThreadTerminating())
                return;
            MasterScriptData->BountyHuntWithinTimeLimit = true;
        }
        Game->GiveHeroMorality(pGlobalScriptDef->BountyHuntCompletionMorality);
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
    }
    AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>("WatchForAllGruntsKilled", 0, this, &CQ_BountyHuntScript::WatchForAllGruntsKilled), CCharString(""));
    if (QuestFailed) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->IsRegionLoaded("Fisherman")) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->MsgOnRegionLoaded(regionName) && !((Deputy1Escaped || Deputy1Killed) && AllGruntsKilled)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        else if (Game->IsRegionLoaded("GreatwoodLake")) {
            if (IsActiveThreadTerminating())
                return;
            while (!Game->MsgOnRegionLoaded(regionName) && !((LeaderEscaped || LeaderKilled) && AllGruntsKilled)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    else if (QuestSucceeded) {
        if (IsActiveThreadTerminating())
            return;
        if (Game->IsRegionLoaded("GreatwoodLake")) {
            if (IsActiveThreadTerminating())
                return;
            while (!(LakeComplete && AllGruntsKilled) && !Game->MsgOnRegionLoaded(regionName)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    Game->SetCreatureGeneratorsCreatureGroupAsEnabled((ECreatureGroup)4, true);
    Game->SetCreatureGeneratorsCreatureGroupAsEnabled((ECreatureGroup)0x8000, true);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
