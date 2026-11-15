// CEngineShadowRenderer::CalcClipVolume -- original shadow caster and receiver volumes.
// Leaf: ?CalcClipVolume@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z
// Flags: /d2inlT81
// Inline-budget: /d2inlT81 -- the clip-volume matrix helper: the default threshold (74) and T80 call it out of line (1439 bytes); T81 is the smallest threshold that expands it as retail (2179 bytes; INLINE_LADDER.json).
// Retail: 0x00B4BCC0,2179bytes. Both original PDBs confirm identity and locals.
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

void CEngineShadowRenderer::CalcClipVolume(const CEngineCamera& camera)
{
    C3DVector corners[4] = {
        C3DVector(0.0f, 0.0f, 0.0f),
        C3DVector(1.0f, 0.0f, 0.0f),
        C3DVector(1.0f, 1.0f, 0.0f),
        C3DVector(0.0f, 1.0f, 0.0f)
    };
    CMatrix4x4 buffer_to_world;
    buffer_to_world.SetInverse(SceneData.WorldToShadowBufferMatrix);
    for (long n = 0; n < 4; ++n)
        buffer_to_world.PerspectiveTransformPoint(corners[n]);

    SceneData.CasterClipVolume.Reset();
    SceneData.RecieverClipVolume.Reset();


    SceneData.CasterClipVolume.AddPlane(CPlane(corners[0], corners[1], corners[0] + SceneData.ShadowLightVector * SceneData.ShadowFadeStart));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[1], corners[2], corners[1] + SceneData.ShadowLightVector * SceneData.ShadowFadeStart));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[2], corners[3], corners[2] + SceneData.ShadowLightVector * SceneData.ShadowFadeStart));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[3], corners[0], corners[3] + SceneData.ShadowLightVector * SceneData.ShadowFadeStart));

    // Original donor02d76ba0..02d76bf6 retains these visualization loops;
    // their per-plane/per-corner debug-only body is absent in the retail configuration.
    for (long pl = 0; pl < 4; ++pl)
        for (long cor = 0; cor < 4; ++cor) { }

    CPlane cam_planes[4] = {
        camera.PeekFrustrumPlane(0),
        camera.PeekFrustrumPlane(1),
        camera.PeekFrustrumPlane(2),
        camera.PeekFrustrumPlane(3)
    };
    for (long n = 0; n < 4; ++n)
        if (cam_planes[n].GetNormal().Dot(SceneData.ShadowLightVector) > 0.0f)
            SceneData.CasterClipVolume.AddPlane(cam_planes[n]);

    SceneData.RecieverClipVolume.SetSphere(C3DBoundingSphere(camera.PeekCameraPos(), SceneData.ShadowFadeEnd));
}