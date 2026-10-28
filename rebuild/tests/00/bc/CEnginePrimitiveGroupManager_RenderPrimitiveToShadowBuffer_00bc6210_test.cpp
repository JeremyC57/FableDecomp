// Behaviour test for CEnginePrimitiveGroupManager::RenderPrimitiveToShadowBuffer (0x00BC6210): each non-null child goes to its type's manager; the result is the largest (signed) ERendered.
#include <cstdio>
struct Child { int Type; int answer; int calls; };
struct Manager { int RenderPrimitiveToShadowBuffer(Child& c) { ++c.calls; return c.answer; } };
static Manager* g_managers[47];
static int RenderGroup(Child** start, unsigned int count)
{
    int result = 0;
    for (unsigned int i = 0; i < count; ++i) if (start[i] != 0) { int r = g_managers[start[i]->Type]->RenderPrimitiveToShadowBuffer(*start[i]); if (r > result) result = r; }
    return result;
}
int main()
{
    Manager m; for (int i = 0; i < 47; ++i) g_managers[i] = &m;
    Child a = { 3, 1, 0 }, b = { 7, 2, 0 }, c = { 1, -1, 0 }; Child* list[4] = { &a, 0, &b, &c };
    if (RenderGroup(list, 4) != 2 || a.calls != 1 || b.calls != 1 || c.calls != 1) { std::printf("00bc6210_TEST FAIL\n"); return 1; }
    Child* neg[1] = { &c }; if (RenderGroup(neg, 1) != 0 || RenderGroup(list, 0) != 0) { std::printf("00bc6210_TEST FAIL\n"); return 1; }
    std::printf("00bc6210_TEST PASS\n");
    return 0;
}