// NScript::CV_TravellingHeroesScript::Main() -- original script entry logic.
// Leaf: ?Main@CV_TravellingHeroesScript@NScript@@UAEXXZ
#define FABLE_RETAIL_CENTITYSCRIPTBINDING
#define FABLE_RETAIL_CSCRIPTTHING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CV_TravellingHeroesScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CEntityScriptBinding.h"

void NScript::CV_TravellingHeroesScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_ThunderBoy", this, &CTH_ThunderBoy::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_ThunderGirl", this, &CTH_ThunderGirl::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Maze", this, &CTH_Maze::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Thunder", this, &CTH_Thunder::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Whisper", this, &CTH_Whisper::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_SnowTroll", this, &CTH_SnowTroll::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Trigger_Bowerstone", this, &CTH_Trigger_Bowerstone::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Trigger_Guild_Maze", this, &CTH_Trigger_Guild_Maze::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Trigger_Guild_Rose", this, &CTH_Trigger_Guild_Rose::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Trigger_Oakvale", this, &CTH_Trigger_Oakvale::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Trigger_Oakvale_Gossip", this, &CTH_Trigger_Oakvale_Gossip::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_MagicBarrier", this, &CTH_MagicBarrier::Alloc, 0));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_TravellingHeroesScript>("TH_Trigger_NorthernWastes", this, &CTH_Trigger_NorthernWastes::Alloc, 0));
    PostAddScriptedEntities();
    Game.PostAddScriptedEntities();
    Game.SetCreatureGeneratorsEnabled("LostBay", false);
}
