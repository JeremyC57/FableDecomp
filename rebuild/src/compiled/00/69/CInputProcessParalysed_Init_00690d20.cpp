// Leaf: ?Init@CInputProcessParalysed_Methods@@QAEXXZ
#include "engine/CInputProcessParalysed.h"  // retyped onto the PDB layout; byte parity re-verified
struct CInputProcessParalysed_Methods : CInputProcessParalysed {
    void Init();
};
void CInputProcessParalysed_Methods::Init(){ this->ButtonsMashed = 0u; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?Init@CInputProcessParalysed@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
