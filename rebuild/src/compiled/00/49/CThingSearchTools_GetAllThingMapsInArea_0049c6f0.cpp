// CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI& area, std::vector<NThingMap::CThingMap*>& maps) const -- forwards to the thing map manager's GetAllActiveThingMapsInArea
// Leaf: ?GetAllThingMapsInArea@CThingSearchTools@@IBEXABVC2DBoxI@@AAV?$vector@PAVCThingMap@NThingMap@@V?$allocator@PAVCThingMap@NThingMap@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0049C6F0: mov ecx,[ecx+4] ; jmp 0x006384B0 (the arguments pass through).
// Callee: NThingMap::CThingMapManager::GetAllActiveThingMapsInArea 0x006384B0 (landed; PThingMapManager at +0x4).
// Name fix (agentNF94, q11280 / q11283): Ego_r.pdb 0x004A6580 + FableWin.pdb 0x01CEDC40
//   ?GetAllThingMapsInArea@CThingSearchTools@@IBEXABVC2DBoxI@@AAV?$vector@PAVCThingMap@NThingMap@@V?$allocator@PAVCThingMap@NThingMap@@@std@@@std@@@Z
//   (protected const; engine/CThingSearchTools.h declares it under ORIGINAL_ROUGHAREA / BESTROUGH_BODY; the landed 0x00436D60 binds it so). The
//   old row was the generic `T::Run` sub-object forwarder (catalog _global::Forward_CThingMapManager_GetOverlappingThings_via0x4_0049c6f0); it is
//   now on the shared headers (overlay opt-in FABLE_RETAIL_CTHINGMAPMANAGER_GETALLACTIVETHINGMAPSINAREA for the callee).
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGMAPMANAGER
#define FABLE_RETAIL_CTHINGMAPMANAGER_GETALLACTIVETHINGMAPSINAREA
#include "stlport/fable_containers.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingMapManager.h"
void CThingSearchTools::GetAllThingMapsInArea(const C2DBoxI& area, std::vector<NThingMap::CThingMap*>& maps) const
{
    PThingMapManager->GetAllActiveThingMapsInArea(area, maps);
}
