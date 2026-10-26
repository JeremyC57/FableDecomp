// behaviour test for 00b32660 CEnginePrimitiveRenderer::FrameStart: shadow shader selection, frame number, manager
// FrameStart calls, LOD tolerance refresh and the mipmap / FOV-adjusted LOD sizes
#include <cstdio>
#include <cmath>
static int g_calls = 0;
static unsigned short g_frame = 7;
struct CMgr { virtual void FrameStart() { ++g_calls; } };
class CPixelShader { public: int id; };
struct CRenderer {
    float LODErr, CharLODErr, LODScreen, LODScreenFOV, MipmapQuality, MipmapMul;
    CPixelShader Depth[8], Colour[8];
    CPixelShader* Selected[8];
    CMgr* managers[3]; unsigned long managerCount; CMgr* cloth;
    void FrameStart(int shadowMode, bool havePool, float poolErr, float poolCharErr, long width, float zoom)
    {
        for (int i = 0; i < 8; ++i) Selected[i] = shadowMode == 2 ? &Colour[i] : &Depth[i];
        ++g_frame;
        for (unsigned long i = 0; i < managerCount; ++i) managers[i]->FrameStart();
        cloth->FrameStart();
        if (havePool) { LODErr = poolErr; CharLODErr = poolCharErr; }
        float w = (float)width;
        float screen = w < LODScreen ? w : LODScreen;
        MipmapMul = zoom * MipmapQuality * (unsigned long)screen;
        LODScreenFOV = zoom * LODScreen;
    }
};
int main()
{
    CMgr m;
    CRenderer r;
    r.managers[0] = r.managers[1] = r.managers[2] = &m; r.managerCount = 3; r.cloth = &m;
    r.LODScreen = 640.0f; r.MipmapQuality = 0.5f; r.LODErr = 1; r.CharLODErr = 2;
    r.FrameStart(2, true, 3.0f, 4.0f, 800, 2.0f);
    if (r.Selected[3] != &r.Colour[3] || g_frame != 8 || g_calls != 4 || r.LODErr != 3.0f || r.CharLODErr != 4.0f) { std::printf("00b32660_TEST FAIL\n"); return 1; }
    if (std::fabs(r.MipmapMul - 640.0f) > 1e-3f || std::fabs(r.LODScreenFOV - 1280.0f) > 1e-3f) { std::printf("00b32660_TEST FAIL\n"); return 1; }
    r.FrameStart(1, false, 9.0f, 9.0f, 320, 1.0f);
    if (r.Selected[0] != &r.Depth[0] || r.LODErr != 3.0f || std::fabs(r.MipmapMul - 160.0f) > 1e-3f) { std::printf("00b32660_TEST FAIL\n"); return 1; }
    std::printf("00b32660_TEST PASS\n");
    return 0;
}