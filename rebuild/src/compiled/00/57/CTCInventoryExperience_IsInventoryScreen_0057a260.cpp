// CTCInventoryExperience::IsInventoryScreen() const -- this screen is not the inventory screen (returns false)
// Leaf: ?IsInventoryScreen@CTCInventoryExperience@@UBE_NXZ
// Identity: slot 3 of the NHeroInformationScreens::CBase secondary vtable 0x0124F108 of CTCInventoryExperience (+0x130; engine/NHeroInformationScreens_CBase.h declares
//   that base's four virtuals in this order: GetScreenName, GetPlayerMode, GetScreenType, IsInventoryScreen; the Xbox 360 Test PDB agrees);
//   Ego_r/FableWin name set: ?IsInventoryScreen@CTCInventoryExperience@@UBE_NXZ; the body returns the value the enum names for this screen.
// Header: prelude as in the landed row CTCInventoryExperience_IsComponentSerialisable_0057a25d.cpp.
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_HISCREEN
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_MENULIST
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_VIRTUALS
#include "engine/CTCInventoryExperience.h"
bool CTCInventoryExperience::IsInventoryScreen() const
{
    return false;
}
