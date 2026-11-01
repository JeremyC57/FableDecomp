#include <cstdio>
static int g_AutoTinyFieldFirstCalls = 0;
static int g_AutoTinyFieldSecondCalls = 0;
struct AutoTinyFieldMethodsTarget
{
    unsigned int value;
    void First();
    void Second_0099a300();
};
AutoTinyFieldMethodsTarget g_AutoTinyFieldMethodsObject_013d2de8 = {0};
void AutoTinyFieldMethodsTarget::First()
{
    ++g_AutoTinyFieldFirstCalls;
}
void AutoTinyFieldMethodsTarget::Second_0099a300()
{
    ++g_AutoTinyFieldSecondCalls;
}
void __fastcall ClearCachedDefs()
{
    g_AutoTinyFieldMethodsObject_013d2de8.value = 0x0129a064;
    g_AutoTinyFieldMethodsObject_013d2de8.First();
    g_AutoTinyFieldMethodsObject_013d2de8.Second_0099a300();
}
int main()
{
    ClearCachedDefs();
    if (g_AutoTinyFieldMethodsObject_013d2de8.value == 0x0129a064 && g_AutoTinyFieldFirstCalls == 1 && g_AutoTinyFieldSecondCalls == 1) { std::printf("AUTO_TINY_0122ab50_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122ab50_TEST FAIL\n");
    return 1;
}