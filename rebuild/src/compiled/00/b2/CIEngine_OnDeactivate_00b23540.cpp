// Leaf: ?OnDeactivate@CIEngine_Methods@@QAEXXZ
#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    void OnDeactivate();
};
void CIEngine_Methods::OnDeactivate(){ this->Active = 0u; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnDeactivate@CIEngine@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
