// behaviour test for 0x005BD46D: the getter returns the def's value and leaves its reference count unchanged
#include <cstdio>
struct Def { unsigned long refs; float v[4]; };
struct Ptr { Def* o; Ptr(Def* d) : o(d) { if (o) ++o->refs; } ~Ptr() { --o->refs; } };
static float Get(Def* d) { Ptr p(d); return p.o->v[3]; }
int main()
{
    Def d = { 1, { 0.1f, 0.2f, 0.3f, 0.4f } };
    float r = Get(&d);
    if (r != d.v[3] || d.refs != 1) { std::printf("005bd46d_TEST FAIL\n"); return 1; }
    std::printf("005bd46d_TEST PASS\n");
    return 0;
}