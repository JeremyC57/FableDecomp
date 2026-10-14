#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_flags = -1; struct CData { virtual void Destroy(long f) { g_flags = f; } long m_4, m_8, m_c; long m_references; }; struct CGrid { long m_0; CData* m_pData; void Clear() { CData* d = m_pData; if (d) { if (--d->m_references == 0) d->Destroy(1); } m_pData = 0; } };

int main() {
    CData d; d.m_references = 2; CGrid g; g.m_pData = &d; g.Clear(); if (g.m_pData || d.m_references != 1 || g_flags != -1) { std::printf("00b8ea80_TEST FAIL\n"); return 1; } g.m_pData = &d; g.Clear(); if (g.m_pData || d.m_references != 0 || g_flags != 1) { std::printf("00b8ea80_TEST FAIL\n"); return 1; } g_flags = -1; g.Clear(); if (g_flags != -1) { std::printf("00b8ea80_TEST FAIL\n"); return 1; }
    std::printf("00b8ea80_TEST PASS\n"); return 0;
}
