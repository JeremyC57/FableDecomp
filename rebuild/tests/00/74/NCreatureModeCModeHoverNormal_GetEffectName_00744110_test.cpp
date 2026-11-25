// Behaviour test for 0x00744110 NCreatureMode::CModeHoverNormal::GetEffectName: "FLIGHT" (retail 0x01268848), length -1, into the return slot.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object and the
// callees below are defined under the row's spellings; it asserts the literal text the row passes (a mutated literal fails).
#include <cstdio>
#include <cstring>
class CCharString { public: char* p; CCharString(const char* s, long n); };
namespace NCreatureMode { struct CModeHoverNormal { CCharString GetEffectName(); }; }
static const char* g_s = 0; static long g_n = 0; static int g_calls = 0; static const void* g_this = 0;
CCharString::CCharString(const char* s, long n) { g_s = s; g_n = n; ++g_calls; g_this = this; p = 0; }
int main()
{
    NCreatureMode::CModeHoverNormal o;
    CCharString r = o.GetEffectName();
    bool ok = g_calls == 1 && g_n == -1 && g_s != 0 && std::strcmp(g_s, "FLIGHT") == 0 && g_this == &r;
    std::printf(ok ? "00744110_TEST PASS\n" : "00744110_TEST FAIL\n");
    return ok ? 0 : 1;
}
