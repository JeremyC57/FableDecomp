#include <cstdio>
#include <cstdlib>
#include <cstring>
static unsigned long g_size = 0, g_align = 0; static void* g_freed = 0; static char g_buffer[64]; static void* AllocateAligned(unsigned long s, unsigned long a) { g_size = s; g_align = a; return g_buffer; } static void DeallocateAligned(void* b) { g_freed = b; }
struct CBase { virtual void Destroy(int) { } virtual void Delete() { } unsigned long IVRefCount; CBase() : IVRefCount(1) { } ~CBase() { } };
struct CMemoryBlock : CBase { void* m_pData; bool m_owned; unsigned long m_size; long m_used; CMemoryBlock(unsigned long size) : CBase(), m_size(size), m_used(0) { m_pData = AllocateAligned(size, 16); m_owned = true; } ~CMemoryBlock() { if (m_owned) DeallocateAligned(m_pData); } };

int main() {
    { CMemoryBlock b(8); } if (g_freed != g_buffer) { std::printf("00a5c6e0_TEST FAIL\n"); return 1; } g_freed = 0; { CMemoryBlock b(8); b.m_owned = false; } if (g_freed) { std::printf("00a5c6e0_TEST FAIL\n"); return 1; }
    std::printf("00a5c6e0_TEST PASS\n"); return 0;
}
