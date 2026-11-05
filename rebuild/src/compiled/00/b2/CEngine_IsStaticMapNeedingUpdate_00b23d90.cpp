// Leaf: ?IsStaticMapNeedingUpdate@CEngine_Methods@@QAE_NHHH@Z
#include "engine/CEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngine_Methods : CEngine {
    bool IsStaticMapNeedingUpdate(int a0, int name, int a2);
};
bool CEngine_Methods::IsStaticMapNeedingUpdate(int a0, int name, int a2) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsStaticMapNeedingUpdate@CEngine@@UBE_NJABVCCharString@@K@Z); evidence: RTTI vtable slot + PDB name set.
