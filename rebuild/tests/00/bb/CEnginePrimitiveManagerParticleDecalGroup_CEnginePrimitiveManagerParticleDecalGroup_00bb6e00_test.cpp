#include <cstdio>
#include <cstring>
// Model of the particle decal group manager constructor: the enable switch, the point-light shader index table
// {0,1,1,1,2,2}, then the landscape/mesh decal vertex shaders (3 lighting variants each), the two-pass stencil and
// stencil-clear (occlusion box) vertex shaders and four pixel shaders looked up by name in declaration order.
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
    unsigned long PointLightShaderIndex[6];
    Shader LandscapeDecalVertexShader[3];
    Shader MeshDecalVertexShader[3];
    Shader MeshDecalTwoPassStencilVertexShader;
    Shader MeshDecalStencilClearVertexShader;
    Shader LandscapeDecalPixelShader;
    Shader MeshDecalPixelShader;
    Shader MeshDecalTwoPassStencilPixelShader;
    Shader MeshDecalStencilClearPixelShader;
    Manager()
    {
        RegisterEnable("EnableGroupDecals");
        PointLightShaderIndex[0] = 0; PointLightShaderIndex[1] = 1; PointLightShaderIndex[2] = 1;
        PointLightShaderIndex[3] = 1; PointLightShaderIndex[4] = 2; PointLightShaderIndex[5] = 2;
        LandscapeDecalVertexShader[0] = GetVertexShader("VSHADER_LANDSCAPE_DECAL_DIRLIGHT");
        LandscapeDecalVertexShader[1] = GetVertexShader("VSHADER_LANDSCAPE_DECAL_3POINTLIGHTS");
        LandscapeDecalVertexShader[2] = GetVertexShader("VSHADER_LANDSCAPE_DECAL_5POINTLIGHTS");
        MeshDecalVertexShader[0] = GetVertexShader("VSHADER_MESH_DECAL_DIRLIGHT");
        MeshDecalVertexShader[1] = GetVertexShader("VSHADER_MESH_DECAL_3POINTLIGHTS");
        MeshDecalVertexShader[2] = GetVertexShader("VSHADER_MESH_DECAL_5POINTLIGHTS");
        MeshDecalTwoPassStencilVertexShader = GetVertexShader("VSHADER_MESH_DECAL_STENCIL_TWO_PASS");
        MeshDecalStencilClearVertexShader = GetVertexShader("VSHADER_OCCLUSION_BOX");
        LandscapeDecalPixelShader = GetPixelShader("PSHADER_DECAL");
        MeshDecalPixelShader = GetPixelShader("PSHADER_DECAL");
        MeshDecalTwoPassStencilPixelShader = GetPixelShader("PSHADER_DECAL_STENCIL_TWO_PASS");
        MeshDecalStencilClearPixelShader = GetPixelShader("PSHADER_DECAL_STENCIL_TWO_PASS");
    }
};
int main()
{
    int bad = 0;
    Manager m;
    static const char* expected[12] = { "VSHADER_LANDSCAPE_DECAL_DIRLIGHT", "VSHADER_LANDSCAPE_DECAL_3POINTLIGHTS", "VSHADER_LANDSCAPE_DECAL_5POINTLIGHTS", "VSHADER_MESH_DECAL_DIRLIGHT", "VSHADER_MESH_DECAL_3POINTLIGHTS", "VSHADER_MESH_DECAL_5POINTLIGHTS", "VSHADER_MESH_DECAL_STENCIL_TWO_PASS", "VSHADER_OCCLUSION_BOX", "PSHADER_DECAL", "PSHADER_DECAL", "PSHADER_DECAL_STENCIL_TWO_PASS", "PSHADER_DECAL_STENCIL_TWO_PASS" };
    if (g_n != 12) bad = 1;
    for (int i = 0; i < g_n && i < 12; ++i) if (std::strcmp(g_names[i], expected[i]) != 0) bad = 1;
    if (!Is(m.LandscapeDecalVertexShader[0], 'V', "VSHADER_LANDSCAPE_DECAL_DIRLIGHT")) bad = 1;
    if (!Is(m.LandscapeDecalVertexShader[1], 'V', "VSHADER_LANDSCAPE_DECAL_3POINTLIGHTS")) bad = 1;
    if (!Is(m.LandscapeDecalVertexShader[2], 'V', "VSHADER_LANDSCAPE_DECAL_5POINTLIGHTS")) bad = 1;
    if (!Is(m.MeshDecalVertexShader[0], 'V', "VSHADER_MESH_DECAL_DIRLIGHT")) bad = 1;
    if (!Is(m.MeshDecalVertexShader[1], 'V', "VSHADER_MESH_DECAL_3POINTLIGHTS")) bad = 1;
    if (!Is(m.MeshDecalVertexShader[2], 'V', "VSHADER_MESH_DECAL_5POINTLIGHTS")) bad = 1;
    if (!Is(m.MeshDecalTwoPassStencilVertexShader, 'V', "VSHADER_MESH_DECAL_STENCIL_TWO_PASS")) bad = 1;
    if (!Is(m.MeshDecalStencilClearVertexShader, 'V', "VSHADER_OCCLUSION_BOX")) bad = 1;
    if (!Is(m.LandscapeDecalPixelShader, 'P', "PSHADER_DECAL")) bad = 1;
    if (!Is(m.MeshDecalPixelShader, 'P', "PSHADER_DECAL")) bad = 1;
    if (!Is(m.MeshDecalTwoPassStencilPixelShader, 'P', "PSHADER_DECAL_STENCIL_TWO_PASS")) bad = 1;
    if (!Is(m.MeshDecalStencilClearPixelShader, 'P', "PSHADER_DECAL_STENCIL_TWO_PASS")) bad = 1;
    if (!g_enable || std::strcmp(g_enable, "EnableGroupDecals") != 0) bad = 1;
    if (g_built != 0) bad = 1;
    { static const unsigned long idx[6] = { 0, 1, 1, 1, 2, 2 }; for (int i = 0; i < 6; ++i) if (m.PointLightShaderIndex[i] != idx[i]) bad = 1; }
    if (bad) { std::printf("00bb6e00_TEST FAIL\n"); return 1; }
    std::printf("00bb6e00_TEST PASS\n");
    return 0;
}