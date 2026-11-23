// Leaf: ?OnReleaseDefaultPoolResources@CEngineScreenEffectGlowRenderer@@UAEXXZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CEngineScreenEffectGlowRenderer.h (was ?OnReleaseDefaultPoolResources@CEngineScreenEffectGlowRenderer_Methods@@QAEXXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER_RTTI_DECLS
#include "engine/CEngineScreenEffectGlowRenderer.h"  // retyped onto the PDB layout; byte parity re-verified
void CEngineScreenEffectGlowRenderer::OnReleaseDefaultPoolResources() { this->DynamicTexturePageHandle = 0xffffffff; }
