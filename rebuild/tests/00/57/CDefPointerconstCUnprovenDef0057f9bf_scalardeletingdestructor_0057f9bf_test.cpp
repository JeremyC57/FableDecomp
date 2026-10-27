#include <cstdio>
// Behaviour model of the CDefPointer scalar deleting destructor: release the counted def, clear, free when flagged.
struct Def { long RefCount; int destroyed; void Destroy() const { ++((Def*)this)->destroyed; } void Release() { if (--RefCount == 0) Destroy(); } };
struct Ptr { Def* Object; };
static int g_freed;
static Ptr* scalar_deleting_dtor(Ptr* p, unsigned int flags)
{
    if (p->Object) { p->Object->Release(); p->Object = 0; }
    if (flags & 1) ++g_freed;
    return p;
}
int main()
{
    Def d = { 2, 0 };
    Ptr a = { &d }, b = { &d }, c = { 0 };
    if (scalar_deleting_dtor(&a, 0) != &a || a.Object != 0 || d.RefCount != 1 || d.destroyed != 0 || g_freed != 0) { std::printf("0057f9bf_TEST FAIL\n"); return 1; }
    scalar_deleting_dtor(&b, 1);
    if (b.Object != 0 || d.RefCount != 0 || d.destroyed != 1 || g_freed != 1) { std::printf("0057f9bf_TEST FAIL\n"); return 1; }
    scalar_deleting_dtor(&c, 1);
    if (d.destroyed != 1 || g_freed != 2) { std::printf("0057f9bf_TEST FAIL\n"); return 1; }
    std::printf("0057f9bf_TEST PASS\n");
    return 0;
}