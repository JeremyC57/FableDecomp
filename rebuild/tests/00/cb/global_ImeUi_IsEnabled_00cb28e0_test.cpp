#include <cstdio>
// Links the source object: ImeUi_IsEnabled returns g_bImeEnabled (DXUT ImeUi.cpp).
bool g_bImeEnabled;
bool __fastcall ImeUi_IsEnabled();
int main()
{
    g_bImeEnabled = false; if (ImeUi_IsEnabled()) { std::printf("00cb28e0_TEST FAIL\n"); return 1; }
    g_bImeEnabled = true; if (!ImeUi_IsEnabled()) { std::printf("00cb28e0_TEST FAIL\n"); return 1; }
    std::printf("00cb28e0_TEST PASS\n");
    return 0;
}
