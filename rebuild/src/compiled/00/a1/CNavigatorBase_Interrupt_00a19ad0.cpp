// Leaf: ?Interrupt@CNavigatorBase_Methods@@QAEXXZ
#include "engine/CNavigatorBase.h"  // retyped onto the PDB layout; byte parity re-verified
struct CNavigatorBase_Methods : CNavigatorBase {
    void Interrupt();
};
void CNavigatorBase_Methods::Interrupt(){ this->Interrupted = 1; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Interrupt@CNavigatorBase@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
