// Behaviour test for CSmallVector<CCharString,8>::Allocate (0x009D2160): Clear, new[] n default strings, byte-sized count.
#include <cstdio>

static int g_live = 0;
struct CCharString {
    const char* PData;
    CCharString() : PData("") { ++g_live; }
    ~CCharString() { --g_live; }
};

struct CSmallVector8 {
    CCharString* Start;
    signed char Size;
    void Clear() { delete[] Start; Start = 0; Size = 0; }
    void Allocate(unsigned long n) { Clear(); Start = new CCharString[n]; Size = (signed char)n; }
};

int main()
{
    CSmallVector8 v;
    v.Start = 0;
    v.Size = 0;
    v.Allocate(3);
    if (v.Start == 0 || v.Size != 3 || g_live != 3 || v.Start[2].PData[0] != 0) { std::printf("009d2160_TEST FAIL\n"); return 1; }
    v.Allocate(5);
    if (v.Size != 5 || g_live != 5) { std::printf("009d2160_TEST FAIL\n"); return 1; }
    v.Allocate(0);
    if (v.Size != 0 || g_live != 0) { std::printf("009d2160_TEST FAIL\n"); return 1; }
    v.Clear();
    if (v.Start != 0 || g_live != 0) { std::printf("009d2160_TEST FAIL\n"); return 1; }
    std::printf("009d2160_TEST PASS\n");
    return 0;
}