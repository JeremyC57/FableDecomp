// CGameScriptInterface::IsRegionLoaded(const CCharString& regionName) const -- whether the named region is the world map's loaded region
// Leaf: ?IsRegionLoaded@CGameScriptInterface@@UBE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0088E300.
// Callees: CWorld virtual slot 13 PeekWorldMap() const (+0x34) twice, CWorldMap::GetRegionNumberFromName(const CCharString&) const
//   0x004FC210, CWorldMap::GetLoadedRegion() const 0x004FB150.
// Name fix (agentNF26, 2026-11-12): 0x004FB150 is Ego_r.pdb + FableWin.pdb ?GetLoadedRegion@CWorldMap@@QBEJXZ (`mov eax,[ecx+9Ch]`; manifest
//   "CWorld::DrawGetEnvironment" is propagated) and 0x004FC210 ?GetRegionNumberFromName@CWorldMap@@QBEJABVCCharString@@@Z (both PDBs);
//   the row now reaches both through engine/CWorld.h / engine/CWorldMap.h. Was CWorldEnvironment::DrawGetEnvironment and
//   CWorldMapLookup::GetRegionNumberFromName (stand-ins returning CIDrawEnvironment*) over a CWorldProvider vtable stand-in.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"

bool CGameScriptInterface::IsRegionLoaded(const CCharString& regionName) const
{
    long region = World->PeekWorldMap().GetRegionNumberFromName(regionName);
    long current = World->PeekWorldMap().GetLoadedRegion();
    return (bool)(region == current);
}
