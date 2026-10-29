// Behaviour test for the implicit ~CVertexShaderConstantLayout (0x0098D710): the reserved-presets storage is freed, then
// every CShaderConstantRange member (incl. the four texture transforms) is destroyed, then the base.
#include <stdio.h>
#include <stdlib.h>
static int g_rangeDtors = 0, g_baseDtors = 0, g_frees = 0;
class CBaseClass { public: virtual ~CBaseClass() { g_baseDtors++; } };
class CShaderConstantRange : public CBaseClass { public:
    long Offset, Count;
    ~CShaderConstantRange() { g_rangeDtors++; }
};
class CVertexShaderConstant { public: float X, Y, Z, W; };
class CPresetArray { public:
    CVertexShaderConstant* Start;
    CPresetArray() : Start(0) { }
    ~CPresetArray() { if (Start != 0) { free(Start); g_frees++; } }
};
class CVertexShaderConstantLayout : public CBaseClass { public:
    CShaderConstantRange Reserved, User, Compression, Ambient, FogColour, CameraPos, LightArray, LightGlobals,
        LightAttenuations, CombinedProjectionMatrix, FogTransform, WorldMatrix, SphereMapMatrix, TextureTransform[4],
        BoneMatrices, ShadowTextureTransform, ShadowFadeTransform, ShadowFadeFactors, ShadowedSpotlightFalloffTransform,
        ShadowedSpotlightAttenuation, ShadowedSpotlightColour, ScreenSpaceTransformScale, ScreenSpaceTransformOffset;
    unsigned long LightSize;                         // a plain count, not a range
    CPresetArray ReservedPresets;
};
int main()
{
    {
        CVertexShaderConstantLayout layout;
        layout.ReservedPresets.Start = (CVertexShaderConstant*)malloc(4 * sizeof(CVertexShaderConstant));
    }
    // 26 ranges (22 single + 4 texture transforms), each a CBaseClass, plus the layout's own base
    if (g_frees != 1 || g_rangeDtors != 26 || g_baseDtors != 27) { printf("0098d710_TEST FAIL\n"); return 1; }
    {
        CVertexShaderConstantLayout empty;               // no presets: nothing freed
    }
    if (g_frees != 1 || g_rangeDtors != 52 || g_baseDtors != 54) { printf("0098d710_TEST FAIL\n"); return 1; }
    printf("0098d710_TEST PASS\n");
    return 0;
}