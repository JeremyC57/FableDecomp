// CTCInventory::GetScreenType() const -- the screen's type, NHeroInformationScreens::STANDARD_INVENTORY_SCREEN (0)
// Leaf: ?GetScreenType@CTCInventory@@UBE?AW4EScreenType@NHeroInformationScreens@@XZ
// Identity: slot 2 of the NHeroInformationScreens::CBase secondary vtable 0x01250DD0 of CTCInventory (+0x130; engine/NHeroInformationScreens_CBase.h declares
//   that base's four virtuals in this order: GetScreenName, GetPlayerMode, GetScreenType, IsInventoryScreen; the Xbox 360 Test PDB agrees);
//   Ego_r/FableWin name set: ?GetScreenType@CTCInventory@@UBE?AW4EScreenType@NHeroInformationScreens@@XZ; the body returns the value the enum names for this screen.
// Header: prelude as in the landed row CTCInventory_OnChangeCategory_00594be7.cpp.
#define FABLE_RETAIL_CTCINVENTORY_HISCREEN
#define FABLE_RETAIL_CTCINVENTORY_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CMenuEntry.h"
NHeroInformationScreens::EScreenType CTCInventory::GetScreenType() const
{
    return NHeroInformationScreens::STANDARD_INVENTORY_SCREEN;
}
