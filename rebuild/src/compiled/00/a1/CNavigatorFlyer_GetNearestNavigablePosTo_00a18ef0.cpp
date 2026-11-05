// Leaf: ?GetNearestNavigablePosTo@CNavigatorFlyer_Methods@@QAE_NHHH@Z
#include "engine/CNavigatorFlyer.h"  // retyped onto the PDB layout; byte parity re-verified
struct CNavigatorFlyer_Methods : CNavigatorFlyer {
    bool GetNearestNavigablePosTo(int a0, int a1, int a2);
};
bool CNavigatorFlyer_Methods::GetNearestNavigablePosTo(int a0, int a1, int a2) { return false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?GetNearestNavigablePosTo@CNavigatorFlyer@@UBE_NABVC3DVector@@MAAV2@@Z); evidence: RTTI vtable slot + PDB name set.
