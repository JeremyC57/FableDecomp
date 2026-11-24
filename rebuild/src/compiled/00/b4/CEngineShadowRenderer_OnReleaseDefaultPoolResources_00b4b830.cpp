// CEngineShadowRenderer::OnReleaseDefaultPoolResources() -- when the default-pool resources are held (+0x2af): uninitialise the render target at +0x2c4, release the two shader holders at +0x308 / +0x2f8 and the two buffers at +0x318 / +0x2e8, then clear the flag
// Leaf: ?OnReleaseDefaultPoolResources@CEngineShadowRenderer@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4B830, length 77 bytes.
// Callees: CRenderTarget::Uninitialise @ 0x00A10E30, the holder release @ 0x009F2E20 twice, the buffer release @ 0x009F9F70 twice (each on the member by address).
// Shape: five `lea ecx ; call` pairs in that order, then an immediate byte clear.
// Naming (NF142, 2026-11-20, q6305): the CShaderHolder_009f2e20 stand-in and the local CTexture / CRenderTarget models replaced by the shared
//   engine/CSurface.h, engine/CTexture.h and engine/CRenderTarget.h (Ego_r.pdb CEngineShadowRenderer: RenderTarget +0x2C4, ShadowBufferTexture
//   +0x2E8, RenderSurface +0x2F0, ShadowBufferDepthSurface +0x2F8 / ShadowBufferColourSurface +0x308 (CSurface), ShadowBufferColourTexture
//   +0x318): the two surface calls bind ?Uninitialise@CSurface@@QAEXXZ 0x009F2E20 (Ego_r.pdb 0x00B46AF0 + FableWin.pdb 0x030C99C0; no PDB has a
//   Release there). The row's local CEngineShadowRenderer model (own leaf unchanged) keeps its token; the 8 bytes at +0x2F0 are RenderSurface.
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CSURFACE
#include "engine/CTexture.h"
#include "engine/CRenderTarget.h"

class CEngineShadowRenderer { public:
    unsigned char          _pad[0x2ab];
    bool                   m_defaultPoolHeld;      // +0x2af
    unsigned char          _pad2b0[0x14];
    CRenderTarget          m_renderTarget;         // +0x2c4
    CTexture               m_bufferA;              // +0x2e8
    unsigned char          _pad2f0[8];             // +0x2f0 (RenderSurface)
    CSurface               m_shaderA;              // +0x2f8
    CSurface               m_shaderB;              // +0x308
    CTexture               m_bufferB;              // +0x318
    virtual void OnReleaseDefaultPoolResources();
};

void CEngineShadowRenderer::OnReleaseDefaultPoolResources()
{
    if (m_defaultPoolHeld) {
        m_renderTarget.Uninitialise();
        m_shaderB.Uninitialise();
        m_shaderA.Uninitialise();
        m_bufferB.Uninitialise();
        m_bufferA.Uninitialise();
        m_defaultPoolHeld = false;
    }
}
