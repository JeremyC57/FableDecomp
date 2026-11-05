// Leaf: ?DrawGetEngineThemeAt@CIDrawMap_Methods@@QAEHHHH@Z
#include "engine/CIDrawMap.h"  // retyped onto the PDB layout; byte parity re-verified
struct CIDrawMap_Methods : CIDrawMap {
    int DrawGetEngineThemeAt(int x0, int x1, int x2);
};
int CIDrawMap_Methods::DrawGetEngineThemeAt(int x0, int x1, int x2) { return 0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?DrawGetEngineThemeAt@CIDrawMap@@UBEJJJJ@Z); evidence: RTTI vtable slot + PDB name set.
