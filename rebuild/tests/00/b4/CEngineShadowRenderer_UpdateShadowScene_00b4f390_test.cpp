// Behaviour test for 0x00B4F390 CEngineShadowRenderer::UpdateShadowScene: latch unless locked, dispatch on technique / flag.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 { float m[12]; };
struct CEngineCamera { C3DVector WorldPosition; CMatrix3x4 ViewTransform; C3DVector LookVector; };
struct CSpot { int id; };

static int g_call = 0; static void* g_arg = 0;

struct CEngineShadowRenderer {
    C3DVector CameraPos; C3DVector CameraLookVector; CMatrix3x4 CameraViewMatrix;
    bool SceneOriginLocked; long RenderingTechnique;
    void SetShadowClipVolumesToNeverShadow() { g_call = 1; g_arg = 0; }
    void UpdateSpotLightShadowSpace(CSpot* s) { g_call = 2; g_arg = s; }
    void UpdateShadowSpace(CEngineCamera* c)  { g_call = 3; g_arg = c; }
    void UpdateShadowScene(CEngineCamera* camera, bool spotLight, CSpot* spot)
    {
        if (!SceneOriginLocked) {
            CameraPos = camera->WorldPosition;
            CameraLookVector = camera->LookVector;
            CameraViewMatrix = camera->ViewTransform;
        }
        if (RenderingTechnique == 0) SetShadowClipVolumesToNeverShadow();
        else if (spotLight) UpdateSpotLightShadowSpace(spot);
        else UpdateShadowSpace(camera);
    }
};

int main()
{
    CEngineCamera cam; CSpot spot;
    cam.WorldPosition.X = 1; cam.WorldPosition.Y = 2; cam.WorldPosition.Z = 3;
    cam.LookVector.X = 4; cam.LookVector.Y = 5; cam.LookVector.Z = 6;
    for (int i = 0; i < 12; ++i) cam.ViewTransform.m[i] = (float)(10 + i);
    CEngineShadowRenderer r;
    r.CameraPos.X = -1; r.CameraViewMatrix.m[11] = -1;
    r.SceneOriginLocked = true; r.RenderingTechnique = 0;
    r.UpdateShadowScene(&cam, true, &spot);
    if (r.CameraPos.X != -1 || g_call != 1) { std::printf("00b4f390_TEST FAIL\n"); return 1; }
    r.SceneOriginLocked = false; r.RenderingTechnique = 2;
    r.UpdateShadowScene(&cam, true, &spot);
    if (r.CameraPos.Z != 3 || r.CameraLookVector.Y != 5 || r.CameraViewMatrix.m[11] != 21 || g_call != 2 || g_arg != &spot) { std::printf("00b4f390_TEST FAIL\n"); return 1; }
    r.UpdateShadowScene(&cam, false, &spot);
    if (g_call != 3 || g_arg != &cam) { std::printf("00b4f390_TEST FAIL\n"); return 1; }
    std::printf("00b4f390_TEST PASS\n");
    return 0;
}