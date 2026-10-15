// Behaviour test for CShaderRenderManager::SetProjectionTransform (0x00988540): stores the transposed matrix and ORs 0xC990 into the flags.
#include <cstdio>
struct CMatrix4x4 {
    float M[4][4];
    void Transpose() { for (int r = 0; r < 4; ++r) for (int c = r + 1; c < 4; ++c) { float t = M[r][c]; M[r][c] = M[c][r]; M[c][r] = t; } }
};
struct CShaderRenderManager {
    CMatrix4x4 ProjectionMatrix;
    unsigned long UpdateFlags;
    void SetProjectionTransform(const CMatrix4x4& projection)
    {
        ProjectionMatrix = projection;
        ProjectionMatrix.Transpose();
        UpdateFlags |= 0xc990;
    }
};
int main()
{
    CMatrix4x4 p;
    for (int r = 0; r < 4; ++r) for (int c = 0; c < 4; ++c) p.M[r][c] = (float)(r * 4 + c);
    CShaderRenderManager m; m.UpdateFlags = 1;
    m.SetProjectionTransform(p);
    if (m.ProjectionMatrix.M[0][1] != 4.0f || m.ProjectionMatrix.M[1][0] != 1.0f || m.ProjectionMatrix.M[3][2] != 11.0f || m.ProjectionMatrix.M[2][2] != 10.0f ||
        p.M[0][1] != 1.0f || m.UpdateFlags != 0xc991) { std::printf("00988540_TEST FAIL\n"); return 1; }
    std::printf("00988540_TEST PASS\n");
    return 0;
}