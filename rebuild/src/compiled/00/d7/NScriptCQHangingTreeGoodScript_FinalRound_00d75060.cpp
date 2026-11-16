// NScript::CQ_HangingTreeGoodScript::FinalRound() -- the execution-tree showdown: set objective 2, make sure the executioner exists, ally him with the hero and the guards, walk him and the prisoner to the chopping block, spawn four freedom fighters at the final ambush point, then hold until the fighters and the boss are dead (failing if the executioner dies, fighting back when he is attacked)
// Inline-budget: scoped #pragma inline_depth(0) regions (owner decision 2026-11-08, device b) round three statements whose exits retail calls out of line
//   while c2's static frequencies would expand them: the post-approach terminate check (retail 0x00D75700 -> call pad 0x00D75FCD), the spawn-loop
//   terminate check (0x00D7596A -> 0x00D75FCD) and the spawn-loop scope end (FreedomFighter dtor called at 0x00D75A92). Traces (tools/ffq.sh, vq.sh):
//   default decisions there expand ~STB / ~CScriptThing (f 0.94 / 4.2 / 4.2, score < 74); each region is necessary (removing any: 4361 / 4361 / 4076 bytes).
// FITTED: the approach-loop and spawn-loop terminate checks are written as the statement macro `do { if (IsActiveThreadTerminating()) return; } while (0);`
//   (docs/pipeline/INLINE_DEPTH_REVISIT.md levers 2/3, landed precedent; retail's construct is unproven): a wrapped in-loop return does not drain its loop
//   in c2's frequency model, which is what gives the post-spawn check (0x00D75AAA) retail's expanded ~STB / ~CScriptThing pad with ~vector called
//   (f 0.4688: STB score 69 < 74, vector score 74). EVIDENCED: the spawn loop is `do { ... } while (++n < 4);` (retail destroys FreedomFighter at
//   0x00D75A92 before `inc ebp; cmp ebp,4; jl` at 0x00D75A97); Executioner / Boss / Nearest bound as `CScriptThing&` to the returned temporaries
//   (IsAlive called virtually through [+12Ch] at 7 sites as retail; landed precedent 0x00CE0920; an MSVC extension -- coordinator decision).
// Leaf: ?FinalRound@CQ_HangingTreeGoodScript@NScript@@QAEXXZ
// Flags: none
// Retail: Fable.exe @ 0x00D75060, 4044 bytes (two rets: 0x00D75CB2 and the last instruction).
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
FABLE_FORCEINLINE CScriptThing& CScriptThing::operator=(const CScriptThing& other) { PImp = other.PImp; return *this; }
inline const C3DVector& CScriptThing::GetPos() const { if (PImp.IsNull()) return NullCoord; return PImp->GetPos(); }
inline CCharString CScriptThing::GetDataString() const { if (PImp.IsNull()) return NullString; return PImp->GetDataString(); }
bool FABLE_FASTCALL AreAllThingsInVectorDead(const std::vector<CScriptThing,std::allocator<CScriptThing> >&);  // 0x00CBED00
void NScript::CQ_HangingTreeGoodScript::FinalRound()
{
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_HANGING_TREE_GOOD_OBJECTIVE_02", "", "BowerstoneSlums");
    CScriptThing FinalAmbushPoint = Game->GetThingWithScriptName("HTG_FinalAmbushPoint");
    CScriptThing& Boss = Game->GetThingWithScriptName("HTG_Boss");
    CScriptThing ExecutionerStandPoint = Game->GetThingWithScriptName("HTG_ExecutionerStandPoint");
    CScriptThing ChoppingBlock = Game->GetThingWithScriptName("HTG_ChoppingBlock");
    std::vector<CScriptThing,std::allocator<CScriptThing> > FreedomFighters;
    CScriptThing& Executioner = Game->GetThingWithScriptName("HTG_Executioner");
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
        do { if (IsActiveThreadTerminating()) return; } while (0);
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating()) return;
#pragma inline_depth()
    ExecutionerResource.ClearCommands();
    PrisonerResource.FollowThing(Executioner, 1.0f, true);
    ExecutionerResource.MoveToPosition(ExecutionerStandPoint.GetPos(), 0.0f, (EScriptEntityMoveType)0, false, true);
    long n = 0;
    do {
#pragma inline_depth(0)
        do { if (IsActiveThreadTerminating()) return; } while (0);
#pragma inline_depth()
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
#pragma inline_depth(0)
    } while (++n < 4);
#pragma inline_depth()
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
            CScriptThing& Nearest = Game->GetNearestWithScriptName(Executioner, "FinalHTG_FreedomFighter");
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