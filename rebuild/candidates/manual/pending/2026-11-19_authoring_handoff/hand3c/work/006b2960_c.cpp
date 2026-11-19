// CGameNavigatorManagerExternalInterface::GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const
// Leaf: ?GetNavigationInformation@CGameNavigatorManagerExternalInterface@@UBEXABVC2DBoxF@@AAVCNavigationAreaUpdate@@@Z
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_ORIGINAL_FOUR_COORD_CTOR
#define FABLE_RETAIL_CNAVIGATORMANAGEREXTERNALINTERFACE_GETNAVIGATIONINFORMATION
#include "fable_string.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DBoxI.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CGameNavigatorManagerExternalInterface.h"

void CGameNavigatorManagerExternalInterface::GetNavigationInformation(const C2DBoxF& box, CNavigationAreaUpdate& update) const
{
    C2DBoxI box_in_cells(GFFloatToLongFloor(box.TLX), GFFloatToLongFloor(box.TLY), GFFloatToLongNear(box.BRX), GFFloatToLongNear(box.BRY));
    long map_number = World->PeekWorldMap().GetMapNumberAt(box_in_cells.GetTLPos());
    {
        CCharString name("NULL");
        World->PeekWorldMap().GetMapNavigationAreaUpdateInfo(map_number, box_in_cells, name, update);
    }
}
