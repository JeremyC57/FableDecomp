#include <cstdio>
// Links the source object: ImeUi_SetScreenDimension stores the size when g_bInitialized (DXUT ImeUi.cpp).
bool g_bInitialized; unsigned int g_screenWidth; unsigned int g_screenHeight;
void __fastcall ImeUi_SetScreenDimension(unsigned int width, unsigned int height);
int main() {
    g_bInitialized = false; ImeUi_SetScreenDimension(640, 480);
    if (g_screenWidth || g_screenHeight) { std::printf("00cb2ee0_TEST FAIL\n"); return 1; }
    g_bInitialized = true; ImeUi_SetScreenDimension(640, 480);
    if (g_screenWidth != 640 || g_screenHeight != 480) { std::printf("00cb2ee0_TEST FAIL\n"); return 1; }
    std::printf("00cb2ee0_TEST PASS\n"); return 0;
}
