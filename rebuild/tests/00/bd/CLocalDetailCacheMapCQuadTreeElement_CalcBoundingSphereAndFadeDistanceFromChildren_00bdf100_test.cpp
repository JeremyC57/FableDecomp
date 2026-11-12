#include <cstdio>
#include <vector>
struct Sphere { float x, y, z, r; };
struct Coll { Coll* Next; Sphere s; float fade; unsigned long mask; };
struct Elem { Coll* colls; Elem* ch[4]; Sphere s; float fade; unsigned long mask; int built; long count; };
static std::vector<Sphere> sphere_list;
static void Calc(Elem* e)
{
    sphere_list.resize(0);
    e->fade = 0; e->mask = 0; e->built = 0;
    for (int i = 0; i < 4; ++i)
        if (e->ch[i] && e->ch[i]->s.r > 0.0f) {
            sphere_list.push_back(e->ch[i]->s);
            if (e->fade < e->ch[i]->fade) e->fade = e->ch[i]->fade;
            e->mask |= e->ch[i]->mask;
        }
    for (Coll* c = e->colls; c; c = c->Next)
        if (c->s.r > 0.0f) {
            if (e->fade < c->fade) e->fade = c->fade;
            e->mask |= c->mask;
            sphere_list.push_back(c->s);
        }
    if (sphere_list.size() > 0) { e->built = 1; e->count = (long)sphere_list.size(); }
    else e->s.r = 0;
}
int main()
{
    Elem a = {0}, b = {0}, c = {0};
    b.s.r = 2; b.fade = 5; b.mask = 1;
    c.s.r = 0; c.fade = 50; c.mask = 8;          // skipped: empty sphere
    Coll k2 = {0, {0,0,0,1}, 7, 4};
    Coll k1 = {&k2, {0,0,0,-1}, 99, 16};          // skipped
    a.ch[0] = &b; a.ch[2] = &c; a.colls = &k1; a.s.r = 3;
    Calc(&a);
    if (!a.built || a.count != 2 || a.fade != 7 || a.mask != 5) { std::printf("00bdf100_TEST FAIL\n"); return 1; }
    Elem z = {0}; z.s.r = 9;
    Calc(&z);
    if (z.built || z.s.r != 0 || z.fade != 0 || z.mask != 0) { std::printf("00bdf100_TEST FAIL\n"); return 1; }
    std::printf("00bdf100_TEST PASS\n");
    return 0;
}