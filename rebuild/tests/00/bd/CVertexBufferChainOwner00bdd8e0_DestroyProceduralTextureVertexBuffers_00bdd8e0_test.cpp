#include <cstdio>
struct SNode { unsigned char _pad[4]; SNode* m_pNext; };
struct COwner;
static int g_calls = 0; static SNode* g_nodes[4]; static const COwner* g_owner = 0;
struct CMM { void Dispose(SNode* n, COwner* o) { g_nodes[g_calls++] = n; g_owner = o; } };
class CEngine { public: unsigned char _pad[0x6b0]; CMM* m_pLandscapeMemoryManager; };
static CEngine g_engine; static CEngine* g_pEngine = &g_engine;
struct COwner { unsigned char _pad[8]; SNode* m_pHead;
    void Destroy() { CMM* mm = g_pEngine->m_pLandscapeMemoryManager; for (SNode* n = m_pHead; n; n = n->m_pNext) mm->Dispose(n, this); } };
int main()
{
    CMM mm; g_engine.m_pLandscapeMemoryManager = &mm; SNode c = { {0}, 0 }, b = { {0}, &c }, a = { {0}, &b };
    COwner o; o.m_pHead = 0; o.Destroy(); if (g_calls != 0) { std::printf("00bdd8e0_TEST FAIL\n"); return 1; }
    o.m_pHead = &a; o.Destroy();
    if (g_calls != 3 || g_nodes[0] != &a || g_nodes[1] != &b || g_nodes[2] != &c || g_owner != &o) { std::printf("00bdd8e0_TEST FAIL\n"); return 1; }
    std::printf("00bdd8e0_TEST PASS\n"); return 0;
}