#include <cstdio>
struct CFloatRecord_004d4033 {
    float m_start, m_scale, m_offset; long m_count; float m_a, m_b, m_c;
    CFloatRecord_004d4033() : m_start(0.0f), m_scale(1.0f), m_offset(0.0f), m_count(1), m_a(0.0f), m_b(0.0f), m_c(0.0f) { }
};
int main()
{
    CFloatRecord_004d4033 r;
    if (r.m_start != 0.0f || r.m_scale != 1.0f || r.m_offset != 0.0f || r.m_count != 1 || r.m_a != 0.0f || r.m_b != 0.0f || r.m_c != 0.0f || sizeof(r) != 28) { std::printf("004d4033_TEST FAIL\n"); return 1; }
    std::printf("004d4033_TEST PASS\n"); return 0;
}