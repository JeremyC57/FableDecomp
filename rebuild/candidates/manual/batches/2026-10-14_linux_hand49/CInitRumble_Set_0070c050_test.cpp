#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CInitRumble { long m_0; bool m_4; bool m_5; long m_8; float m_c, m_10, m_14; long m_18, m_1c, m_20, m_24, m_28;
    void Set(bool a, long b, long c, long d, bool e, long f, long g) { m_c = 1.0f; m_10 = 1.0f; m_14 = 1.0f; m_18 = 0; m_1c = 0; m_0 = d; m_4 = a; m_8 = c; m_5 = e; m_20 = f; m_24 = b; m_28 = g; } };

int main() {
    CInitRumble r; std::memset(&r, 0xff, sizeof r); r.Set(true, 2, 3, 4, false, 6, 7); if (r.m_0 != 4 || !r.m_4 || r.m_5 || r.m_8 != 3 || r.m_c != 1.0f || r.m_10 != 1.0f || r.m_14 != 1.0f || r.m_18 || r.m_1c || r.m_20 != 6 || r.m_24 != 2 || r.m_28 != 7) { std::printf("0070c050_TEST FAIL\n"); return 1; }
    std::printf("0070c050_TEST PASS\n"); return 0;
}
