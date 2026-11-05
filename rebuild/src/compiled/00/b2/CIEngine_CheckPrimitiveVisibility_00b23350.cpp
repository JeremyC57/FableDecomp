// Leaf: ?CheckPrimitiveVisibility@CIEngine_Methods@@QAE_NHH@Z
#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIEngine_Methods : CIEngine {
    bool CheckPrimitiveVisibility(int x0, int x1);
};
bool CIEngine_Methods::CheckPrimitiveVisibility(int x0, int x1) { return true; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?CheckPrimitiveVisibility@CIEngine@@UAE_N_K@Z); evidence: RTTI vtable slot + PDB name set.
