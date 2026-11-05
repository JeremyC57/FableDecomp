// Leaf: ?GetThingManager@CWorld_Methods@@QAEAAVCThingManager@@XZ
#include "engine/CWorld.h"  // retyped onto the PDB layout; byte parity re-verified
class CThingManager;
struct CWorld_Methods : CWorld {
    CThingManager& GetThingManager();
};
CThingManager& CWorld_Methods::GetThingManager(){ return *this->PThingManager_ptr; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetThingManager@CWorld@@UAEAAVCThingManager@@XZ); evidence: RTTI vtable slot + PDB name set.
