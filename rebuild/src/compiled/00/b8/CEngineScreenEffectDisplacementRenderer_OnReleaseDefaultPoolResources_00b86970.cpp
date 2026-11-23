// Leaf: ?OnReleaseDefaultPoolResources@CEngineScreenEffectDisplacementRenderer@@UAEXXZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CEngineScreenEffectDisplacementRenderer.h (was ?OnReleaseDefaultPoolResources@CEngineScreenEffectDisplacementRenderer_Methods@@QAEXXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_RTTI_DECLS
#include "engine/CEngineScreenEffectDisplacementRenderer.h"  // retyped onto the PDB layout; byte parity re-verified
void CEngineScreenEffectDisplacementRenderer::OnReleaseDefaultPoolResources() { this->DynamicTexturePageHandle = 0xffffffff; }
