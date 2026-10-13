#include <cstdio>

static void* g_on = (void*)1;
struct Th { virtual void SetThingStandingOn(Th* t) { g_on = t; } };
struct Ph { void* vt; Th* sub; void Set(Ph* o) { Th* t = sub; if (o) t->SetThingStandingOn(o->sub); else t->SetThingStandingOn(0); } };
int main()
{
    Th a, b; Ph p, q; p.sub = &a; q.sub = &b; p.Set(&q); if (g_on != &b) { std::printf("006af6d0_TEST FAIL\n"); return 1; }
    p.Set(0); if (g_on != 0) { std::printf("006af6d0_TEST FAIL\n"); return 1; }
    std::printf("006af6d0_TEST PASS\n");
    return 0;
}