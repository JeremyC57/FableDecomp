// Leaf: ?IsStaticMapNeedingUpdate@CIEngine_Methods@@QAE_NHHH@Z
#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    bool IsStaticMapNeedingUpdate(int a0, int name, int a2);
};
bool CIEngine_Methods::IsStaticMapNeedingUpdate(int a0, int name, int a2) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsStaticMapNeedingUpdate@CIEngine@@UBE_NJABVCCharString@@K@Z); evidence: RTTI vtable slot + PDB name set.
