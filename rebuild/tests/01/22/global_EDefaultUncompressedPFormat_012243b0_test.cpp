#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fmt = -1;
class CPixelFormat {
public:
    int FormatIndex;
    CPixelFormat(int format) { Initialise(format); }
    void Initialise(int format) { g_fmt = format; FormatIndex = 3; }
};

int main()
{
    CPixelFormat p(21);                                   // D3DFMT_A8R8G8B8
    if (g_fmt != 21 || p.FormatIndex != 3) do { std::printf("012243b0_TEST FAIL\n"); return 1; } while (0);
    std::printf("012243b0_TEST PASS\n");
    return 0;
}