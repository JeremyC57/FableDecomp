#include <cstdio>
#include <cstring>
// Model of the particle mesh group manager constructor: the point-light vertex shader vector is resized to 5 and
// filled, then the displacement vertex shader and five mesh-group pixel shaders are looked up (no buffer build).
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
    Shader PixelShaderAdditiveAlpha;
    Shader PixelShaderAdditiveAlphaStipple;
    Shader PixelShaderDisplacement;
    Shader* vecStart; Shader* vecFinish;
    void resize(int n) { vecStart = new Shader[n]; vecFinish = vecStart + n; for (int i = 0; i < n; ++i) { vecStart[i].name = 0; vecStart[i].kind = 0; } }
    ~Manager() { delete[] vecStart; }
    Manager()
    {
        resize(5);
        vecStart[0] = GetVertexShader("VSHADER_MESH_GROUP_DIRLIGHT_FOG");
        vecStart[1] = GetVertexShader("VSHADER_MESH_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
        vecStart[2] = GetVertexShader("VSHADER_MESH_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
        vecStart[3] = GetVertexShader("VSHADER_MESH_GROUP_DIRLIGHT_3POINTLIGHTS_FOG");
        vecStart[4] = GetVertexShader("VSHADER_MESH_GROUP_DIRLIGHT_5POINTLIGHTS_FOG");
        VertexShaderDisplacement = GetVertexShader("VSHADER_MESH_GROUP_DISPLACEMENT");
        PixelShader = GetPixelShader("PSHADER_MESH_GROUP");
        PixelShaderStipple = GetPixelShader("PSHADER_MESH_GROUP_STIPPLE");
        PixelShaderAdditiveAlpha = GetPixelShader("PSHADER_MESH_GROUP_ADDITIVE_ALPHA");
        PixelShaderAdditiveAlphaStipple = GetPixelShader("PSHADER_MESH_GROUP_ADDITIVE_ALPHA_STIPPLE");
        PixelShaderDisplacement = GetPixelShader("PSHADER_MESH_GROUP_DISPLACEMENT");
    }
};
int main()
{
    int bad = 0;
    Manager m;
    static const char* expected[11] = { "VSHADER_MESH_GROUP_DIRLIGHT_FOG", "VSHADER_MESH_GROUP_DIRLIGHT_3POINTLIGHTS_FOG", "VSHADER_MESH_GROUP_DIRLIGHT_3POINTLIGHTS_FOG", "VSHADER_MESH_GROUP_DIRLIGHT_3POINTLIGHTS_FOG", "VSHADER_MESH_GROUP_DIRLIGHT_5POINTLIGHTS_FOG", "VSHADER_MESH_GROUP_DISPLACEMENT", "PSHADER_MESH_GROUP", "PSHADER_MESH_GROUP_STIPPLE", "PSHADER_MESH_GROUP_ADDITIVE_ALPHA", "PSHADER_MESH_GROUP_ADDITIVE_ALPHA_STIPPLE", "PSHADER_MESH_GROUP_DISPLACEMENT" };
    if (g_n != 11) bad = 1;
    for (int i = 0; i < g_n && i < 11; ++i) if (std::strcmp(g_names[i], expected[i]) != 0) bad = 1;
    if (!Is(m.vecStart[0], 'V', "VSHADER_MESH_GROUP_DIRLIGHT_FOG")) bad = 1;
    if (!Is(m.vecStart[1], 'V', "VSHADER_MESH_GROUP_DIRLIGHT_3POINTLIGHTS_FOG")) bad = 1;
    if (!Is(m.vecStart[2], 'V', "VSHADER_MESH_GROUP_DIRLIGHT_3POINTLIGHTS_FOG")) bad = 1;
    if (!Is(m.vecStart[3], 'V', "VSHADER_MESH_GROUP_DIRLIGHT_3POINTLIGHTS_FOG")) bad = 1;
    if (!Is(m.vecStart[4], 'V', "VSHADER_MESH_GROUP_DIRLIGHT_5POINTLIGHTS_FOG")) bad = 1;
    if (!Is(m.VertexShaderDisplacement, 'V', "VSHADER_MESH_GROUP_DISPLACEMENT")) bad = 1;
    if (!Is(m.PixelShader, 'P', "PSHADER_MESH_GROUP")) bad = 1;
    if (!Is(m.PixelShaderStipple, 'P', "PSHADER_MESH_GROUP_STIPPLE")) bad = 1;
    if (!Is(m.PixelShaderAdditiveAlpha, 'P', "PSHADER_MESH_GROUP_ADDITIVE_ALPHA")) bad = 1;
    if (!Is(m.PixelShaderAdditiveAlphaStipple, 'P', "PSHADER_MESH_GROUP_ADDITIVE_ALPHA_STIPPLE")) bad = 1;
    if (!Is(m.PixelShaderDisplacement, 'P', "PSHADER_MESH_GROUP_DISPLACEMENT")) bad = 1;
    if (g_built != 0) bad = 1;
    if (m.vecFinish - m.vecStart != 5) bad = 1;
    if (bad) { std::printf("00bc34d0_TEST FAIL\n"); return 1; }
    std::printf("00bc34d0_TEST PASS\n");
    return 0;
}