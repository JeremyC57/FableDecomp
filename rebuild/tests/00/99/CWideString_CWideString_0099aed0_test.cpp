// Behaviour test for 0x0099AED0 CWideString::CWideString(): no string data, one more live instance counted.
#include <cstdio>

static long g_instances = 0;
class WideString {
public:
    void* PStringData;
    WideString() : PStringData(0) { ++g_instances; }
};

int main()
{
    WideString a;
    if (a.PStringData != 0 || g_instances != 1) { std::printf("0099aed0_TEST FAIL\n"); return 1; }
    WideString b[3];
    if (b[2].PStringData != 0 || g_instances != 4) { std::printf("0099aed0_TEST FAIL\n"); return 1; }
    std::printf("0099aed0_TEST PASS\n");
    return 0;
}