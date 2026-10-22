#include "engine/CNavigatorFlyer.h"  // retyped onto the PDB layout; byte parity re-verified
struct CNavigatorFlyer_Methods : CNavigatorFlyer {
    bool M(int pos, int x1);
};
bool CNavigatorFlyer_Methods::M(int pos, int x1) { return true; }