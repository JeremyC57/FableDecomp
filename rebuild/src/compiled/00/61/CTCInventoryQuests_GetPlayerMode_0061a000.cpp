// CTCInventoryQuests::GetPlayerMode() const -- the screen's player mode, PLAYER_MODE_NAVIGATE_INVENTORY_QUESTS_SCREEN (13)
// Leaf: ?GetPlayerMode@CTCInventoryQuests@@UBE?AW4EPlayerMode@@XZ
// Identity: slot 1 of the NHeroInformationScreens::CBase secondary vtable 0x012585A0 of CTCInventoryQuests (+0x130; engine/NHeroInformationScreens_CBase.h declares
//   that base's four virtuals in this order: GetScreenName, GetPlayerMode, GetScreenType, IsInventoryScreen; the Xbox 360 Test PDB agrees);
//   Ego_r/FableWin name set: ?GetPlayerMode@CTCInventoryQuests@@UBE?AW4EPlayerMode@@XZ; the body returns the value the enum names for this screen.
// Header: prelude as in the landed row CTCInventoryQuests_OnChangeCategory_00624da0.cpp.
#define FABLE_RETAIL_CTCINVENTORYQUESTS_HISCREEN
#define FABLE_RETAIL_CTCINVENTORYQUESTS_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ONCREATE
#define FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS
#define FABLE_RETAIL_CCREATUREDEF_SCREENINVENTORIES
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGISTER
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_CPLAYERGUI
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CTCInventoryQuests.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
#include "engine/CPlayerGui.h"
EPlayerMode CTCInventoryQuests::GetPlayerMode() const
{
    return PLAYER_MODE_NAVIGATE_INVENTORY_QUESTS_SCREEN;
}
