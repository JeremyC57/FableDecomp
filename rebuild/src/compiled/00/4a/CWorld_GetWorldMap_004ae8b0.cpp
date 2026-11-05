// Leaf: ?GetWorldMap@CWorld_Methods@@QAEAAVCWorldMap@@XZ
#include "engine/CWorld.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorldMap;
struct CWorld_Methods : CWorld {
    CWorldMap& GetWorldMap();
};
CWorldMap& CWorld_Methods::GetWorldMap(){ return *this->PWorldMap_ptr; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetWorldMap@CWorld@@UAEAAVCWorldMap@@XZ); evidence: RTTI vtable slot + PDB name set.
