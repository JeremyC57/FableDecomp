#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
struct CMovableResource { virtual void v00() { } }; static void* g_blocks[5]; static long g_n = 0; static CMovableResource* g_owner = 0;
struct CEngineLandscapeMemoryManager { void RebindBlockOwner(void* b, CMovableResource* o) { note("B"); g_blocks[g_n++] = b; g_owner = o; } void RebindBufferOwner(void* b, CMovableResource* o) { note("b"); g_blocks[g_n++] = b; g_owner = o; } };
struct SEngine { unsigned char _pad[0x6b0]; CEngineLandscapeMemoryManager* m_pLandscapeMemory; }; static CEngineLandscapeMemoryManager g_mem; static SEngine g_engine; static SEngine* g_pEngine = &g_engine;
struct CMesh : CMovableResource { unsigned char _pad04[0x40]; void* m_pBuffer44; void* m_pBlock48; void* m_pBuffer4c; void* m_pBlock50; void* m_pBuffer54;
    void OnPostRelocation() { CEngineLandscapeMemoryManager* manager = g_pEngine->m_pLandscapeMemory; manager->RebindBlockOwner(m_pBlock50, this); manager->RebindBufferOwner(m_pBuffer54, this); manager->RebindBufferOwner(m_pBuffer44, this); manager->RebindBlockOwner(m_pBlock48, this); manager->RebindBufferOwner(m_pBuffer4c, this); } };

int main()
{
    g_engine.m_pLandscapeMemory = &g_mem; CMesh m; int b44, b48, b4c, b50, b54; m.m_pBuffer44 = &b44; m.m_pBlock48 = &b48; m.m_pBuffer4c = &b4c; m.m_pBlock50 = &b50; m.m_pBuffer54 = &b54;
    m.OnPostRelocation(); if (std::strcmp(g_seq, "BbbBb") || g_blocks[0] != &b50 || g_blocks[1] != &b54 || g_blocks[2] != &b44 || g_blocks[3] != &b48 || g_blocks[4] != &b4c || g_owner != &m) { std::printf("00bf9b40_TEST FAIL\n"); return 1; }
    std::printf("00bf9b40_TEST PASS\n");
    return 0;
}
