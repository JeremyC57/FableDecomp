// Behaviour test for 0x00B50510 CEngineShadowRenderer::SetupShadowFuncRenderStates(long, pack): the forwarded matrix is identity.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 {
    C3DVector Right, Up, Look, Pos;
    void SetIdentity()
    {
        Right.Y = Right.Z = Up.X = Up.Z = Look.X = Look.Y = Pos.X = Pos.Y = Pos.Z = 0.0f;
        Right.X = Up.Y = Look.Z = 1.0f;
    }
};
struct CColorBufferDepthPackParameters { int tag; };

static long g_stage = -1;
static CMatrix3x4 g_seen;
static CColorBufferDepthPackParameters* g_pack = 0;

struct CEngineShadowRenderer {
    void SetupShadowFuncRenderStates(long stage, CMatrix3x4* textureTransform, CColorBufferDepthPackParameters* pack)
    {
        g_stage = stage; g_seen = *textureTransform; g_pack = pack;
    }
    void SetupShadowFuncRenderStates(long stage, CColorBufferDepthPackParameters* pack)
    {
        CMatrix3x4 identity;
        identity.SetIdentity();
        SetupShadowFuncRenderStates(stage, &identity, pack);
    }
};

int main()
{
    CEngineShadowRenderer r;
    CColorBufferDepthPackParameters p;
    r.SetupShadowFuncRenderStates(3, &p);
    const float* f = &g_seen.Right.X;
    for (int i = 0; i < 12; ++i) {
        float want = (i == 0 || i == 4 || i == 8) ? 1.0f : 0.0f;
        if (f[i] != want) { std::printf("00b50510_TEST FAIL\n"); return 1; }
    }
    if (g_stage != 3 || g_pack != &p) { std::printf("00b50510_TEST FAIL\n"); return 1; }
    std::printf("00b50510_TEST PASS\n");
    return 0;
}
