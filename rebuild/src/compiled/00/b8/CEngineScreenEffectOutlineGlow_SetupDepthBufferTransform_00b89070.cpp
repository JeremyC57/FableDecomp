// CEngineScreenEffectOutlineGlow::SetupDepthBufferTransform(const CMatrix3x4&) -- empty on the PC build (`ret 4`) -- NF37 (2026-11-16) port off the address-suffixed stand-in onto the shared engine/CEngineScreenEffectOutlineGlow.h.
// Leaf: ?SetupDepthBufferTransform@CEngineScreenEffectOutlineGlow@@QAEXABVCMatrix3x4@@@Z
// Evidence: Ego_r.pdb / FableWin.pdb ?SetupDepthBufferTransform@CEngineScreenEffectOutlineGlow@@QAEXABVCMatrix3x4@@@Z (the header MEMBERS block spells it).
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW
#define FABLE_RETAIL_CENGINESCREENEFFECTOUTLINEGLOW_MEMBERS
#include "engine/CEngineScreenEffectOutlineGlow.h"

void CEngineScreenEffectOutlineGlow::SetupDepthBufferTransform(const CMatrix3x4& transform)
{
}
