// Behaviour test for 0x00BDB2F0 CEngineVSConstantLayoutLightsBones::PeekName: "LayoutLightsBones" (retail 0x012A7D0C), length -1.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object and the
// callees below are defined under the row's spellings; it asserts the literal text the row passes (a mutated literal fails).
#include <cstdio>
#include <cstring>
class CCharString { public: int d; CCharString(const char* s, long n); };
class CEngineVSConstantLayoutLightsBones { public: int x; CCharString PeekName(); };
static const char* g_s = 0; static long g_n = 0; static int g_calls = 0; static const void* g_this = 0;
CCharString::CCharString(const char* s, long n) { d = 0x1234; g_s = s; g_n = n; ++g_calls; g_this = this; }
int main()
{
    CEngineVSConstantLayoutLightsBones obj; obj.x = 7;
    CCharString r = obj.PeekName();
    if (r.d == 0x1234 && g_calls == 1 && g_n == -1 && g_this == &r && g_s != 0 && std::strcmp(g_s, "LayoutLightsBones") == 0) {
        std::printf("00bdb2f0_TEST PASS\n"); return 0;
    }
    std::printf("FAIL d=%d n=%ld s=%s\n", r.d, g_n, g_s ? g_s : "(null)");
    return 1;
}
