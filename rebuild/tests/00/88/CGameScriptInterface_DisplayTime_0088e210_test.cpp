#include <cstdio>

void __fastcall CGameScriptInterface_DisplayTime(void* self, int edx_unused, int display)
{
    (void)self;
    (void)edx_unused;
    (void)display;
}

int main()
{
    int marker = 12345;
    CGameScriptInterface_DisplayTime(&marker, 0, 99);
    if (marker != 12345) { std::printf("CGameScriptInterface_0088e210_TEST FAIL\n"); return 1; }
    std::printf("CGameScriptInterface_0088e210_TEST PASS\n");
    return 0;
}