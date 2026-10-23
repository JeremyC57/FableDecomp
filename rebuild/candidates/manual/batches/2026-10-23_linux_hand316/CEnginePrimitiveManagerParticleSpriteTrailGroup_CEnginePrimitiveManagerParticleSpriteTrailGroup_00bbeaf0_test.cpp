#include <cstdio>
#include <cstring>
// Model of the sprite-trail group manager constructor: the trail manager counted pointer is copied (one more
// reference), then the point-light vertex shader vector is resized to 5 and filled, and the displacement vertex
// shader and the sprite-group pixel shaders are looked up.
struct Shader { const char* name; char kind; };
static const char* g_names[32]; static char g_kinds[32]; static int g_n = 0;
static int g_built = 0; static const char* g_enable = 0;
static Shader Lookup(char kind, const char* name)
{
    Shader s; s.name = name; s.kind = kind;
    g_names[g_n] = name; g_kinds[g_n] = kind; ++g_n;
    return s;
}
static Shader GetVertexShader(const char* name) { return Lookup('V', name); }
static Shader GetPixelShader(const char* name) { return Lookup('P', name); }
static void RegisterEnable(const char* name) { g_enable = name; }
static bool Is(const Shader& s, char kind, const char* name) { return s.kind == kind && s.name && std::strcmp(s.name, name) == 0; }
struct Manager {
    Shader VertexShaderDisplacement;
    Shader PixelShader;
    Shader PixelShaderStipple;
    Shader PixelShaderDisplacement;
    void* TrailData;
    long* TrailRefCount;
    Shader* vecStart; Shader* vecFinish;
    void resize(int n) { vecStart = new Shader[n]; vecFinish = vecStart + n; for (int i = 0; i < n; ++i) { vecStart[i].name = 0; vecStart[i].kind = 0; } }
    ~Manager() { delete[] vecStart; }
    Manager(void* data, long* refCount)
    {
        TrailData = data; TrailRefCount = refCount; if (TrailRefCount) ++*TrailRefCount;
        resize(5);
        vecStart[0] = GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_FOG");
        vecStart[1] = GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
        vecStart[2] = GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
        vecStart[3] = GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
        vecStart[4] = GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_5POINTLIGHTS_FOG");
        VertexShaderDisplacement = GetVertexShader("VSHADER_SPRITE_TRAIL_GROUP_DISPLACEMENT");
        PixelShader = GetPixelShader("PSHADER_SPRITE_GROUP");
        PixelShaderStipple = GetPixelShader("PSHADER_SPRITE_GROUP_STIPPLE");
        PixelShaderDisplacement = GetPixelShader("PSHADER_SPRITE_GROUP_DISPLACEMENT");
    }
};
int main()
{
    int bad = 0;
    int trails = 0; long refs = 1;
    Manager m(&trails, &refs);
    static const char* expected[9] = { "VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_FOG", "VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG", "VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG", "VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG", "VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_5POINTLIGHTS_FOG", "VSHADER_SPRITE_TRAIL_GROUP_DISPLACEMENT", "PSHADER_SPRITE_GROUP", "PSHADER_SPRITE_GROUP_STIPPLE", "PSHADER_SPRITE_GROUP_DISPLACEMENT" };
    if (g_n != 9) bad = 1;
    for (int i = 0; i < g_n && i < 9; ++i) if (std::strcmp(g_names[i], expected[i]) != 0) bad = 1;
    if (!Is(m.vecStart[0], 'V', "VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_FOG")) bad = 1;
    if (!Is(m.vecStart[1], 'V', "VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG")) bad = 1;
    if (!Is(m.vecStart[2], 'V', "VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG")) bad = 1;
    if (!Is(m.vecStart[3], 'V', "VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_3POINTLIGHTS_FOG")) bad = 1;
    if (!Is(m.vecStart[4], 'V', "VSHADER_SPRITE_TRAIL_GROUP_DIRLIGHT_5POINTLIGHTS_FOG")) bad = 1;
    if (!Is(m.VertexShaderDisplacement, 'V', "VSHADER_SPRITE_TRAIL_GROUP_DISPLACEMENT")) bad = 1;
    if (!Is(m.PixelShader, 'P', "PSHADER_SPRITE_GROUP")) bad = 1;
    if (!Is(m.PixelShaderStipple, 'P', "PSHADER_SPRITE_GROUP_STIPPLE")) bad = 1;
    if (!Is(m.PixelShaderDisplacement, 'P', "PSHADER_SPRITE_GROUP_DISPLACEMENT")) bad = 1;
    if (g_built != 0) bad = 1;
    if (m.vecFinish - m.vecStart != 5) bad = 1;
    if (m.TrailData != &trails || refs != 2) bad = 1;
    if (bad) { std::printf("00bbeaf0_TEST FAIL\n"); return 1; }
    std::printf("00bbeaf0_TEST PASS\n");
    return 0;
}
