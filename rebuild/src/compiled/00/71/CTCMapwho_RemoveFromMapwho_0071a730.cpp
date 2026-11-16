// CTCMapwho::RemoveFromMapwho() -- when the component is in the map, removes it through the world map's thing-map manager
// Leaf: ?RemoveFromMapwho@CTCMapwho@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071A730, 25 bytes: test [esi+50h],1 ; CTCBase::GetWorldMap 0x00686D30 ;
//   NThingMap::CThingMapManager::RemoveThing 0x00637A10 on [eax+10h] with this.
// Name: Ego_r.pdb 0x0072D990 + FableWin.pdb 0x0259A2C0 ?RemoveFromMapwho@CTCMapwho@@QAEXXZ (engine/CTCMapwho.h); OnDie is vtable slot 13
//   0x0071A7C0. Body as the landed ~CTCMapwho 0x0071A790 expands it. NF33 (2026-11-16): was the made-up free CTCMapwho_OnDie stand-in.
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
