
// Leaf: ?PeekMapAt@CWorldMap@@UBEABVCMap@@ABVC2DCoordI@@@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CWorldMap.h under FABLE_RETAIL_CWORLDMAP with the PDB parameter / return types (was ?PeekMapAt@CWorldMap_Methods@@QAEPAVCEditMap@@PBVC2DCoordI@@@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CWORLDMAP
#include "engine/CMapInfo.h"
#include "engine/CWorldMap.h"

const CMap& CWorldMap::PeekMapAt(const C2DCoordI& coordinate) const {
    return *Maps[GetMapNumberAt(coordinate)].PMap;   // virtual slot 17 (0x004FC080 ?GetMapNumberAt@CWorldMap@@UBEJABVC2DCoordI@@@Z, labels_rtti_port.tsv); CMapInfo is 0x48 bytes, PMap.Data at +0x10
}
