// behaviour test for 0x00B91840 CEngineScene::UpdatePosition
#include <cstdio>

struct Prim;
struct Manager { int added; unsigned long lastFlags; Prim* last; void AddPrimitive(Prim& p, unsigned long f) { ++added; lastFlags = f; last = &p; } };
struct Prim { long RefCount; long Type; int removed; void RemoveFromList() { ++removed; } };
struct TypeInfo { Manager* Managers[3]; };
static TypeInfo g_info;

static void UpdatePosition(Prim& primitive, unsigned long flags)
{
    if (!(flags & 0x40)) {
        primitive.RemoveFromList();
        g_info.Managers[primitive.Type]->AddPrimitive(primitive, flags);
    }
}

int main()
{
    Manager m0 = { 0, 0, 0 }, m1 = { 0, 0, 0 }, m2 = { 0, 0, 0 };
    g_info.Managers[0] = &m0; g_info.Managers[1] = &m1; g_info.Managers[2] = &m2;
    Prim p = { 1, 2, 0 };
    UpdatePosition(p, 0x41);
    if (p.removed != 0 || m2.added != 0) { std::printf("00b91840_TEST FAIL\n"); return 1; }
    UpdatePosition(p, 0x3);
    if (p.removed != 1 || m2.added != 1 || m2.lastFlags != 3 || m2.last != &p || m0.added || m1.added) { std::printf("00b91840_TEST FAIL\n"); return 1; }
    std::printf("00b91840_TEST PASS\n");
    return 0;
}