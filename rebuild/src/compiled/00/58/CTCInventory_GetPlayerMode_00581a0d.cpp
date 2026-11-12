// CTCInventory::GetPlayerMode() const -- the screen's player mode, PLAYER_MODE_NAVIGATE_INVENTORY (4)
// Leaf: ?GetPlayerMode@CTCInventory@@UBE?AW4EPlayerMode@@XZ
// Identity: slot 1 of the NHeroInformationScreens::CBase secondary vtable 0x01250DD0 of CTCInventory (+0x130; engine/NHeroInformationScreens_CBase.h declares
//   that base's four virtuals in this order: GetScreenName, GetPlayerMode, GetScreenType, IsInventoryScreen; the Xbox 360 Test PDB agrees);
//   Ego_r/FableWin name set: ?GetPlayerMode@CTCInventory@@UBE?AW4EPlayerMode@@XZ; the body returns the value the enum names for this screen.
// Header: prelude as in the landed row CTCInventory_OnChangeCategory_00594be7.cpp.
#pragma optimize("s",on)
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
EPlayerMode CTCInventory::GetPlayerMode() const
{
    return PLAYER_MODE_NAVIGATE_INVENTORY;
}
