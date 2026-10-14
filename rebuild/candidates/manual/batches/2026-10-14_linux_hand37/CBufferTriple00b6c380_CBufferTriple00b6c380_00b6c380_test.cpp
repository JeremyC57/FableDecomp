#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
struct CBufferTriple_00b6c380 { void* m_bufferA; unsigned char _pad04[8]; void* m_bufferB; unsigned char _pad10[8]; void* m_bufferC; ~CBufferTriple_00b6c380() { if (m_bufferC) free(m_bufferC); if (m_bufferB) free(m_bufferB); if (m_bufferA) free(m_bufferA); } };
int main() {
    int x1, x2, x3; { CBufferTriple_00b6c380 t; t.m_bufferA = &x1; t.m_bufferB = &x2; t.m_bufferC = &x3; } if (g_freed != 3 || g_freedPtr[0] != &x3 || g_freedPtr[1] != &x2 || g_freedPtr[2] != &x1) { std::printf("00b6c380_TEST FAIL\n"); return 1; }
    { CBufferTriple_00b6c380 t; std::memset(&t, 0, sizeof t); } if (g_freed != 3) { std::printf("00b6c380_TEST FAIL\n"); return 1; }
    std::printf("00b6c380_TEST PASS\n"); return 0;
}
