// CTCInventoryStats::GetScreenType() const -- the stats screen's type, INVENTORY_STATS_SCREEN (9)
// Leaf: ?GetScreenType@CTCInventoryStats@@UBE?AW4EScreenType@NHeroInformationScreens@@XZ
// Header: prelude as in the landed row CTCInventoryStats_OnChangeCategory_005d87a1.cpp.
#pragma optimize("s",on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CTCTargetingBase.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCQuickAccessMenu.h"
NHeroInformationScreens::EScreenType CTCInventoryStats::GetScreenType() const
{
    return NHeroInformationScreens::INVENTORY_STATS_SCREEN;
}
