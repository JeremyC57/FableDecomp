// CTCMapwho::MoveInMapwho(const C3DVector& pos) -- re-place the thing in the mapwho: take it out of the thing-map manager when it is in
//   (RemoveThing), then, if that left it out, place it at the new position (PlaceThing)
// Leaf: ?MoveInMapwho@CTCMapwho@@QAEXABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071A750, length 54 bytes.
// Name: Ego_r.pdb 0x0072D9B0 + FableWin.pdb 0x0259A310 ?MoveInMapwho@CTCMapwho@@QAEXABVC3DVector@@@Z (engine/CTCMapwho.h); the landed
//   referencing row 0x006B0C10 already binds it.
// Callees: CTCBase::GetWorldMap 0x00686D30 (`[g_pMainGameComponent]+0x28`, `this` unused), NThingMap::CThingMapManager::RemoveThing
//   0x00637A10 and PlaceThing 0x00637FC0 (Ego_r.pdb / FableWin.pdb ?RemoveThing@CThingMapManager@NThingMap@@QAEXAAVCTCMapwho@@@Z and
//   ?PlaceThing@CThingMapManager@NThingMap@@QAEXAAVCTCMapwho@@ABVC3DVector@@@Z; engine/CThingMapManager.h), the manager at world map +0x10.
// Shape: the second half is retail's inline PlaceInMapwho (landed 0x0071A710), written out here at its one expanded site (GOTCHAS landing fx)
//   so this TU defines no second strong copy of 0x0071A710; the InMapwho bit is re-tested after the removal (the call may clear it).
// NF171 (2026-11-20, q5023): re-authored on the shared headers in the form of the landed siblings 0x0071A730 / 0x0071A790; was
//   ?MoveInMapwho@CTCMapwhoMethods@@QAEXABVC3DVector@@@Z on a CTCMapwhoLayout include-rename and a local CThingMapManager stand-in, binding
//   0x00637FC0 as ?PlaceThing@CThingMapManager@@QAEXPAVCTCMapwho@@ABVC3DVector@@@Z and 0x00637A10 as ?DestroyMapwhoNode@CThingMapManager@@..
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

void CTCMapwho::MoveInMapwho(const C3DVector& pos)
{
    if (IsInMap())
        GetWorldMap().GetThingMapManager().RemoveThing(*this);
    if (!IsInMap())
        GetWorldMap().GetThingMapManager().PlaceThing(*this, pos);
}
