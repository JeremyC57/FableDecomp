// CTCInventoryClothing::GetScreenType() const -- the screen's type, NHeroInformationScreens::INVENTORY_CLOTHING_SCREEN (5)
// Leaf: ?GetScreenType@CTCInventoryClothing@@UBE?AW4EScreenType@NHeroInformationScreens@@XZ
// Identity: slot 2 of the NHeroInformationScreens::CBase secondary vtable 0x01243558 of CTCInventoryClothing (+0x130; engine/NHeroInformationScreens_CBase.h declares
//   that base's four virtuals in this order: GetScreenName, GetPlayerMode, GetScreenType, IsInventoryScreen; the Xbox 360 Test PDB agrees);
//   Ego_r/FableWin name set: ?GetScreenType@CTCInventoryClothing@@UBE?AW4EScreenType@NHeroInformationScreens@@XZ; the body returns the value the enum names for this screen.
// Header: prelude as in the landed row CTCInventoryClothing_RefreshDescriptions_005b690b.cpp.
#pragma optimize("s",on)
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_HISCREEN
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_SCREEN
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_REFRESH
#include "engine/CTCInventoryClothing.h"
#include "engine/NUISystem_CText.h"
NHeroInformationScreens::EScreenType CTCInventoryClothing::GetScreenType() const
{
    return NHeroInformationScreens::INVENTORY_CLOTHING_SCREEN;
}
