#include <cstdio>
#include <cstdlib>
// Behaviour model of the counted-pointer release: run the deleter and free the block on the last reference, then clear.
struct Info { long RefCount; int deleted; };
struct Ptr { void* Data; Info* Ref; };
static int g_freed;
static void release(Ptr& p)
{
    if (p.Ref) { --p.Ref->RefCount; if (p.Ref->RefCount == 0) { ++p.Ref->deleted; ++g_freed; } }
    p.Ref = 0; p.Data = 0;
}
int main()
{
    Info i = { 2, 0 }; int x;
    Ptr a = { &x, &i }, b = { &x, &i }, c = { 0, 0 };
    release(a);
    if (a.Ref != 0 || a.Data != 0 || i.RefCount != 1 || i.deleted != 0) { std::printf("00b44120_TEST FAIL\n"); return 1; }
    release(b);
    if (i.RefCount != 0 || i.deleted != 1 || g_freed != 1) { std::printf("00b44120_TEST FAIL\n"); return 1; }
    release(c);
    if (g_freed != 1) { std::printf("00b44120_TEST FAIL\n"); return 1; }
    std::printf("00b44120_TEST PASS\n");
    return 0;
}