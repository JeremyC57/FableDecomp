#include <cstdio>
// Behaviour model of the deleting destructor: release the counted pointer, run the base part, free only when asked.
static int g_deleter, g_base, g_freed;
struct Info { long RefCount; };
struct Obj { void* Data; Info* Ref; };
static Obj* dtor(Obj* o, unsigned flags)
{
    if (o->Ref) { --o->Ref->RefCount; if (o->Ref->RefCount == 0) ++g_deleter; }
    o->Data = 0; o->Ref = 0;
    ++g_base;
    if (flags & 1) ++g_freed;
    return o;
}
int main()
{
    Info i = { 1 }; int x;
    Obj a = { &x, &i };
    if (dtor(&a, 1) != &a || g_deleter != 1 || g_base != 1 || g_freed != 1 || a.Ref != 0) { std::printf("006963f0_TEST FAIL\n"); return 1; }
    Info j = { 2 };
    Obj b = { &x, &j };
    dtor(&b, 0);
    if (g_deleter != 1 || j.RefCount != 1 || g_freed != 1 || g_base != 2) { std::printf("006963f0_TEST FAIL\n"); return 1; }
    std::printf("006963f0_TEST PASS\n");
    return 0;
}