// CEngineShadowRenderer::XBoxReallocateShadowBuffer() -- the Xbox-only shadow-buffer re-allocation; empty on PC
// Leaf: ?XBoxReallocateShadowBuffer@CEngineShadowRenderer@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4B8A0, a bare `ret`.
// Name (NF152, 2026-11-20, q4598): Ego_r.pdb 0x009EBCE0 / FableWin.pdb 0x02D78070 ?XBoxReallocateShadowBuffer@CEngineShadowRenderer@@QAEXXZ.
//   Its only reference is the tail jump of CEngine::ReallocateShadowBuffer 0x00B23F80, which already binds this spelling. Was the
//   placeholder T::Hook (gap-scan row).
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_XBOXREALLOCATESHADOWBUFFER
#include "engine/CEngineShadowRenderer.h"

void CEngineShadowRenderer::XBoxReallocateShadowBuffer()
{
}
