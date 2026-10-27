// behaviour test for 0x0040db10 std::allocator<CDateAndTime>::allocate(n, hint): n ? malloc(n * 28) : 0
#include <cstdio>
#include <cstdlib>
struct DT { int Year; unsigned int Month, Day, Hour, Minute, Second, Millisecond; };
static unsigned int gBytes = 0; static int gCalls = 0;
static void* Malloc(unsigned int n) { ++gCalls; gBytes = n; return std::malloc(n); }
static DT* Allocate(unsigned int n, const void* = 0) { return n != 0 ? (DT*)Malloc(n * sizeof(DT)) : 0; }
int main()
{
    int fails = 0;
    if (sizeof(DT) != 28) fails |= 1;
    DT* p = Allocate(3);
    if (!p || gCalls != 1 || gBytes != 3 * sizeof(DT)) fails |= 2;
    p[2].Millisecond = 999;
    std::free(p);
    if (Allocate(0) != 0 || gCalls != 1) fails |= 4;
    if (fails) { std::printf("0040db10_TEST FAIL %d\n", fails); return 1; }
    std::printf("0040db10_TEST PASS\n");
    return 0;
}