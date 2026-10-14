#include <cstdio>
#include <cstdlib>
#include <cstring>
struct STrailer { long m_guard; unsigned long m_allocatorIndex; long m_size; }; static long g_size = 0; static long g_calls = 0;
struct CMemoryAllocator { long m_bytesByIndex[0x104]; unsigned long m_no; unsigned char m_currentIndex; long GetAllocationSize(void*) { ++g_calls; return g_size; }
    void InitDebugInfo(void* block, long tag, long size) { GetAllocationSize(block); STrailer* t = (STrailer*)((char*)block + GetAllocationSize(block) - 0x14); t->m_guard = -1; t->m_allocatorIndex = m_currentIndex; t->m_size = size; ((long*)block)[-1] = -1; m_bytesByIndex[m_currentIndex] += size; } };

int main() {
    CMemoryAllocator* m = new CMemoryAllocator; std::memset(m, 0, sizeof *m); m->m_currentIndex = 5; m->m_bytesByIndex[5] = 100; long buf[16]; std::memset(buf, 0, sizeof buf); void* block = &buf[2]; g_size = 40; m->InitDebugInfo(block, 9, 24);
    STrailer* t = (STrailer*)((char*)block + 40 - 0x14); if (t->m_guard != -1 || t->m_allocatorIndex != 5 || t->m_size != 24 || buf[1] != -1 || m->m_bytesByIndex[5] != 124 || g_calls != 2) { std::printf("009f28d0_TEST FAIL\n"); return 1; } delete m;
    std::printf("009f28d0_TEST PASS\n"); return 0;
}
