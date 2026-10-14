#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CEngineSubPrimitive_00b842d0; static int g_n = 0; static CEngineSubPrimitive_00b842d0* g_subs[4]; static void* g_params = 0;
struct CSubPrimitiveManager_00b842d0 { virtual void RenderSubPrimitive(CEngineSubPrimitive_00b842d0* s, void* p) { g_subs[g_n++] = s; g_params = p; } };
struct CEngineSubPrimitive_00b842d0 { unsigned char _pad[0x10]; CEngineSubPrimitive_00b842d0* m_pNext; CSubPrimitiveManager_00b842d0* m_pManager; };
struct SRenderEntry_00b842d0 { long m_key; CEngineSubPrimitive_00b842d0* m_pFirst; };
struct CRetailEntryVector_00b842d0 { SRenderEntry_00b842d0* m_pFirst; SRenderEntry_00b842d0* m_pLast; SRenderEntry_00b842d0* m_pEnd; unsigned long GetCount() const { return m_pLast - m_pFirst; } };
struct CEngineSubPrimitiveRenderer { static void RenderList(CRetailEntryVector_00b842d0* list, void* params) { unsigned long count = list->GetCount(); for (unsigned long i = 0; i < count; ++i) { CEngineSubPrimitive_00b842d0* sub = list->m_pFirst[i].m_pFirst; do { sub->m_pManager->RenderSubPrimitive(sub, params); sub = sub->m_pNext; } while (sub); } } };
int main() {
    CSubPrimitiveManager_00b842d0 mgr; CEngineSubPrimitive_00b842d0 s1, s2, s3; s1.m_pManager = &mgr; s2.m_pManager = &mgr; s3.m_pManager = &mgr; s1.m_pNext = &s2; s2.m_pNext = 0; s3.m_pNext = 0;
    SRenderEntry_00b842d0 es[2] = { { 0, &s1 }, { 0, &s3 } }; CRetailEntryVector_00b842d0 v = { es, es + 2, es + 2 }; int params; CEngineSubPrimitiveRenderer::RenderList(&v, &params);
    if (g_n != 3 || g_subs[0] != &s1 || g_subs[1] != &s2 || g_subs[2] != &s3 || g_params != &params) { std::printf("00b842d0_TEST FAIL\n"); return 1; }
    v.m_pLast = es; CEngineSubPrimitiveRenderer::RenderList(&v, &params); if (g_n != 3) { std::printf("00b842d0_TEST FAIL\n"); return 1; }
    std::printf("00b842d0_TEST PASS\n"); return 0;
}
