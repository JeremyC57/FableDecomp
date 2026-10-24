#include <cstdio>
#include <cmath>
// Behaviour test for CEnginePrimitiveRenderer2D::Layer2DDistanceToZ(float) @ 0x00B4A6C0: the view-space depth of a point
// at that distance (distance * E33 + E43 + E23 + E13) mapped through the camera's z-buffer factor and offset.
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
};
class CEngineCamera {
public:
    CMatrix3x4 ViewTransform;
    float      ZBufferFactor;
    float      ZBufferOffset;
};
static CEngineCamera g_camera;
static CEngineCamera* g_pEngineCamera = &g_camera;
class CEnginePrimitiveRenderer2D {
public:
    float Layer2DDistanceToZ(float distance)
    {
        const CEngineCamera* camera = g_pEngineCamera;
        const CMatrix3x4& view = camera->ViewTransform;
        float z = distance * view.E33 + view.E43 + view.E23 + view.E13;
        float oneOverZ = 1.0f / z;
        return oneOverZ * camera->ZBufferFactor + camera->ZBufferOffset;
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) <= 1e-5f * (1.0f + std::fabs(b)); }
int main()
{
    CEnginePrimitiveRenderer2D r;
    CMatrix3x4 v = { 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 };
    g_camera.ViewTransform = v;
    g_camera.ZBufferFactor = -0.5f; g_camera.ZBufferOffset = 1.0f;
    if (!Near(r.Layer2DDistanceToZ(2.0f), 0.75f)) { std::printf("00b4a6c0_TEST FAIL\n"); return 1; }
    g_camera.ViewTransform.E13 = 0.5f; g_camera.ViewTransform.E23 = 0.25f; g_camera.ViewTransform.E33 = 2.0f; g_camera.ViewTransform.E43 = 1.25f;
    g_camera.ZBufferFactor = 8.0f; g_camera.ZBufferOffset = 0.125f;
    if (!Near(r.Layer2DDistanceToZ(3.0f), 8.0f / 8.0f + 0.125f)) { std::printf("00b4a6c0_TEST FAIL\n"); return 1; }
    if (!Near(r.Layer2DDistanceToZ(1.0f), 2.125f)) { std::printf("00b4a6c0_TEST FAIL\n"); return 1; }
    std::printf("00b4a6c0_TEST PASS\n");
    return 0;
}