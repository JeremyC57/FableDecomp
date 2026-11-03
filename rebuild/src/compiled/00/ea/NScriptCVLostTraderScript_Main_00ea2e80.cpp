// NScript::CV_LostTraderScript::Main() -- original script entry logic.
// Leaf: ?Main@CV_LostTraderScript@NScript@@UAEXXZ
// Grade: RELOCATION_MATCH(1542v1542) -- original binding and thread header ctors need stronger expansion.
// Annotation: Newest Owner conditional header-helper preference; ordinary1-15 retained, original keyword unproven.
#define FABLE_RETAIL_CENTITYSCRIPTBINDING_FORCEINLINE_CTOR
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CV_LostTraderScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFunc.h"

inline CScriptThing::~CScriptThing() {}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float);

void NScript::CV_LostTraderScript::Main()
{
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_LOST_TRADER_OBJECTIVE_01", "BanditCampPath1", "");
    bool HeroEnteredTheLevelFromCorrectEntrance = false;
    while (!HeroEnteredTheLevelFromCorrectEntrance) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        while (!Game->IsLevelLoaded("BanditCampPath_1")) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        HeroEnteredTheLevelFromCorrectEntrance = IsDistanceBetweenThingsUnder(Game->GetHero(), Game->GetThingWithScriptName("LostTraderBrother"), 20.0f);
        if (!HeroEnteredTheLevelFromCorrectEntrance) {
            if (IsActiveThreadTerminating()) return;
            Game->RemoveThing(Game->GetThingWithScriptName("LostTraderTrader"), false, false);
            Game->MiniMapAddMarker(Game->GetThingWithScriptName("LostTraderBrother"), "HUD_ORB_QUEST_OPTIONAL");
            while (Game->IsLevelLoaded("BanditCampPath_1") != false) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
        } else {
            if (IsActiveThreadTerminating()) return;
        }
    }
    if (IsActiveThreadTerminating()) return;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_LostTraderScript>("LostTraderTrader", this, &CLostTraderTrader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_LostTraderScript>("LostTraderBrother", this, &CLostTraderBrother::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_LostTraderScript>("LostTraderBanditSpawn", this, &CLostTraderBanditSpawn::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_LostTraderScript>("WatchForQuestFailure", 0, this, &CV_LostTraderScript::WatchForQuestFailure), CCharString(""));
    while (!ReachedTrader) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_LOST_TRADER_OBJECTIVE_02", "BanditCampPath1", "");
}