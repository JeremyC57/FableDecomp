#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CSceneScale_00b91920 { unsigned char _pad[0x50]; float m_scale; };
class CEngineSceneMap { public: unsigned char _pad0[4]; float m_origin; unsigned char _pad8[4]; float m_end; unsigned char _pad10[4]; long m_levels; unsigned char _pad18[0xc]; CSceneScale_00b91920* m_pScale;
    long CalculateLevel(float extent) { for (long level = m_levels - 1; level > 0; --level) { if ((m_end - m_origin) / (float)(1 << level) * m_pScale->m_scale >= extent) return level; } return 0; } };
int main() {
    CEngineSceneMap m; CSceneScale_00b91920 s; s.m_scale = 1.0f; m.m_origin = 0.0f; m.m_end = 64.0f; m.m_levels = 5; m.m_pScale = &s;
    if (m.CalculateLevel(4.0f) != 4 || m.CalculateLevel(5.0f) != 3 || m.CalculateLevel(40.0f) != 0 || m.CalculateLevel(32.0f) != 1) { std::printf("00b91920_TEST FAIL\n"); return 1; }
    m.m_levels = 1; if (m.CalculateLevel(0.0f) != 0) { std::printf("00b91920_TEST FAIL\n"); return 1; }
    std::printf("00b91920_TEST PASS\n"); return 0;
}