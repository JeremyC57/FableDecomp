#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_block = 0; struct CLandscapeMemory { void FreeBlock(void* p) { std::strcat(g_seq, "B"); g_block = p; } }; struct SEngine { unsigned char _pad[0x6b0]; CLandscapeMemory* m_pMem; }; static CLandscapeMemory g_mem; static SEngine g_engine; static SEngine* g_pEngine = &g_engine;
struct COwner { unsigned char _pad[0x38]; void* m_pBlock; void ClearBlock() { if (m_pBlock) { g_pEngine->m_pMem->FreeBlock(m_pBlock); m_pBlock = 0; } } };

int main() {
    g_engine.m_pMem = &g_mem; COwner o; int block; o.m_pBlock = &block; o.ClearBlock(); if (std::strcmp(g_seq, "B") || g_block != &block || o.m_pBlock) { std::printf("00bfdd40_TEST FAIL\n"); return 1; } o.ClearBlock(); if (std::strcmp(g_seq, "B")) { std::printf("00bfdd40_TEST FAIL\n"); return 1; }
    std::printf("00bfdd40_TEST PASS\n"); return 0;
}
