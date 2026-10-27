// Test for 0x00A3A9C0 CCompressorZlib::Alloc: zlib alloc hook -- refuses empty or > 2 GB requests (64-bit product), else mallocs items * size.
#include <stdio.h>
#include <stdlib.h>
#ifdef _MSC_VER
typedef __int64 s64; typedef unsigned __int64 u64;
#else
typedef long long s64; typedef unsigned long long u64;
#endif
static unsigned int g_lastMalloc = 0;
static void* MallocMock(unsigned int n) { g_lastMalloc = n; return malloc(n); }
class CCompressorZlib {
public:
    static void* Alloc(void* opaque, unsigned int items, unsigned int size)
    {
        s64 total = (s64)((u64)items * size);
        if (total == 0 || total < 0 || total > 0x7fffffff) return 0;
        return MallocMock(items * size);
    }
};
int main()
{
    void* p = CCompressorZlib::Alloc(0, 16, 4);
    bool ok = p != 0 && g_lastMalloc == 64;
    free(p);
    if (CCompressorZlib::Alloc(0, 0, 4) != 0) ok = false;
    if (CCompressorZlib::Alloc(0, 0x10000, 0x10000) != 0) ok = false;
    if (CCompressorZlib::Alloc(0, 0x80000000u, 1) != 0) ok = false;
    if (g_lastMalloc != 64) ok = false;
    puts(ok ? "00a3a9c0_TEST PASS" : "00a3a9c0_TEST FAIL");
    return ok ? 0 : 1;
}