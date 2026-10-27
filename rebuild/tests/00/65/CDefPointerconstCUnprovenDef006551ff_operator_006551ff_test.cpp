#include <cstdio>
// Behaviour model of the counted def pointer assignment.
struct Def { long RefCount; int destroyed; };
struct Ptr {
    Def* Object;
    Ptr& assign(const Ptr& o)
    {
        if (Object != o.Object) {
            if (Object) { if (--Object->RefCount == 0) ++Object->destroyed; Object = 0; }
            Object = o.Object;
            if (Object) ++Object->RefCount;
        }
        return *this;
    }
};
int main()
{
    Def a = { 1, 0 }, b = { 1, 0 };
    Ptr p = { &a }, q = { &b }, n = { 0 };
    if (&p.assign(q) != &p || p.Object != &b || b.RefCount != 2 || a.RefCount != 0 || a.destroyed != 1) { std::printf("006551ff_TEST FAIL\n"); return 1; }
    p.assign(q);                                          // same pointee: no change
    if (b.RefCount != 2) { std::printf("006551ff_TEST FAIL\n"); return 1; }
    p.assign(n);                                          // assign null
    if (p.Object != 0 || b.RefCount != 1 || b.destroyed != 0) { std::printf("006551ff_TEST FAIL\n"); return 1; }
    std::printf("006551ff_TEST PASS\n");
    return 0;
}