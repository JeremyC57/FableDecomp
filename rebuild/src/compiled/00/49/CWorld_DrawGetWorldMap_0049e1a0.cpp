// Leaf: ?DrawGetWorldMap@CWorld@@UBEPBVCIDrawWorldMap@@XZ
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CWorld.h under FABLE_RETAIL_CWORLD, FABLE_RETAIL_CWORLDMAP with the PDB parameter / return types (was ?DrawGetWorldMap@CWorld_Methods@@QAEPAXXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
const CIDrawWorldMap* CWorld::DrawGetWorldMap() const {
  return PWorldMap_ptr;   // CIDrawWorld this (+4): mov eax,[ecx+0x10] = PWorldMap +0x14; the null-tested upcast to the CIDrawWorldMap base (+4)
}
