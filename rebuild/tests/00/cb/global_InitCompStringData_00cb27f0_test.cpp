#include <cstdio>
#include <cstring>

// InitCompStringData: the composition string, its attribute string and both cursor positions are zeroed.
unsigned short g_szCompositionString[256];
unsigned char  g_szCompAttrString[256];
unsigned long  g_IMECursorBytes;
unsigned long  g_IMECursorChars;
unsigned long  g_after = 0x12345678UL;

void InitCompStringData()
{
    std::memset(g_szCompositionString, 0, sizeof(g_szCompositionString));
    std::memset(g_szCompAttrString, 0, sizeof(g_szCompAttrString));
    g_IMECursorBytes = 0;
    g_IMECursorChars = 0;
}

int main()
{
    for (int i = 0; i < 256; ++i) { g_szCompositionString[i] = (unsigned short)(0x41 + i); g_szCompAttrString[i] = (unsigned char)i; }
    g_IMECursorBytes = 12; g_IMECursorChars = 6;
    InitCompStringData();
    bool ok = g_IMECursorBytes == 0 && g_IMECursorChars == 0 && g_after == 0x12345678UL && sizeof(g_szCompositionString) == 0x200;
    for (int k = 0; k < 256; ++k)
        if (g_szCompositionString[k] != 0 || g_szCompAttrString[k] != 0) ok = false;
    if (!ok) {
        std::printf("00cb27f0_TEST FAIL\n");
        return 1;
    }
    std::printf("00cb27f0_TEST PASS\n");
    return 0;
}