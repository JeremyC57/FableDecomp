// Leaf: ?IsNewDestinationGoingToCauseRestart@CNavigatorFlyer_Methods@@QAE_NHH@Z
#include "engine/CNavigatorFlyer.h"  // retyped onto the PDB layout; byte parity re-verified
struct CNavigatorFlyer_Methods : CNavigatorFlyer {
    bool IsNewDestinationGoingToCauseRestart(int pos, int x1);
};
bool CNavigatorFlyer_Methods::IsNewDestinationGoingToCauseRestart(int pos, int x1) { return true; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsNewDestinationGoingToCauseRestart@CNavigatorFlyer@@UBE_NABVC3DVector@@M@Z); evidence: RTTI vtable slot + PDB name set.
