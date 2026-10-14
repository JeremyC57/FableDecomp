#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_rebuilt = 0;
struct CAppearance_00caddc0 { void Rebuild() { ++g_rebuilt; } };
struct CAppearanceOwner_00ca4990 { long m_inv; CAppearanceOwner_00ca4990() : m_inv(-1) { } virtual void Invalidate(long f) { m_inv = f; } CAppearance_00caddc0* m_pAppearance;
    long Rebuild() { if (m_pAppearance) m_pAppearance->Rebuild(); Invalidate(1); return 0; } };
int main() {
    CAppearanceOwner_00ca4990 o; CAppearance_00caddc0 a; o.m_pAppearance = 0; if (o.Rebuild() != 0 || g_rebuilt || o.m_inv != 1) { std::printf("00ca4990_TEST FAIL\n"); return 1; }
    o.m_pAppearance = &a; if (o.Rebuild() != 0 || g_rebuilt != 1 || o.m_inv != 1) { std::printf("00ca4990_TEST FAIL\n"); return 1; }
    std::printf("00ca4990_TEST PASS\n"); return 0;
}
