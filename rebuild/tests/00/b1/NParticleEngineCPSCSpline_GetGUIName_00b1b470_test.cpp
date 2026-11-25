// Behaviour test for 0x00B1B470 NParticleEngine::CPSCSpline::GetGUIName: "Spline" (retail 0x0129F608), length -1, into the return slot.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object and the
// callees below are defined under the row's spellings; it asserts the literal text the row passes (a mutated literal fails).
#include <cstdio>
#include <cstring>
class CCharString { public: char* m_data; CCharString(const char* s, long n); };
namespace NParticleEngine { class CPSCSpline { public: CCharString GetGUIName(); }; }
static const char* g_s = 0; static long g_n = 0; static int g_calls = 0; static const void* g_this = 0;
CCharString::CCharString(const char* s, long n) { g_s = s; g_n = n; ++g_calls; g_this = this; m_data = 0; }
int main()
{
    NParticleEngine::CPSCSpline o;
    CCharString r = o.GetGUIName();
    bool ok = g_calls == 1 && g_n == -1 && g_s != 0 && std::strcmp(g_s, "Spline") == 0 && g_this == &r;
    std::printf(ok ? "00b1b470_TEST PASS\n" : "00b1b470_TEST FAIL\n");
    return ok ? 0 : 1;
}
