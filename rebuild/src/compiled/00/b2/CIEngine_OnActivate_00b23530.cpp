// Leaf: ?OnActivate@CIEngine_Methods@@QAEXXZ
#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    void OnActivate();
};
void CIEngine_Methods::OnActivate(){ this->Active = 1u; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnActivate@CIEngine@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
