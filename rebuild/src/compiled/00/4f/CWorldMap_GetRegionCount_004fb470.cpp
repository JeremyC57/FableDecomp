// CWorldMap::GetRegionCount() const -- the number of regions: (end - begin) / sizeof(CRegion) over the Regions vector (+0x2C)
// Leaf: ?GetRegionCount@CWorldMap@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004FB470, thiscall, `ret`.
// Identity (agentNF54 name fix, e207 target self row; was CWorldMap::DrawGetNoMaps over a local CWorldMap model): Ego_r.pdb +
//   FableWin.pdb ?GetRegionCount@CWorldMap@@QBEJXZ, declared by engine/CWorldMap.h (FABLE_RETAIL_CWORLDMAP_REGIONCOUNT, out of line at
//   0x004FB470: `(end - begin) / 0x58` over +0x2C / +0x30, the vector GetRegion 0x004FC180 indexes with stride 0x58 = sizeof(CRegion)).
//   DrawGetNoMaps is the CIDrawWorldMap virtual slot (another address); the e207 binder 0x0057CDB5 already moved onto this spelling.
// Shape: STLport vector<CRegion>::size(), the long return.
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_REGIONCOUNT
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"

long CWorldMap::GetRegionCount() const
{
    return Regions.size();
}
