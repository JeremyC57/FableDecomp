#include <cstdio>
// Behaviour model: the entry point is resolved once, then every call goes straight to it.
static int resolves = 0, calls = 0;
static void Impl(unsigned long, long, long) { ++calls; }
typedef void (*PFN)(unsigned long, long, long);
struct Ctx { PFN fn; void Call(unsigned long s, long p, long v) { if (!fn) { ++resolves; fn = Impl; } fn(s, p, v); } };
int main()
{
    Ctx c = { 0 };
    c.Call(1, 2, 3); c.Call(1, 2, 3);
    if (resolves != 1 || calls != 2) { std::printf("00c88050_TEST FAIL\n"); return 1; }
    std::printf("00c88050_TEST PASS\n");
    return 0;
}