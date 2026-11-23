// Leaf: ?PeekWorldMap@CWorld@@UBEABVCWorldMap@@XZ
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CWorld.h under FABLE_RETAIL_CWORLD with the PDB parameter / return types (was ?PeekWorldMap@CWorld_Methods@@QAEAAVCWorldMap@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CWORLD
#include "engine/CWorld.h"
class CWorldMap;
const CWorldMap& CWorld::PeekWorldMap() const { return *this->PWorldMap_ptr; }
