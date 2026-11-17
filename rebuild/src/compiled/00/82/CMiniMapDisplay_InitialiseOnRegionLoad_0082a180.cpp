// CMiniMapDisplay::InitialiseOnRegionLoad(const CCharString& name) -- outside the editor, start loading the mini-map files of the
// world map's loaded region.
// Leaf: ?InitialiseOnRegionLoad@CMiniMapDisplay@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082A180. Ego_r.pdb 0x00732840 / FableWin.pdb 0x027CF970 name it; its caller
//   NPlayerGui::CDrawMiniMap::SetRegion 0x0064DFDE already binds that symbol.
// NF47 (round 7): was a free `__fastcall InitialiseOnRegionLoad(Self*, void*, CCharString*)` over hand vtables (GObjV / GVtbl / SubObj /
//   SubVtbl / Obj / Self) on the stand-in `GObjV* g_singleton`. Now: 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A read
//   through the inline CMainGameComponent::Get(); its slot 9 (+0x24) is CMainGameComponent::IsEditorActive() const (engine/CMainGameComponent.h
//   EDITOR), its PWorld +0x24 through PeekWorld(), whose slot 13 (+0x34) is CWorld::PeekWorldMap() const (engine/CWorld.h); then
//   CWorldMap::GetLoadedRegion() const 0x004FB150 / GetRegion(long) const 0x004FC180 (engine/CWorldMap.h) and
//   CMiniMapDisplay::InitialiseMiniMapFileLoading_Region 0x00829D90 (overlay opt-in FABLE_RETAIL_CMINIMAPDISPLAY_INITIALISEONREGIONLOAD).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_CMINIMAPDISPLAY_INITIALISEONREGIONLOAD
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CMainGameComponent.h"
#include "engine/CMiniMapDisplay.h"

void CMiniMapDisplay::InitialiseOnRegionLoad(const CCharString& name)
{
    if (CMainGameComponent::Get()->IsEditorActive())
        return;
    const CWorldMap& world_map = CMainGameComponent::Get()->PeekWorld().PeekWorldMap();
    const CRegion& region = world_map.GetRegion(world_map.GetLoadedRegion());   // a named local: the region is fetched before the pushes
    InitialiseMiniMapFileLoading_Region(region, name, true);
}
