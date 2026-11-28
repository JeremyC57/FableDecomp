// Retail 0x006A4DD0, 20 bytes.
// Leaf: ?GetMapSizeZBelowAt@CThingPhysical@@QBEMABVC3DVector@@@Z
// Flags: /O2 /Oy /W3
#define FABLE_RETAIL_CREATURE_NAV_FLEE_COVER_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CWORLDMAP
#include "fable_string.h"
#include "engine/CThingPhysical.h"
#include "engine/CWorldMap.h"

float CThingPhysical::GetMapSizeZBelowAt(const C3DVector& pos) const
{
    return PeekWorldMap().GetGroundSizeZAt(pos);
}
