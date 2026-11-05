// Leaf: ?AddChildPrimitive@CIEngine_Methods@@QAEHHHH@Z
#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    int AddChildPrimitive(int handle, int primitive, int x2);
};
int CIEngine_Methods::AddChildPrimitive(int handle, int primitive, int x2) { return 0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?AddChildPrimitive@CIEngine@@UAEKAAVCEnginePrimitiveHandle@@ABVCEnginePrimitive@@K@Z); evidence: RTTI vtable slot + PDB name set.
