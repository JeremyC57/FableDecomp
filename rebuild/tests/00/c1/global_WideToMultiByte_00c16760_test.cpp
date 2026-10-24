// Behaviour test for 00c16760: WideToMultiByte -- converts a NUL-terminated UTF-16 string into dest (at most destSize-1 bytes plus a NUL, " " for unmappable characters); an empty buffer is left alone, a failed conversion leaves ""
#include <cstdio>
#include <cstring>

static unsigned int WideLength(const unsigned short* text) { unsigned int n = 0; while (text[n]) ++n; return n; }

static int g_lastFlags = 0;
// stand-in for the Win32 call: ASCII maps 1:1, anything else becomes the default char; a too-small buffer fails
static int FakeWideCharToMultiByte(unsigned int, unsigned long flags, const unsigned short* wide, int wideCount,
                                   char* multi, int multiSize, const char* defaultChar, int* usedDefaultChar)
{
    g_lastFlags = (int)flags;
    if (wideCount > multiSize) return wide[0] == 0x7FFF ? -1 : 0;
    *usedDefaultChar = 0;
    for (int i = 0; i < wideCount; ++i) {
        if (wide[i] < 0x80) multi[i] = (char)wide[i];
        else { multi[i] = defaultChar[0]; *usedDefaultChar = 1; }
    }
    return wideCount;
}

void WideToMultiByte(const unsigned short* source, char* dest, int destSize)
{
    if (destSize) {
        int usedDefault;
        int written = FakeWideCharToMultiByte(0, 0x400, source, WideLength(source), dest, destSize - 1, " ", &usedDefault);
        if (written < 0) {
            dest[0] = 0;
            return;
        }
        dest[written] = 0;
    }
}

int main()
{
    unsigned short hello[] = { 'B', 'a', 'n', 'k', 0x263A, '1', 0 };
    char out[16];
    std::memset(out, 'x', sizeof(out));
    WideToMultiByte(hello, out, 16);
    if (std::strcmp(out, "Bank 1") != 0 || g_lastFlags != 0x400) { std::printf("00c16760_TEST FAIL\n"); return 1; }
    std::memset(out, 'x', sizeof(out));
    WideToMultiByte(hello, out, 6);        // room for 5 + NUL: the call fails and returns 0 -> ""
    if (out[0] != 0 || out[1] != 'x') { std::printf("00c16760_TEST FAIL\n"); return 1; }
    unsigned short odd[] = { 0x7FFF, 'a', 0 };
    std::memset(out, 'x', sizeof(out));
    WideToMultiByte(odd, out, 2);          // negative result: dest[0] = 0 only
    if (out[0] != 0 || out[1] != 'x') { std::printf("00c16760_TEST FAIL\n"); return 1; }
    std::memset(out, 'x', sizeof(out));
    WideToMultiByte(hello, out, 0);        // no buffer: untouched
    if (out[0] != 'x') { std::printf("00c16760_TEST FAIL\n"); return 1; }
    std::printf("00c16760_TEST PASS\n");
    return 0;
}