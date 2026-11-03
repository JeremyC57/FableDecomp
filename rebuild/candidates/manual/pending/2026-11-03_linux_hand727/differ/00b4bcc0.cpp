// CEngineShadowRenderer::CalcClipVolume(const CEngineCamera& camera) -- build the shadow caster / receiver clip volumes for a directional light
// Leaf: ?CalcClipVolume@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4BCC0, length 2179 bytes, thiscall `ret 4`, no EH frame.
// Grade: DIFFER(ours v retail) -- best dl=27: the clip-volume reset stores, the `lea edi` of the first plane and that plane's
//   pushes are scheduled about 3 slots earlier than retail inside the last corner transform; the frustum-plane copy interleaves
//   edx/eax slightly differently. Same instructions, scheduling window only (60+ grades: /G5 /G6 /G7 /GB, loop / unrolled
//   transforms, Reset placement, empty CMatrix4x4/CPlane ctors, every T-expression spelling).
// Callees: CMatrix4x4::SetInverse 0x00A5CB00 (labelled "CLayer::SetAllPolys": wrong), CPlane::Initialise 0x00A42170 (four
//   times through CPlane(p0,p1,p2)); PerspectiveTransformPoint, AddPlane, Reset, SetSphere, PeekFrustrumPlane, PeekCameraPos
//   are inline (FableWin.pdb).
// Shape: the shadow buffer's four near corners (0,0)(1,0)(1,1)(0,1) go back to world space through the inverse
//   world-to-shadow-buffer matrix (a loop: VC7.1 clones the inlined transform with one x87 sum order, retail's; four explicit
//   calls rotate the order); both volumes are reset; four caster side planes from consecutive corners and the corner pushed
//   along the light by ShadowFadeStart; the camera frustum planes whose normal faces the light are added; the receiver
//   volume is the sphere at the camera position with radius ShadowFadeEnd.
// Layout: engine/CShadowRendererSceneData.h (Ego_r CShadowRendererSceneData, 0x240) at CEngineShadowRenderer +0x0C;
//   engine/CEngineCamera.h retail branch adds NoScaleViewTransform .. FrustrumPlanes[4] (+0x114 .. +0x200).
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
    CMatrix4x4 shadowBufferToWorld;
    shadowBufferToWorld.SetInverse(SceneData.WorldToShadowBufferMatrix);
    for (long i = 0; i < 4; ++i)
        shadowBufferToWorld.PerspectiveTransformPoint(corners[i]);

    SceneData.CasterClipVolume.Reset();
    SceneData.RecieverClipVolume.Reset();


    SceneData.CasterClipVolume.AddPlane(CPlane(corners[0], corners[1], corners[0] + SceneData.ShadowLightVector * SceneData.ShadowFadeStart));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[1], corners[2], corners[1] + SceneData.ShadowLightVector * SceneData.ShadowFadeStart));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[2], corners[3], corners[2] + SceneData.ShadowLightVector * SceneData.ShadowFadeStart));
    SceneData.CasterClipVolume.AddPlane(CPlane(corners[3], corners[0], corners[3] + SceneData.ShadowLightVector * SceneData.ShadowFadeStart));

    CPlane frustrumPlanes[4] = {
        camera.PeekFrustrumPlane(0),
        camera.PeekFrustrumPlane(1),
        camera.PeekFrustrumPlane(2),
        camera.PeekFrustrumPlane(3)
    };
    if (frustrumPlanes[0].GetNormal().Dot(SceneData.ShadowLightVector) > 0.0f)
        SceneData.CasterClipVolume.AddPlane(frustrumPlanes[0]);
    if (frustrumPlanes[1].GetNormal().Dot(SceneData.ShadowLightVector) > 0.0f)
        SceneData.CasterClipVolume.AddPlane(frustrumPlanes[1]);
    if (frustrumPlanes[2].GetNormal().Dot(SceneData.ShadowLightVector) > 0.0f)
        SceneData.CasterClipVolume.AddPlane(frustrumPlanes[2]);
    if (frustrumPlanes[3].GetNormal().Dot(SceneData.ShadowLightVector) > 0.0f)
        SceneData.CasterClipVolume.AddPlane(frustrumPlanes[3]);

    SceneData.RecieverClipVolume.SetSphere(C3DBoundingSphere(camera.PeekCameraPos(), SceneData.ShadowFadeEnd));
}
