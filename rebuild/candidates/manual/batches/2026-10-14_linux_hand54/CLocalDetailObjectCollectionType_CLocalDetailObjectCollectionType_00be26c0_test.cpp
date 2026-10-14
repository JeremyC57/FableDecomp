#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SBlock { long m_words[13]; };
static long g_base = 0; struct CBaseNC { virtual void v00() { } CBaseNC() { ++g_base; } }; struct CType : CBaseNC { long m_4, m_8, m_c, m_10, m_14, m_18, m_1c; SBlock m_block; CType() { }
    CType(const CType& o) : CBaseNC() { m_14 = o.m_14; m_1c = o.m_1c; m_18 = o.m_18; m_10 = o.m_10; m_4 = o.m_4; m_c = o.m_c; m_8 = o.m_8; m_block = o.m_block; } };

int main() {
    CType a; a.m_4 = 4; a.m_8 = 8; a.m_c = 12; a.m_10 = 16; a.m_14 = 20; a.m_18 = 24; a.m_1c = 28; for (int i = 0; i < 13; ++i) a.m_block.m_words[i] = 100 + i; g_base = 0; CType b(a); if (g_base != 1 || b.m_4 != 4 || b.m_8 != 8 || b.m_c != 12 || b.m_10 != 16 || b.m_14 != 20 || b.m_18 != 24 || b.m_1c != 28 || std::memcmp(&a.m_block, &b.m_block, sizeof(SBlock))) { std::printf("00be26c0_TEST FAIL\n"); return 1; }
    std::printf("00be26c0_TEST PASS\n"); return 0;
}
