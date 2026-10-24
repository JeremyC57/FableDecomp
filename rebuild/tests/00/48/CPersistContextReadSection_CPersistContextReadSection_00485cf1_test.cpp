// Behaviour test for CPersistContextReadSection::~CPersistContextReadSection (0x00485CF1): Text, Parser, then InputStream.
#include <cstdio>
static int g_log[8]; static int g_n = 0;
class CPersistContext { public: unsigned char Data[0x2c]; };
class CMemoryDataInputStream { public: unsigned char Data[0x1c]; ~CMemoryDataInputStream() { g_log[g_n++] = 3; } };
class CStringParser { public: unsigned char Data[0x28]; ~CStringParser() { g_log[g_n++] = 2; } };
class CCharString { public: void* PStringData; ~CCharString() { g_log[g_n++] = 1; } };
class CPersistContextReadSection { public: CPersistContext Context; CMemoryDataInputStream InputStream; CStringParser Parser; CCharString Text; ~CPersistContextReadSection() {} };
int main()
{
    { CPersistContextReadSection s; (void)s; }
    if (g_n != 3 || g_log[0] != 1 || g_log[1] != 2 || g_log[2] != 3) { std::printf("00485cf1_TEST FAIL\n"); return 1; }
    std::printf("00485cf1_TEST PASS\n");
    return 0;
}