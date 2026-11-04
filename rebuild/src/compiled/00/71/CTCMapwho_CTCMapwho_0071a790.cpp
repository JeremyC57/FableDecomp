// CTCMapwho::~CTCMapwho() -- mapwho component dtor: take the thing out of the thing-map manager while it is in the mapwho, then ~CTCBase
// Leaf: ??1CTCMapwho@@UAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071A790.
// Identity: Ego_r.pdb ??1CTCMapwho@@UAE@XZ; RemoveFromMapwho 0x0071A730 (Ego_r ?RemoveFromMapwho@CTCMapwho@@QAEXXZ) inlined.
// Callees: NThingMap::CThingMapManager::RemoveThing(CTCMapwho&) 0x00637A10 through CWorldMap::GetThingMapManager().
// Layout: CTCMapwho retail branch (hdr overlay FABLE_RETAIL_CTCMAPWHO): InMapwho bit at +0x50.
// Shape: `RemoveFromMapwho();` defined in the TU (inlined, /Ob2).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_THINGMAPMANAGER
#include "engine/CWorldMap.h"
#define FABLE_RETAIL_CTHINGMAPMANAGER
#include "engine/CThingMapManager.h"
#define FABLE_RETAIL_CTCMAPWHO
#include "engine/CTCMapwho.h"

void CTCMapwho::RemoveFromMapwho()
{
    if (IsInMap())
        GetWorldMap().GetThingMapManager().RemoveThing(*this);
}

CTCMapwho::~CTCMapwho()
{
    RemoveFromMapwho();
}