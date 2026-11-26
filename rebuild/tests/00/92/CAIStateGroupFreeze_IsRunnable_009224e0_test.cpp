// Behaviour test for 0x009224E0 CAIStateGroup_Freeze::IsRunnable(): returns the bool flag @ 0x013BB68C as loaded.
#include <cstdio>
bool g_flag_013bb68c = false;
class CAIStateGroup_Freeze { public: virtual ~CAIStateGroup_Freeze() { } virtual bool IsRunnable(); };
bool CAIStateGroup_Freeze::IsRunnable() { return g_flag_013bb68c; }
int main()
{
    CAIStateGroup_Freeze f;
    g_flag_013bb68c = false; if (f.IsRunnable()) { std::printf("009224e0_TEST FAIL\n"); return 1; }
    g_flag_013bb68c = true; if (!f.IsRunnable()) { std::printf("009224e0_TEST FAIL\n"); return 1; }
    std::printf("009224e0_TEST PASS\n"); return 0;
}
