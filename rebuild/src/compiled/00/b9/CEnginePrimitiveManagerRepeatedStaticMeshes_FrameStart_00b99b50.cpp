// CEnginePrimitiveManagerRepeatedStaticMeshes::FrameStart() -- point the four current-buffer pointers (+0x80..+0x8c) at the shadow buffer pairs (+0x60..+0x78) when the render settings' mode at +0x334 is 2, else at the normal pairs (+0x40..+0x58); then the common frame start
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B99B50, length 96 bytes.
// Callees: the common frame start @ 0x00B99860 (a tail jump from both arms; the manifest label "engine_copy_global_transform_to_array" is propagated). Globals: the
// render settings @ 0x01436E60. Shape: each arm forms the four addresses with `lea` pairs (edx/eax) and stores them in order; `this` stays in ecx throughout.
// NF44 (round 6): the 0x01436E60 global is NEngineInternal::GPShadowRenderer (Ego_r.pdb / FableWin.pdb
// ?GPShadowRenderer@NEngineInternal@@3PAVCEngineShadowRenderer@@A) and +0x334 its RenderingTechnique, read through the FableWin.pdb inline
// PeekRenderingTechnique() (engine/CEngineShadowRenderer.h _MEMBERS); were the SRenderSettings_01436e60 / g_pRenderSettings_01436e60 stand-ins.
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#include "engine/CEngineShadowRenderer.h"
namespace NEngineInternal { extern CEngineShadowRenderer* GPShadowRenderer; }   // 0x01436E60

struct SMeshBuffer { void* m_pA; void* m_pB; };   // 8 bytes

class CEnginePrimitiveManagerRepeatedStaticMeshes { public:
    unsigned char _pad[0x40];
    SMeshBuffer   m_normal[4];                     // +0x40 .. +0x60
    SMeshBuffer   m_shadow[4];                     // +0x60 .. +0x80
    SMeshBuffer*  m_pCurrent[4];                   // +0x80 .. +0x90
    void FrameStartCommon();                       // 0x00B99860
    void FrameStart();
};

void CEnginePrimitiveManagerRepeatedStaticMeshes::FrameStart()
{
    if (NEngineInternal::GPShadowRenderer->PeekRenderingTechnique() == CEngineShadowRenderer::SHADOW_RENDERING_TECHNIQUE_COLOUR_BUFFER) {
        m_pCurrent[0] = &m_shadow[0];
        m_pCurrent[1] = &m_shadow[1];
        m_pCurrent[2] = &m_shadow[2];
        m_pCurrent[3] = &m_shadow[3];
    } else {
        m_pCurrent[0] = &m_normal[0];
        m_pCurrent[1] = &m_normal[1];
        m_pCurrent[2] = &m_normal[2];
        m_pCurrent[3] = &m_normal[3];
    }
    FrameStartCommon();
}