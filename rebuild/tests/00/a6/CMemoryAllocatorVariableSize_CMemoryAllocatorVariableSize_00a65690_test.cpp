#include <cstdio>
struct CMemoryAreaChunk { CMemoryAreaChunk* next; };
static CMemoryAreaChunk* g_counter = 0; static long g_usage = 0; static int g_released = 0;
struct CMemoryAllocatorVariableSize { virtual void v00() { } unsigned char _pad04[0xc14 - 4]; long m_usage; CMemoryAreaChunk* m_chunkCounter; CMemoryAreaChunk* PFirstChunk;
    void ReleaseChunk(CMemoryAreaChunk* c) { ++g_released; PFirstChunk = c->next; }
    ~CMemoryAllocatorVariableSize() { g_counter = m_chunkCounter; g_usage = m_usage; while (PFirstChunk) ReleaseChunk(PFirstChunk); } };
int main() { CMemoryAreaChunk a, b; a.next = &b; b.next = 0; CMemoryAreaChunk marker;
    { CMemoryAllocatorVariableSize m; m.m_usage = 5; m.m_chunkCounter = &marker; m.PFirstChunk = &a; }
    if (g_counter != &marker || g_usage != 5 || g_released != 2) { std::printf("00a65690_TEST FAIL\n"); return 1; } std::printf("00a65690_TEST PASS\n"); return 0; }