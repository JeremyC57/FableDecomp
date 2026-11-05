// Leaf: ?OnActivate@CEngine_Methods@@QAEXXZ
#include "engine/CEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngine_Methods : CEngine {
    void OnActivate();
};
void CEngine_Methods::OnActivate(){ this->f = 1u; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnActivate@CEngine@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
