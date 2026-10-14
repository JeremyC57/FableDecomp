#include <cstdio>
struct Owner { }; struct Thing { }; struct Str { }; struct ActB { };
static long g_sum = 0; static void* g_f = 0;
struct Base { virtual ~Base() {} unsigned char body[0xb4]; Base(Owner* o, const Str& s, Thing* t, long a, long b, ActB* f) { g_sum = a * 10 + b; g_f = f; } };
struct Act : Base { ActB* m_haste; Act(ActB* h, Owner* o, const Str& s, Thing* t, long a, long b, ActB* f) : Base(o, s, t, a, b, f), m_haste(h) {} };
int main()
{
    Owner o; Thing t; Str s; ActB h, f; Act a(&h, &o, s, &t, 3, 4, &f);
    if (g_sum != 34 || g_f != &f || a.m_haste != &h) { std::printf("0069f1f0_TEST FAIL\n"); return 1; }
    std::printf("0069f1f0_TEST PASS\n");
    return 0;
}