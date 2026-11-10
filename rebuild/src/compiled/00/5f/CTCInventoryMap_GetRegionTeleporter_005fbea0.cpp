// CTCInventoryMap::GetRegionTeleporter(const CRegion& region) const -- the first teleporter thing found in any of the region's maps whose teleporter component is active, or 0
// Leaf: ?GetRegionTeleporter@CTCInventoryMap@@ABEPAVCThing@@ABVCRegion@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FBEA0, 246 bytes (+ int3 padding), thiscall `ret 4`. Ego_r.pdb 0x0064C5B0
//   ?GetRegionTeleporter@CTCInventoryMap@@ABEPAVCThing@@ABVCRegion@@@Z (private const).
// Callees: CTCBase::PeekWorld 0x00686A80 (the world map once, the search tools per map), CWorld slot 13 PeekWorldMap (+0x34),
//   CThingSearchTools::GetPFirstThing<CFilter_Bind2<CThingFilter_HasTC,CThingFilter_InMap>,CThing> 0x00615750 (same-TU body),
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the inline GetTC<CTCTeleporter>, TCI_TELEPORTER 0xDD).
// Shape: the found thing starts at 0 per map; the bound filter is GFFilter_Bind2 of two argument temporaries (as the landed
//   sibling TeleportToActiveRegion 0x005FBFA0); the component pointer is an uninitialised local that GetTC fills only when the
//   interface bit is set, then tested with the teleporter's Active flag (+0x10, IsActive).
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

CThing* CTCInventoryMap::GetRegionTeleporter(const CRegion& region) const
{
    const CWorldMap& world_map = PeekWorld().PeekWorldMap();
    for (unsigned long i = 0; i < region.GetMaps().size(); ++i) {
        CThing* teleporter = 0;
        if (PeekWorld().PeekThingSearchTools().GetPFirstThing(
                GFFilter_Bind2(CThingFilter_HasTC(TCI_TELEPORTER), CThingFilter_InMap(world_map, region.GetMaps()[i])), &teleporter)) {
            CTCTeleporter* tc_teleporter;
            teleporter->GetTC(&tc_teleporter);
            if (tc_teleporter && tc_teleporter->IsActive())
                return teleporter;
        }
    }
    return 0;
}