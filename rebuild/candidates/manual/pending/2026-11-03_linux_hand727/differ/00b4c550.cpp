// CEngineShadowRenderer::CalcSpotLightClipVolume(const CEngineLightingManagerSpotlightParams& params) -- build the caster clip volume from the six faces of the shadow-buffer frustum (the unit cube taken back through the inverse world-to-shadow-buffer matrix) and copy it to the receiver volume
// Leaf: ?CalcSpotLightClipVolume@CEngineShadowRenderer@@AAEXABUCEngineLightingManagerSpotlightParams@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4C550, length 2242 bytes, thiscall `ret 4`, no EH frame.
// Grade: DIFFER(ours v retail) -- best dl=20: in the last inlined corner transform the first plane's pushes and `lea` come 6
//   slots early and one `inc edx` sits elsewhere; scheduling window only (30+ grades, the same levers as 0x00B4BCC0).
// Callees: CMatrix4x4::SetInverse 0x00A5CB00, CPlane::Initialise 0x00A42170 (six faces); Reset / AddPlane /
//   PerspectiveTransformPoint inline. The spotlight params argument is not read.
// Shape: both volumes reset; the eight unit-cube corners go back through the inverse world-to-shadow-buffer matrix; the caster
//   volume is reset again and takes the six faces (0,2,1) (4,5,6) (0,4,2) (1,3,5) (0,1,4) (2,6,3); the receiver volume is a
//   copy of it (`rep movsd`, 0x98 bytes).
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#include "engine/CEngineShadowRenderer.h"

void CEngineShadowRenderer::CalcSpotLightClipVolume(const CEngineLightingManagerSpotlightParams& params)
{
    SceneData.CasterClipVolume.Reset();
    SceneData.RecieverClipVolume.Reset();

    C3DVector corners[8] = {
        C3DVector(0.0f, 0.0f, 0.0f),
        C3DVector(1.0f, 0.0f, 0.0f),
        C3DVector(0.0f, 1.0f, 0.0f),
        C3DVector(1.0f, 1.0f, 0.0f),
        C3DVector(0.0f, 0.0f, 1.0f),
        C3DVector(1.0f, 0.0f, 1.0f),
        C3DVector(0.0f, 1.0f, 1.0f),
        C3DVector(1.0f, 1.0f, 1.0f)
    };
    CMatrix4x4 shadowBufferToWorld;
    shadowBufferToWorld.SetInverse(SceneData.WorldToShadowBufferMatrix);
    for (long i = 0; i < 8; ++i)
        shadowBufferToWorld.PerspectiveTransformPoint(corners[i]);

    SceneData.CasterClipVolume.Reset();
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[0], corners[2], corners[1]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[4], corners[5], corners[6]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[0], corners[4], corners[2]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[1], corners[3], corners[5]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[0], corners[1], corners[4]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[2], corners[6], corners[3]));

    SceneData.RecieverClipVolume = SceneData.CasterClipVolume;
}
