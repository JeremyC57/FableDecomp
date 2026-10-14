#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
static int g_base = 0;
struct CIVCountedPointeeBase { virtual void v00() { } unsigned char _pad[4]; ~CIVCountedPointeeBase() { ++g_base; } };
struct CCountedBufferPair_00a97850 : CIVCountedPointeeBase { void* m_bufferA; unsigned char _pad0c[8]; void* m_bufferB; ~CCountedBufferPair_00a97850() { if (m_bufferB) free(m_bufferB); if (m_bufferA) free(m_bufferA); } };
int main() {
    int x1, x2; { CCountedBufferPair_00a97850 p; p.m_bufferA = &x1; p.m_bufferB = &x2; } if (g_freed != 2 || g_freedPtr[0] != &x2 || g_freedPtr[1] != &x1 || g_base != 1) { std::printf("00a97850_TEST FAIL\n"); return 1; }
    std::printf("00a97850_TEST PASS\n"); return 0;
}
