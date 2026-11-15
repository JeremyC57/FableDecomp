// CEngineShadowRenderer::CalcSpotLightClipVolume -- original spot-light clip volumes.
// Leaf: ?CalcSpotLightClipVolume@CEngineShadowRenderer@@AAEXABUCEngineLightingManagerSpotlightParams@@@Z
// Flags: /d2inlT81
// Inline-budget: /d2inlT81 -- the same matrix helper as 0x00B4BCC0: T80 calls it out of line (723 bytes); T81 expands it as retail (2242 bytes; INLINE_LADDER.json).
// Retail: 0x00B4C550,2242 bytes; original spot_params/corners/buffer_to_world/n names.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_CLIPVOLUMES_PASS22
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_SHADOWFRUSTRUM_PASS22
#define FABLE_RETAIL_CPLANE_LINE
#define FABLE_RETAIL_CPLANE_THREEPOINT_CONSTRUCTOR_PASS22
#define FABLE_RETAIL_CMATRIX4X4_SHADOW_PERSPECTIVE_PASS22
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINECAMERA
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineCamera.h"

void CEngineShadowRenderer::CalcSpotLightClipVolume(const CEngineLightingManagerSpotlightParams& spot_params)
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
    CMatrix4x4 buffer_to_world;
    buffer_to_world.SetInverse(SceneData.WorldToShadowBufferMatrix);
    for (long n = 0; n < 8; ++n)
        buffer_to_world.PerspectiveTransformPoint(corners[n]);

    SceneData.CasterClipVolume.Reset();
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[0], corners[2], corners[1]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[4], corners[5], corners[6]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[0], corners[4], corners[2]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[1], corners[3], corners[5]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[0], corners[1], corners[4]));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[2], corners[6], corners[3]));

    SceneData.RecieverClipVolume = SceneData.CasterClipVolume;
}