// behaviour test for 00b36ce0 CEnginePrimitiveRendererSharedData ctor: base, interpolation info, then 40 pixel shaders in order
#include <cstdio>
static int g_order[64];
static int g_n = 0;
struct CBase { CBase() { g_order[g_n++] = 1; } };
class CEngineInterpolationInfo { public: long body[28]; CEngineInterpolationInfo() { g_order[g_n++] = 2; } };
class CPixelShader { public: void* p; void* q; CPixelShader() : p(0), q(0) { g_order[g_n++] = 3; } };
struct CEnginePrimitiveRendererSharedData : CBase {
    bool BlockOnLoad;
    CEngineInterpolationInfo CurrentInterpolationInfo;
    long Scalars[25];
    CPixelShader Shaders[40];
    CPixelShader* Pointers[8];
    CEnginePrimitiveRendererSharedData() {}
};
int main()
{
    CEnginePrimitiveRendererSharedData* d = new CEnginePrimitiveRendererSharedData;
    if (g_n != 42 || g_order[0] != 1 || g_order[1] != 2) { std::printf("00b36ce0_TEST FAIL\n"); return 1; }
    for (int i = 2; i < 42; ++i)
        if (g_order[i] != 3) { std::printf("00b36ce0_TEST FAIL\n"); return 1; }
    if (d->Shaders[39].p != 0) { std::printf("00b36ce0_TEST FAIL\n"); return 1; }
    delete d;
    std::printf("00b36ce0_TEST PASS\n");
    return 0;
}