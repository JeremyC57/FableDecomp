#include <cstdio>
// Behaviour model: the entry point is resolved once, then every call goes straight to it.
static int resolves = 0, calls = 0;
static void Impl(long, unsigned long*) { ++calls; }
typedef void (*PFN)(long, unsigned long*);
struct Ctx { PFN fn; void Call(long n, unsigned long* ids) { if (!fn) { ++resolves; fn = Impl; } fn(n, ids); } };
int main()
{
    Ctx c = { 0 }; unsigned long ids[2];
    c.Call(2, ids); c.Call(2, ids);
    if (resolves != 1 || calls != 2) { std::printf("00c4d8c0_TEST FAIL\n"); return 1; }
    std::printf("00c4d8c0_TEST PASS\n");
    return 0;
}