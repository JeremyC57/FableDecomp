// CEngineShadowRenderer::CEngineShadowRenderer() -- CEngineComponent base, the scene data (two cleared shadow clip volumes), the capture block, render target, textures and surfaces, then the default shadow distances / biases / perspective interpolation / video options, then InitialiseTextures()
// Grade: DIFFER(406v406) -- constant allocation / store scheduling only: retail holds 1.0 eax, 8.0 ecx, 0.1 edx at once (loads hoisted, stores in source order, 0x00B5045F..0x00B504B4); ours reuses eax per constant and pairs each constant's stores
// Leaf: ??0CEngineShadowRenderer@@QAE@XZ
// Retail: Fable.exe 0x00B50370, 406 bytes, plain `ret` (Ego_r 0x009EEE10 / FableWin 0x02D739D0). Ego_d: no locals (debug length 2829: FableWin's /Od body is 611i v this text's 95i -- debug-only code, not asserts).
// Overlay opt-ins: FABLE_RETAIL_CSHADOWCLIPVOLUME_CTOR (FableWin ??0CShadowClipVolume@@QAE@XZ, fwsize SAME 21i/69b), FABLE_RETAIL_CENGINESHADOWRENDERER_INITIALISETEXTURES.
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_INITIALISETEXTURES
#define FABLE_RETAIL_CSHADOWCLIPVOLUME_CTOR
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE_CTOR
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CMATRIX3X4
#include "engine/CEngineShadowRenderer.h"

CEngineShadowRenderer::CEngineShadowRenderer()
{
    SceneData.ShadowFadeEnd = 32.0f;
    MaxShadowDistance = 32.0f;
    SceneData.ShadowFadeStart = 8.0f;
    ForegroundBufferSpan = 8.0f;
    ShadowProjectionZOffset = 0.0001f;
    ShadowProjectionZSlopeOffset = 2.0f;
    ColourBufferShadowTextureZBias = -0.001f;
    ColourBufferShadowMinWClamp = 0.1f;
    TexturesAllocated = false;
    SceneOriginLocked = false;
    ShadowFadeRatio = 0.5f;
    PerspectiveInterpStart = 0.1f;
    PerspectiveInterpEnd = 0.2f;
    UserVideoOptionShadowBufferSize = 0x400;
    UserVideoOptionShadowDistanceScale = 1.0f;
    RenderingTechnique = SHADOW_RENDERING_TECHNIQUE_NONE;
    PerspectiveShadowsEnabled = true;
    EnableFrustrumDebugDisplay = false;
    ShadowBufferTextureUVScaler.Initialise(1.0f, 1.0f, 1.0f);
    InitialiseTextures();
}
