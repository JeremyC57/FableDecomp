#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct CBufferPair_00b7ec90 { unsigned char _pad00[0x4]; void* m_buffer04; unsigned char _pad08[0xc]; void* m_buffer14; ~CBufferPair_00b7ec90() { if (m_buffer14) free(m_buffer14); if (m_buffer04) free(m_buffer04); } };

int main() {
    int x0, x1; { CBufferPair_00b7ec90 o; o.m_buffer04 = &x0; o.m_buffer14 = &x1; } if (g_freed != 2 || g_freedPtr[0] != &x1 || g_freedPtr[1] != &x0) { std::printf("00b7ec90_TEST FAIL\n"); return 1; }
    { CBufferPair_00b7ec90 o; std::memset(&o, 0, sizeof o); } if (g_freed != 2) { std::printf("00b7ec90_TEST FAIL\n"); return 1; }
    std::printf("00b7ec90_TEST PASS\n"); return 0;
}
