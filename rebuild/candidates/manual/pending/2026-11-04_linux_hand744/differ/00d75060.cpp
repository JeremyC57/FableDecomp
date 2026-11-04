// NScript::CQ_HangingTreeGoodScript::FinalRound() -- the execution-tree showdown: set objective 2, make sure the executioner exists, ally him with the hero and the guards, walk him and the prisoner to the chopping block, spawn four freedom fighters at the final ambush point, then hold until the fighters and the boss are dead (failing if the executioner dies, fighting back when he is attacked)
// Grade: DIFFER(5237v4044) -- first draft: statement order and calls follow retail, but the four exit flavours differ (retail expands ~CScriptThing / ~CScriptGameResourceObjectScriptedThingBase / ~vector inline at the early guard-loop and approach-loop exits, ~vector out of line after the spawn loop, everything out of line at the final loop's exits) and the frame slots differ; next idea: per-exit do{}while(0) / loop-guard shapes (INLINE_DEPTH_REVISIT 2026-11-03) and declaration-order changes for the slots
// Leaf: ?FinalRound@CQ_HangingTreeGoodScript@NScript@@QAEXXZ
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CQ_HANGINGTREEGOODSCRIPT
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST, SCRIPT_AI_PRIORITY_LOW, SCRIPT_AI_PRIORITY_MEDIUM, SCRIPT_AI_PRIORITY_HIGH, SCRIPT_AI_PRIORITY_HIGHEST };
enum EScriptEntityMoveType { MOVE_TYPE_WALK };
#include "engine/C3DVector.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CQ_HangingTreeGoodScript.h"
#include "engine/fable_script_distance.h"
extern "C" int __cdecl rand();
inline CScriptThing::~CScriptThing() { }
__forceinline CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline CCharString CScriptThing::GetDataString() const { if (PImp.IsNull()) return NullString; return PImp->GetDataString(); }
bool FABLE_FASTCALL AreAllThingsInVectorDead(const std::vector<CScriptThing,std::allocator<CScriptThing> >&);  // 0x00CBED00
void NScript::CQ_HangingTreeGoodScript::FinalRound()
{
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HANGING_TREE_GOOD_OBJECTIVE_02", "", "BowerstoneSlums");
    CScriptThing FinalAmbushPoint = Game->GetThingWithScriptName("HTG_FinalAmbushPoint");
    CScriptThing Boss = Game->GetThingWithScriptName("HTG_Boss");
    CScriptThing ExecutionerStandPoint = Game->GetThingWithScriptName("HTG_ExecutionerStandPoint");
    CScriptThing ChoppingBlock = Game->GetThingWithScriptName("HTG_ChoppingBlock");
    std::vector<CScriptThing,std::allocator<CScriptThing> > FreedomFighters;
    CScriptThing Executioner = Game->GetThingWithScriptName("HTG_Executioner");
    if (!Executioner.IsAlive()) {
        if (IsActiveThreadTerminating()) return;
        Executioner = Game->CreateCreature("CREATURE_EXECUTIONER", ExecutionerStandPoint.GetPos(), "HTG_Executioner", false);
    }
    Game->MiniMapAddMarker(Executioner, "HUD_ORB_QUEST_CORE");
    Game->EntitySetInFaction(Executioner, "FACTION_VILLAGER");
    Game->EntitySetThingAsAllyOfThing(Executioner, Game->GetHero());
    Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Executioner);
    for (unsigned long i = 0; i < CurrentGuardVector.size(); ++i) {
        if (IsActiveThreadTerminating()) return;
        Game->EntitySetThingAsAllyOfThing(Executioner, CurrentGuardVector[i]);
        Game->EntitySetThingAsAllyOfThing(CurrentGuardVector[i], Executioner);
    }
    if (IsActiveThreadTerminating()) return;
    Game->EntitySetAsOpinionSource(Executioner, "OPINION_SOURCE_INANIMATE_EVIL_HIGH");
    Game->EntitySetAsOpinionSource(Boss, "OPINION_SOURCE_INANIMATE_EVIL_HIGH");
    Game->EntitySetAsOpinionSource(Game->GetHero(), "OPINION_SOURCE_INANIMATE_GOOD_HIGH");
    CScriptGameResourceObjectScriptedThingBase ExecutionerResource;
    CScriptGameResourceObjectScriptedThingBase PrisonerResource;
    CScriptGameResourceObjectScriptedThingBase BossResource;
    Game->StartScriptingEntity(Executioner, ExecutionerResource, SCRIPT_AI_PRIORITY_HIGHEST);
    Game->StartScriptingEntity(Prisoner, PrisonerResource, SCRIPT_AI_PRIORITY_HIGHEST);
    Game->StartScriptingEntity(Boss, BossResource, SCRIPT_AI_PRIORITY_HIGH);
    ExecutionerResource.FollowThing(Prisoner, 1.0f, true);
    while (!IsDistanceBetweenThingsUnder(Executioner, Prisoner, 5.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    ExecutionerResource.ClearCommands();
    PrisonerResource.FollowThing(Executioner, 1.0f, true);
    ExecutionerResource.MoveToPosition(ExecutionerStandPoint.GetPos(), 0.0f, (EScriptEntityMoveType)0, false, true);
    for (long n = 0; n < 4; ++n) {
        if (IsActiveThreadTerminating()) return;
        CScriptThing FreedomFighter = Game->CreateCreatureNearby(FinalAmbushPoint.GetDataString(), FinalAmbushPoint.GetPos(), 1.0f, "FinalHTG_FreedomFighter", false);
        switch (rand() % 2) {
        case 0:
            Game->GiveThingBestEnemyTarget(FreedomFighter, Executioner);
            break;
        case 1:
            Game->GiveThingBestEnemyTarget(FreedomFighter, Game->GetHero());
            break;
        }
        Game->EntitySetAsOpinionSource(FreedomFighter, "OPINION_SOURCE_INANIMATE_EVIL_HIGH");
    }
    if (IsActiveThreadTerminating()) return;
    Game->GetAllThingsWithScriptName("FinalHTG_FreedomFighter", FreedomFighters);
    Game->GiveThingBestEnemyTarget(Boss, Executioner);
    BossResource.FollowThing(Executioner, 1.0f, true);
    while (!AreAllThingsInVectorDead(FreedomFighters) || Boss.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!Executioner.IsAlive()) {
            if (IsActiveThreadTerminating()) return;
            ReasonForFailing = "TEXT_QST_030_FAILURE_EXECUTIONER_KILLED";
            MissionFailed = true;
            return;
        }
        if (ExecutionerBeenAttackedByHero) {
            if (IsActiveThreadTerminating()) return;
        } else if (ExecutionerBeenAttacked) {
            if (IsActiveThreadTerminating()) return;
            ExecutionerResource.ClearCommands();
            Game->EntitySetCombatAbility(Executioner, 20.0f);
            if (ExecutionerResource.IsValid())
                ExecutionerResource.Release();
            CScriptThing Nearest = Game->GetNearestWithScriptName(Executioner, "FinalHTG_FreedomFighter");
            Game->GiveThingBestEnemyTarget(Executioner, Nearest);
            while (Executioner.IsAlive() && Nearest.IsAlive()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            Game->EntitySetCombatAbility(Executioner, 100.0f);
            Game->StartScriptingEntity(Executioner, ExecutionerResource, SCRIPT_AI_PRIORITY_HIGHEST);
            ExecutionerBeenAttacked = false;
        }
        if (IsDistanceBetweenThingsUnder(Executioner, ExecutionerStandPoint, 1.0f)) {
            if (IsActiveThreadTerminating()) return;
            PrisonerResource.ClearCommands();
            PrisonerResource.MoveToPosition(ChoppingBlock.GetPos(), 0.0f, (EScriptEntityMoveType)0, false, true);
        }
        if (!ExecutionerResource.IsPerformingScriptTask()) {
            if (IsActiveThreadTerminating()) return;
            ExecutionerResource.MoveToPosition(ExecutionerStandPoint.GetPos(), 1.0f, (EScriptEntityMoveType)0, false, true);
        }
    }
    if (IsActiveThreadTerminating()) return;
}
