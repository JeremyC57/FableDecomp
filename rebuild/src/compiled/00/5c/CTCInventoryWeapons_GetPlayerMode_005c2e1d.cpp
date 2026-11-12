// CTCInventoryWeapons::GetPlayerMode() const -- the screen's player mode, PLAYER_MODE_NAVIGATE_INVENTORY_WEAPONS (6)
// Leaf: ?GetPlayerMode@CTCInventoryWeapons@@UBE?AW4EPlayerMode@@XZ
// Identity: slot 1 of the NHeroInformationScreens::CBase secondary vtable 0x01253FE0 of CTCInventoryWeapons (+0x130; engine/NHeroInformationScreens_CBase.h declares
//   that base's four virtuals in this order: GetScreenName, GetPlayerMode, GetScreenType, IsInventoryScreen; the Xbox 360 Test PDB agrees);
//   Ego_r/FableWin name set: ?GetPlayerMode@CTCInventoryWeapons@@UBE?AW4EPlayerMode@@XZ; the body returns the value the enum names for this screen.
// Header: prelude as in the landed row CTCInventoryWeapons_OnChangeCategory_005cc4f8.cpp.
#pragma optimize("s",on)
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_HISCREEN
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCCarrying.h"
EPlayerMode CTCInventoryWeapons::GetPlayerMode() const
{
    return PLAYER_MODE_NAVIGATE_INVENTORY_WEAPONS;
}
