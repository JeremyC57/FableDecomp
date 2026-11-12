// CTCInventoryExperience::GetPlayerMode() const -- the screen's player mode, PLAYER_MODE_NAVIGATE_INVENTORY_EXPERIENCE_SCREEN (11)
// Leaf: ?GetPlayerMode@CTCInventoryExperience@@UBE?AW4EPlayerMode@@XZ
// Identity: slot 1 of the NHeroInformationScreens::CBase secondary vtable 0x0124F108 of CTCInventoryExperience (+0x130; engine/NHeroInformationScreens_CBase.h declares
//   that base's four virtuals in this order: GetScreenName, GetPlayerMode, GetScreenType, IsInventoryScreen; the Xbox 360 Test PDB agrees);
//   Ego_r/FableWin name set: ?GetPlayerMode@CTCInventoryExperience@@UBE?AW4EPlayerMode@@XZ; the body returns the value the enum names for this screen.
// Header: prelude as in the landed row CTCInventoryExperience_IsComponentSerialisable_0057a25d.cpp.
#pragma optimize("s",on)
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_HISCREEN
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_MENULIST
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VIRTUALS
#include "engine/CTCInventoryExperience.h"
EPlayerMode CTCInventoryExperience::GetPlayerMode() const
{
    return PLAYER_MODE_NAVIGATE_INVENTORY_EXPERIENCE_SCREEN;
}
