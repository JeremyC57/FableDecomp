#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SRec { unsigned char m_body[4]; }; static long g_n = 0, g_vis = 0; static void* g_rec = 0; static void* g_last = 0; class CMap { public: void SetMapVisibility(SRec* r, long v) { ++g_n; g_rec = r; g_vis = v; g_last = this; } }; struct CVec { CMap** m_first; CMap** m_last; CMap** m_end; unsigned long size() const { return m_last - m_first; } };
struct COwner { unsigned char _pad[0x20]; CVec m_maps; SRec m_record; void SetMapVisibility(long v) { for (unsigned long i = 0; i < m_maps.size(); ++i) { if (m_maps.m_first[i]) m_maps.m_first[i]->SetMapVisibility(&m_record, v); } } };

int main() {
    CMap m1, m2; CMap* p[3] = { &m1, 0, &m2 }; COwner o; o.m_maps.m_first = p; o.m_maps.m_last = p + 3; o.SetMapVisibility(9); if (g_n != 2 || g_rec != &o.m_record || g_vis != 9 || g_last != &m2) { std::printf("00b423b0_TEST FAIL\n"); return 1; }
    std::printf("00b423b0_TEST PASS\n"); return 0;
}