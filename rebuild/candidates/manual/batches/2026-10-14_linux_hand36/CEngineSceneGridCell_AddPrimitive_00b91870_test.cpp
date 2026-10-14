#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_inv = 0; static void* g_list = 0;
struct CSceneGridOwner_00b91710 { void InvalidateAll() { ++g_inv; } };
struct CEngineSceneGridCell;
struct CEngineInternalPrimitiveBase { unsigned char _pad[0x20]; CEngineSceneGridCell* m_pCell; void AddToList(void* l) { g_list = l; } };
struct CEngineSceneGridCell { CSceneGridOwner_00b91710* m_pOwner; unsigned char m_list[0x30]; long m_count; unsigned char _pad38[0xc]; unsigned char m_dirtyFlags;
    void AddPrimitive(CEngineInternalPrimitiveBase* p) { p->m_pCell = this; p->AddToList(m_list); ++m_count; if (m_dirtyFlags & 7) { m_dirtyFlags &= ~7; if (m_pOwner) m_pOwner->InvalidateAll(); } } };
int main() {
    CEngineSceneGridCell c; CSceneGridOwner_00b91710 o; std::memset(&c, 0, sizeof c); c.m_pOwner = &o; CEngineInternalPrimitiveBase p; c.AddPrimitive(&p);
    if (p.m_pCell != &c || g_list != c.m_list || c.m_count != 1 || g_inv) { std::printf("00b91870_TEST FAIL\n"); return 1; }
    c.m_dirtyFlags = 0x0b; c.AddPrimitive(&p); if (c.m_count != 2 || c.m_dirtyFlags != 8 || g_inv != 1) { std::printf("00b91870_TEST FAIL\n"); return 1; }
    c.m_dirtyFlags = 1; c.m_pOwner = 0; c.AddPrimitive(&p); if (c.m_dirtyFlags != 0 || g_inv != 1) { std::printf("00b91870_TEST FAIL\n"); return 1; }
    std::printf("00b91870_TEST PASS\n"); return 0;
}
