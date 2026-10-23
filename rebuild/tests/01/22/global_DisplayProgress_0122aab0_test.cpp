#include <cstdio>
static int g_AutoTinyOptionalValue = 0;
static void* g_AutoTinyObservedObject = 0;
void* g_AutoTinyOptionalObject_013d2d1c = &g_AutoTinyOptionalValue;
void __cdecl AutoTinyOptionalTarget(void* object)
{
    g_AutoTinyObservedObject = object;
}
void __fastcall DisplayProgress()
{
    if (g_AutoTinyOptionalObject_013d2d1c != 0)
        AutoTinyOptionalTarget(g_AutoTinyOptionalObject_013d2d1c);
}
int main()
{
    DisplayProgress();
    if (g_AutoTinyObservedObject == g_AutoTinyOptionalObject_013d2d1c) { std::printf("AUTO_TINY_0122aab0_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_0122aab0_TEST FAIL\n");
    return 1;
}