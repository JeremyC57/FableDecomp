// Leaf: ?PeekSceneFilterFlags@CEngineSubPrimitiveRenderer_Methods@@QAEKXZ
#include "engine/CEngineSubPrimitiveRenderer.h"  // retyped onto the PDB layout; byte parity re-verified
struct CEngineSubPrimitiveRenderer_Methods : CEngineSubPrimitiveRenderer {
    unsigned long PeekSceneFilterFlags();
};
unsigned long CEngineSubPrimitiveRenderer_Methods::PeekSceneFilterFlags() { return 3; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?PeekSceneFilterFlags@CEngineSubPrimitiveRenderer@@UBEKXZ); evidence: RTTI vtable slot + PDB name set.
