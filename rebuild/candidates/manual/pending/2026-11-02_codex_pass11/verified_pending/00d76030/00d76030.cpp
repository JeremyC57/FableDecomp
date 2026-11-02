// NScript::CQ_HangingTreeGoodScript::SpawnParty(long,CScriptThing &) -- spawn the guard party at the supplied script thing.
// Leaf: ?SpawnParty@CQ_HangingTreeGoodScript@NScript@@QAEXJAAVCScriptThing@@@Z
#include "engine/fable_hanging_tree.h"
#include "engine/CQ_HangingTreeGoodScript.h"
void NScript::CQ_HangingTreeGoodScript::SpawnParty(long RemainingGuards,CScriptThing& SpawnPoint)
{
    UpdateWayPoint();
    CScriptThing Executioner=Game.GetThingWithScriptName("HTG_Executioner");
    for(long i=0;i<RemainingGuards;++i) {
        if(IsActiveThreadTerminating()) return;
        CScriptThing NewGuard=Game.CreateCreatureNearby("CREATURE_BS_GUARD",SpawnPoint.GetPos(),7.0f,"HTG_Guard",false);
        Game.AddQuestInfoBarHealth(NewGuard,CRGBColour(255,255,255,255),"HUD_QUEST_ICON_GUARD",1.0f);
        Game.EntityAttachToScript(NewGuard,Game.GetActiveQuestName());
        if(Executioner.IsAlive()) Game.EntitySetThingAsAllyOfThing(NewGuard,Executioner);
        SpawnPoint=NewGuard;
    }
    if(IsActiveThreadTerminating()) return;
    Game.DisplayQuestInfo(true);
    ReassignLeader();
    Prisoner=Game.CreateCreatureNearby("CREATURE_PRISONER",SpawnPoint.GetPos(),1.0f,"HTG_Prisoner",false);
    Game.EntityAttachToScript(Prisoner,Game.GetActiveQuestName());
    PartySuccessfullySetUp=true;
    AddSpawnedFunction(new CSpawnedFunc<CQ_HangingTreeGoodScript>("WatchForHeroNearExit",0,this,&CQ_HangingTreeGoodScript::WatchForHeroNearExit),CCharString(""));
}
