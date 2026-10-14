#include <cstdio>
static void* g_baseArg = 0;
struct CStatBarInfo { int x; };
namespace NPlayerGui {
struct CStatBase { virtual void v00() { } unsigned char _body[0x6c - 4]; CStatBase(CStatBarInfo* i) { g_baseArg = i; } };
struct CCrimeStatBar : CStatBase { float NormalisedTimeLeft; CCrimeStatBar(CStatBarInfo* i) : CStatBase(i), NormalisedTimeLeft(0.0f) { } }; }
int main()
{
    CStatBarInfo info; NPlayerGui::CCrimeStatBar bar(&info);
    if (g_baseArg != &info || bar.NormalisedTimeLeft != 0.0f || (char*)&bar.NormalisedTimeLeft - (char*)&bar != 0x6c) { std::printf("00654392_TEST FAIL\n"); return 1; }
    std::printf("00654392_TEST PASS\n"); return 0;
}
