// NScript::CV_BanditTollScript::Main() -- original script entry logic.
// Leaf: ?Main@CV_BanditTollScript@NScript@@UAEXXZ
// Grade: RELOCATION_MATCH(1819v1819) -- exact original spawned-function header ctor needs stronger expansion.
// Annotation: Newest Owner conditional header-helper preference; original keyword unproven, ordinary history retained.
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CV_BanditTollScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBinding.h"

#include "engine/CSpawnedFunc.h"
inline const C3DVector& CScriptThing::GetPos() const
{
    if (!PImp.Data) return NullCoord;
    return PImp.Data->GetPos();
}

void NScript::CV_BanditTollScript::Main()
{
    while (!Game->IsRegionLoaded("GreatwoodBanditToll")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("M_TollBandit", this, &CM_TollBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("TollBandit", this, &CTollBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("M_TauntingBandit", this, &CM_TauntingBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("TauntingBandit", this, &CTauntingBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("BlockingBandit", this, &CBlockingBandit::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("ReluctantTrader", this, &CReluctantTrader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("SafeTrader", this, &CSafeTrader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("M_BanditLeader", this, &CM_BanditLeader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("BanditLeader", this, &CBanditLeader::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BanditTollScript>("GuildSealWarning", this, &CGuildSealWarning::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    CScriptThing ReluctantTraderSpawn = Game->GetThingWithScriptName("M_ReluctantTraderSpawnPoint");
    if (!TraderDead) {
        if (IsActiveThreadTerminating()) return;
        Game->CreateCreature("CREATURE_TRADER_01", ReluctantTraderSpawn.GetPos(), "ReluctantTrader", false);
    }
    CScriptThing SafeTraderSpawn = Game->GetThingWithScriptName("M_SafeTraderSpawnPoint");
    if (!SafeTraderAppeared) {
        if (IsActiveThreadTerminating()) return;
        Game->CreateCreature("CREATURE_TRADER_01", SafeTraderSpawn.GetPos(), "SafeTrader", false);
    }
    AddSpawnedFunction(new CSpawnedFunc<CV_BanditTollScript>("CheckForLeaderDead", 0, this, &CV_BanditTollScript::CheckForLeaderDead), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BanditTollScript>("ResetToll", 0, this, &CV_BanditTollScript::ResetToll), CCharString(""));
}