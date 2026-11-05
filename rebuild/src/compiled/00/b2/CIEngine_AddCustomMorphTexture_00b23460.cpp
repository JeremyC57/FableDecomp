// Leaf: ?AddCustomMorphTexture@CIEngine_Methods@@QAEKHH@Z
#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    unsigned long AddCustomMorphTexture(int texture, int name);
};
unsigned long CIEngine_Methods::AddCustomMorphTexture(int texture, int name) { return 0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?AddCustomMorphTexture@CIEngine@@UBEKAAVCTexture@@ABVCCharString@@@Z); evidence: RTTI vtable slot + PDB name set.
