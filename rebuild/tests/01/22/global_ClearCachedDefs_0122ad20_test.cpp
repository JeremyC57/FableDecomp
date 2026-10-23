#include <cstdio>
static int g_AutoTinyFieldFirstCalls = 0;
static int g_AutoTinyFieldSecondCalls = 0;
struct AutoTinyFieldMethodsTarget
{
    unsigned int value;
    void First_00b48d40();
    void Second();
};
AutoTinyFieldMethodsTarget g_AutoTinyFieldMethodsObject_01436fb0 = {0};
void AutoTinyFieldMethodsTarget::First_00b48d40()
{
    ++g_AutoTinyFieldFirstCalls;
}
void AutoTinyFieldMethodsTarget::Second()
{
    ++g_AutoTinyFieldSecondCalls;
}
void __fastcall ClearCachedDefs()
{
    g_AutoTinyFieldMethodsObject_01436fb0.value = 0x012a20f0;
    g_AutoTinyFieldMethodsObject_01436fb0.First_00b48d40();
    g_AutoTinyFieldMethodsObject_01436fb0.Second();
}
int main()
{
    ClearCachedDefs();
    if (g_AutoTinyFieldMethodsObject_01436fb0.value == 0x012a20f0 && g_AutoTinyFieldFirstCalls == 1 && g_AutoTinyFieldSecondCalls == 1) { std::printf("AUTO_TINY_0122ad20_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122ad20_TEST FAIL\n");
    return 1;
}