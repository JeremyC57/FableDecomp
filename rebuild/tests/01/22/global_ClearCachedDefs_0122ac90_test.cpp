#include <cstdio>
static int g_AutoTinyFieldFirstCalls = 0;
static int g_AutoTinyFieldSecondCalls = 0;
struct AutoTinyFieldMethodsTarget
{
    unsigned int value;
    void First_00af8860();
    void Second();
};
AutoTinyFieldMethodsTarget g_AutoTinyFieldMethodsObject_013d7e50 = {0};
void AutoTinyFieldMethodsTarget::First_00af8860()
{
    ++g_AutoTinyFieldFirstCalls;
}
void AutoTinyFieldMethodsTarget::Second()
{
    ++g_AutoTinyFieldSecondCalls;
}
void __fastcall ClearCachedDefs()
{
    g_AutoTinyFieldMethodsObject_013d7e50.value = 0x0129f644;
    g_AutoTinyFieldMethodsObject_013d7e50.First_00af8860();
    g_AutoTinyFieldMethodsObject_013d7e50.Second();
}
int main()
{
    ClearCachedDefs();
    if (g_AutoTinyFieldMethodsObject_013d7e50.value == 0x0129f644 && g_AutoTinyFieldFirstCalls == 1 && g_AutoTinyFieldSecondCalls == 1) { std::printf("AUTO_TINY_0122ac90_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122ac90_TEST FAIL\n");
    return 1;
}