// CEngineShadowRenderer::UpdateShadowScene(CEngineCamera*, bool, CEngineLightingManagerSpotlightParams*) -- latch the camera position, look vector and view matrix (unless the scene origin is locked), then rebuild the shadow space for the active technique
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4F390, length 144 bytes.
// Callees: SetShadowClipVolumesToNeverShadow @ 0x00B4CE20, UpdateSpotLightShadowSpace(spot) @ 0x00B4EF80, UpdateShadowSpace(camera) @ 0x00B4D370.
// Layout: engine/CEngineShadowRenderer.h -- SceneOriginLocked +0x2ad, CameraLookVector +0x26d, CameraViewMatrix +0x27c,
// RenderingTechnique +0x334, SceneData +0xc (CShadowRendererSceneData, CameraPos at +0x104 of it = +0x110);
// engine/CEngineCamera.h -- CameraDesc +0x14 (CEngineCameraDesc, WorldPosition at +0x40 of it = +0x54), ViewTransform +0x80,
// LookVector +0x1b4. The headers keep these composites as byte arrays, so they are viewed through C3DVector / CMatrix3x4.
// Shape: the two C3DVector copies go through `lea` of source and destination and three dword moves; the 12-dword
// CMatrix3x4 is a POD block copy (`rep movsd`); each branch tail-calls with its own `ret 0Ch`; the camera stays in edx.
// Leaf: ?UpdateShadowScene@CEngineShadowRenderer@@QAEXABVCEngineCamera@@_NABUCEngineLightingManagerSpotlightParams@@@Z
// Name (round 7, NF53): Ego_r.pdb ?UpdateShadowScene@CEngineShadowRenderer@@QAEXABVCEngineCamera@@_NABUCEngineLightingManagerSpotlightParams@@@Z
//   (the landed caller 0x00B484B0 binds it so) and its private callees ?UpdateShadowSpace@CEngineShadowRenderer@@AAEXABVCEngineCamera@@@Z
//   0x00B4D370 (Ego_r 0x009F0370 / FableWin 0x02D74FB0), ?UpdateSpotLightShadowSpace@..@@AAEXABUCEngineLightingManagerSpotlightParams@@@Z
//   0x00B4EF80, ?SetShadowClipVolumesToNeverShadow@..@@AAEXXZ 0x00B4CE20. Was the derived view CEngineShadowRenderer_UpdateShadowScene over
//   the flattened layout (pointer parameters, byte offsets); now the retail engine/CEngineShadowRenderer.h members (SceneData.CameraPos,
//   CameraLookVector, CameraViewMatrix, SceneOriginLocked, RenderingTechnique) and the CEngineCamera accessors, with the overlay opt-in
//   FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESPACE.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESCENE
#define FABLE_RETAIL_CENGINESHADOWRENDERER_UPDATESPACE
#define FABLE_RETAIL_CENGINESHADOWRENDERER_PACKED_VECTORS
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE
#define FABLE_RETAIL_CENGINECAMERA_LOOKFOV
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineCamera.h"

void CEngineShadowRenderer::UpdateShadowScene(const CEngineCamera& camera, bool spot_light, const CEngineLightingManagerSpotlightParams& spot)
{
    if (!SceneOriginLocked) {
        SceneData.CameraPos = camera.PeekCameraPos();
        CameraLookVector = camera.PeekLookVector();
        CameraViewMatrix = camera.PeekViewTransform();
    }
    if (RenderingTechnique == SHADOW_RENDERING_TECHNIQUE_NONE)
        SetShadowClipVolumesToNeverShadow();
    else if (spot_light)
        UpdateSpotLightShadowSpace(spot);
    else
        UpdateShadowSpace(camera);
}
