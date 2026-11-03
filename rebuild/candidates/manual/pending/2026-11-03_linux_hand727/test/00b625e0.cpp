#include <cstdio>
#include <cstring>
#include <cwchar>
// Model of the sky renderer constructor: cloud vectors zeroed, cloud settings copied from the environment def (min speed 0),
// thirteen shader lookups in retail order (star-field vertex shader twice; the cloud pixel shader is the SIMPLE one when
// InitFlags bit 0x100 is set), StarCount 0, the SKY def (index 1) taken only when the lookup finds one, the star paths, BuildBuffers.
struct Shader { const char* name; char kind; };
static const char* g_names[32]; static int g_n = 0; static int g_built = 0;
static Shader Lookup(char kind, const char* name) { Shader s; s.name = name; s.kind = kind; if (g_n < 32) g_names[g_n] = name; ++g_n; return s; }
static Shader V(const char* n) { return Lookup('V', n); }
static Shader P(const char* n) { return Lookup('P', n); }
struct Env { float SpeedMul, MaxSpeed, TexMul, TexOff, Height; };
static const void* g_skyDef = 0;
static bool GetDef(const char* name, long index, const void** out)
{
    if (index <= 0) return false;
    if (g_skyDef && std::strcmp(name, "SKY") == 0) { *out = g_skyDef; return true; }
    return false;
}
struct Sky {
    float CloudHeightOffset, CloudTextureOffset, CloudTextureMulitplier, CloudSpeedMultiplier, CloudMaxSpeed, CloudMinSpeed;
    Shader Cloud_VS, Sky_VS, Star_VS, Star_PS, Sprite_VS, Cloud_PS, Sky_PS, Sprite_PS, Band_VS, Band_PS, Screen_VS, Screen_PS;
    long StarCount; const wchar_t* StarBitmap; const wchar_t* StarVB;
    float CloudVector[2][2]; float CloudVectorHistory[2][2][2]; bool EnableClouds; const void* PSkyDef;
    Sky(const Env& e, long flags)
    {
        PSkyDef = 0; EnableClouds = true;
        for (int i = 0; i < 2; ++i) { for (int j = 0; j < 2; ++j) CloudVectorHistory[i][j][0] = CloudVectorHistory[i][j][1] = 0.0f; CloudVector[i][0] = CloudVector[i][1] = 0.0f; }
        CloudSpeedMultiplier = e.SpeedMul; CloudMaxSpeed = e.MaxSpeed; CloudMinSpeed = 0.0f;
        CloudTextureOffset = e.TexOff; CloudTextureMulitplier = e.TexMul; CloudHeightOffset = e.Height;
        Cloud_VS = V("VSHADER_INNER_SKY"); Sky_VS = V("VSHADER_OUTER_SKY");
        Star_VS = V("VSHADER_SKY_STAR_FIELD"); Star_VS = V("VSHADER_SKY_STAR_FIELD"); Star_PS = P("PSHADER_SKY_STAR_FIELD");
        Sprite_VS = V("VSHADER_SKY_SPRITE"); Sky_PS = P("PSHADER_OUTER_SKY");
        Cloud_PS = (flags & 0x100) ? P("PSHADER_INNER_SKY_SIMPLE") : P("PSHADER_INNER_SKY");
        Sprite_PS = P("PSHADER_SKY_SPRITE"); Band_VS = V("VSHADER_SKY_BASE_BAND"); Band_PS = P("PSHADER_DIFFUSE_ONLY");
        Screen_VS = V("VSHADER_SKY_SCREEN_SPACE_SPRITE"); Screen_PS = P("PSHADER_SKY_SPRITE");
        StarCount = 0; GetDef("SKY", 1, &PSkyDef);
        StarBitmap = L"Data\\Misc\\star_chart.tga"; StarVB = L"Data\\Misc\\stars.dat"; ++g_built;
    }
};
int main()
{
    int bad = 0; Env e = { 1.5f, 2.5f, 3.5f, 4.5f, 5.5f };
    Sky a(e, 0);
    static const char* exp[13] = { "VSHADER_INNER_SKY", "VSHADER_OUTER_SKY", "VSHADER_SKY_STAR_FIELD", "VSHADER_SKY_STAR_FIELD", "PSHADER_SKY_STAR_FIELD",
        "VSHADER_SKY_SPRITE", "PSHADER_OUTER_SKY", "PSHADER_INNER_SKY", "PSHADER_SKY_SPRITE", "VSHADER_SKY_BASE_BAND", "PSHADER_DIFFUSE_ONLY",
        "VSHADER_SKY_SCREEN_SPACE_SPRITE", "PSHADER_SKY_SPRITE" };
    if (g_n != 13) bad = 1;
    for (int i = 0; i < 13 && i < g_n; ++i) if (std::strcmp(g_names[i], exp[i]) != 0) bad = 1;
    if (a.CloudSpeedMultiplier != 1.5f || a.CloudMaxSpeed != 2.5f || a.CloudMinSpeed != 0.0f || a.CloudTextureMulitplier != 3.5f
        || a.CloudTextureOffset != 4.5f || a.CloudHeightOffset != 5.5f) bad = 1;
    if (a.PSkyDef != 0 || a.StarCount != 0 || !a.EnableClouds || g_built != 1) bad = 1;
    if (a.CloudVector[1][1] != 0.0f || a.CloudVectorHistory[1][1][1] != 0.0f) bad = 1;
    static int def = 7; g_skyDef = &def; g_n = 0;
    Sky b(e, 0x100);
    if (b.PSkyDef != &def || std::strcmp(g_names[7], "PSHADER_INNER_SKY_SIMPLE") != 0) bad = 1;
    if (std::wcscmp(b.StarBitmap, L"Data\\Misc\\star_chart.tga") != 0 || std::wcscmp(b.StarVB, L"Data\\Misc\\stars.dat") != 0) bad = 1;
    if (bad) { std::printf("00b625e0_TEST FAIL\n"); return 1; }
    std::printf("00b625e0_TEST PASS\n");
    return 0;
}
