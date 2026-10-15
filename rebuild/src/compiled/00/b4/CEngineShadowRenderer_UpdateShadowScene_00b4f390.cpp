// CEngineShadowRenderer::UpdateShadowScene(CEngineCamera*, bool, CEngineLightingManagerSpotlightParams*) -- latch the camera position, look vector and view matrix (unless the scene origin is locked), then rebuild the shadow space for the active technique
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4F390, length 144 bytes.
// Callees: SetShadowClipVolumesToNeverShadow @ 0x00B4CE20, UpdateSpotLightShadowSpace(spot) @ 0x00B4EF80, UpdateShadowSpace(camera) @ 0x00B4D370.
// Layout: engine/CEngineShadowRenderer.h -- SceneOriginLocked +0x2ad, CameraLookVector +0x26d, CameraViewMatrix +0x27c,
// RenderingTechnique +0x334, SceneData +0xc (CShadowRendererSceneData, CameraPos at +0x104 of it = +0x110);
// engine/CEngineCamera.h -- CameraDesc +0x14 (CEngineCameraDesc, WorldPosition at +0x40 of it = +0x54), ViewTransform +0x80,
// LookVector +0x1b4. The headers keep these composites as byte arrays, so they are viewed through C3DVector / CMatrix3x4.
// Shape: the two C3DVector copies go through `lea` of source and destination and three dword moves; the 12-dword
// CMatrix3x4 is a POD block copy (`rep movsd`); each branch tail-calls with its own `ret 0Ch`; the camera stays in edx.
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineCamera.h"

struct C3DVector {
    float X, Y, Z;
};

struct CMatrix3x4 {
    float m[12];
};

struct CEngineLightingManagerSpotlightParams;

struct CEngineShadowRenderer_UpdateShadowScene : CEngineShadowRenderer {
    void SetShadowClipVolumesToNeverShadow();                                  // 0x00B4CE20
    void UpdateSpotLightShadowSpace(CEngineLightingManagerSpotlightParams* spot);   // 0x00B4EF80
    void UpdateShadowSpace(CEngineCamera* camera);                             // 0x00B4D370
    void UpdateShadowScene(CEngineCamera* camera, bool spotLight, CEngineLightingManagerSpotlightParams* spot);
};

void CEngineShadowRenderer_UpdateShadowScene::UpdateShadowScene(CEngineCamera* camera, bool spotLight, CEngineLightingManagerSpotlightParams* spot)
{
    if (!SceneOriginLocked) {
        *(C3DVector*)(SceneData + 0x104) = *(C3DVector*)(camera->CameraDesc + 0x40);   // SceneData.CameraPos = CameraDesc.WorldPosition
        *(C3DVector*)CameraLookVector = *(C3DVector*)&camera->LookVector_X;
        *(CMatrix3x4*)CameraViewMatrix = *(CMatrix3x4*)camera->ViewTransform;
    }
    if (*(long*)RenderingTechnique == 0)
        SetShadowClipVolumesToNeverShadow();
    else if (spotLight)
        UpdateSpotLightShadowSpace(spot);
    else
        UpdateShadowSpace(camera);
}