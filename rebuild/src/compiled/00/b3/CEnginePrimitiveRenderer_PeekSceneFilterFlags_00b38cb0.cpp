// Leaf: ?PeekSceneFilterFlags@CEnginePrimitiveRenderer_Methods@@QAEKXZ
#include "engine/CEnginePrimitiveRenderer.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEnginePrimitiveRenderer_Methods : CEnginePrimitiveRenderer {
    unsigned long PeekSceneFilterFlags();
};
unsigned long CEnginePrimitiveRenderer_Methods::PeekSceneFilterFlags() { return 3; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?PeekSceneFilterFlags@CEnginePrimitiveRenderer@@UBEKXZ); evidence: RTTI vtable slot + PDB name set.
