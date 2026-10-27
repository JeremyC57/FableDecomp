// behaviour test for 0x005C2F07: GetInitialItems returns the def's list and leaves the def's reference count unchanged
#include <cstdio>
struct Def { long refs; int destroyed; long items[4]; void Destroy() const { } };
struct Ptr { Def* o; Ptr(Def* d) : o(d) { if (o) ++o->refs; } ~Ptr() { if (o && --o->refs == 0) o->destroyed = 1; } };
static long* Get(Def* d) { Ptr p(d); return &p.o->items[1]; }
int main()
{
    Def d = { 1, 0, { 0, 0, 0, 0 } };
    long* r = Get(&d);
    if (r != &d.items[1] || d.refs != 1 || d.destroyed) { std::printf("005c2f07_TEST FAIL\n"); return 1; }
    std::printf("005c2f07_TEST PASS\n");
    return 0;
}