// CTCInventoryMap::TeleportToActiveRegion() -- when the highlighted region is not the loaded one, find a teleporter thing in any of its maps, close the inventory and send the hero through that teleporter
// Leaf: ?TeleportToActiveRegion@CTCInventoryMap@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FBFA0, 277 bytes (+ int3 padding), plain `ret`. Ego_r.pdb 0x0064CAC0
//   ?TeleportToActiveRegion@CTCInventoryMap@@AAEXXZ (private; donor order GetRegionTeleporter 0x0064C5B0 < TeleportToActiveRegion,
//   retail 0x005FBEA0 < 0x005FBFA0).
// Callees: CTCBase::PeekWorld 0x00686A80 (twice: the world map, then the search tools), CWorld slot 13 PeekWorldMap (+0x34),
//   CWorldMap::GetLoadedRegion 0x004FB150 / GetRegion(long) const 0x004FC180,
//   CThingSearchTools::GetPFirstThing<CFilter_Bind2<CThingFilter_HasTC,CThingFilter_InMap>,CThing> 0x00615750 (same-TU body),
//   the CTCInventoryBase slot-32 AddCloseInventoryEvent (+0x80), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the
//   inline GetTC<CTCTeleporter>, TCI_TELEPORTER 0xDD), CTCTeleporter::TeleportHero 0x007DFCF0. Data: GTeleportOpenedFromMap 0x0137D76C.
// Shape: the bound filter is GFFilter_Bind2 of two argument temporaries (VC7.1 hoists the two loop-invariant address stores); the
//   teleporter component pointer is not tested (an absent component calls through the uninitialised word, as retail).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CTCINVENTORYMAP_TELEPORT
#define FABLE_RETAIL_CTCTELEPORTER_TELEPORTHERO
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#include "engine/CTCInventoryMap.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_InMap.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CTCTeleporter.h"

extern bool GTeleportOpenedFromMap;                       // 0x0137D76C

void CTCInventoryMap::TeleportToActiveRegion()
{
    const CWorldMap& world_map = PeekWorld().PeekWorldMap();
    if (world_map.GetLoadedRegion() != ActiveRegionIndex) {
        const CRegion& region = world_map.GetRegion(ActiveRegionIndex);
        for (unsigned long i = 0; i < region.GetMaps().size(); ++i) {
            CThing* teleporter;
            if (PeekWorld().PeekThingSearchTools().GetPFirstThing(
                    GFFilter_Bind2(CThingFilter_HasTC(TCI_TELEPORTER), CThingFilter_InMap(world_map, region.GetMaps()[i])), &teleporter)) {
                GTeleportOpenedFromMap = false;
                AddCloseInventoryEvent();
                CTCTeleporter* tc_teleporter;
                teleporter->GetTC(&tc_teleporter);
                tc_teleporter->TeleportHero();
                return;
            }
        }
    }
}