
// Leaf: ?IsMapChangeable@CWorldMap@@UBE_NJ@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CWorldMap.h under FABLE_RETAIL_CWORLDMAP with the PDB parameter / return types (was ?IsMapChangeable@CWorldMap_Methods@@QAE_NH@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CWORLDMAP
#include "engine/CWorldMap.h"

bool CWorldMap::IsMapChangeable(long mapIndex) const {
    return IsMapLoaded(mapIndex);   // virtual slot 10 (`call [eax+28h]`): CWorldMap vtable slot 10 = 0x004FC050 ?IsMapLoaded@CWorldMap@@UBE_NJ@Z (labels_rtti_port.tsv; the header's Ego_r.pdb slot order)
}
