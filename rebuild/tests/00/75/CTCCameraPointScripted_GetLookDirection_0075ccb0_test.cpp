#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SFilterParams_0075ccb0 { float m_weight; long m_a; long m_b; };
static const SFilterParams_0075ccb0* g_in = 0; static void* g_view = 0;
class CEnvironmentSpellEffectInterface { public: unsigned char _pad[0x58]; SFilterParams_0075ccb0 m_filter;
    void TransformVector(SFilterParams_0075ccb0& out, const SFilterParams_0075ccb0& in, void* view) { g_in = &in; g_view = view; out.m_weight = in.m_weight * 2.0f; out.m_a = in.m_a + 1; out.m_b = in.m_b + 2; }
    void Get(SFilterParams_0075ccb0* result, void* view) { SFilterParams_0075ccb0 p; TransformVector(p, m_filter, view); result->m_weight = p.m_weight; result->m_a = p.m_a; result->m_b = p.m_b; } };
int main() {
    CEnvironmentSpellEffectInterface e; e.m_filter.m_weight = 1.5f; e.m_filter.m_a = 10; e.m_filter.m_b = 20; SFilterParams_0075ccb0 r; int v; e.Get(&r, &v);
    if (g_in != &e.m_filter || g_view != &v || r.m_weight != 3.0f || r.m_a != 11 || r.m_b != 22) { std::printf("0075ccb0_TEST FAIL\n"); return 1; }
    std::printf("0075ccb0_TEST PASS\n"); return 0;
}