// behaviour test for 00bce900 (standalone copy of the model)
#include <cstdio>

class CVertexShader { public: int id; };
static const CVertexShader* g_applied = 0; static bool g_force = false;
class CShaderRenderManager { public: void ApplyVertexShader(const CVertexShader& s, bool f) { g_applied = &s; g_force = f; } };
static CShaderRenderManager g_mgr; static CShaderRenderManager* g_pShaderRenderManager = &g_mgr;
struct M { unsigned long PointLightShaderIndex[6]; CVertexShader* VertexShadersPalSkinSpotShadow;
    void Apply(unsigned long n, unsigned long) { if (n > 5) n = 5; g_pShaderRenderManager->ApplyVertexShader(VertexShadersPalSkinSpotShadow[PointLightShaderIndex[n]], true); } };

int main()
{
    CVertexShader sh[4]; M m; m.VertexShadersPalSkinSpotShadow = sh;
    for (int i = 0; i < 6; i++) m.PointLightShaderIndex[i] = (unsigned long)(i % 4);
    m.Apply(2, 0);
    if (!(g_applied == &sh[2] && g_force)) { std::printf("00bce900_TEST FAIL\n"); return 1; }
    m.Apply(9, 0);
    if (!(g_applied == &sh[1])) { std::printf("00bce900_TEST FAIL\n"); return 1; }
    std::printf("00bce900_TEST PASS\n");
    return 0;
}