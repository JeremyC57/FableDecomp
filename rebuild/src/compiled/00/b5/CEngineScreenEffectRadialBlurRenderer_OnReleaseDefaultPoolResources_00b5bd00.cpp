// Leaf: ?OnReleaseDefaultPoolResources@CEngineScreenEffectRadialBlurRenderer@@UAEXXZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CEngineScreenEffectRadialBlurRenderer.h (was ?OnReleaseDefaultPoolResources@CEngineScreenEffectRadialBlurRenderer_Methods@@QAEXXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTRADIALBLURRENDERER_RTTI_DECLS
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"  // retyped onto the PDB layout; byte parity re-verified
void CEngineScreenEffectRadialBlurRenderer::OnReleaseDefaultPoolResources() { this->DynamicTexturePageHandle = 0xffffffff; }
