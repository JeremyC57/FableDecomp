#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct CBufferTriple_0075bee0 { unsigned char _pad00[0x44]; void* m_buffer44; unsigned char _pad48[0xb0]; void* m_bufferf8; unsigned char _padfc[0xb0]; void* m_buffer1ac; ~CBufferTriple_0075bee0() { if (m_buffer1ac) free(m_buffer1ac); if (m_bufferf8) free(m_bufferf8); if (m_buffer44) free(m_buffer44); } };

int main() {
    int x0, x1, x2; { CBufferTriple_0075bee0 o; o.m_buffer44 = &x0; o.m_bufferf8 = &x1; o.m_buffer1ac = &x2; } if (g_freed != 3 || g_freedPtr[0] != &x2 || g_freedPtr[1] != &x1 || g_freedPtr[2] != &x0) { std::printf("0075bee0_TEST FAIL\n"); return 1; }
    { CBufferTriple_0075bee0 o; std::memset(&o, 0, sizeof o); } if (g_freed != 3) { std::printf("0075bee0_TEST FAIL\n"); return 1; }
    std::printf("0075bee0_TEST PASS\n"); return 0;
}