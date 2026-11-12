// CTCInventoryMap::GetScreenType() const -- the screen's type, NHeroInformationScreens::INVENTORY_MAP_SCREEN (8)
// Leaf: ?GetScreenType@CTCInventoryMap@@UBE?AW4EScreenType@NHeroInformationScreens@@XZ
// Identity: slot 2 of the NHeroInformationScreens::CBase secondary vtable 0x01257A84 of CTCInventoryMap (+0x130; engine/NHeroInformationScreens_CBase.h declares
//   that base's four virtuals in this order: GetScreenName, GetPlayerMode, GetScreenType, IsInventoryScreen; the Xbox 360 Test PDB agrees);
//   Ego_r/FableWin name set: ?GetScreenType@CTCInventoryMap@@UBE?AW4EScreenType@NHeroInformationScreens@@XZ; the body returns the value the enum names for this screen.
// Header: prelude as in the landed row CTCInventoryMap_HasFrameUpdate_00619f40.cpp.
#define FABLE_RETAIL_CTCINVENTORYMAP_HISCREEN
#define FABLE_RETAIL_CTCINVENTORYMAP_VSLOTS
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CTCInventoryMap.h"
NHeroInformationScreens::EScreenType CTCInventoryMap::GetScreenType() const
{
    return NHeroInformationScreens::INVENTORY_MAP_SCREEN;
}
