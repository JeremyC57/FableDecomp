#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_destroyed = 0; static void* g_deleted = 0; static void Destroy(void* o) { g_destroyed = o; std::strcat(g_seq, "D"); } static void DeleteBlock(void* p) { g_deleted = p; std::strcat(g_seq, "d"); }
struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
struct SRecord { long m_0; void* m_pObject; SCountBlock* m_pCount; SRecord& operator=(const SRecord& other) { m_0 = other.m_0; void* object = other.m_pObject; SCountBlock* count = other.m_pCount; if (m_pCount != count) { if (m_pCount) { --m_pCount->m_references; if (m_pCount->m_references == 0) { m_pCount->m_pfnDestroy(m_pCount->m_pObject); DeleteBlock(m_pCount); } } m_pObject = object; m_pCount = count; if (count) ++count->m_references; } return *this; } };

int main() {
    SCountBlock c1, c2; int o1, o2; c1.m_references = 1; c1.m_pfnDestroy = Destroy; c1.m_pObject = &o1; c2.m_references = 1; c2.m_pfnDestroy = Destroy; c2.m_pObject = &o2;
    SRecord a, b; a.m_0 = 1; a.m_pObject = &o1; a.m_pCount = &c1; b.m_0 = 2; b.m_pObject = &o2; b.m_pCount = &c2; a = b; if (a.m_0 != 2 || a.m_pObject != &o2 || a.m_pCount != &c2 || c2.m_references != 2 || std::strcmp(g_seq, "Dd") || g_destroyed != &o1 || g_deleted != &c1) { std::printf("00a7faf0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; a = b; if (g_seq[0] || c2.m_references != 2) { std::printf("00a7faf0_TEST FAIL\n"); return 1; } SRecord e; e.m_0 = 0; e.m_pObject = 0; e.m_pCount = 0; a = e; if (a.m_pCount || a.m_pObject || c2.m_references != 1 || g_seq[0]) { std::printf("00a7faf0_TEST FAIL\n"); return 1; }
    std::printf("00a7faf0_TEST PASS\n"); return 0;
}