#include <cstdio>
#include <cstring>
static int g_marker;
void* g_default = &g_marker;
struct SRecord {
    long m_a; void* m_default; long m_c, m_d; short m_e, m_f; long m_g, m_h, m_i;
    SRecord() : m_a(0), m_default(g_default), m_c(0), m_d(0), m_e(0), m_f(0), m_g(0), m_h(0), m_i(0) {}
};
int main()
{
    int ok = 1;
    SRecord r;
    if (r.m_a || r.m_default != &g_marker || r.m_c || r.m_d || r.m_e || r.m_f || r.m_g || r.m_h || r.m_i) ok = 0;
    g_default = 0;
    SRecord s;
    if (s.m_default != 0) ok = 0;
    if (!ok) { std::printf("00c3b930_TEST FAIL\n"); return 1; }
    std::printf("00c3b930_TEST PASS\n");
    return 0;
}