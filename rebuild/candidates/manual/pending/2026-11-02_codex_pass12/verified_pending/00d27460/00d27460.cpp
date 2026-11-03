// NScript::CQ_EndGameScript::Main() -- associate the quest regions, bind its entities and spawn its three original threads.
// Leaf: ?Main@CQ_EndGameScript@NScript@@UAEXXZ
#define FABLE_RETAIL_CSCRIPTBASE_PRIVATE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PERSONAL_SCRIPT
#define FABLE_RETAIL_BANDIT_CUTSCENE
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_END_GAME
#include "engine/CCharString.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_EndGameScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFunc.h"
#include "engine/TRetVoid.h"
void NScript::CQ_EndGameScript::Main()
{
    TRetVoid _Fail_Val;
    Game.AddQuestRegion(Game.GetActiveQuestName(),"Darkwood5");
    Game.AddQuestRegion(Game.GetActiveQuestName(),"HeroGuildComplexInside");
    Game.AddQuestRegion(Game.GetActiveQuestName(),"HookCoast");
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameScript>("M_UndeadSpawn",this,&CM_UndeadSpawn::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameScript>("GatewayUndead",this,&CGatewayUndead::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameScript>("HookCoastTeleporter",this,&CHookCoastTeleporter::Alloc,1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameScript>("MK_GHC_HERO3",this,&CMK_GHC_HERO3::Alloc,0));
    AddEntityScriptBinding(new CEntityScriptBinding<CQ_EndGameScript>("MazeJournal",this,&CMazeJournal::Alloc,0));
    PostAddScriptedEntities();Game.PostAddScriptedEntities();
    Game.SetQuestCardObjective(Game.GetActiveQuestName(),"TEXT_QUEST_GATEWAY_TO_HOOK_COAST_OBJECTIVE_01","Darkwood5","BarrowFields");
    AddSpawnedFunction(new CSpawnedFunc<CQ_EndGameScript>("CheckRegionLeaving",0,this,&CQ_EndGameScript::CheckRegionLeaving),CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_EndGameScript>("CheckGuildEntering",0,this,&CQ_EndGameScript::CheckGuildEntering),CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CQ_EndGameScript>("DoTheCoolEffects",0,this,&CQ_EndGameScript::DoTheCoolEffects),"Darkwood5");
}
