#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_base = 0; struct CBaseNC { virtual void v00() { } CBaseNC() { ++g_base; } };
struct SState { long m_position; long m_size; const unsigned char* m_pData; long m_flags; long m_sizeCopy; SState(const unsigned char* d, long s) : m_position(0), m_size(s), m_pData(d), m_flags(0), m_sizeCopy(s) { } };
struct CMemoryDataInputStream : CBaseNC, SState { const unsigned char* m_pDataCopy; CMemoryDataInputStream(const unsigned char* d, long s) : CBaseNC(), SState(d, s), m_pDataCopy(d) { } };

int main() {
    unsigned char data[4]; CMemoryDataInputStream s(data, 4); if (g_base != 1 || s.m_position || s.m_size != 4 || s.m_pData != data || s.m_flags || s.m_sizeCopy != 4 || s.m_pDataCopy != data) { std::printf("00994930_TEST FAIL\n"); return 1; }
    std::printf("00994930_TEST PASS\n"); return 0;
}
