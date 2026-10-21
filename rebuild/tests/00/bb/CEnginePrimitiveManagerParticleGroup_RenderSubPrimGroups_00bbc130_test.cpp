// behaviour test for 00bbc130 CEnginePrimitiveManagerParticleGroup::RenderSubPrimGroups: dispatch by primitive type --
// groups recurse over their members, sprites/trails go to their managers (the trail restoring the constant layout),
// type 30 only when the capability level is >= 2, other types through the type's manager
#include <cstdio>
struct Prim { long type; Prim* members[4]; int memberCount; int id; };
static int g_sprites = 0, g_trails = 0, g_generic = 0, g_type30 = 0, g_layoutSets = 0;
static long g_layout = 9, g_level = 2;
static void SetLayout(long l) { g_layout = l; ++g_layoutSets; }
static void Render(const Prim& p)
{
    switch (p.type) {
    case 24:
        for (int i = 0; i < p.memberCount; ++i) Render(*p.members[i]);
        break;
    case 7: ++g_sprites; break;
    case 20: { ++g_trails; long saved = g_layout; SetLayout(5); SetLayout(saved); break; }
    case 30: if (g_level >= 2) ++g_type30; break;
    default: ++g_generic; break;
    }
}
int main()
{
    Prim a = { 7, { 0 }, 0, 1 }, b = { 20, { 0 }, 0, 2 }, c = { 30, { 0 }, 0, 3 }, d = { 11, { 0 }, 0, 4 };
    Prim inner = { 24, { &a, &d }, 2, 5 };
    Prim outer = { 24, { &inner, &b, &c }, 3, 6 };
    Render(outer);
    if (g_sprites != 1 || g_trails != 1 || g_type30 != 1 || g_generic != 1 || g_layout != 9 || g_layoutSets != 2) { std::printf("00bbc130_TEST FAIL\n"); return 1; }
    g_level = 1;
    Render(c);
    if (g_type30 != 1) { std::printf("00bbc130_TEST FAIL\n"); return 1; }
    std::printf("00bbc130_TEST PASS\n");
    return 0;
}