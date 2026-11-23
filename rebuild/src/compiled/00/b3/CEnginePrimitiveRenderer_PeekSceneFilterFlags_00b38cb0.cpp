// Leaf: ?PeekSceneFilterFlags@CEnginePrimitiveRenderer@@UBEKXZ
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER   // alias route: engine/CEnginePrimitiveRenderer.h declares PeekSceneFilterFlags under this class-model switch
#include "engine/CEnginePrimitiveRenderer.h"  // retyped onto the PDB layout; byte parity re-verified
unsigned long CEnginePrimitiveRenderer::PeekSceneFilterFlags() const { return 3; }
// Was defined on a stand-in alias class (owner rule 2026-11-05); now the real member, through the class-model switch above (alias route, 2026-11-23).
