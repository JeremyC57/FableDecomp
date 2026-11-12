// CTCInventoryTrade::IsInventoryScreen() const -- this screen is not the inventory screen (returns false)
// Leaf: ?IsInventoryScreen@CTCInventoryTrade@@UBE_NXZ
// Identity: slot 3 of the NHeroInformationScreens::CBase secondary vtable 0x0125301C of CTCInventoryTrade (+0x130; engine/NHeroInformationScreens_CBase.h declares
//   that base's four virtuals in this order: GetScreenName, GetPlayerMode, GetScreenType, IsInventoryScreen; the Xbox 360 Test PDB agrees);
//   Ego_r/FableWin name set: ?IsInventoryScreen@CTCInventoryTrade@@UBE_NXZ; the body returns the value the enum names for this screen.
// Header: prelude as in the landed row CTCInventoryTrade_OnChangeCategory_005b306f.cpp.
#define FABLE_RETAIL_CTCINVENTORYTRADE_HISCREEN
#define FABLE_RETAIL_CTCINVENTORYTRADE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_PCINVENTORY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_enums.h"
bool CTCInventoryTrade::IsInventoryScreen() const
{
    return false;
}
