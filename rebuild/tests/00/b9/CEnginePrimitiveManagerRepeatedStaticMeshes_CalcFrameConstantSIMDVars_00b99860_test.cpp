// Behaviour test for CEnginePrimitiveManagerRepeatedStaticMeshes::CalcFrameConstantSIMDVars (0x00B99860): the camera
// position and the four frustum-plane normals are splatted into all four lanes of the SIMD block's camera vectors.
#include <cstdio>

class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {} };
class CPlane { public: C3DVector Normal; float Distance; };
struct CSIMDVariableBlock {
    float CameraPosX[4], CameraPosY[4], CameraPosZ[4];
    float CameraClipPlaneLeftX[4], CameraClipPlaneLeftY[4], CameraClipPlaneLeftZ[4];
    float CameraClipPlaneRightX[4], CameraClipPlaneRightY[4], CameraClipPlaneRightZ[4];
    float CameraClipPlaneTopX[4], CameraClipPlaneTopY[4], CameraClipPlaneTopZ[4];
    float CameraClipPlaneBottomX[4], CameraClipPlaneBottomY[4], CameraClipPlaneBottomZ[4];
    float FadeDistance[4];
};
class CEngineCamera {
public:
    C3DVector WorldPosition;
    CPlane FrustrumPlanes[4];
    const C3DVector& GetPosition() const { return WorldPosition; }
    const CPlane& GetFrustrumPlane(long i) const { return FrustrumPlanes[i]; }
};
static CEngineCamera* g_pEngineCamera;

class CEnginePrimitiveManagerRepeatedStaticMeshes {
public:
    CSIMDVariableBlock* SIMDVariableBlock;
    void CalcFrameConstantSIMDVars()
    {
        for (long i = 0; i < 4; ++i) {
            SIMDVariableBlock->CameraPosX[i] = g_pEngineCamera->GetPosition().X;
            SIMDVariableBlock->CameraPosY[i] = g_pEngineCamera->GetPosition().Y;
            SIMDVariableBlock->CameraPosZ[i] = g_pEngineCamera->GetPosition().Z;
            C3DVector left = g_pEngineCamera->GetFrustrumPlane(0).Normal;
            C3DVector right = g_pEngineCamera->GetFrustrumPlane(1).Normal;
            C3DVector top = g_pEngineCamera->GetFrustrumPlane(2).Normal;
            C3DVector bottom = g_pEngineCamera->GetFrustrumPlane(3).Normal;
            SIMDVariableBlock->CameraClipPlaneLeftX[i] = left.X;
            SIMDVariableBlock->CameraClipPlaneLeftY[i] = left.Y;
            SIMDVariableBlock->CameraClipPlaneLeftZ[i] = left.Z;
            SIMDVariableBlock->CameraClipPlaneRightX[i] = right.X;
            SIMDVariableBlock->CameraClipPlaneRightY[i] = right.Y;
            SIMDVariableBlock->CameraClipPlaneRightZ[i] = right.Z;
            SIMDVariableBlock->CameraClipPlaneTopX[i] = top.X;
            SIMDVariableBlock->CameraClipPlaneTopY[i] = top.Y;
            SIMDVariableBlock->CameraClipPlaneTopZ[i] = top.Z;
            SIMDVariableBlock->CameraClipPlaneBottomX[i] = bottom.X;
            SIMDVariableBlock->CameraClipPlaneBottomY[i] = bottom.Y;
            SIMDVariableBlock->CameraClipPlaneBottomZ[i] = bottom.Z;
        }
    }
};

int main()
{
    CEngineCamera cam;
    cam.WorldPosition.X = 1.0f; cam.WorldPosition.Y = 2.0f; cam.WorldPosition.Z = 3.0f;
    int p;
    for (p = 0; p < 4; ++p) {
        cam.FrustrumPlanes[p].Normal.X = 10.0f * p + 1.0f;
        cam.FrustrumPlanes[p].Normal.Y = 10.0f * p + 2.0f;
        cam.FrustrumPlanes[p].Normal.Z = 10.0f * p + 3.0f;
        cam.FrustrumPlanes[p].Distance = 99.0f;
    }
    g_pEngineCamera = &cam;
    CSIMDVariableBlock block;
    int i;
    for (i = 0; i < 4; ++i) block.FadeDistance[i] = -1.0f;
    CEnginePrimitiveManagerRepeatedStaticMeshes mgr;
    mgr.SIMDVariableBlock = &block;
    mgr.CalcFrameConstantSIMDVars();
    for (i = 0; i < 4; ++i) {
        if (block.CameraPosX[i] != 1.0f || block.CameraPosY[i] != 2.0f || block.CameraPosZ[i] != 3.0f) { std::printf("00b99860_TEST FAIL\n"); return 1; }
        if (block.CameraClipPlaneLeftX[i] != 1.0f || block.CameraClipPlaneLeftZ[i] != 3.0f) { std::printf("00b99860_TEST FAIL\n"); return 1; }
        if (block.CameraClipPlaneRightY[i] != 12.0f || block.CameraClipPlaneTopX[i] != 21.0f) { std::printf("00b99860_TEST FAIL\n"); return 1; }
        if (block.CameraClipPlaneBottomZ[i] != 33.0f || block.FadeDistance[i] != -1.0f) { std::printf("00b99860_TEST FAIL\n"); return 1; }
    }
    std::printf("00b99860_TEST PASS\n");
    return 0;
}