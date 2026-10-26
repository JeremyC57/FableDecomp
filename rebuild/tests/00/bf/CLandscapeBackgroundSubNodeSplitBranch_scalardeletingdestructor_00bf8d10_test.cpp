#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_freed = 0; struct CLandscapeMemory { void FreeVirtual(void* p) { std::strcat(g_seq, "F"); g_freed = p; } }; struct SEngine { unsigned char _pad[0x6b0]; CLandscapeMemory* m_pMem; }; static CLandscapeMemory g_mem; static SEngine g_engine; static SEngine* g_pEngine = &g_engine;
class CMovableResource { public: virtual void v00() { } ~CMovableResource() { } }; struct CTreeNode { unsigned char _body[0xe8]; ~CTreeNode() { std::strcat(g_seq, "T"); } };
struct CBranch : CMovableResource { CTreeNode m_nodeA; CTreeNode m_nodeB; ~CBranch() { } static void operator delete(void* p) { g_pEngine->m_pMem->FreeVirtual(p); } static void* operator new(size_t n) { return std::malloc(n); } };

int main() {
    g_engine.m_pMem = &g_mem; CBranch* p = new CBranch; delete p; if (std::strcmp(g_seq, "TTF") || g_freed != p) { std::printf("00bf8d10_TEST FAIL\n"); return 1; }
    std::printf("00bf8d10_TEST PASS\n"); return 0;
}