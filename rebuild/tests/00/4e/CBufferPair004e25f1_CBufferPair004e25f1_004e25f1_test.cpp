#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct CBufferPair_004e25f1 { unsigned char _pad00[0x14]; void* m_buffer14; unsigned char _pad18[0x8]; void* m_buffer20; ~CBufferPair_004e25f1() { if (m_buffer20) free(m_buffer20); if (m_buffer14) free(m_buffer14); } };

int main() {
    int x0, x1; { CBufferPair_004e25f1 o; o.m_buffer14 = &x0; o.m_buffer20 = &x1; } if (g_freed != 2 || g_freedPtr[0] != &x1 || g_freedPtr[1] != &x0) { std::printf("004e25f1_TEST FAIL\n"); return 1; }
    { CBufferPair_004e25f1 o; std::memset(&o, 0, sizeof o); } if (g_freed != 2) { std::printf("004e25f1_TEST FAIL\n"); return 1; }
    std::printf("004e25f1_TEST PASS\n"); return 0;
}