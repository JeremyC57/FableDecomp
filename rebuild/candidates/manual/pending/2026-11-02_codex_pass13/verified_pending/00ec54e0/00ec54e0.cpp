// NScript::CV_SickChildScript::Main() -- original script entry logic.
// Leaf: ?Main@CV_SickChildScript@NScript@@UAEXXZ
#define FABLE_RETAIL_CENTITYSCRIPTBINDING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CV_SickChildScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBinding.h"

void NScript::CV_SickChildScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("SickChildsMother", this, &CSickChildsMother::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("SickChild", this, &CSickChild::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("SickChildsSister", this, &CSickChildsSister::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("Witch", this, &CWitch::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("ManInLove", this, &CManInLove::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("MansLover", this, &CMansLover::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("IngredientOwner", this, &CIngredientOwner::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("TalkingTrader1", this, &CTalkingTrader1::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("TalkingTrader2", this, &CTalkingTrader2::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("WomanToAttract", this, &CWomanToAttract::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_SickChildScript>("SickChildFishingSpot", this, &CSickChildFishingSpot::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    while (!Game.IsQuestCompleted("QS_GuardianSisterInfo")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.ActivateQuest("V_SickChild_Activate");
    Game.ActivateQuest("V_SickChildBarrowFields");
    while (!FinishedQuest) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.SetQuestAsCompleted(Game.GetActiveQuestName(), false, false, false);
    while (Game.IsRegionLoaded("BowerstoneSlums") != false) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.DeactivateQuestLater("V_SickChildBarrowFields", 0);
    Game.DeactivateQuestLater("V_SickChild_Activate", 0);
    Game.DeactivateQuestLater(Game.GetActiveQuestName(), 0);
}
