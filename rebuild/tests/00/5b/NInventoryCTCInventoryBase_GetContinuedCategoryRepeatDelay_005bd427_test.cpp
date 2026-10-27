// behaviour test for 0x005BD427: the getter returns the def's value and leaves its reference count unchanged
#include <cstdio>
struct Def { unsigned long refs; float v[4]; };
struct Ptr { Def* o; Ptr(Def* d) : o(d) { if (o) ++o->refs; } ~Ptr() { --o->refs; } };
static float Get(Def* d) { Ptr p(d); return p.o->v[1]; }
int main()
{
    Def d = { 1, { 0.1f, 0.2f, 0.3f, 0.4f } };
    float r = Get(&d);
    if (r != d.v[1] || d.refs != 1) { std::printf("005bd427_TEST FAIL\n"); return 1; }
    std::printf("005bd427_TEST PASS\n");
    return 0;
}