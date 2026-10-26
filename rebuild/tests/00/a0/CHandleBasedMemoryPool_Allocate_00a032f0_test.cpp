#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static bool g_ok = true; static long g_index = 0, g_alignment = 0, g_size = 0, g_freedIndex = -1; static void* g_freedPool = 0;
class CHandleBasedMemoryPool; struct CPoolHandle { CHandleBasedMemoryPool* m_pPool; long m_index; };
class CHandleBasedMemoryPool { public: bool InternalAllocate(long& index, long alignment, long size) { std::strcat(g_seq, "A"); g_alignment = alignment; g_size = size; if (g_ok) index = g_index; return g_ok; } void Free(long index) { std::strcat(g_seq, "F"); g_freedPool = this; g_freedIndex = index; }
    bool Allocate(CPoolHandle& handle, long alignment, long size) { if (InternalAllocate(size, alignment, size)) { if (handle.m_pPool) { handle.m_pPool->Free(handle.m_index); handle.m_pPool = 0; handle.m_index = -1; } handle.m_pPool = this; handle.m_index = size; return true; } return false; } };

int main() {
    CHandleBasedMemoryPool p, other; CPoolHandle h; h.m_pPool = &other; h.m_index = 9; g_ok = true; g_index = 42;
    if (!p.Allocate(h, 16, 100) || h.m_pPool != &p || h.m_index != 42 || std::strcmp(g_seq, "AF") || g_freedPool != &other || g_freedIndex != 9 || g_alignment != 16 || g_size != 100) { std::printf("00a032f0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; h.m_pPool = 0; h.m_index = -1; if (!p.Allocate(h, 8, 50) || h.m_pPool != &p || std::strcmp(g_seq, "A")) { std::printf("00a032f0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_ok = false; h.m_pPool = &other; h.m_index = 3; if (p.Allocate(h, 8, 50) || h.m_pPool != &other || h.m_index != 3 || std::strcmp(g_seq, "A")) { std::printf("00a032f0_TEST FAIL\n"); return 1; }
    std::printf("00a032f0_TEST PASS\n"); return 0;
}