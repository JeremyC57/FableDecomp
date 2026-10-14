#include <cstdio>
static bool g_dummyTypeInUse0, g_dummyTypeInUse1, g_dummyTypeInUse2;
struct CAIStateGroup_ApprenticeHero {
    static bool IsDummyTypeLegal(long t) { switch (t) { case 0: return !g_dummyTypeInUse0; case 1: return !g_dummyTypeInUse1; case 2: return !g_dummyTypeInUse2; } return false; }
};
int main()
{
    g_dummyTypeInUse1 = true;
    if (!CAIStateGroup_ApprenticeHero::IsDummyTypeLegal(0) || CAIStateGroup_ApprenticeHero::IsDummyTypeLegal(1) || !CAIStateGroup_ApprenticeHero::IsDummyTypeLegal(2)
        || CAIStateGroup_ApprenticeHero::IsDummyTypeLegal(3) || CAIStateGroup_ApprenticeHero::IsDummyTypeLegal(-1)) { std::printf("00929f70_TEST FAIL\n"); return 1; }
    g_dummyTypeInUse1 = false; g_dummyTypeInUse2 = true;
    if (!CAIStateGroup_ApprenticeHero::IsDummyTypeLegal(1) || CAIStateGroup_ApprenticeHero::IsDummyTypeLegal(2)) { std::printf("00929f70_TEST FAIL\n"); return 1; }
    std::printf("00929f70_TEST PASS\n"); return 0;
}
