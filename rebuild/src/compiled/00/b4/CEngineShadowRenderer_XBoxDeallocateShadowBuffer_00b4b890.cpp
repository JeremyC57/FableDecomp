// CEngineShadowRenderer::XBoxDeallocateShadowBuffer() -- the Xbox-only shadow-buffer release; on PC it does nothing and returns false
// Leaf: ?XBoxDeallocateShadowBuffer@CEngineShadowRenderer@@QAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4B890, `xor al,al ; ret` (3 bytes).
// Name (NF152, 2026-11-20, q4597): Ego_r.pdb 0x009EBCD0 / FableWin.pdb 0x02D78060 ?XBoxDeallocateShadowBuffer@CEngineShadowRenderer@@QAE_NXZ.
//   Its only reference is the tail jump of CEngine::DeallocateShadowBuffer 0x00B23F70 on the shadow renderer 0x01436E60, which already binds
//   this spelling (no other call / jmp / data reference, so no identical-code sharing). Was the placeholder T::Get.
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_XBOXDEALLOCATESHADOWBUFFER
#include "engine/CEngineShadowRenderer.h"

bool CEngineShadowRenderer::XBoxDeallocateShadowBuffer()
{
    return false;
}
