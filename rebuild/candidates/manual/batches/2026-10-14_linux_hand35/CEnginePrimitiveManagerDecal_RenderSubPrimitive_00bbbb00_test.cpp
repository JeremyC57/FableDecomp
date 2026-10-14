#include <cstdio>
struct CEngineInternalPrimitiveDecal { unsigned char _pad[0x48]; bool m_landscape; };
struct CEngineSubPrimitiveBase { unsigned char _pad[0xc]; CEngineInternalPrimitiveDecal* m_pPrimitive; };
static CEngineInternalPrimitiveDecal* g_rendered = 0;
struct CEnginePrimitiveManagerDecal { void RenderLandscapeDecal(CEngineInternalPrimitiveDecal* d) { g_rendered = d; }
    void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, long) { CEngineInternalPrimitiveDecal* decal = sub.m_pPrimitive; if (decal->m_landscape) RenderLandscapeDecal(decal); } };
int main() {
    CEnginePrimitiveManagerDecal m; CEngineInternalPrimitiveDecal d; d.m_landscape = false; CEngineSubPrimitiveBase s; s.m_pPrimitive = &d;
    m.RenderSubPrimitive(s, 1); if (g_rendered) { std::printf("00bbbb00_TEST FAIL\n"); return 1; }
    d.m_landscape = true; m.RenderSubPrimitive(s, 1); if (g_rendered != &d) { std::printf("00bbbb00_TEST FAIL\n"); return 1; }
    std::printf("00bbbb00_TEST PASS\n"); return 0;
}
