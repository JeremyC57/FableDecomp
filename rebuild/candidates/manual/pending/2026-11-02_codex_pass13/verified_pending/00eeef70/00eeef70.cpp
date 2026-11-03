// NScript::CTestQuestCard1Script::Main() -- exercise quest boasts and objectives across region messages.
// Leaf: ?Main@CTestQuestCard1Script@NScript@@UAEXXZ
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CSPAWNEDFUNC
#include "engine/CTestQuestCard1Script.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CSpawnedFunc.h"

inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const
{
    if (!PImp.Data) return false;
    return PImp.Data->IsAlive();
}

void NScript::CTestQuestCard1Script::Main()
{
    Accepted4SecondBoast = false;
    CScriptThing chest = Game.GetThingWithScriptName("MagicChest");
    if (chest.IsAlive())
        Game.EntityPlayObjectAnimation(chest, "CHEST_OPEN", false);
    Game.AddBoast("I'll do this quest without trousers!", 1, 80, 150, false, "", 0);
    Game.AddBoast("I'll do this quest without using the flame sword!", 2, 100, 180, false, "", 0);
    Game.AddBoast("I'm not going to do this quest at all! Screw you all!", 3, 1, -5, false, "", 0);
    Game.AddBoast("I'll do this quest in 4 seconds flat!", 4, 150, 500, false, "", 0);
    AddSpawnedFunction(new CSpawnedFunc<CTestQuestCard1Script>("WaitForBoasts", 0, this, &CTestQuestCard1Script::WaitForBoasts), CCharString(""));
    Game.GiveHeroNewQuestObjective("Go to the teddy bear region", 1);
    Game.GiveHeroNewQuestObjective("Then come back to the region you came from!", 2);
    while (!Game.MsgIsRegionLoaded("Region 1")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game.TellHeroQuestObjectiveCompleted(1);
    while (!Game.MsgIsRegionLoaded("Region 2")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    if (Accepted4SecondBoast) {
        if (IsActiveThreadTerminating()) return;
    }
    Game.TellHeroQuestObjectiveFailed(2);
    Game.GiveHeroNewQuestObjective("Do it again, idiot!", 3);
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game.NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    while (!Game.MsgIsRegionLoaded("Region 2")) {
        Game.NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    KillSpawnedFunction("WaitForBoasts", 0);
    Game.TellHeroQuestObjectiveCompleted(3);
    Game.SetQuestAsCompleted(Game.GetActiveQuestName(), true, true, false);
}
