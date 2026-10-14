#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CEngineInternalPrimitiveBase;
static int g_rendered = 0; static CEngineInternalPrimitiveBase* g_last = 0;
struct CEnginePrimitiveManagerBase_00b917e0 { virtual void RenderPrimitive(CEngineInternalPrimitiveBase* p) { ++g_rendered; g_last = p; } };
struct CEngine_01436e84 { unsigned char _pad[0x10]; CEnginePrimitiveManagerBase_00b917e0* m_managers[1]; }; static CEngine_01436e84* g_pEngine_01436e84;
struct CEngineInternalPrimitiveBase { unsigned char _pad0[8]; long m_type; unsigned char _pad0c[0x24]; unsigned long RenderLayerMask; unsigned char _pad34[4]; CEngineInternalPrimitiveBase* m_pNextRendered; unsigned char _pad3c[4]; CEngineInternalPrimitiveBase* m_pNextSkipped; };
struct CEngineScene { void RenderPrimitiveList(CEngineInternalPrimitiveBase* first, unsigned long mask, long, long) { CEngineInternalPrimitiveBase* p = first; while (p) { if (p->RenderLayerMask & mask) { g_pEngine_01436e84->m_managers[p->m_type]->RenderPrimitive(p); p = p->m_pNextRendered; } else { p = p->m_pNextSkipped; } } } };
int main() {
    CEnginePrimitiveManagerBase_00b917e0 mgr; CEngine_01436e84 eng; eng.m_managers[0] = &mgr; g_pEngine_01436e84 = &eng; CEngineInternalPrimitiveBase p[3]; std::memset(p, 0, sizeof p);
    p[0].RenderLayerMask = 1; p[0].m_pNextRendered = &p[1]; p[1].RenderLayerMask = 2; p[1].m_pNextSkipped = &p[2]; p[2].RenderLayerMask = 1;
    CEngineScene s; s.RenderPrimitiveList(&p[0], 1, 0, 0); if (g_rendered != 2 || g_last != &p[2]) { std::printf("00b917e0_TEST FAIL\n"); return 1; }
    s.RenderPrimitiveList(0, 1, 0, 0); if (g_rendered != 2) { std::printf("00b917e0_TEST FAIL\n"); return 1; }
    std::printf("00b917e0_TEST PASS\n"); return 0;
}
