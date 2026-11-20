// CTCMapwho::PlaceInMapwho(const C3DVector& pos) -- when the component is not in the map yet, places it at pos through the world map's
//   thing-map manager
// Leaf: ?PlaceInMapwho@CTCMapwho@@QAEXABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071A710, 26 bytes: test [esi+50h],1 ; CTCBase::GetWorldMap 0x00686D30 ;
//   NThingMap::CThingMapManager::PlaceThing 0x00637FC0 on [eax+10h] with this and pos (`ret 4`).
// Name: Ego_r.pdb 0x0072D970 + FableWin.pdb 0x0259A060 ?PlaceInMapwho@CTCMapwho@@QAEXABVC3DVector@@@Z (engine/CTCMapwho.h); the four landed
//   referencing rows 0x004C8C00, 0x006B0B30, 0x00724630, 0x0075A530 already bind it.
// Callee: Ego_r.pdb 0x00678BF0 + FableWin.pdb 0x02220DB0 ?PlaceThing@CThingMapManager@NThingMap@@QAEXAAVCTCMapwho@@ABVC3DVector@@@Z
//   (engine/CThingMapManager.h).
// NF171 (2026-11-20, q5023): re-authored on the shared headers in the form of the landed siblings 0x0071A730 / 0x0071A790; was the free
//   __fastcall stand-in ?CTCMapwho_PlaceInMapwho@@YIXPAVCTCMapwho@@HPAVC3DVector@@@Z over local CTCMapwho / Helper / Slot models, binding
//   0x00637FC0 as ?Place@Slot@@QAEXPAVCTCMapwho@@PAVC3DVector@@@Z and 0x00686D30 as the free _GetMapwhoHelper.
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

void CTCMapwho::PlaceInMapwho(const C3DVector& pos)
{
    if (!IsInMap())
        GetWorldMap().GetThingMapManager().PlaceThing(*this, pos);
}
