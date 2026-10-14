#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct CBufferPair_0070b830 { unsigned char _pad[4]; void* m_bufferA; unsigned char _pad08[8]; void* m_bufferB; ~CBufferPair_0070b830() { if (m_bufferB) free(m_bufferB); if (m_bufferA) free(m_bufferA); } };
static void Destroy_CBufferPair_0070b830(CBufferPair_0070b830* o) { delete o; }
int main() {
    int x1, x2; CBufferPair_0070b830* o = new CBufferPair_0070b830; o->m_bufferA = &x1; o->m_bufferB = &x2; Destroy_CBufferPair_0070b830(o); if (g_freed != 2 || g_freedPtr[0] != &x2 || g_freedPtr[1] != &x1) { std::printf("0070b830_TEST FAIL\n"); return 1; }
    std::printf("0070b830_TEST PASS\n"); return 0;
}