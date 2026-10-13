#include <cstdio>

struct H { unsigned char pad[0xd6]; bool Pending; };
static H g_hero; static int g_hit = 0;
H* GFGetHeroCreature() { return &g_hero; }
struct C { void On() { ++g_hit; } void Walk(long t) { if (t == 1) { H* h = GFGetHeroCreature(); if (h->Pending) On(); } } };
int main()
{
    C c; g_hero.Pending = true; c.Walk(2); if (g_hit != 0) { std::printf("0077db90_TEST FAIL\n"); return 1; }
    c.Walk(1); if (g_hit != 1) { std::printf("0077db90_TEST FAIL\n"); return 1; }
    g_hero.Pending = false; c.Walk(1); if (g_hit != 1) { std::printf("0077db90_TEST FAIL\n"); return 1; }
    std::printf("0077db90_TEST PASS\n");
    return 0;
}
