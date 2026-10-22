// Behaviour test for CEngineShadowRenderer::BuildDerivedShadowMatriciesFromWorldToShadowMatrix @ 0x00B4BA60
#include <cstdio>
#include <cmath>

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CMatrix4x4 {
    float M[4][4];
    void Scale(const C3DVector& s)
    {
        for (int i = 0; i < 4; ++i) { M[i][0] *= s.X; M[i][1] *= s.Y; M[i][2] *= s.Z; }
    }
    void Translate(const C3DVector& t)
    {
        for (int i = 0; i < 4; ++i) {
            M[i][0] += M[i][3] * t.X; M[i][1] += M[i][3] * t.Y; M[i][2] += M[i][3] * t.Z;
        }
    }
};
struct SceneData { CMatrix4x4 W2S, HSS, TS; };
struct Renderer {
    SceneData* Data;
    long Technique;
    float UVScaleX, UVScaleY, UVScaleZ;
    float ZBias;
    void Build()
    {
        Data->HSS = Data->W2S;
        Data->HSS.Scale(C3DVector(2.0f, -2.0f, 1.0f));
        Data->HSS.Translate(C3DVector(-1.0f, 1.0f, 0.0f));
        Data->TS = Data->W2S;
        Data->TS.Scale(C3DVector(UVScaleX, UVScaleY, UVScaleZ));
        if (Technique == 2)
            Data->TS.Translate(C3DVector(0.0f, 0.0f, ZBias));
    }
};

static int near_(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    SceneData d;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            d.W2S.M[i][j] = (float)(i * 4 + j + 1);
    Renderer r;
    r.Data = &d; r.Technique = 2; r.UVScaleX = 0.5f; r.UVScaleY = -0.5f; r.UVScaleZ = 1.0f; r.ZBias = 0.25f;
    r.Build();
    // row 1: (5,6,7,8): HSS x = 5*2 + 8*-1 = 2 ; y = 6*-2 + 8 = -4 ; z = 7 ; w = 8
    if (!near_(d.HSS.M[1][0], 2.0f) || !near_(d.HSS.M[1][1], -4.0f) || !near_(d.HSS.M[1][2], 7.0f) || !near_(d.HSS.M[1][3], 8.0f)) {
        std::printf("00b4ba60_TEST FAIL\n"); return 1;
    }
    // TS row 1: x = 2.5, y = -3, z = 7 + 8*0.25 = 9
    if (!near_(d.TS.M[1][0], 2.5f) || !near_(d.TS.M[1][1], -3.0f) || !near_(d.TS.M[1][2], 9.0f)) {
        std::printf("00b4ba60_TEST FAIL\n"); return 1;
    }
    r.Technique = 1;
    r.Build();
    if (!near_(d.TS.M[1][2], 7.0f)) { std::printf("00b4ba60_TEST FAIL\n"); return 1; }
    std::printf("00b4ba60_TEST PASS\n");
    return 0;
}
