// Behaviour test for CVertexShaderConstantLayout ctor (0x0098D490): every range empty, LightSize 0, presets empty.
#include <cstdio>
#include <cstring>
#include <new>
static int g_bases = 0;
struct CBase { CBase() { ++g_bases; } virtual ~CBase() {} };
struct CShaderConstantRange : CBase { long Offset; unsigned long Count; CShaderConstantRange() : Offset(0), Count(0) {} };
struct CVertexShaderConstantArray { void* Data; long Count; long Allocated; CVertexShaderConstantArray() : Data(0), Count(0), Allocated(0) {} };
struct CVertexShaderConstantLayout : CBase {
    CShaderConstantRange Reserved, User, Compression, Ambient, FogColour, CameraPos;
    unsigned long LightSize;
    CShaderConstantRange LightArray, LightGlobals, LightAttenuations, CombinedProjectionMatrix, FogTransform, WorldMatrix, SphereMapMatrix;
    CShaderConstantRange TextureTransform[4];
    CShaderConstantRange BoneMatrices, ShadowTextureTransform, ShadowFadeTransform, ShadowFadeFactors,
        ShadowedSpotlightFalloffTransform, ShadowedSpotlightAttenuation, ShadowedSpotlightColour,
        ScreenSpaceTransformScale, ScreenSpaceTransformOffset;
    CVertexShaderConstantArray ReservedPresets;
    CVertexShaderConstantLayout() : LightSize(0) {}
};
int main()
{
    static unsigned char storage[sizeof(CVertexShaderConstantLayout) + 16];
    std::memset(storage, 0xAB, sizeof(storage));
    CVertexShaderConstantLayout* l = new (storage) CVertexShaderConstantLayout;
    int ok = g_bases == 1 + 6 + 7 + 4 + 9;
    ok = ok && l->LightSize == 0 && l->Reserved.Offset == 0 && l->TextureTransform[3].Count == 0;
    ok = ok && l->ScreenSpaceTransformOffset.Offset == 0 && l->ReservedPresets.Data == 0 && l->ReservedPresets.Allocated == 0;
    if (!ok) { std::printf("0098d490_TEST FAIL\n"); return 1; }
    std::printf("0098d490_TEST PASS\n");
    return 0;
}