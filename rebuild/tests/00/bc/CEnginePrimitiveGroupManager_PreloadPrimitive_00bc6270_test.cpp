// Behaviour test for CEnginePrimitiveGroupManager::PreloadPrimitive (0x00BC6270): every non-null child is preloaded through its type's manager; true when any child reports true.
#include <cstdio>
struct V { float X, Y, Z; };
struct Child { int Type; bool answer; int calls; unsigned int flags; };
struct Manager { bool PreloadPrimitive(Child& c, const V&, unsigned int f) { ++c.calls; c.flags = f; return c.answer; } };
static Manager* g_managers[47];
static bool PreloadGroup(Child** start, unsigned int count, const V& pos, unsigned int flags)
{
    bool result = false;
    for (unsigned int i = 0; i < count; ++i) if (start[i] != 0) result |= g_managers[start[i]->Type]->PreloadPrimitive(*start[i], pos, flags);
    return result;
}
int main()
{
    Manager m; for (int i = 0; i < 47; ++i) g_managers[i] = &m; V p = { 0, 0, 0 };
    Child a = { 2, false, 0, 0 }, b = { 5, true, 0, 0 }; Child* list[3] = { &a, 0, &b };
    if (!PreloadGroup(list, 3, p, 4u) || a.calls != 1 || b.calls != 1 || a.flags != 4u) { std::printf("00bc6270_TEST FAIL\n"); return 1; }
    if (PreloadGroup(list, 1, p, 0u) || a.calls != 2) { std::printf("00bc6270_TEST FAIL\n"); return 1; }
    std::printf("00bc6270_TEST PASS\n");
    return 0;
}