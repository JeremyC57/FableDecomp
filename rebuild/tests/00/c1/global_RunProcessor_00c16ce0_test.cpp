// Behaviour test for 0x00C16CE0 (see src/00c16ce0.cpp).
#include <cstdio>
struct Settings { unsigned long a, b, c; unsigned char o[6]; };
struct Processor { unsigned long a, b; unsigned char f[8]; int loaded; };
static void Run(Processor* p, const Settings* s)
{
    p->a = s->a; p->b = s->b;
    p->f[5] = s->o[0]; p->f[6] = s->o[1]; p->f[2] = s->o[2]; p->f[0] = s->o[3]; p->f[1] = s->o[4];
    if (s->o[5]) { p->f[3] = 1; p->f[4] = 1; }
    p->loaded = 1;
}
int main()
{
    Settings s = { 7, 9, 0, { 1, 2, 3, 4, 5, 1 } };
    Processor p = { 0, 0, { 0 }, 0 };
    Run(&p, &s);
    if (p.a != 7 || p.b != 9 || p.f[5] != 1 || p.f[6] != 2 || p.f[2] != 3 || p.f[0] != 4 || p.f[1] != 5 || p.f[3] != 1 || p.f[4] != 1 || !p.loaded) { std::printf("00c16ce0_TEST FAIL\n"); return 1; }
    Settings t = { 1, 2, 0, { 0, 0, 0, 0, 0, 0 } }; Processor q = { 0, 0, { 0 }, 0 };
    Run(&q, &t);
    if (q.f[3] != 0 || q.f[4] != 0) { std::printf("00c16ce0_TEST FAIL\n"); return 1; }
    std::printf("00c16ce0_TEST PASS\n"); return 0;
}