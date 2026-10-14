#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SPoly { unsigned char m_v[0x80]; long m_count; }; struct CLayer { SPoly m_polys[256]; CLayer* SetAllPolys() { SPoly* p = m_polys; for (long n = 256; n; --n, ++p) { p->m_count = 0; std::memset(p->m_v, 0, sizeof(p->m_v)); } return this; } };

int main() {
    CLayer* l = new CLayer; std::memset(l, 0xab, sizeof(CLayer)); if (l->SetAllPolys() != l) { std::printf("0050ee40_TEST FAIL\n"); return 1; } for (int i = 0; i < 256; ++i) { if (l->m_polys[i].m_count) { std::printf("0050ee40_TEST FAIL\n"); return 1; } for (int j = 0; j < 0x80; ++j) if (l->m_polys[i].m_v[j]) { std::printf("0050ee40_TEST FAIL\n"); return 1; } } delete l;
    std::printf("0050ee40_TEST PASS\n"); return 0;
}
