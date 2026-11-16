// CEngineScreenEffectDisplacementRenderer::SetupShadowHWAsDepthBuffer(const CMatrix3x4*) -- empty on the PC build (`ret 4`) -- NF37 (2026-11-16) port off the address-suffixed stand-in onto the shared engine/CEngineScreenEffectDisplacementRenderer.h.
// Leaf: ?SetupShadowHWAsDepthBuffer@CEngineScreenEffectDisplacementRenderer@@QAEXPBVCMatrix3x4@@@Z
// Evidence: Ego_r.pdb ?SetupShadowHWAsDepthBuffer@CEngineScreenEffectDisplacementRenderer@@QAEXPBVCMatrix3x4@@@Z; the header SHADOWHW opt-in (chunk 2059, row 0x00BC25F0) names 0x00B86A00.
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER_SHADOWHW
#include "engine/CEngineScreenEffectDisplacementRenderer.h"

void CEngineScreenEffectDisplacementRenderer::SetupShadowHWAsDepthBuffer(const CMatrix3x4* transform)
{
}
