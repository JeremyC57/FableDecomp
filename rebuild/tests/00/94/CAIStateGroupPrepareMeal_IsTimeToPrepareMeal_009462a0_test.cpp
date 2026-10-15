#include <cstdio>
// PrepareMeal::IsTimeToPrepareMeal: village cook time, with the village def held (addref/release) across the call
struct Def { long refCount; int destroyed; void Destroy() { ++destroyed; } };
struct DefPtr { Def* p; DefPtr(Def* d) : p(d) { if (p) ++p->refCount; } ~DefPtr() { if (p) { if (--p->refCount == 0) p->Destroy(); } } };
struct Village { Def* def; bool cookTime; bool IsCookTime() { return cookTime; } };
static Village* g_village = 0;
struct Group { bool IsTimeToPrepareMeal() { if (g_village != 0) { DefPtr def(g_village->def); return g_village->IsCookTime(); } return false; } };
int main()
{
    Def d; d.refCount = 1; d.destroyed = 0; Village v; v.def = &d; v.cookTime = true; g_village = &v; Group g;
    if (!g.IsTimeToPrepareMeal() || d.refCount != 1 || d.destroyed != 0) { std::printf("009462a0_TEST FAIL\n"); return 1; }
    v.cookTime = false; if (g.IsTimeToPrepareMeal()) { std::printf("009462a0_TEST FAIL\n"); return 1; }
    d.refCount = 0; v.cookTime = true; if (!g.IsTimeToPrepareMeal() || d.destroyed != 1) { std::printf("009462a0_TEST FAIL\n"); return 1; }
    v.def = 0; if (!g.IsTimeToPrepareMeal()) { std::printf("009462a0_TEST FAIL\n"); return 1; }
    g_village = 0; if (g.IsTimeToPrepareMeal()) { std::printf("009462a0_TEST FAIL\n"); return 1; }
    std::printf("009462a0_TEST PASS\n"); return 0;
}