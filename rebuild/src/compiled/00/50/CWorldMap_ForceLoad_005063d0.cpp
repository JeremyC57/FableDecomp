// Leaf: ?ForceLoad@CWorldMap@@UAEXJ@Z
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CWorldMap.h (class-model switch FABLE_RETAIL_CWORLDMAP; was ?ForceLoad@CWorldMap_Methods@@QAEXJ@Z);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
#define FABLE_RETAIL_CWORLDMAP
#include "engine/CWorldMap.h"  // retyped onto the PDB layout; byte parity re-verified
extern void __stdcall fl_helper(long x, int z);
void CWorldMap::ForceLoad(long x){ fl_helper(x, 0); }