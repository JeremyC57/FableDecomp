// Leaf: ?GetThingManager@CWorld@@UAEAAVCThingManager@@XZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CWorld.h (class-model switch FABLE_RETAIL_CWORLD; was ?GetThingManager@CWorld_Methods@@QAEAAVCThingManager@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
#define FABLE_RETAIL_CWORLD
#include "engine/CWorld.h"  // retyped onto the PDB layout; byte parity re-verified
class CThingManager;
CThingManager& CWorld::GetThingManager(){ return *this->PThingManager_ptr; }
