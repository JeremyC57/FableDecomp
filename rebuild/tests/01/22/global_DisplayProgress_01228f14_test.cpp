#include <cstdio>
static int g_AutoTinyOptionalValue = 0;
static void* g_AutoTinyObservedObject = 0;
void* g_AutoTinyOptionalObject = &g_AutoTinyOptionalValue;
static int g_free_hook = 0;   // the CRT frees its own blocks before main: count only the frees under test
extern "C" void __cdecl free(void* object)
{
    if (!g_free_hook) return;
    g_AutoTinyObservedObject = object;
}
void __fastcall DisplayProgress()
{
    if (g_AutoTinyOptionalObject != 0)
        free(g_AutoTinyOptionalObject);
}
int main()
{
    g_free_hook = 1;   // from here on free() is the hooked MSVCR71 free (0x00BFEA14)
    DisplayProgress();
    if (g_AutoTinyObservedObject == g_AutoTinyOptionalObject) { std::printf("AUTO_TINY_01228f14_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_01228f14_TEST FAIL\n");
    return 1;
}