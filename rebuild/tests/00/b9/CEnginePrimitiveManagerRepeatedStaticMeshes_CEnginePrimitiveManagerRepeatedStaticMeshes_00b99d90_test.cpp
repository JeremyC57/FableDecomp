#include <cstdio>
#include <cstring>
// Model of the repeated static mesh manager constructor: the enable switch, four pixel/vertex shader pairs, the
// colour-buffer shadow pairs only when the texture memory from the display quality is >= 1400, the shadowed
// shader pointers aimed at the depth-buffer pairs and a 0xc00-byte 16-aligned SIMD variable block.
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
static long g_allocSize = 0, g_allocAlign = 0;
struct Manager {
    Shader PixelShader;
    Shader VertexShader;
    Shader PixelShaderStippleAlpha;
    Shader VertexShaderStippleAlpha;
    Shader PixelShaderDepthBufferShadowed;
    Shader VertexShaderDepthBufferShadowed;
    Shader PixelShaderDepthBufferShadowedStippleAlpha;
    Shader VertexShaderDepthBufferShadowedStippleAlpha;
    Shader PixelShaderColourBufferShadowed;
    Shader VertexShaderColourBufferShadowed;
    Shader PixelShaderColourBufferShadowedStippleAlpha;
    Shader VertexShaderColourBufferShadowedStippleAlpha;
    Shader* PixelShaderShadowed;
    Shader* VertexShaderShadowed;
    Shader* PixelShaderShadowedStippleAlpha;
    Shader* VertexShaderShadowedStippleAlpha;
    Manager(long textureMemory)
    {
        RegisterEnable("EnableRepeatedMeshes");
        PixelShaderColourBufferShadowed.name = 0; PixelShaderColourBufferShadowed.kind = 0;
        VertexShaderColourBufferShadowed.name = 0; VertexShaderColourBufferShadowed.kind = 0;
        PixelShaderColourBufferShadowedStippleAlpha.name = 0; PixelShaderColourBufferShadowedStippleAlpha.kind = 0;
        VertexShaderColourBufferShadowedStippleAlpha.name = 0; VertexShaderColourBufferShadowedStippleAlpha.kind = 0;
        PixelShader = GetPixelShader("PSHADER_REPEATED_MESH");
        VertexShader = GetVertexShader("VSHADER_REPEATED_MESH");
        PixelShaderStippleAlpha = GetPixelShader("PSHADER_REPEATED_MESH_STIPPLE_ALPHA");
        VertexShaderStippleAlpha = GetVertexShader("VSHADER_REPEATED_MESH_STIPPLE_ALPHA");
        PixelShaderDepthBufferShadowed = GetPixelShader("PSHADER_REPEATED_MESH_SHADOWED");
        VertexShaderDepthBufferShadowed = GetVertexShader("VSHADER_REPEATED_MESH_SHADOWED");
        PixelShaderDepthBufferShadowedStippleAlpha = GetPixelShader("PSHADER_REPEATED_MESH_SHADOWED_STIPPLE_ALPHA");
        VertexShaderDepthBufferShadowedStippleAlpha = GetVertexShader("VSHADER_REPEATED_MESH_SHADOWED_STIPPLE_ALPHA");
        if (textureMemory >= 1400) {
            PixelShaderColourBufferShadowed = GetPixelShader("PSHADER_REPEATED_MESH_COLBUFF_SHADOWED");
            VertexShaderColourBufferShadowed = GetVertexShader("VSHADER_REPEATED_MESH_COLBUFF_SHADOWED");
            PixelShaderColourBufferShadowedStippleAlpha = GetPixelShader("PSHADER_REPEATED_MESH_COLBUFF_SHADOWED_STIPPLE_ALPHA");
            VertexShaderColourBufferShadowedStippleAlpha = GetVertexShader("VSHADER_REPEATED_MESH_COLBUFF_SHADOWED_STIPPLE_ALPHA");
        }
        PixelShaderShadowed = &PixelShaderDepthBufferShadowed; VertexShaderShadowed = &VertexShaderDepthBufferShadowed;
        PixelShaderShadowedStippleAlpha = &PixelShaderDepthBufferShadowedStippleAlpha; VertexShaderShadowedStippleAlpha = &VertexShaderDepthBufferShadowedStippleAlpha;
        g_allocSize = 0xc00; g_allocAlign = 0x10; ++g_built;
    }
};
int main()
{
    int bad = 0;
    Manager low(1399);
    if (g_n != 8) bad = 1;
    g_n = 0;
    Manager high(1400);
    if (g_n != 12) bad = 1;
    g_n = 8;
    Manager& m = high;
    static const char* expected[8] = { "PSHADER_REPEATED_MESH", "VSHADER_REPEATED_MESH", "PSHADER_REPEATED_MESH_STIPPLE_ALPHA", "VSHADER_REPEATED_MESH_STIPPLE_ALPHA", "PSHADER_REPEATED_MESH_SHADOWED", "VSHADER_REPEATED_MESH_SHADOWED", "PSHADER_REPEATED_MESH_SHADOWED_STIPPLE_ALPHA", "VSHADER_REPEATED_MESH_SHADOWED_STIPPLE_ALPHA" };
    if (g_n != 8) bad = 1;
    for (int i = 0; i < g_n && i < 8; ++i) if (std::strcmp(g_names[i], expected[i]) != 0) bad = 1;
    if (!Is(m.PixelShader, 'P', "PSHADER_REPEATED_MESH")) bad = 1;
    if (!Is(m.VertexShader, 'V', "VSHADER_REPEATED_MESH")) bad = 1;
    if (!Is(m.PixelShaderStippleAlpha, 'P', "PSHADER_REPEATED_MESH_STIPPLE_ALPHA")) bad = 1;
    if (!Is(m.VertexShaderStippleAlpha, 'V', "VSHADER_REPEATED_MESH_STIPPLE_ALPHA")) bad = 1;
    if (!Is(m.PixelShaderDepthBufferShadowed, 'P', "PSHADER_REPEATED_MESH_SHADOWED")) bad = 1;
    if (!Is(m.VertexShaderDepthBufferShadowed, 'V', "VSHADER_REPEATED_MESH_SHADOWED")) bad = 1;
    if (!Is(m.PixelShaderDepthBufferShadowedStippleAlpha, 'P', "PSHADER_REPEATED_MESH_SHADOWED_STIPPLE_ALPHA")) bad = 1;
    if (!Is(m.VertexShaderDepthBufferShadowedStippleAlpha, 'V', "VSHADER_REPEATED_MESH_SHADOWED_STIPPLE_ALPHA")) bad = 1;
    if (!g_enable || std::strcmp(g_enable, "EnableRepeatedMeshes") != 0) bad = 1;
    if (g_built != 2) bad = 1;
    if (m.PixelShaderShadowed != &m.PixelShaderDepthBufferShadowed || m.VertexShaderShadowedStippleAlpha != &m.VertexShaderDepthBufferShadowedStippleAlpha) bad = 1;
    if (g_allocSize != 0xc00 || g_allocAlign != 0x10 || low.PixelShaderColourBufferShadowed.name != 0) bad = 1;
    if (!Is(high.PixelShaderColourBufferShadowed, 'P', "PSHADER_REPEATED_MESH_COLBUFF_SHADOWED")) bad = 1;
    if (!Is(high.VertexShaderColourBufferShadowed, 'V', "VSHADER_REPEATED_MESH_COLBUFF_SHADOWED")) bad = 1;
    if (!Is(high.PixelShaderColourBufferShadowedStippleAlpha, 'P', "PSHADER_REPEATED_MESH_COLBUFF_SHADOWED_STIPPLE_ALPHA")) bad = 1;
    if (!Is(high.VertexShaderColourBufferShadowedStippleAlpha, 'V', "VSHADER_REPEATED_MESH_COLBUFF_SHADOWED_STIPPLE_ALPHA")) bad = 1;
    if (bad) { std::printf("00b99d90_TEST FAIL\n"); return 1; }
    std::printf("00b99d90_TEST PASS\n");
    return 0;
}